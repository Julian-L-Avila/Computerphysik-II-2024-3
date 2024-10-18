#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>

constexpr int kSize = 5;
constexpr double kGravity = 9.81;

class MatrixInitializer {
  public:
    MatrixInitializer(int size, double lower_bound, double upper_bound)
      : size_(size), scale_((upper_bound - lower_bound) / 2.0), offset_((upper_bound + lower_bound) / 2.0) {}

    Eigen::MatrixXd RandomFill() const {
      return (Eigen::MatrixXd::Random(1, size_).array() * scale_ + offset_).matrix();
    }

  private:
    int size_;
    double scale_;
    double offset_;
};

auto ComputeFrequencies(const Eigen::MatrixXd& springs, const Eigen::MatrixXd& masses, const Eigen::MatrixXd& lengths) {
  return (Eigen::MatrixXd(2, springs.cols()) << springs.array() / masses.array(), kGravity / lengths.array()).finished();
}

auto BuildCoupledMatrix(const Eigen::MatrixXd& masses, const Eigen::MatrixXd& lengths, const Eigen::MatrixXd& natural_frequencies_square) {
  Eigen::MatrixXd linear_system = Eigen::MatrixXd::Zero(kSize, kSize);
  for (int j = 0; j < kSize; ++j) {
    linear_system(j, j) = -natural_frequencies_square(1, j);
    double left_interaction = (j > 0) ? masses(0, j - 1) * natural_frequencies_square(0, j - 1) * lengths(0, j - 1) / (masses(0, j) * lengths(0, j)) : 0.0;
    double right_interaction = (j < kSize - 1) ? natural_frequencies_square(0, j) * lengths(0, j + 1) / lengths(0, j) : 0.0;
    linear_system(j, j) -= (left_interaction + right_interaction);
    if (j > 0) linear_system(j, j - 1) = left_interaction;
    if (j < kSize - 1) linear_system(j, j + 1) = right_interaction;
  }
  return linear_system;
}

class SystemParameters {
 public:
  SystemParameters(int size, double lower_bound_masses, double upper_bound_masses,
                   double lower_bound_lengths, double upper_bound_lengths,
                   double lower_bound_springs, double upper_bound_springs)
      : masses_initializer_(size, lower_bound_masses, upper_bound_masses),
        lengths_initializer_(size, lower_bound_lengths, upper_bound_lengths),
        springs_initializer_(size, lower_bound_springs, upper_bound_springs) {}

  Eigen::MatrixXd GenerateMasses() const {
    return masses_initializer_.RandomFill();
  }

  Eigen::MatrixXd GenerateLengths() const {
    return lengths_initializer_.RandomFill();
  }

  Eigen::MatrixXd GenerateSprings() const {
    return springs_initializer_.RandomFill();
  }

 private:
  MatrixInitializer masses_initializer_;
  MatrixInitializer lengths_initializer_;
  MatrixInitializer springs_initializer_;
};


void ComputeEigenValuesAndVectors(const Eigen::MatrixXd& matrix) {
  Eigen::EigenSolver<Eigen::MatrixXd> solver(matrix);
  if (solver.info() != 0) {
    std::cerr << "Error computing eigenvalues and eigenvectors." << std::endl;
    return;
  }

  std::cout << "Eigenvalues:\n" << solver.eigenvalues() << "\n";
  std::cout << "Eigenvectors:\n" << solver.eigenvectors() << "\n";
}

std::vector<double> CharacteristicPolinomial(const Eigen::MatrixXd& matrix) {
  int n = matrix.rows();

  Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n, n);

  std::vector<double> p(n + 1);
  std::vector<double> s(n + 1);

  for (int i = 1; i <= n; ++i) {
    I = I * matrix;
    s[i] = I.trace();
  }

  p[0] = 1.0;
  p[1] = -s[1];
  for (int i = 2; i <= n; i++) {
    p[i] = -s[i] / i;
    for (int j = 1; j < i; j++) {
      p[i] -= s[i - j] * p[j] / i;
    }
  }

  for (int i = 0; i <= n; i++) {
    std::cout << "x^" << n-i << ": " << p[i] << std::endl;
  }
  return p;
}

int main() {
  SystemParameters system_params(kSize, 1.0, 2.0, 1.0, 2.0, 0.5, 2.5);

  const auto masses = system_params.GenerateMasses();
  const auto lengths = system_params.GenerateLengths();
  const auto springs = system_params.GenerateSprings();

  const auto natural_frequencies_square = ComputeFrequencies(springs, masses, lengths);

  std::cout << "Masses:\n" << masses << "\n"
    << "Lengths:\n" << lengths << "\n"
    << "Springs:\n" << springs << "\n"
    << "Natural Frequencies Square:\n" << natural_frequencies_square << "\n";

  const auto linear_system = BuildCoupledMatrix(masses, lengths, natural_frequencies_square);
  std::cout << "Linear System:\n" << linear_system << "\n";

  std::cout << "Characteristic polynomial coefficients:" << std::endl;
  std::vector<double> polynomial = CharacteristicPolinomial(linear_system);

  ComputeEigenValuesAndVectors(linear_system);

  return 0;
}
