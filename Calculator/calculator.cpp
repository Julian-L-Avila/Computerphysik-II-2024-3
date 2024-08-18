#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

bool GetValidNumber(std::string& input, double& a, double& previousResult) {
	while (true) {
		std::cin >> input;
		if (input == "ans") {
			a = previousResult;
			return true;
		} else {
			std::istringstream iss(input);
			if (iss >> a) {
				return true;
			}
		}
		std::cout << "Invalid input. Please enter a number or 'ans': ";
	}
}

class Operation {
public:
	virtual int ArgumentsNumber() const = 0;
	virtual double Calculate(double a, double b=0, double c=0) const = 0;
	virtual std::string Name() const = 0;
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

class Sum : public BinaryOperation {
public:
	double Calculate(double a, double b, double c=0) const override { return a + b; }
	std::string Name() const override { return "Sum: a+b"; }
};

class Subtraction : public BinaryOperation {
public:
	double Calculate(double a, double b, double c=0) const override { return a - b; }
	std::string Name() const override { return "Subtraction: a-b"; }
};

class Product : public BinaryOperation {
public:
	double Calculate(double a, double b, double c=0) const override { return a * b; }
	std::string Name() const override { return "Product: ab"; }
};

class Division : public BinaryOperation {
public:
	double Calculate(double a, double b, double c=0) const override { return a / b; }
	std::string Name() const override { return "Division: a/b"; }
};

class Absolute : public UnaryOperation {
public:
	double Calculate(double a, double b, double c=0) const override { return std::abs(a); }
	std::string Name() const override { return "Absolute value: |a|"; }
};

class Power: public BinaryOperation {
public:
	double Calculate(double a, double b, double c=0) const override { return std::pow(a, b); }
	std::string Name() const override { return "nth Power: a^b"; }
};

class Root: public BinaryOperation {
public:
	double Calculate(double a, double b, double c=0) const override { return std::pow(a, 1/b); }
	std::string Name() const override { return "nth Root: a^(1/b)"; }
};

class Exp: public UnaryOperation {
public:
	double Calculate(double a, double b=0, double c=0) const override { return std::exp(a); }
	std::string Name() const override { return "Exponential: e^(a)"; }
};

class NaturalLog: public UnaryOperation {
public:
	double Calculate(double a, double b=0, double c=0) const override { return std::log(a); }
	std::string Name() const override { return "Natural log: log(a)"; }
};

class Log: public BinaryOperation {
public:
	double Calculate(double base, double a, double c=0) const override {
		return std::log(a) / std::log(base);
	}
	std::string Name() const override { return "Log base a: log_a(b)"; }
};

class Sine: public UnaryOperation {
public:
	double Calculate(double a, double b=0, double c=0) const override { return std::sin(a); }
	std::string Name() const override { return "Sine: sin(a)"; }
};

class Cosine: public UnaryOperation {
public:
	double Calculate(double a, double b=0, double c=0) const override { return std::cos(a); }
	std::string Name() const override { return "Cosine: cos(a)"; }
};

class Tangent: public UnaryOperation {
public:
	double Calculate(double a, double b=0, double c=0) const override { return std::tan(a); }
	std::string Name() const override { return "Tangent: tan(a)"; }
};

class Factorial: public UnaryOperation {
public:
	double Calculate(double a, double b=0, double c=0) const override { return std::tgamma(a + 1); }
	std::string Name() const override { return "Factorial: a!"; }
};

class Calculator {
public:
	Calculator() {
		operations.push_back(std::make_unique<Sum>());
		operations.push_back(std::make_unique<Subtraction>());
		operations.push_back(std::make_unique<Product>());
		operations.push_back(std::make_unique<Division>());
		operations.push_back(std::make_unique<Absolute>());
		operations.push_back(std::make_unique<Power>());
		operations.push_back(std::make_unique<Root>());
		operations.push_back(std::make_unique<Exp>());
		operations.push_back(std::make_unique<NaturalLog>());
		operations.push_back(std::make_unique<Log>());
		operations.push_back(std::make_unique<Sine>());
		operations.push_back(std::make_unique<Cosine>());
		operations.push_back(std::make_unique<Tangent>());
		operations.push_back(std::make_unique<Factorial>());
	}

	void Menu() const {
		std::cout << "Choose an operation:\n";
		int optionNumber = 1;
		std::for_each(operations.begin(), operations.end(), [&](const auto& op) {
			std::cout << optionNumber++ << ". " << op -> Name() << '\n';
		});
		std::cout << "Exit with any other key\n";
	}

	void Run(int& ch, double& a, double& b, double& c) {
		double previousResult = 0;
		while (true) {
			std::string input;
			Menu();
			std::cin >> ch;

			if (ch > operations.size()) {
				throw std::invalid_argument("Invalid input");
			}

			std::cout << "Insert number:\na = ";
			GetValidNumber(input, a, previousResult);

			if (operations[ch - 1] -> ArgumentsNumber() > 1) {
				std::cout << "Insert number:\nb = ";
				GetValidNumber(input, b, previousResult);
			}

			previousResult = operations[ch - 1] -> Calculate(a, b, c);
			std::cout << "Result = " << previousResult << '\n';
		};
	}

private:
	std::vector<std::unique_ptr<Operation>> operations;
};

int main() {
	Calculator calc;
	int ch;
	double a, b, c;

	calc.Run(ch, a, b, c);
	std::cout << "Thanks for using.\nBye Bye." << std::endl;
}
