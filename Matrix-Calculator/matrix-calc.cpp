#include <iostream>
#include <stdexcept>

constexpr int kMax = 10;

class Matrix {
	public:
		Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
			if (rows <= 0 || cols <= 0 || rows > kMax || cols > kMax) {
				throw std::out_of_range("Invalid matrix dimensions.");
			}
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					matrix_[i][j] = 0;
				}
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

		bool IsSquare() const {
			return rows_ == cols_;
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
			CheckSameDimensions(other);
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
				}
			}
			return result;
		}

		Matrix Subtract(const Matrix& other) const {
			CheckSameDimensions(other);
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
				}
			}
			return result;
		}

		Matrix Multiply(const Matrix& other) const {
			if (cols_ != other.rows_) {
				throw std::invalid_argument("Invalid matrix dimensions for multiplication.");
			}

			Matrix result(rows_, other.cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < other.cols_; ++j) {
					for (int k = 0; k < cols_; ++k) {
						result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
					}
				}
			}
			return result;
		}

		Matrix MultiplyHadamard(const Matrix& other) const {
			CheckSameDimensions(other);
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] * other.matrix_[i][j];
				}
			}
			return result;
		}

		Matrix Transpose() const {
			Matrix result(cols_, rows_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[j][i] = matrix_[i][j];
				}
			}
			return result;
		}

		double Trace() const {
			if (!IsSquare()) {
				throw std::invalid_argument("Trace is only defined for square matrices.");
			}

			double trace = 0;
			for (int i = 0; i < rows_; ++i) {
				trace += matrix_[i][i];
			}
			return trace;
		}

	private:
		void CheckSameDimensions(const Matrix& other) const {
			if (rows_ != other.rows_ || cols_ != other.cols_) {
				throw std::invalid_argument("Matrices must have the same dimensions.");
			}
		}

		double matrix_[kMax][kMax];
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
		char DisplayMenu() const {
			std::cout << "Matrix Calculator\n"
				<< "a. Add\n"
				<< "s. Subtract\n"
				<< "m. Multiply\n"
				<< "h. Hadamard Multiply\n"
				<< "p. Scalar Product\n"
				<< "t. Transpose\n"
				<< "r. Trace\n"
				<< "q. Quit\n"
				<< "Choose an operation: ";
			char operation;
			std::cin >> operation;
			return operation;
		}

		void PerformOperation(char operation) {
			Matrix matrix1 = GetMatrixInput();
			switch (operation) {
				case 'a': matrix1.Add(GetMatrixInput()).Display(); break;
				case 's': matrix1.Subtract(GetMatrixInput()).Display(); break;
				case 'm': matrix1.Multiply(GetMatrixInput()).Display(); break;
				case 'h': matrix1.MultiplyHadamard(GetMatrixInput()).Display(); break;
				case 'p': {
										double scalar;
										std::cout << "Enter scalar: ";
										std::cin >> scalar;
										matrix1.ScalarProduct(scalar).Display();
										break;
									}
				case 't': matrix1.Transpose().Display(); break;
				case 'r': matrix1.Trace(); break;
				default: std::cout << "Invalid operation.\n"; break;
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

