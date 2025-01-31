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

double LagrangeInterpolation(const std::vector<Point>& points, double x) {
  size_t n = points.size();
  double result = 0.0;

  for (size_t i = 0; i < n; ++i) {
    double term = points[i].y;
    for (size_t j = 0; j < n; ++j) {
      if (i != j) {
        term *= (x - points[j].x) / (points[i].x - points[j].x);
      }
    }
    result += term;
  }

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

  std::ofstream output("./2m-data-lagrange.tsv");
  output << "#x\tTrue\tInterpolated\n";

  std::ofstream error_output("./2m-error-lagrange.tsv");
  error_output << "#x\tAbsError\n";

  std::ofstream iae_output("./2m-incremental-IAE-lagrange.tsv");
  iae_output << "#Data_Point\tIAE\n";

  const int precision = 12;
  std::cout << std::fixed << std::setprecision(precision);

  double r2_lagrange = ComputeR2(data, train, LagrangeInterpolation);
  std::cout << "R² (Lagrange): " << r2_lagrange << std::endl;

  double iae = 0.0;

  for (const auto& p : data) {
    double lagrange_pred = LagrangeInterpolation(train, p.x);
    double abs_err = AbsoluteError(p.y, lagrange_pred);

    iae += abs_err;

    iae_output << p.x << "\t" << iae << "\n";
    output << p.x << "\t" << p.y << "\t" << lagrange_pred << "\n";

    error_output << p.x << "\t" << abs_err << "\n";
  }

  output.close();
  error_output.close();
  iae_output.close();

  return 0;
}

