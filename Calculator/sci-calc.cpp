#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <cmath>

bool GetValidNumber(std::string& input, double& a, double& previousResult) {
	while (true) {
		std::cin >> input;
		if (input == "pi") { a = M_PI; return true; }
		if (input == "e") { a = M_E; return true; }
		if (input == "ans") { a = previousResult; return true; }
		std::istringstream iss(input);
		if (iss >> a) {
				return true;
		}
		std::cout << "Invalid input. Please enter a number or 'ans': " << std::endl;
	}
}

class Operation {
public:
	virtual int ArgumentsNumber() const = 0;
	virtual double Calculate(double a, double b=0, double c=0) const = 0;
	virtual std::string Name() const = 0;
	virtual std::string Category() const = 0;
	virtual ~Operation() = default;
};

class BinaryOperation : public Operation {
public:
	int ArgumentsNumber() const override { return 2; }
};

class UnaryOperation : public Operation {
public:
	int ArgumentsNumber() const override {return 1; }
};

#define DEFINE_OPERATION_CLASS(NAME, BASE, FUNC, DESC, CAT) \
class NAME : public BASE { \
public: \
	double Calculate(double a, double b=0, double c=0) const override { return FUNC; } \
	std::string Name() const override { return DESC; } \
	std::string Category() const override {return CAT; } \
};

DEFINE_OPERATION_CLASS(Sum, BinaryOperation, a + b, "Sum: a + b",
		"Basic Arithmetic")
DEFINE_OPERATION_CLASS(Subtraction, BinaryOperation, a - b,
		"Subtraction: a - b", "Basic Arithmetic")
DEFINE_OPERATION_CLASS(Product, BinaryOperation, a * b, "Product: a * b",
		"Basic Arithmetic")
DEFINE_OPERATION_CLASS(Division, BinaryOperation, a / b, "Division: a / b",
		"Basic Arithmetic")

DEFINE_OPERATION_CLASS(Sine, UnaryOperation, std::sin(a), "Sine: sin(a)",
		"Trigonometric Functions")
DEFINE_OPERATION_CLASS(Cosine, UnaryOperation, std::cos(a), "Cosine: cos(a)",
		"Trigonometric Functions")
DEFINE_OPERATION_CLASS(Tangent, UnaryOperation, std::tan(a), "Tangent: tan(a)",
		"Trigonometric Functions")

DEFINE_OPERATION_CLASS(Arcsine, UnaryOperation, std::asin(a),
		"Arcsine: arcsin(a)", "Inverse Trigonometric Functions")
DEFINE_OPERATION_CLASS(Arccosine, UnaryOperation, std::acos(a),
		"Arccosine: arccos(a)", "Inverse Trigonometric Functions")
DEFINE_OPERATION_CLASS(Arctangent, UnaryOperation, std::atan(a),
		"Arctangent: arctan(a)", "Inverse Trigonometric Functions")

DEFINE_OPERATION_CLASS(Sinh, UnaryOperation, std::sinh(a),
		"Hyperbolic sine: sinh(a)", "Hyperbolic Functions")
DEFINE_OPERATION_CLASS(Cosh, UnaryOperation, std::cosh(a),
		"Hyperbolic cosine: cosh(a)", "Hyperbolic Functions")
DEFINE_OPERATION_CLASS(Tanh, UnaryOperation, std::tanh(a),
		"Hyperbolic tangent: tanh(a)", "Hyperbolic Functions")

DEFINE_OPERATION_CLASS(NaturalLog, UnaryOperation, std::log(a),
		"Natural log: ln(a)", "Logarithmic Functions")
DEFINE_OPERATION_CLASS(Log10, UnaryOperation, std::log10(a),
		"Base-10 log: log10(a)", "Logarithmic Functions")

DEFINE_OPERATION_CLASS(Absolute, UnaryOperation, std::abs(a),
		"Absolute value: |a|", "Miscellaneous")
DEFINE_OPERATION_CLASS(Power, BinaryOperation, std::pow(a, b), "Power: a^b",
		"Miscellaneous")
DEFINE_OPERATION_CLASS(Root, BinaryOperation, std::pow(a, 1/b), "Root: a^(1/b)",
		"Miscellaneous")
DEFINE_OPERATION_CLASS(Exp, UnaryOperation, std::exp(a), "Exponential: e^(a)",
		"Miscellaneous")
DEFINE_OPERATION_CLASS(Factorial, UnaryOperation, std::tgamma(a + 1),
		"Factorial: a!", "Miscellaneous")

class Calculator {
public:
	Calculator() {
		operations.emplace_back(std::make_unique<Sum>());
		operations.emplace_back(std::make_unique<Subtraction>());
		operations.emplace_back(std::make_unique<Product>());
		operations.emplace_back(std::make_unique<Division>());
		operations.emplace_back(std::make_unique<Absolute>());
		operations.emplace_back(std::make_unique<Power>());
		operations.emplace_back(std::make_unique<Root>());
		operations.emplace_back(std::make_unique<Exp>());
		operations.emplace_back(std::make_unique<NaturalLog>());
		operations.emplace_back(std::make_unique<Log>());
		operations.emplace_back(std::make_unique<Sine>());
		operations.emplace_back(std::make_unique<Cosine>());
		operations.emplace_back(std::make_unique<Tangent>());
		operations.emplace_back(std::make_unique<Arcsine>());
		operations.emplace_back(std::make_unique<Arccosine>());
		operations.emplace_back(std::make_unique<Arctangent>());
		operations.emplace_back(std::make_unique<Factorial>());
	}

	void Run() {
		double previousResult = 0;
		while (true) {
			try {
				int ch = GetUserChoice();
				if (ch < 0 || ch >= operations.size()) break;

				double a, b;
				std::cout << "Insert number:\na = ";
				GetValidNumber(input, a, previousResult);

				if (operations[ch] -> ArgumentsNumber() > 1) {
					std::cout << "Insert number:\nb = ";
					GetValidNumber(input, b, previousResult);
				}

				previousResult = operations[ch] -> Calculate(a, b);
				std::cout << "Result = " << previousResult << '\n';
			} catch (const std::invalid_argument& e) {
				std::cout << e.what() << '\n';
			}
		}
	}

private:
	std::vector<std::unique_ptr<Operation>> operations;
	std::string input;

	int GetUserChoice() const {
		std::cout << "Choose an operation:\n";
		for (size_t i = 0; i < operations.size(); ++i) {
			std::cout << i + 1 << ". " << operations[i] -> Name() << '\n';
		}
		std::cout << "Exit with any other key'\n";

		int ch;
		std::cin >> ch;
		return ch - 1;
	}
};

int main() {
	Calculator calc;
	calc.Run();
	std::cout << "Thanks for using.\nBye Bye." << std::endl;
}
