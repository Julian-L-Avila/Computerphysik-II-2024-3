#include <iostream>

constexpr int kMax = 100;

void inputMatrix(int matrix[kMax][kMax], int& row, int& col) {
  std::cout << "Enter elements:\n";
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      std::cin >> matrix[i][j];
    }
  }
}

void displayMatrix(int matrix[kMax][kMax], int& row, int& col) {
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void addMatrices(int a[kMax][kMax], int b[kMax][kMax], int result[kMax][kMax],
                 int& row, int& col) {
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      result[i][j] = a[i][j] + b[i][j];
    }
  }
}

void subtractMatrices(int a[kMax][kMax], int b[kMax][kMax],
                      int result[kMax][kMax], int& row, int& col) {
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      result[i][j] = a[i][j] - b[i][j];
    }
  }
}

void multiplyMatrices(int a[kMax][kMax], int b[kMax][kMax],
                      int result[kMax][kMax], int& row1, int& col1, int& col2) {
  for (int i = 0; i < row1; ++i) {
    for (int j = 0; j < col2; ++j) {
      result[i][j] = 0;
      for (int k = 0; k < col1; ++k) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

int main() {
  int a[kMax][kMax], b[kMax][kMax], result[kMax][kMax];
  int row1, col1, row2, col2;
  int choice;

  std::cout << "Enter rows and columns for first matrix: ";
  std::cin >> row1 >> col1;
  std::cout << "Enter rows and columns for second matrix: ";
  std::cin >> row2 >> col2;

  std::cout << "Enter elements of first matrix:\n";
  inputMatrix(a, row1, col1);

  std::cout << "Enter elements of second matrix:\n";
  inputMatrix(b, row2, col2);

  std::cout << "Choose operation:\n1. Addition\n2. Subtraction\n3. Multiplication\n";
  std::cin >> choice;

  switch (choice) {
    case 1:
      if (row1 == row2 && col1 == col2) {
        addMatrices(a, b, result, row1, col1);
        std::cout << "Resultant Matrix:\n";
        displayMatrix(result, row1, col1);
      } else {
        std::cerr << "Matrices must have the same dimensions for addition.\n";
      }
      break;
    case 2:
      if (row1 == row2 && col1 == col2) {
        subtractMatrices(a, b, result, row1, col1);
        std::cout << "Resultant Matrix:\n";
        displayMatrix(result, row1, col1);
      } else {
        std::cerr << "Matrices must have the same dimensions for subtraction.\n";
      }
      break;
    case 3:
      if (col1 != row2) {
        std::cerr << "Matrices cannot be multiplied (incompatible dimensions).\n";
        return 1;
      }
      multiplyMatrices(a, b, result, row1, col1, col2);
      std::cout << "Resultant Matrix:\n";
      displayMatrix(result, row1, col2);
      break;
    default:
      std::cerr << "Invalid choice!\n";
  }

  return 0;
}
