#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <cmath>

struct Point {
  double x, y;
};

std::vector<Point> ReadData(const std::string& filename) {
  std::ifstream file(filename);
  std::vector<Point> data;
  double x, y;
  while (file >> x >> y) data.push_back({x, y});
  return data;
}

std::vector<Point> SelectEveryNth(const std::vector<Point>& data, size_t n) {
  std::vector<Point> selected;
  for (size_t i = 0; i < data.size(); i += n) selected.push_back(data[i]);
  return selected;
}

Eigen::VectorXd SolveCoefficients(const Eigen::MatrixXd& A, const Eigen::VectorXd& b) {
  return A.colPivHouseholderQr().solve(b);
}

double QuadraticRegression(const std::vector<Point>& points, double x) {
  size_t n = points.size();
  Eigen::MatrixXd A(n, 3);
  Eigen::VectorXd b(n);

  for (size_t i = 0; i < n; ++i) {
    double xi = points[i].x;
    b(i) = points[i].y;
    A(i, 0) = xi * xi;
    A(i, 1) = xi;
    A(i, 2) = 1.0;
  }

  Eigen::VectorXd coeffs = SolveCoefficients(A, b);
  return coeffs(0) * x * x + coeffs(1) * x + coeffs(2);
}

double ComputeR2(const std::vector<Point>& data, const std::vector<Point>& train) {
  double ss_total = 0.0, ss_residual = 0.0, mean_y = 0.0;
  for (const auto& p : data) mean_y += p.y;
  mean_y /= data.size();

  for (const auto& p : data) {
    double pred_y = QuadraticRegression(train, p.x);
    ss_total += std::pow(p.y - mean_y, 2);
    ss_residual += std::pow(p.y - pred_y, 2);
  }
  return 1.0 - (ss_residual / ss_total);
}

int main() {
  std::vector<Point> data = ReadData("../Data/2m-fall.tsv");
  size_t step_size = 7;
  std::vector<Point> train = SelectEveryNth(data, step_size);

  std::ofstream output("./2m-data-quadratic.tsv");
  output << "#x\tTrue\tQuadratic\n";

  std::ofstream error_output("./2m-error-quadratic.tsv");
  error_output << "#x\tAbsError_Quad\n";

  std::ofstream iae_output("./2m-incremental-IAE-quadratic.tsv");
  iae_output << "#Data_Point\tIAE_Quadratic\n";

  const int precision = 12;
  std::cout << std::fixed << std::setprecision(precision);

  double r2_quad = ComputeR2(data, train);
  std::cout << "R² (Quadratic): " << r2_quad << std::endl;

  double iae_quad = 0.0;

  for (const auto& p : data) {
    double quad_pred = QuadraticRegression(train, p.x);
    double abs_err_quad = std::abs(p.y - quad_pred);

    iae_quad += abs_err_quad;
    iae_output << p.x << "\t" << iae_quad << "\n";

    output << p.x << "\t" << p.y << "\t" << quad_pred << "\n";
    error_output << p.x << "\t" << abs_err_quad << "\n";
  }

  output.close();
  error_output.close();
  iae_output.close();

  return 0;
}

