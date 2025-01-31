#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
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

double NewtonInterpolation(const std::vector<Point>& points, double x) {
  size_t n = points.size();
  std::vector<double> coefficients(n);

  for (size_t i = 0; i < n; ++i) coefficients[i] = points[i].y;

  for (size_t j = 1; j < n; ++j)
    for (size_t i = n - 1; i >= j; --i)
      coefficients[i] = (coefficients[i] - coefficients[i - 1]) / (points[i].x - points[i - j].x);

  double result = coefficients[n - 1];
  for (size_t i = n - 1; i > 0; --i) result = result * (x - points[i - 1].x) + coefficients[i - 1];

  return result;
}

double AbsoluteError(double true_value, double predicted_value) {
  return std::abs(true_value - predicted_value);
}

double ComputeR2(const std::vector<Point>& data, const std::vector<Point>& train, double (*interp_func)(const std::vector<Point>&, double)) {
  double ss_total = 0.0, ss_residual = 0.0, mean_y = 0.0;
  for (const auto& p : data) mean_y += p.y;
  mean_y /= data.size();

  for (const auto& p : data) {
    double pred_y = interp_func(train, p.x);
    ss_total += std::pow(p.y - mean_y, 2);
    ss_residual += std::pow(p.y - pred_y, 2);
  }
  return 1.0 - (ss_residual / ss_total);
}

int main() {
  std::vector<Point> data = ReadData("../Data/2m-fall.tsv");
  size_t step_size = 10;
  std::vector<Point> train = SelectEveryNth(data, step_size);

  std::ofstream output("./2m-data-newton.tsv");
  output << "#x\tTrue\tInterpolated\n";

  std::ofstream error_output("./2m-error-newton.tsv");
  error_output << "#x\tAbsError\n";

  std::ofstream iae_output("./2m-incremental-IAE-newton.tsv");
  iae_output << "#Data_Point\tIAE\n";

  const int precision = 12;
  std::cout << std::fixed << std::setprecision(precision);

  double r2_newton = ComputeR2(data, train, NewtonInterpolation);
  std::cout << "R² (Newton): " << r2_newton << std::endl;

  double iae = 0.0;

  for (const auto& p : data) {
    double newton_pred = NewtonInterpolation(train, p.x);
    double abs_err = AbsoluteError(p.y, newton_pred);

    iae += abs_err;

    iae_output << p.x << "\t" << iae << "\n";
    output << p.x << "\t" << p.y << "\t" << newton_pred << "\n";

    error_output << p.x << "\t" << abs_err << "\n";
  }

  output.close();
  error_output.close();
  iae_output.close();

  return 0;
}
