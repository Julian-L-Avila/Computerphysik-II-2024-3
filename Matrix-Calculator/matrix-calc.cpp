#include <iostream>
#include <stdexcept>
#include <cmath>

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

		double Determinant() {
			if (!IsSquare()) {
				throw std::invalid_argument("Determinant is only defined for square matrices.");
			}

			int determinant = 0;
			if (rows_ == 1 && cols_ == 1) {
				determinant = matrix_[0][0];
			} else if (rows_ == 2 && cols_ == 2) {
				determinant = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
			} else {
				for (int j = 0; j < cols_; j++) {
					determinant += matrix_[0][j] * Cofactor(0, j);
				}
			}
			return determinant;
		}


		Matrix CofactorMatrix(){
			Matrix cofactorMatrix(rows_, cols_);
			for(int i = 0; i < rows_; i++){
				for(int j = 0; j < cols_; j++){
					cofactorMatrix.matrix_[i][j]=Cofactor(i,j);
				}
			}
			return cofactorMatrix;
		}

		Matrix AdjointMatrix(){
			Matrix cofactorMatrix(rows_,cols_);
			cofactorMatrix=CofactorMatrix();

			return cofactorMatrix.Transpose();
		}

		Matrix InverseMatrix() {
			if (!IsSquare()) {
				throw std::invalid_argument("Inverse is only defined for square matrices.");
			}

			double determinant = Determinant();
			if (determinant == 0) {
				throw std::invalid_argument("Matrix is singular and cannot be inverted.");
			}

			Matrix adjointMatrix(cols_, rows_);
			adjointMatrix = AdjointMatrix();

			Matrix inverseMatrix(cols_, rows_);

			for (int i = 0; i < cols_; ++i) {
				for (int j = 0; j < rows_; ++j) {
					inverseMatrix.matrix_[i][j] = adjointMatrix.matrix_[i][j] / determinant;
				}
			}
			return inverseMatrix;
		}

	private:
		void CheckSameDimensions(const Matrix& other) const {
			if (rows_ != other.rows_ || cols_ != other.cols_) {
				throw std::invalid_argument("Matrices must have the same dimensions.");
			}
		}

		double Cofactor(int rowToRemove, int colToRemove) {
			Matrix submatrix(rows_ - 1, cols_ - 1);
			int submatrixRow = 0;
			int submatrixCol = 0;

			for (int i = 0; i < rows_; ++i) {
				if (i == rowToRemove) continue;
				submatrixCol = 0;
				for (int j = 0; j < cols_; ++j) {
					if (j == colToRemove) continue;
					submatrix.matrix_[submatrixRow][submatrixCol] = matrix_[i][j];
					++submatrixCol;
				}
				++submatrixRow;
			}

			return pow(-1, rowToRemove + colToRemove) * submatrix.Determinant();
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
				case 'r': std::cout<<matrix1.Trace()<<std::endl; break;
				case 'd': std::cout<<matrix1.Determinant()<<std::endl; break;
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

