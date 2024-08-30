#include <iostream>
#include <stdexcept>

constexpr int kMax = 10;

class Matrix {
	public:
		Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
			if (rows <= 0 || cols <= 0 || rows > kMax || cols > kMax) {
				throw std::out_of_range("Invalid matrix dimensions.");
			}
		}

		void Input() {
			std::cout << "Enter elements (" << rows_ << "x" << cols_ << "):\n";
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					std::cin >> matrix_[i][j];
				}
			}
		}

		void Display() const {
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					std::cout << matrix_[i][j] << " ";
				}
				std::cout << std::endl;
			}
		}

		bool HaveSameDimensions(const Matrix& other) const {
			return rows_ == other.rows_ && cols_ == other.cols_;
		}

		bool CanMultiply(const Matrix& other) const {
			return cols_ == other.rows_;
		}

		Matrix ScalarProduct(double scalar) const {
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = scalar * matrix_[i][j];
				}
			}
			return result;
		}

		Matrix Add(const Matrix& other) const {
			if (!HaveSameDimensions(other)) {
				throw std::invalid_argument("Matrices must have the same dimensions for addition.");
			}

			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
				}
			}
			return result;
		}

		Matrix Subtract(const Matrix& other) const {
			if (!HaveSameDimensions(other)) {
				throw std::invalid_argument("Matrices must have the same dimensions for subtraction.");
			}

			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
				}
			}
			return result;
		}

		Matrix Multiply(const Matrix& other) const {
			if (!CanMultiply(other)) {
				throw std::invalid_argument("Invalid matrix dimensions for multiplication.");
			}

			Matrix result(rows_, other.cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < other.cols_; ++j) {
					result.matrix_[i][j] = 0;
					for (int k = 0; k < cols_; ++k) {
						result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
					}
				}
			}
			return result;
		}

		Matrix MultiplyHadamard(const Matrix& other) const {
			if (!HaveSameDimensions(other)) {
				throw std::invalid_argument("Matrices must have the same dimensions for Hadamard product.");
			}

			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] * other.matrix_[i][j];
				}
			}
			return result;
		}

		Matrix Transpose() const {
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[j][i];
				}
			}
			return result;
		}

	private:
		int matrix_[kMax][kMax];
		int rows_, cols_;
};

class MatrixCalculator {
	public:
		void Run() {
			char operation;
			while ((operation = DisplayMenu()) != 'q') {
				PerformOperation(operation);
			}
		}
	private:
		char DisplayMenu() {
			std::cout << "Matrix Calculator\n"
				<< "a. Add\n"
				<< "s. Subtract\n"
				<< "m. Multiply\n"
				<< "h. Hadamard Multiply\n"
				<< "p. Scalar Product\n"
				<< "q. Quit\n"
				<< "Choose an operation: ";
		char operation;
		std::cin >> operation;
		return operation;
		}

		void PerformOperation(char operation) {
			Matrix matrix1 = GetMatrixInput();
			Matrix result = HandleOperation(operation, matrix1);
			result.Display();
		}

		Matrix HandleOperation(char operation, Matrix& matrix1) {
			switch (operation) {
				case 'a':
					return matrix1.Add(GetMatrixInput());
				case 's':
					return matrix1.Subtract(GetMatrixInput());
				case 'm':
					return matrix1.Multiply(GetMatrixInput());
				case 'h':
					return matrix1.MultiplyHadamard(GetMatrixInput());
				case 'p': {
					double scalar;
					std::cout << "Enter scalar: ";
					std::cin >> scalar;
					return matrix1.ScalarProduct(scalar);
				}
				default:
					std::cout << "Invalid operation.\n";
					return matrix1;
			}
		}

		Matrix GetMatrixInput() {
			int rows, cols;
			std::cout << "Enter matrix dimensions (rows cols): ";
			std::cin >> rows >> cols;
			Matrix matrix(rows, cols);
			matrix.Input();
			return matrix;
		}
};

int main() {
	MatrixCalculator calculator;
	calculator.Run();
	return 0;
}

