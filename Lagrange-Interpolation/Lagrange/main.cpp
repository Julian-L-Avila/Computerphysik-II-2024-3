#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>

const std::vector<double> interpolation_points = {0.15, 3.15, 5.00, 7.95};

class DataLoader {
	public:
		static void LoadData(const std::string& file_path, std::vector<double>& x_values, std::vector<double>& y_values) {
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

class PlotScriptGenerator {
	public:
		static std::string GenerateCardinalNumeratorsScript(const std::vector<double>& x_values, const std::vector<double>& cardinal_denominators) {
			std::ostringstream script;
			for (size_t i = 0; i < x_values.size(); ++i) {
				script << "l_" << i << "(x) = (1.0 / " << cardinal_denominators[i] << ")";
				for (size_t j = 0; j < x_values.size(); ++j) {
					if (j != i) {
						script << " * (x - " << x_values[j] << ")";
					}
				}
				script << "\n";
			}
			return script.str();
		}

		static std::string GenerateInterpolationPolynomialScript(const std::vector<double>& y_values) {
			std::ostringstream script;
			script << "L(x) = 0.0 ";
			for (size_t i = 0; i < y_values.size(); ++i) {
				script << "+ " << y_values[i] << " * l_" << i << "(x) ";
			}
			return script.str();
		}
};

class LagrangeInterpolator {
	private:
		std::vector<double> x_values_;
		std::vector<double> y_values_;
		std::vector<double> cardinal_denominators_;

		void PrecomputeCardinalDenominators() {
			cardinal_denominators_.resize(x_values_.size());
			for (size_t i = 0; i < cardinal_denominators_.size(); ++i) {
				cardinal_denominators_[i] = CardinalDenominator(i);
			}
		}

		double CardinalDenominator(size_t index) const {
			double result = 1.0;
			for (size_t i = 0; i < x_values_.size(); ++i) {
				if (i != index) {
					result *= (x_values_[index] - x_values_[i]);
				}
			}
			return result;
		}

		std::vector<double> MultiplyPolynomials(const std::vector<double>& p1, const std::vector<double>& p2) const {
			std::vector<double> result(p1.size() + p2.size() - 1, 0.0);
			for (size_t i = 0; i < p1.size(); ++i) {
				for (size_t j = 0; j < p2.size(); ++j) {
					result[i + j] += p1[i] * p2[j];
				}
			}
			return result;
		}

		std::vector<double> ExpandCardinalNumerator(size_t index) const {
			std::vector<double> numerator = {1.0};
			for (size_t i = 0; i < x_values_.size(); ++i) {
				if (i != index) {
					numerator = MultiplyPolynomials(numerator, {-x_values_[i], 1.0});
				}
			}
			return numerator;
		}

	public:
		LagrangeInterpolator(const std::vector<double>& x, const std::vector<double>& y)
			: x_values_(x), y_values_(y) {
				PrecomputeCardinalDenominators();
			}

		double Interpolate(double target) const {
			double result = 0.0;
			for (size_t i = 0; i < x_values_.size(); ++i) {
				double numerator = 1.0;
				for (size_t j = 0; j < x_values_.size(); ++j) {
					if (j != i) {
						numerator *= (target - x_values_[j]);
					}
				}
				result += y_values_[i] * numerator / cardinal_denominators_[i];
			}
			return result;
		}

		void SaveInterpolatedValues(const std::string& output_file, const std::vector<double>& points) const {
			std::ofstream file(output_file);
			if (!file.is_open()) {
				throw std::runtime_error("Could not open file: " + output_file);
			}
			for (double point : points) {
				file << point << " " << Interpolate(point) << "\n";
			}
		}

		void GenerateGraphingScript(const std::string& data_path, const std::string& interpolation_path) const {
			auto plot_script = PlotScriptGenerator::GenerateCardinalNumeratorsScript(x_values_, cardinal_denominators_);
			auto interpolation_script = PlotScriptGenerator::GenerateInterpolationPolynomialScript(y_values_);

			std::ofstream plot_file("./interpolation.gnu");
			plot_file << "set term qt\nset grid\nset style line 1 lw 4 ps 2\n";
			plot_file << "set title 'Lagrange Cardinals'\n\n";
			plot_file << plot_script;
			plot_file << "\np '" << data_path << "' tit 'data', l_0(x)";
			for (size_t i = 1; i < x_values_.size(); ++i) {
				plot_file << ", l_" << i << "(x)";
			}
			plot_file << "\npause -1\n";
			plot_file << interpolation_script << "\n";
			plot_file << "set auto xy\nset title 'Interpolation Polynomial'\n";
			plot_file << "\np '" << data_path << "' tit 'data', L(x) tit 'polynomial', '" << interpolation_path << "' tit 'interpolated'\n";
			plot_file << "pause -1\n";
		}

		void PrintExpandedPolynomial() const {
			std::vector<double> polynomial(x_values_.size(), 0.0);
			for (size_t i = 0; i < x_values_.size(); ++i) {
				auto numerator = ExpandCardinalNumerator(i);
				double coefficient = y_values_[i] / cardinal_denominators_[i];
				for (size_t j = 0; j < numerator.size(); ++j) {
					polynomial[j] += numerator[j] * coefficient;
				}
			}

			std::cout << "L(x) = ";
			bool first = true;
			for (size_t i = 0; i < polynomial.size(); ++i) {
				if (std::abs(polynomial[i]) > 1e-10) {
					if (!first && polynomial[i] > 0) std::cout << " + ";
					else if (polynomial[i] < 0) std::cout << " - ";
					std::cout << std::fixed << std::setprecision(4) << std::abs(polynomial[i]);
					if (i > 0) std::cout << "x" << (i > 1 ? "^" + std::to_string(i) : "");
					first = false;
				}
			}
			std::cout << std::endl;
		}
};

int main() {
	const std::string file_path = "./data.dat";
	const std::string interpolation_path = "./interpolated_values.dat";

	try {
		std::vector<double> x_values, y_values;
		DataLoader::LoadData(file_path, x_values, y_values);

		std::cout << "Loaded Data:\n";
		for (size_t i = 0; i < x_values.size(); ++i) {
			std::cout << "x: " << x_values[i] << ", y: " << y_values[i] << '\n';
		}

		LagrangeInterpolator interpolator(x_values, y_values);

		std::cout << "\nInterpolated Values:\n";
		for (double target : interpolation_points) {
			std::cout << "x = " << target << ", L(x) = " << interpolator.Interpolate(target) << '\n';
		}

		interpolator.SaveInterpolatedValues(interpolation_path, interpolation_points);
		interpolator.GenerateGraphingScript(file_path, interpolation_path);

		std::cout << "\nExpanded Polynomial:\n";
		interpolator.PrintExpandedPolynomial();

		std::string command = "gnuplot interpolation.gnu";
		system(command.c_str());
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}

	return 0;
}

