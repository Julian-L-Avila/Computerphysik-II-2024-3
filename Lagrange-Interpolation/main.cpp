#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

const std::vector<double> test_points = {0.15, 3.15, 5.00, 7.95};

class DataLoader {
	public:
		static void LoadData(const std::string& file_path,
				std::vector<double>& x_values,
				std::vector<double>& y_values) {
			std::ifstream file(file_path);
			if (!file.is_open()) {
				throw std::runtime_error("Could not open file: " + file_path);
			}

			double x, y;
			while (file >> x >> y) {
				x_values.push_back(x);
				y_values.push_back(y);
			}

			if (x_values.empty() || y_values.empty()) {
				throw std::runtime_error("File is empty or data format is incorrect.");
			}
		}
};

class LagrangeInterpolator {
	private:
		std::vector<double> x_values_;
		std::vector<double> y_values_;
		std::vector<double> cardinal_denominators_;

		double CardinalDenominator(int index) const {
			double result = 1.0;
			for (size_t i = 0; i < x_values_.size(); ++i) {
				if (i != index) {
					result *= (x_values_[index] - x_values_[i]);
				}
			}
			return result;
		}

		double CardinalNumerator(int index, double target) const {
			double result = 1.0;
			for (size_t i = 0; i < x_values_.size(); ++i) {
				if (i != index) {
					result *= (target - x_values_[i]);
				}
			}
			return result;
		}

	public:
		LagrangeInterpolator(const std::vector<double>& x, const std::vector<double>& y)
			: x_values_(x), y_values_(y), cardinal_denominators_(x.size()) {
				for (size_t i = 0; i < cardinal_denominators_.size(); ++i) {
					cardinal_denominators_[i] = CardinalDenominator(i);
				}
			}

		double Interpolate(double target) const {
			auto it = std::lower_bound(x_values_.begin(), x_values_.end(), target);
			if (it != x_values_.end() && *it == target) {
				return y_values_[std::distance(x_values_.begin(), it)];
			}

			double result = 0.0;
			for (size_t i = 0; i < x_values_.size(); ++i) {
				result += y_values_[i] * CardinalNumerator(i, target) / cardinal_denominators_[i];
			}
			return result;
		}

		std::string CardinalNumeratorText(int index) const {
			std::string l_i = "l_" + std::to_string(index) + "(x) = " + std::to_string(cardinal_denominators_[index]);
			for (size_t i = 0; i < x_values_.size(); ++i) {
				if (i != index) {
					l_i += " * (x - " + std::to_string(x_values_[i]) + ")";
				}
			}
			return l_i;
		}
};

int main() {
	const std::string file_path = "./data.dat";
	std::vector<double> x_values;
	std::vector<double> y_values;

	try {
		DataLoader::LoadData(file_path, x_values, y_values);
		std::cout << "Loaded Data:\n";
		for (size_t i = 0; i < x_values.size(); ++i) {
			std::cout << "x: " << x_values[i] << ", y: " << y_values[i] << '\n';
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}

	LagrangeInterpolator interpolator(x_values, y_values);
	const std::vector<double> test_points = {0.15, 3.15, 5.00, 7.95};

	for (double target : test_points) {
		std::cout << "Interpolated value at x = " << target << ": "
			<< interpolator.Interpolate(target) << '\n';
	}

	for (size_t i = 0; i < x_values.size(); ++i) {
		std::cout << interpolator.CardinalNumeratorText(i) << '\n';
	}

	return 0;
}

