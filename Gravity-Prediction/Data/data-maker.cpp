#include <iostream>
#include <fstream>

constexpr double kG = 9.81;
constexpr double kTimeStep = 0.1;
constexpr double kTotalTime = 10;
constexpr double kInitialHeight = 100.0;

double TruePosition(double t) {
	return kInitialHeight - 0.5 * kG * t * t;
}

int main() {
	std::ofstream file("./falling-data.tsv");
	for (double t = 0.0; t <= kTotalTime; t += kTimeStep) {
		double position = TruePosition(t);
		file << t << "\t" << position << "\n";
	}
	file.close();

	return 0;
}

