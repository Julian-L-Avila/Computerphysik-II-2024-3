#include <iostream>
#include <eigen3/Eigen/Dense>

constexpr int kSize = 8;
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

void ComputeEigenValuesAndVectors(const Eigen::MatrixXd& matrix) {
  Eigen::EigenSolver<Eigen::MatrixXd> solver(matrix);

  if (solver.info() != 0) {
    std::cerr << "Error computing eigenvalues and eigenvectors." << std::endl;
    return;
  }

  std::cout << "Eigenvalues:\n" << solver.eigenvalues() << "\n";
  std::cout << "Eigenvectors:\n" << solver.eigenvectors() << "\n";
}

int main() {
  MatrixInitializer masses_initializer(kSize, 1.0, 2.0);
  MatrixInitializer lengths_initializer(kSize, 1.0, 2.0);
  MatrixInitializer springs_initializer(kSize, 0.5, 2.5);

  const auto masses = masses_initializer.RandomFill();
  const auto lengths = lengths_initializer.RandomFill();
  const auto springs = springs_initializer.RandomFill();

  const auto natural_frequencies_square = ComputeFrequencies(springs, masses, lengths);

  std::cout << "Masses:\n" << masses << "\n"
    << "Lengths:\n" << lengths << "\n"
    << "Springs:\n" << springs << "\n"
    << "Natural Frequencies Square:\n" << natural_frequencies_square << "\n";

  const auto linear_system = BuildCoupledMatrix(masses, lengths, natural_frequencies_square);
  std::cout << "Linear System:\n" << linear_system << "\n";

  ComputeEigenValuesAndVectors(linear_system);

  return 0;
}
