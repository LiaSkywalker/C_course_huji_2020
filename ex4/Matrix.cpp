/**
 * @file Matrix.c
 * @author  Nitzan Rosen
 * @date 17 june 2020
 * @brief Implement here the matrices and vectors needed to run the program
 */

// ------------------------------ includes ------------------------------
#include "Matrix.h"
#include <string>
// -------------------------- const definitions -------------------------
#define OUT_OF_RANG_MSG "Error: index out of array range"
#define EOF_ERR "Error: end of file"
#define FILE_ERR "Error: while reading file"
#define ALLOCATION_FAILED "Error: allocation failed"
#define DIM_ERR "Error: dimensions of matrices do not match"
#define INVALID_DIMS "Error: given input has negative number of rows or columns"
#define ADDITION " in addition action"
#define MULTIPLICATION " in multiplication action"
// ----------------------------- methods --------------------------------

/**
 * checks if the matrix row and col vars are positive.
 */
void Matrix::_isParamValid()
{
    if (_dims.rows <= 0 || _dims.cols <= 0)
    {
        std::cerr << INVALID_DIMS << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Constructor
 * Constructs Matrix rows × cols
 * Inits all elements to 0
 * @param rows
 * @param cols
 */
Matrix::Matrix(int rows, int cols) :
        _dims{rows, cols}
{
    this->_isParamValid();
    _matrix = new float[rows * cols];
    if (_matrix == nullptr)
    {
        std::cerr << ALLOCATION_FAILED << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < _dims.cols * _dims.rows; ++i)
    {
        _matrix[i] = 0;
    }
}

/**
 * Default Constructor
 * Constructs 1×1 Matrix
 * Inits the single element to 0
 */
Matrix::Matrix() :
        Matrix(1, 1)
{
    _matrix[0] = 0;
}

/**
 *copy Constructor
 * @param m
 */
Matrix::Matrix(const Matrix &m) :
        Matrix(m._dims.rows, m._dims.cols)
{
    for (int i = 0; i < _dims.cols * _dims.rows; ++i)
    {
        _matrix[i] = m._matrix[i];
    }
}

/**
 * Destructor for Matrix objects
 */
Matrix::~Matrix()
{
    delete[] _matrix;
}

/**
 * Getter: returns the amount of rows as int
 * @return the matrix rows
 */
int Matrix::getRows() const
{
    return _dims.rows;
}

/**
 * Getter: returns the amount of cols as int
 * @return the matrix cols
 */
int Matrix::getCols() const
{
    return _dims.cols;
}

/**
 * Prints matrix elements, no return value.
 * prints space after each element (incl. last
 * element in the row)
 * prints newline after each row (incl. last row)
 */
void Matrix::plainPrint()
{
    for (int i = 0; i < _dims.rows; ++i)
    {
        for (int j = 0; j < _dims.cols; ++j)
        {
            std::cout << (_matrix[i * _dims.cols + j]) << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * overload the Assignment operator(=)
 * @param b
 * @return reference to the Matrix that changed
 */
Matrix &Matrix::operator=(const Matrix &bMat)
{
    // self-assignment guard
    if (this == &bMat)
    {
        return *this;
    }

    //copy b dims
    this->_dims.cols = bMat._dims.cols;
    this->_dims.rows = bMat._dims.rows;

    delete[] this->_matrix;
    _matrix = new float[_dims.rows * _dims.cols];

    //copy values
    for (int i = 0; i < _dims.cols * _dims.rows; ++i)
    {
        this->_matrix[i] = bMat._matrix[i];
    }

    // return the existing object so we can chain this operator
    return *this;
}

/**
 * overload Matrix addition operator
 * @param otherMat
 * @return
 */
Matrix Matrix::operator+(const Matrix &otherMat) const
{
    if (_dims.rows != otherMat.getRows() || _dims.cols != otherMat.getCols())
    {
        std::cerr << DIM_ERR << ADDITION << std::endl;
        exit(EXIT_FAILURE);
    }
    Matrix result = Matrix(_dims.rows, _dims.cols);
    result += *this;
    result += otherMat;
    return result;
}

/**
 *overload output stream operator
 * @param os
 * @param m
 * @return output stream
 */
std::ostream &operator<<(std::ostream &os, const Matrix &m)
{
    for (int i = 0; i < m._dims.rows; ++i)
    {
        for (int j = 0; j < m._dims.cols; ++j)
        {
            if (m[i * m._dims.cols + j] <= 0.1)
            {
                os << "  ";
            }
            else
            {
                os << "**";
            }
        }
        os << std::endl;
    }
    return os;
}

/**
 * Transforms a matrix into a coloumn vector
 * Supports function calling concatenation
 * @return reference to matrix after addition
 */
Matrix &Matrix::vectorize()
{
    _dims.rows = _dims.cols * _dims.rows;
    _dims.cols = 1;
    return *this;
}

/**
 * overload operator of Scalar multiplication on the left
 * @param num
 * @param otherMat
 * @return matrix after multiplication
 */
Matrix operator*(const float num, const Matrix &otherMat)
{
    Matrix newMetrix = Matrix(otherMat);
    for (int i = 0; i < newMetrix._dims.rows * newMetrix._dims.cols; ++i)
    {
        newMetrix._matrix[i] *= num;
    }
    return newMetrix;
}

/**
 * overload operator of Scalar multiplication on the right
 * @param num
 * @return matrix after multiplication
 */
Matrix Matrix::operator*(const float num) const
{
    Matrix newMetrix = Matrix(*this);
    for (int i = 0; i < newMetrix._dims.rows * newMetrix._dims.cols; ++i)
    {
        newMetrix._matrix[i] *= num;
    }
    return newMetrix;
}

/**
 * overload Brackets indexing for const instance
 * @param index
 * @return return the index’th element
 */
float Matrix::operator[](const int &index) const
{
    if (0 > index || index > _dims.cols * _dims.rows)
    {
        std::cerr << OUT_OF_RANG_MSG << std::endl;
    }
    return _matrix[index];
}

/**
 * overload Brackets indexing for non-const instance
 * @param index
 * @return reference to the index’th element
 */
float &Matrix::operator[](const int &index)
{
    if (0 > index || index > _dims.cols * _dims.rows)
    {
        std::cerr << OUT_OF_RANG_MSG << std::endl;
    }

    return _matrix[index];
}

/**
 * overload Matrix addition accumulation operator
 * @param b
 * @return reference to matrix after action
 */
Matrix &Matrix::operator+=(const Matrix &b)
{
    if (_dims.rows != b._dims.rows || _dims.cols != b._dims.cols)
    {
        std::cerr << OUT_OF_RANG_MSG << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < _dims.rows * _dims.cols; ++i)
    {
        _matrix[i] += b._matrix[i];
    }
    return *this;
}

/**
 * overload Parenthesis indexing operator for const instance
 * @param i
 * @param j
 * @return the (i,j) element
 */
float Matrix::operator()(const int &i, const int &j) const
{
    if (0 > i || 0 > j || j > _dims.cols || i > _dims.rows)
    {
        std::cerr << OUT_OF_RANG_MSG << std::endl;
    }
    return _matrix[i * _dims.cols + j];
}

/**
 * overload Parenthesis indexing operator for non-const instance
 * @param i
 * @param j
 * @return reference to the (i,j) element
 */
float &Matrix::operator()(const int &i, const int &j)
{
    if (0 > i || 0 > j || j > _dims.cols || i > _dims.rows)
    {
        std::cerr << OUT_OF_RANG_MSG << std::endl;
    }
    return this->_matrix[i * _dims.cols + j];
}

/**
 * overload the Input stream operator
 * @param is
 * @param m
 * @return input stream
 */
std::istream &operator>>(std::istream &is, const Matrix &m)
{
    for (int i = 0; i < m._dims.rows; ++i)
    {
        for (int j = 0; j < m._dims.cols; ++j)
        {
            if (!is.good())
            {
                std::cerr << FILE_ERR << std::endl;
                exit(EXIT_FAILURE);
            }
            is.read((char *)(&(m._matrix[i * m._dims.cols + j])), 4);
        }
    }
    if (is.peek() != EOF)
    {
        std::cerr << EOF_ERR << std::endl;
        exit(EXIT_FAILURE);
    }
    return is;
}

/**
* multiplies two given matrices.
* @return - return a new matrix which is a multiplication of given matrices.
*/
Matrix Matrix::operator*(const Matrix &otherMat) const
{
    if (_dims.cols != otherMat.getRows())
    {
        std::cerr << DIM_ERR << MULTIPLICATION << std::endl;
        exit(EXIT_FAILURE);
    }

    Matrix result = Matrix(_dims.rows, otherMat.getCols()); //matrix to return.
    for (int i = 0; i < result.getRows(); ++i)
    {
        for (int j = 0; j < result.getCols(); ++j)
        {
            for (int k = 0; k < _dims.cols; ++k)
            {
                result(i, j) += ((*this)(i, k) * otherMat(k, j));
            }
        }
    }
    return result;
}


