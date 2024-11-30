#include <iostream>
#include <vector>

const std::vector<double> kX = {0.15, 2.30, 3.15, 4.85, 6.25, 7.95};
const std::vector<double> kY = {4.798667, 4.49013, 4.2243, 3.47313, 2.66674, 1.51909};

class LangrangeInterpol {
	private:
		std::vector<double> x_values;
		std::vector<double> y_values;

		double CardinalDenominator(int& index, std::vector<double>& x_values) {
			double iCardinalDenominator;

			for (int i = 0; i <= x_values.size(); i++) {
				if (i == index) { continue; };

				iCardinalDenominator *= (x_values[index] - x_values[i]);
			};

			return iCardinalDenominator;
		};

	public:
		LangrangeInterpol(const std::vector<double>& x, const std::vector<double>& y) : x_values(x), y_values(y) {}

};
