#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>

class Matrix {
	public:
		Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
			if (rows <= 0 || cols <= 0) {
				throw std::out_of_range("Invalid matrix dimensions.");
			}
			matrix_.reserve(rows);
			for (int i = 0; i < rows; ++i) {
				matrix_.emplace_back(cols, 0.0);
			}
			matrix_.resize(rows, std::vector<double>(cols, 0));
		}

		void Input() {
			std::cout << "Enter elements for matrix: " << '\n';
			for (std::vector<double>& row : matrix_) {
				for (double& elem: row) {
					std::cin >> elem;
				}
			}
		}

		void Display() const {
			for (const std::vector<double>& row : matrix_) {
				for (const double& elem : row) {
					std::cout << elem << " ";
				}
				std::cout << '\n';
			}
		}

		bool IsSquare() const { return rows_ == cols_; }

		// Overload * operator for scalar multiplication
		Matrix operator * (double scalar) const {
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = scalar * matrix_[i][j];
				}
			}
			return result;
		}

		// Overload + operator for matrix addition
		Matrix operator + (const Matrix& other) const {
			CheckSameDimensions(other);
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
				}
			}
			return result;
		}

		// Overload - operator for matrix subtraction
		Matrix operator - (const Matrix& other) const {
			CheckSameDimensions(other);
			Matrix result(rows_, cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < cols_; ++j) {
					result.matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
				}
			}
			return result;
		}

		// Overload * operator for matrix multiplication
		Matrix operator * (const Matrix& other) const {
			if (cols_ != other.rows_) {
				throw std::invalid_argument("Invalid matrix dimensions for multiplication.");
			}

			Matrix result(rows_, other.cols_);
			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < other.cols_; ++j) {
					double sum = 0;
					for (int k = 0; k < cols_; ++k) {
						sum += matrix_[i][k] * other.matrix_[k][j];
					}
					result.matrix_[i][j] = sum;
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

		double Determinant() const {
			if (!IsSquare()) {
				throw std::invalid_argument("Determinant is only defined for square matrices.");
			}

			if (rows_ == 1 ) return matrix_[0][0];
			if (rows_ == 2 ) {
				return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
			}
			double det = 0;
			for (int j = 0; j < cols_; j++) det += matrix_[0][j] * Cofactor(0, j);
			return det;
		}


		Matrix CofactorMatrix() const {
			Matrix cofactorMatrix(rows_, cols_);
			for(int i = 0; i < rows_; i++){
				for(int j = 0; j < cols_; j++){
					cofactorMatrix.matrix_[i][j] = Cofactor(i,j);
				}
			}
			return cofactorMatrix;
		}

		Matrix AdjointMatrix() const {
			return CofactorMatrix().Transpose();
		}

		Matrix InverseMatrix() const {
			double det = Determinant();
			if (det == 0) {
				throw std::invalid_argument("Matrix is singular and cannot be inverted.");
			}
			return AdjointMatrix() * (1.0 / det);
		}

	private:
		void CheckSameDimensions(const Matrix& other) const {
			if (rows_ != other.rows_ || cols_ != other.cols_) {
				throw std::invalid_argument("Matrices must have the same dimensions.");
			}
		}

		double Cofactor(int rowToRemove, int colToRemove) const {
			Matrix submatrix(rows_ - 1, cols_ - 1);
			for (int i = 0, sub_i = 0; i < rows_; ++i) {
				if (i == rowToRemove) continue;
				for (int j = 0, sub_j = 0; j < cols_; ++j) {
					if (j == colToRemove) continue;
					submatrix.matrix_[sub_i][sub_j] = matrix_[i][j];
					++sub_j;
				}
				++sub_i;
			}

			return pow(-1, rowToRemove + colToRemove) * submatrix.Determinant();
		}

		std::vector<std::vector<double>> matrix_;
		int rows_, cols_;
};

Matrix operator * (double scalar, const Matrix& matrix) {
	return matrix * scalar;
}

class MatrixCalculator {
	public:
		void Run() {
			while (true) {
				char operation = DisplayMenu();
				if (operation == 'q') break;
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
				<< "d. Determinant\n"
				<< "c. Cofactor Matrix\n"
				<< "j. Adjoint Matrix\n"
				<< "i. Inverse Matrix\n"
				<< "q. Quit\n"
				<< "Choose an operation: ";
			char operation;
			std::cin >> operation;
			return operation;
		}

		void PerformOperation(char operation) {
			Matrix matrix1 = GetMatrixInput();
			switch (operation) {
				case 'a': (matrix1 + GetMatrixInput()).Display(); break;
				case 's': (matrix1 - GetMatrixInput()).Display(); break;
				case 'm': (matrix1 * GetMatrixInput()).Display(); break;
				case 'h': matrix1.MultiplyHadamard(GetMatrixInput()).Display(); break;
				case 'p': {
					double scalar;
					std::cout << "Enter scalar: ";
					std::cin >> scalar;
					(matrix1 * scalar).Display();
					break;
				}
				case 't': matrix1.Transpose().Display(); break;
				case 'r': std::cout << matrix1.Trace() << '\n'; break;
				case 'd': std::cout << matrix1.Determinant() << '\n'; break;
				case 'c': matrix1.CofactorMatrix().Display(); break;
				case 'j': matrix1.AdjointMatrix().Display(); break;
				case 'i': matrix1.InverseMatrix().Display(); break;
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

