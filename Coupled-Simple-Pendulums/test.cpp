#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <chrono>
#include <stdexcept>
#include <fstream>

constexpr double kGravity = 9.81;

class MatrixInitializer {
  public:
    MatrixInitializer(int size, double lower_bound, double upper_bound)
      : size_(size), scale_((upper_bound - lower_bound) / 2.0), offset_((upper_bound + lower_bound) / 2.0) {
        if (size <= 0) throw std::invalid_argument("Size must be positive.");
        if (lower_bound >= upper_bound) throw std::invalid_argument("Upper bound must be greater than lower bound.");
      }

    Eigen::MatrixXd Fill(bool is_uniform = false, double value = 0.0) const {
    return is_uniform ? Eigen::MatrixXd::Constant(1, size_, value)
                      : Eigen::MatrixXd((Eigen::MatrixXd::Random(1, size_).array() * scale_ + offset_).matrix());
}


  private:
    int size_;
    double scale_;
    double offset_;
};

auto ComputeFrequencies(const Eigen::MatrixXd& springs, const Eigen::MatrixXd& masses, const Eigen::MatrixXd& lengths) {
  return (Eigen::MatrixXd(2, springs.cols()) << springs.array() / masses.array(), kGravity / lengths.array()).finished();
}

auto BuildCoupledMatrix(const Eigen::MatrixXd& masses, const Eigen::MatrixXd& lengths, const Eigen::MatrixXd& natural_frequencies_square, int size) {
  Eigen::MatrixXd linear_system = Eigen::MatrixXd::Zero(size, size);

  for (int j = 0; j < size; ++j) {
    linear_system(j, j) = -natural_frequencies_square(1, j);

    double left_interaction = 0.0;
    double right_interaction = 0.0;

    if (j > 0) {
      left_interaction = masses(0, j - 1) * natural_frequencies_square(0, j - 1) * lengths(0, j - 1) / (masses(0, j) * lengths(0, j));
    }

    if (j < size - 1) {
      right_interaction = natural_frequencies_square(0, j) * lengths(0, j + 1) / lengths(0, j);
    }

    linear_system(j, j) -= (left_interaction + right_interaction);

    if (j > 0) {
      linear_system(j, j - 1) = left_interaction;
    }

    if (j < size - 1) {
      linear_system(j, j + 1) = right_interaction;
    }
  }

  return linear_system;
}

class SystemParameters {
  public:
    SystemParameters(int size,
        double lower_bound_masses = 1.0, double upper_bound_masses = 2.0, bool uniform_masses = false, double uniform_mass_value = 0.0,
        double lower_bound_lengths = 0.5, double upper_bound_lengths = 1.5, bool uniform_lengths = false, double uniform_length_value = 0.0,
        double lower_bound_springs = 0.5, double upper_bound_springs = 2.0, bool uniform_springs = false, double uniform_spring_value = 0.0)
      : masses_initializer_(size, lower_bound_masses, upper_bound_masses),
      lengths_initializer_(size, lower_bound_lengths, upper_bound_lengths),
      springs_initializer_(size, lower_bound_springs, upper_bound_springs),
      uniform_masses_(uniform_masses), uniform_mass_value_(uniform_mass_value),
      uniform_lengths_(uniform_lengths), uniform_length_value_(uniform_length_value),
      uniform_springs_(uniform_springs), uniform_spring_value_(uniform_spring_value) {}

    Eigen::MatrixXd GenerateMasses() const {
      return uniform_masses_ ? masses_initializer_.Fill(true, uniform_mass_value_) : masses_initializer_.Fill();
    }

    Eigen::MatrixXd GenerateLengths() const {
      return uniform_lengths_ ? lengths_initializer_.Fill(true, uniform_length_value_) : lengths_initializer_.Fill();
    }

    Eigen::MatrixXd GenerateSprings() const {
      return uniform_springs_ ? springs_initializer_.Fill(true, uniform_spring_value_) : springs_initializer_.Fill();
    }

  private:
    MatrixInitializer masses_initializer_;
    MatrixInitializer lengths_initializer_;
    MatrixInitializer springs_initializer_;
    bool uniform_masses_;
    double uniform_mass_value_;
    bool uniform_lengths_;
    double uniform_length_value_;
    bool uniform_springs_;
    double uniform_spring_value_;
};

void ComputeEigenValuesAndVectors(const Eigen::MatrixXd& matrix) {
  Eigen::EigenSolver<Eigen::MatrixXd> solver(matrix);

  if (solver.info() != Eigen::Success) {
    throw std::runtime_error("Error computing eigenvalues and eigenvectors.");
  }

  solver.eigenvalues().transpose();
  solver.eigenvectors();
}

std::vector<double> CharacteristicPolynomial(const Eigen::MatrixXd& matrix) {
  int n = matrix.rows();
  std::vector<double> p(n + 1, 0.0), s(n + 1, 0.0);
  Eigen::MatrixXd A = matrix;

  for (int i = 1; i <= n; ++i) {
    s[i] = A.trace();
    A *= matrix;
  }

  p[0] = 1.0;
  for (int i = 1; i <= n; ++i) {
    p[i] = -s[i] / i;
    for (int j = 1; j < i; ++j) {
      p[i] -= s[i - j] * p[j] / i;
    }
  }

  return p;
}

int main() {
  try {
    std::ofstream outfile("results.dat");
    outfile << "# n Time(s)\n";

    for (int n = 1; n <= 1000; ++n) {
      auto start_time = std::chrono::high_resolution_clock::now();

      SystemParameters system_params(n, 1.0, 2.0, false, 1.5, 0.5, 1.5, false, 1.0, 0.5, 2.0, false, 0.5);

      const auto masses = system_params.GenerateMasses();
      const auto lengths = system_params.GenerateLengths();
      const auto springs = system_params.GenerateSprings();
      const auto natural_frequencies_square = ComputeFrequencies(springs, masses, lengths);

      std::cout << "n = " << n << "\n";
      const auto linear_system = BuildCoupledMatrix(masses, lengths, natural_frequencies_square, n);

      auto polynomial = CharacteristicPolynomial(linear_system);
      ComputeEigenValuesAndVectors(linear_system);

      auto end_time = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

      std::cout << "Time taken for n = " << n << ": " << duration << " ms\n\n";

      outfile << n << '\t' << duration << "\n";
    }

    outfile.close();
  } catch (const std::exception& e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
  }

  return 0;
}
