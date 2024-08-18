#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <memory>
#include <exception>
#include <cmath>
#include <algorithm>

class Operation {
public:
	const int argumentsNumber = 2;
	virtual double Calculate(double a, double b) = 0;
	virtual std::string Name() const = 0;

	virtual ~Operation() = default;
};

class Sum : public Operation {
public:
	double Calculate(double a, double b) override { return a + b; }
	std::string Name() const override { return "Sum: a + b"; }
};

class Subtraction : public Operation {
public:
	double Calculate(double a, double b) override { return a - b; }
	std::string Name() const override { return "Subtraction: a - b"; }
};

class Product : public Operation {
public:
	double Calculate(double a, double b) override { return a * b; }
	std::string Name() const override { return "Product: ab"; }
};

class Division : public Operation {
public:
	double Calculate(double a, double b) override { return a / b; }
	std::string Name() const override { return "Division: a / b"; }
};

class Power: public Operation {
public:
	double Calculate(double a, double b) override { return std::pow(a, b); }
	std::string Name() const override { return "nth Power: a ^ b"; }
};

class Root: public Operation {
public:
	double Calculate(double a, double b) override { return std::pow(a, 1/b); }
	std::string Name() const override { return "nth Root: a ^ (1/b)"; }
};

class Exp: public Operation {
public:
	const int argumentsNumber = 1;
	double Calculate(double a, double b) override { return std::exp(a); }
	std::string Name() const override { return "Exponential: e ^ (a)"; }
};

class NaturalLog: public Operation {
public:
	const int argumentsNumber = 1;
	double Calculate(double a, double b) override { return std::log(a); }
	std::string Name() const override { return "Natural log: log(a)"; }
};

class Log: public Operation {
public:
	double Calculate(double base, double a) override {
		return std::log(a) / std::log(base);
	}
	std::string Name() const override { return "Log base a: log_a(b)"; }
};

class Calculator {
public:
	Calculator() {
		operations.push_back(std::make_unique<Sum>());
		operations.push_back(std::make_unique<Subtraction>());
		operations.push_back(std::make_unique<Product>());
		operations.push_back(std::make_unique<Division>());
	}

	void Menu() const {
		std::cout << "Choose an operation:\n";
		int optionNumber = 1;
		std::for_each(operations.begin(), operations.end(), [&](const auto& op) {
			std::cout << optionNumber++ << ". " << op -> Name() << '\n';
		});
		std::cout << "0. Exit\n";
	}

	void Run(int ch, double a, double b) {
		if (ch < 1 || ch > operations.size()) {
			throw std::invalid_argument("Invalid input");
		}
		std::cout << "Result = " << operations[ch - 1] -> Calculate(a, b) << '\n';
	}

private:
	std::vector<std::unique_ptr<Operation>> operations;
};

int main() {
	Calculator calc;
	int ch;
	double a,b;

	while (true) {
		calc.Menu();
		std::cin >> ch;

		if (ch == 0) {
			break;
	}

	std::cout << "Insert two numbers:\n";
	std::cout << "a = ";
	std::cin >> a;
	std::cout << "b = ";
	std::cin >> b;

	try {
		calc.Run(ch, a, b);
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	}
	std::cout << "Thanks for using.\nBye Bye." << std::endl;
}
