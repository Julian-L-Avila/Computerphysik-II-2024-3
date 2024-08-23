#include <iostream>

class Opp {
	public:
		double a, b, c;
		double Result() {
			return c;
		}
		Opp(double a, double b, double c) : a(a), b(b), c(c) {}
};

double num1, num2;
int option;



int main() {
	std::cout << "Enter first number: " << '\n';
	std::cin >> num1;
	std::cout << "Enter second number: " << '\n';
	std::cin >> num2;

	system("clear");

	std::cout << "\n x = " << num1 << " and y = " << num2 << '\n';
	std::cout << "::::::::::::::::::::MENU::::::::::::::::::::" << '\n';
	std::cout << "\t 1. Addition" << '\n';
	std::cout << "\t 2. Subtraction" << '\n';
	std::cout << "\t 3. Multiplication" << '\n';
	std::cout << "\t 4. Division" << '\n';
	std::cout << "\t 5. Modulo" << '\n';
	std::cout << "\t 6. Exit" << '\n';

	std::cout << "Choose an option: " << std::endl;
	std::cin >> option;

Opp Add(num1, num2, num1 + num2);
Opp Sub(num1, num2, num1 - num2);
Opp Mul(num1, num2, num1 * num2);
Opp Div(num1, num2, num1 / num2);

	switch (option) {
		case 1:
			std::cout << num1 << " + " << num2 << " = " << Add.Result() << std::endl;
			break;
		case 2:
			std::cout << num1 << " - (" << num2 << ") = " << Sub.Result() << std::endl;
			break;
		case 3:
			std::cout << num1 << "(" << num2 << ") = " << Mul.Result() << std::endl;
			break;
		case 4:
			std::cout << num1 << "(" << num2 << ")^-1 = " << Div.Result() << std::endl;
			break;
		default:
			std::cout << "Bye Bye" << '\n';
			std::cout << "Made by Julian L. Avila" << std::endl;
			exit(2);
  }

	return 0;
}
