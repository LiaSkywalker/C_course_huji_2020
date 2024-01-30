/**
 * @file Matrix.h
 * @author  Nitzan Rosen
 * @date 17 june 2020
 */

#ifndef MATRIX_H
#define MATRIX_H
// ------------------------------ includes ------------------------------
#include <iostream>
// ------------------------------ methods -----------------------------
/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * Matrix class, that represent a Metrix object
 */
class Matrix
{
private:
    MatrixDims _dims;
    float *_matrix;

    /**
     * checks if the matrix row and col vars are positive.
     * if not- exit the program with error
     */
    void _isParamValid();

public:

    /**
     * constructor
     * Constructs Matrix rows × cols
     * Inits all elements to 0
     * @param rows
     * @param cols
     */
    Matrix(int rows, int cols);

    /**
     * defult constructor for Matrix class
     * Constructs 1×1 Matrix
     * Inits the single element to 0
     */
    Matrix();

    /**
     *copy Constructor
     * @param m
     */
    Matrix(const Matrix &m);

    /**
     * Destructor for Matrix objects
     */
    ~Matrix();

    /**
     * Getter: returns the amount of rows as int
     * @return
     */
    int getRows() const;

    /**
     * Getter: returns the amount of cols as int
     * @return
     */
    int getCols() const;


    /**
     * Transforms a matrix into a coloumn vector
     * Supports function calling concatenation
     * @return reference to matrix after addition
     */
    Matrix &vectorize();

    /**
     *Prints matrix elements, no return value.
     * prints space after each element (incl. last
     * element in the row)
     * prints newline after each row (incl. last row)
     */
    void plainPrint();

    /**
     * overload the Assignment operator(=)
     * @param b
     * @return reference to the Matrix that changed
     */
    Matrix &operator=(const Matrix &bMat);

    /**
     * overload operator of Scalar multiplication on the left
     * @param num
     * @param otherMat
     * @return matrix after multiplication
     */
    friend Matrix operator*(const float num, const Matrix &otherMat);


    /**
     * overload operator of Scalar multiplication on the right
     * @param num
     * @return matrix after multiplication
     */
    Matrix operator*(const float num) const;

    /**
    * multiplies two given matrices.
    * @return - return a new matrix which is a multiplication of given matrices.
    */
    Matrix operator*(const Matrix &otherMat) const;

    /**
     * overload Matrix addition operator
     * @param otherMat
     * @return
     */
    Matrix operator+(const Matrix &otherMat) const;

    /**
     * overload Brackets indexing for const instance
     * @param index
     * @return return the index’th element
     */
    float operator[](const int &index) const;

    /**
     * overload Brackets indexing for non-const instance
     * @param index
     * @return reference to the index’th element
     */
    float &operator[](const int &index);

    /**
     *overload output stream operator
     * @param os
     * @param m
     * @return output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix &m);

    /**
     * overload Matrix addition accumulation operator
     * @param b
     * @return reference to matrix after action
     */
    Matrix &operator+=(const Matrix &b);

    /**
     * overload Parenthesis indexing operator for const instance
     * @param i
     * @param j
     * @return the (i,j) element
     */
    float operator()(const int &i, const int &j) const;

    /**
     * overload Parenthesis indexing operator for non-const instance
     * @param i
     * @param j
     * @return reference to the (i,j) element
     */
    float &operator()(const int &i, const int &j);

    /**
     * overload the Input stream operator
     * @param is
     * @param m
     * @return input stream
     */
    friend std::istream &operator>>(std::istream &is, const Matrix &m);

};

#endif //MATRIX_H
