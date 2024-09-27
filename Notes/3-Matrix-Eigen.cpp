#include <iostream>
#include <ctime>
#include <eigen3/Eigen/Dense>

class LinearAlgebraSolver {
 public:
  explicit LinearAlgebraSolver(int matrix_size) : size_(matrix_size) {}

  std::pair<Eigen::MatrixXd, Eigen::VectorXd> GenerateLinearSystem() const {
    Eigen::MatrixXd A = Eigen::MatrixXd::Random(size_, size_);
    Eigen::VectorXd b = Eigen::VectorXd::Random(size_);
    return std::make_pair(A, b);
  }

  Eigen::VectorXd SolveGaussJordan(Eigen::MatrixXd A, Eigen::VectorXd b) const {
    Eigen::MatrixXd augmented_system(size_, size_ + 1);
    augmented_system << A, b;

    for (int i = 0; i < size_; ++i) {
      double pivot = augmented_system(i, i);
      augmented_system.row(i) /= pivot;

      for (int j = 0; j < size_; ++j) {
        if (i != j) {
          double factor = augmented_system(j, i);
          augmented_system.row(j) -= factor * augmented_system.row(i);
        }
      }
    }
    return augmented_system.col(size_);
  }

  Eigen::VectorXd SolveLU(const Eigen::MatrixXd& A, const Eigen::VectorXd& b) const {
    Eigen::FullPivLU<Eigen::MatrixXd> lu(A);
    return lu.solve(b);
  }

  void CompareExecutionTimes(int num_iterations) const {
    double gauss_jordan_time = 0.0;
    double lu_time = 0.0;

    for (int iter = 0; iter < num_iterations; ++iter) {
      auto [A, b] = GenerateLinearSystem();

      clock_t start_time = clock();
      SolveGaussJordan(A, b);
      gauss_jordan_time += static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;

      start_time = clock();
      SolveLU(A, b);
      lu_time += static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;
    }

    std::cout << "Average Gauss-Jordan time: "
              << gauss_jordan_time / num_iterations << " seconds" << std::endl;
    std::cout << "Average LU decomposition time: "
              << lu_time / num_iterations << " seconds" << std::endl;
  }

 private:
  int size_;
};

int main() {
  LinearAlgebraSolver solver(100);
  solver.CompareExecutionTimes(50);
  return 0;
}
