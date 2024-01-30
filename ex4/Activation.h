/**
 * @file Activation.h
 * @author  Nitzan Rosen
 * @date 17 june 2020
 */
#ifndef ACTIVATION_H
#define ACTIVATION_H
// ------------------------------ includes ------------------------------
#include "Matrix.h"
// ------------------------------ class -----------------------------
/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * This class will use us to define and activate the activation functions
 * The activation function runs on the linear transformation result and the offset connection,
 * And returns the final result of the layer
 */
class Activation
{
private:
    ActivationType _activation;

    /**
     * _relu activation function
     * The function operates on a vector it operates on each coordinate separately
     * @param inputMat
     * @return
     */
    Matrix _relu(const Matrix &inputMat);

    /**
     * _softmax activation function
     * This function normalizes the result to probabilities
     * @param inputMat
     * @return
     */
    Matrix _softmax(const Matrix &inputMat);

public:
    /**
     * constructor for Activation class
     * @param actType
     */
    Activation(ActivationType actType);

    /**
     * getter function-
     * Returns this activation’s type(Relu/Softmax)
     * @return
     */
    ActivationType getActivationType() const;

    /**
     * Applies activation function on input.
     * @param m
     * @return
     */
    Matrix operator()(const Matrix &m);
};

#endif //ACTIVATION_H
