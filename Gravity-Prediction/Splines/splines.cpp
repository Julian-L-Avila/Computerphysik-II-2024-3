#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <eigen3/Eigen/Dense>
#include <vector>

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

double Interpolate(const std::vector<Point>& points, double x, size_t degree) {
  size_t n = points.size();
  Eigen::MatrixXd A(n, degree + 1);
  Eigen::VectorXd b(n);

  for (size_t i = 0; i < n; ++i) {
    double xi = points[i].x;
    b(i) = points[i].y;
    for (size_t j = 0; j <= degree; ++j) {
      A(i, j) = std::pow(xi, degree - j);
    }
  }

  Eigen::VectorXd coeffs = SolveCoefficients(A, b);
  double result = 0.0;
  for (size_t i = 0; i <= degree; ++i) {
    result += coeffs(i) * std::pow(x, degree - i);
  }
  return result;
}

double AbsoluteError(double true_value, double predicted_value) {
  return std::abs(true_value - predicted_value);
}

double TrapezoidalRule(const std::vector<Point>& data, const std::vector<Point>& train, double (*spline_func)(const std::vector<Point>&, double, size_t)) {
  double integral = 0.0;
  size_t n = data.size();
  for (size_t i = 1; i < n; ++i) {
    double x1 = data[i - 1].x, x2 = data[i].x;
    double y_true1 = data[i - 1].y, y_true2 = data[i].y;
    double y_pred1 = spline_func(train, x1, 2), y_pred2 = spline_func(train, x2, 2);
    integral += (AbsoluteError(y_true1, y_pred1) + AbsoluteError(y_true2, y_pred2)) * (x2 - x1) / 2.0;
  }
  return integral;
}

double ComputeR2(const std::vector<Point>& data, const std::vector<Point>& train, double (*spline_func)(const std::vector<Point>&, double, size_t)) {
  double ss_total = 0.0, ss_residual = 0.0, mean_y = 0.0;
  for (const auto& p : data) mean_y += p.y;
  mean_y /= data.size();

  for (const auto& p : data) {
    double pred_y = spline_func(train, p.x, 2);
    ss_total += std::pow(p.y - mean_y, 2);
    ss_residual += std::pow(p.y - pred_y, 2);
  }
  return 1.0 - (ss_residual / ss_total);
}

int main() {
  std::vector<Point> data = ReadData("../Data/1m-fall.tsv");
  size_t step_size = 7;
  std::vector<Point> train = SelectEveryNth(data, step_size);

  std::ofstream output("./1m-data-splines.tsv");
  output << "#x\tTrue\tQuadratic\tCubic\n";

  std::ofstream error_output("./1m-error-splines.tsv");
  error_output << "#x\tAbsError_Quad\tAbsError_Cubic\n";

  std::ofstream iae_output("./1m-incremental-IAE-splines.tsv");
  iae_output << "#Data_Point\tIAE_Quadratic\tIAE_Cubic\n";

  const int precision = 12;
  std::cout << std::fixed << std::setprecision(precision);

  double r2_quad = ComputeR2(data, train, Interpolate);
  double r2_cubic = ComputeR2(data, train, Interpolate);
  std::cout << "R² (Quadratic): " << r2_quad << std::endl;
  std::cout << "R² (Cubic): " << r2_cubic << std::endl;

  double iae_quad = 0.0, iae_cubic = 0.0;

  for (const auto& p : data) {
    double quad_pred = Interpolate(train, p.x, 2);
    double cubic_pred = Interpolate(train, p.x, 3);
    double abs_err_quad = AbsoluteError(p.y, quad_pred);
    double abs_err_cubic = AbsoluteError(p.y, cubic_pred);

    iae_quad += abs_err_quad;
    iae_cubic += abs_err_cubic;

    iae_output << p.x << "\t" << iae_quad << "\t" << iae_cubic << "\n";
    output << p.x << "\t" << p.y << "\t" << quad_pred << "\t" << cubic_pred << "\n";

    if (std::find_if(train.begin(), train.end(), [&](const Point& tp) { return tp.x == p.x; }) == train.end()) {
      error_output << p.x << "\t" << abs_err_quad << "\t" << abs_err_cubic << "\n";
    }
  }

  output.close();
  error_output.close();
  iae_output.close();

  return 0;
}

