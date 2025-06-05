/*
    HPC Project: Matrix Multiplication Library
    We will only be working with 2-D matrix!

    Compile: g++ -std=c++20 main.cpp -o matrix
*/

// ==================  include ================== //
#include <iostream>
#include <random>

// ==================  Matrix Function ================== //

void printMatrix(double** matrix, int rows, int cols)
{
    // Safty check for empty matric
    if (matrix == nullptr || rows == 0 || cols == 0)
    {
        std::cout << "Cannot print NULL matrix! \n";
        return;
    }

    // Just lopping through and printing the matrix
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

double** getMatrixSpace(int rows, int cols)
{

    // Allocating space for each of the rows
    double** matrix = (double**)malloc(rows * sizeof(double*));

    // safty checks
    if (matrix == nullptr)
    {
        std::cout << "Failed to allocate Memory for the rows!\n";
    }
    double* data = (double*)malloc(rows * cols * sizeof(double));

    if (data == nullptr)
    {
        std::cout << "Failed to allocate Memory for the Cols !\n";
    }

    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = &data[i * cols];
    }



    return matrix;
}

double** fillMatrixRandom(double** matrix, int rows, int cols)
{
    // Safty check for empty matric
    if (matrix == nullptr) {
        std::cout << "Cannot fill NULL matrix! \n";
        return nullptr;
    }

    // creating random double device to fill matrix with random numbers
    for (int i = 0; i < rows; ++i) {
        std::mt19937 rng(40 + i * 3);
        std::uniform_real_distribution<double> dist(-10.f, 10.f);
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dist(rng);
        }
    }

    return matrix;
}

// ==================  Matrix Multiplication Functions ================== //

double** normalMatrixMultiplication(double** A, int Arow, int Acol, double** B, int Brow, int Bcol)
{
    // This also has time complexity of O(n^3)!!!!

    if (A == nullptr || B == nullptr || Arow <= 0 || Acol <= 0 || Brow <= 0 || Bcol <= 0)
    {
        std::cout << "Not valid Matrices \n";
        return nullptr;
    }

    // Preform safty checks Acol == Brow ? must be true
    if (Acol != Brow)
    {
        std::cout << "Cannot preform multiplication, due to invalid dimenstions! \n";
        return nullptr;
    }

    // make space for the resulting matrix (using the dimensions)
    double** result = getMatrixSpace(Arow, Bcol);

    for (int i = 0; i < Arow; ++i)
    {
        for (int j = 0; j < Bcol; ++j)
        {
            double elementResult = 0.0;
            for (int k = 0; k < Acol; ++k)
            {
                //std::cout << A[i][k] << " *  " << B[k][j] << "\n";
                elementResult = elementResult + A[i][k] * B[k][j];
            }
            result[i][j] = elementResult;
        }
    }

    return result;
}

double** verticalStack(double** A, double** B, int size) {
    /*
        Given:
            A = 1 2     B = 5 6
                3 4 ,       7 8

        return:
            AB = 1 2
                 3 4
                 5 6
                 7 8
    */

    double** result = getMatrixSpace(2 * size, size);

    for (int i = 0; i < size; ++i) {
        memcpy(result[i], A[i], size * sizeof(double));
        memcpy(result[i + size], B[i], size * sizeof(double));
    }

    return result;
}

double** horizontalStack(double** A, double** B, int size) {
    double** result = getMatrixSpace(size, size * 2);

    for (int i = 0; i < size; ++i) {
        memcpy(result[i], A[i], size * sizeof(double));
        memcpy(result[i] + size, B[i], size * sizeof(double));
    }
    return result;
}

double** matrixAddition(double** A, double** B, int size) {
    double** result = getMatrixSpace(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
    return result;
}

double** matrixSubtraction(double** A, double** B, int size) {
    double** result = getMatrixSpace(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
    return result;
}

double*** splitMatrix(double** Matrix, int size) {
    /*
        Given:
        1,2,3,4
        1,2,3,4
        1,2,3,4
        1,2,3,4

        We will return:
        1,2 | 3,4
        1,2 | 3,4
        ---------
        1,2 | 3,4
        1,2 | 3,4

    */

    int half = size / 2;
    double*** result = (double***)malloc(4 * sizeof(double**));

    for (int i = 0; i < 4; ++i) {
        result[i] = getMatrixSpace(half, half);
    }

    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            result[0][i][j] = Matrix[i][j];
            result[1][i][j] = Matrix[i][j + half];
            result[2][i][j] = Matrix[i + half][j];
            result[3][i][j] = Matrix[i + half][j + half];
        }
    }
    return result;
}

// double** strassenAlgoNormal(double** A, double** B, int size) {

//     double** result = getMatrixSpace(size, size);
//     if (size <= 2) {
//         return normalMatrixMultiplication(A, size, size, B, size, size);
//     }

//     double*** matirxBlockA = splitMatrix(A, size);
//     double*** matirxBlockB = splitMatrix(B, size);

