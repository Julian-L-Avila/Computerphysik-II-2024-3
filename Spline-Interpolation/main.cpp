#include <iostream>
#include <vector>
#include <fstream>
#include <eigen3/Eigen/Dense>

class CubicSpline {
 public:
  CubicSpline(const std::vector<double>& x, const std::vector<double>& y) : x_(x), y_(y), n_(x.size()) {
    ComputeCoefficients();
  }

  double Interpolate(double x) const {
    int i = FindSegment(x);
    double dx = x - x_[i];
    return a_[i] + b_[i] * dx + c_[i] * dx * dx + d_[i] * dx * dx * dx;
  }

 private:
  int n_;
  std::vector<double> x_, y_, a_, b_, c_, d_;

  void ComputeCoefficients() {
    a_ = y_;
    Eigen::VectorXd h(n_ - 1);
    for (int i = 0; i < n_ - 1; ++i) h(i) = x_[i + 1] - x_[i];

    Eigen::VectorXd alpha(n_ - 1);
    for (int i = 1; i < n_ - 1; ++i) {
      alpha(i) = 3.0 * (a_[i + 1] - a_[i]) / h(i) - 3.0 * (a_[i] - a_[i - 1]) / h(i - 1);
    }

    Eigen::VectorXd l(n_), mu(n_), z(n_);
    l(0) = 1.0;
    for (int i = 1; i < n_ - 1; ++i) {
      l(i) = 2.0 * (x_[i + 1] - x_[i - 1]) - h(i - 1) * mu(i - 1);
      mu(i) = h(i) / l(i);
      z(i) = (alpha(i) - h(i - 1) * z(i - 1)) / l(i);
    }
    l(n_ - 1) = 1.0;

    c_.resize(n_, 0);
    b_.resize(n_ - 1);
    d_.resize(n_ - 1);
    for (int j = n_ - 2; j >= 0; --j) {
      c_[j] = z(j) - mu(j) * c_[j + 1];
      b_[j] = (a_[j + 1] - a_[j]) / h(j) - h(j) * (c_[j + 1] + 2.0 * c_[j]) / 3.0;
      d_[j] = (c_[j + 1] - c_[j]) / (3.0 * h(j));
    }
  }

  int FindSegment(double x) const {
    int i = 0, j = n_ - 1;
    while (i < j) {
      int m = (i + j) / 2;
      if (x_[m] < x) i = m + 1;
      else j = m;
    }
    return std::max(0, i - 1);
  }
};

int main() {
  std::vector<double> x = {0, 1, 2, 3, 4};
  std::vector<double> y = {0, 1, 4, 9, 16};
  CubicSpline spline(x, y);

  std::ofstream file("spline_output.dat");
  for (double xq = 0; xq <= 4; xq += 0.01) {
    file << xq << " " << spline.Interpolate(xq) << std::endl;
  }
  file.close();
}

