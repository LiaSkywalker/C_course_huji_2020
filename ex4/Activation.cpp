/**
 * @file Activation.c
 * @author  Nitzan Rosen
 * @date 17 june 2020
 * In this class we will define the operation of the activation function
 */
// ------------------------------ includes ------------------------------
#include "Activation.h"
#include <cmath>

// ------------------------------ methods -----------------------------
/**
 * Constructor
 * Accepts activation type (Relu/Softmax)
 * and defines this instace’s activation accordingly
 * @param actType
 */
Activation::Activation(ActivationType actType)
{
    if (actType == Relu)
    {
        _activation = Relu;
    }
    else if (actType == Softmax)
    {
        _activation = Softmax;
    }
}

/**
 * getter function-
 * Returns this activation’s type(Relu/Softmax)
 * @return
 */
ActivationType Activation::getActivationType() const
{
    return this->_activation;
}

/**
 * Applies activation function on input.
 * @param m
 * @return
 */
Matrix Activation::operator()(const Matrix &m)
{
    if (_activation == Relu)
    {
        return this->_relu(m);
    }
    else
    {
        return this->_softmax(m);
    }
}

/**
 * _relu activation function
 * The function operates on a vector it operates on each coordinate separately
 * @param inputMat
 * @return
 */
Matrix Activation::_relu(const Matrix &inputMat)
{
    Matrix newMat = Matrix(inputMat);
    for (int i = 0; i < newMat.getRows() * newMat.getCols(); ++i)
    {
        if (newMat[i] < 0)
        {
            newMat[i] = 0;
        }
    }
    return newMat;
}

/**
 * _softmax activation function
 * This function normalizes the result to probabilities
 * @param inputMat
 * @return
 */
Matrix Activation::_softmax(const Matrix &inputMat)
{
    Matrix newMat = Matrix(inputMat);
    float normallize = 0;
    for (int k = 0; k < inputMat.getRows(); ++k)
    {
        newMat[k] = std::exp(newMat[k]);
        normallize += newMat[k];
    }
    newMat = (1 / normallize) * newMat;

    return newMat;
}