//     // recursive calls
//     /*
//             a | b          e | f
//        A =  - - -  * B =   - - -
//             c | d          g | h
//     */

//     double** t1 = matrixAddition(matirxBlockA[2], matirxBlockA[3], size / 2);
//     double** t2 = matrixSubtraction(matirxBlockA[3], matirxBlockA[1], size / 2);
//     double** t3 = matrixSubtraction(matirxBlockA[3], matirxBlockA[0], size / 2);
//     double** t4 = matrixSubtraction(matirxBlockB[3], matirxBlockB[1], size / 2);
//     double** t5 = matrixAddition(matirxBlockB[2], matirxBlockB[3], size / 2);
//     double** t6 = matrixSubtraction(matirxBlockB[3], matirxBlockB[2], size / 2);

//     double** M1 = strassenAlgoNormal(matirxBlockA[0], matirxBlockB[0], size / 2);
//     double** M2 = strassenAlgoNormal(matirxBlockA[2], matirxBlockB[2], size / 2);
//     double** M3 = strassenAlgoNormal(matirxBlockA[2], t4, size / 2);
//     double** M4 = strassenAlgoNormal(matirxBlockA[3], matirxBlockB[3], size / 2);
//     double** M5 = strassenAlgoNormal(t1, t5, size / 2);
//     double** M6 = strassenAlgoNormal(t2, t6, size / 2);
//     double** M7 = strassenAlgoNormal(t3, matirxBlockB[2], size / 2);

//     double** C11 = matrixAddition(M1, M2, size / 2);
//     double** C12 = matrixSubtraction(M5, M7, size / 2);
//     double** C21 = matrixAddition(M3, M6, size / 2);
//     double** C22 = matrixSubtraction(matrixAddition(M5, M6, size / 2), matrixSubtraction(M2, M4, size / 2), size / 2);

//     double** top = horizontalStack(C11, C12, size / 2);
//     double** bottom = horizontalStack(C21, C22, size / 2);


//     return verticalStack(top, bottom, size / 2);
// }

double** strassenAlgoNormal(double** A, double** B, int size) {
    if (size <= 2) {
        return normalMatrixMultiplication(A, size, size, B, size, size);
    }

    int newSize = size / 2;

    // Split A into A11, A12, A21, A22
    double*** blockA = splitMatrix(A, size);
    double*** blockB = splitMatrix(B, size);

    double** A11 = blockA[0];
    double** A12 = blockA[1];
    double** A21 = blockA[2];
    double** A22 = blockA[3];

    double** B11 = blockB[0];
    double** B12 = blockB[1];
    double** B21 = blockB[2];
    double** B22 = blockB[3];

    // Compute M1–M7 using Strassen's formula
    double** M1 = strassenAlgoNormal(matrixAddition(A11, A22, newSize),
        matrixAddition(B11, B22, newSize), newSize);

    double** M2 = strassenAlgoNormal(matrixAddition(A21, A22, newSize),
        B11, newSize);

    double** M3 = strassenAlgoNormal(A11,
        matrixSubtraction(B12, B22, newSize), newSize);

    double** M4 = strassenAlgoNormal(A22,
        matrixSubtraction(B21, B11, newSize), newSize);

    double** M5 = strassenAlgoNormal(matrixAddition(A11, A12, newSize),
        B22, newSize);

    double** M6 = strassenAlgoNormal(matrixSubtraction(A21, A11, newSize),
        matrixAddition(B11, B12, newSize), newSize);

    double** M7 = strassenAlgoNormal(matrixSubtraction(A12, A22, newSize),
        matrixAddition(B21, B22, newSize), newSize);

    // Compute result submatrices
    double** C11 = matrixAddition(matrixSubtraction(matrixAddition(M1, M4, newSize), M5, newSize), M7, newSize);
    double** C12 = matrixAddition(M3, M5, newSize);
    double** C21 = matrixAddition(M2, M4, newSize);
    double** C22 = matrixAddition(matrixSubtraction(matrixAddition(M1, M3, newSize), M2, newSize), M6, newSize);

    // Combine into result
    double** top = horizontalStack(C11, C12, newSize);
    double** bottom = horizontalStack(C21, C22, newSize);
    double** result = verticalStack(top, bottom, newSize);

    // Ideally: free all intermediate matrices to avoid memory leaks

    return result;
}


// ==================  Main Function ================== //
int main()
{
    int s = 4;

    double** A = getMatrixSpace(s, s);
    double** B = getMatrixSpace(s, s);

    fillMatrixRandom(A, s, s);
    fillMatrixRandom(B, s, s);

    printMatrix(A, s, s);
    std::cout << "\n";
    printMatrix(B, s, s);
    std::cout << "\n";

    double** C = strassenAlgoNormal(A, B, s);
    double** D = normalMatrixMultiplication(A, s, s, B, s, s);

    printMatrix(C, s, s);
    std::cout << "\n";
    printMatrix(D, s, s);


    return 0;
}