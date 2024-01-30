/**
 * @file Dense.h
 * @author  Nitzan Rosen
 * @date 17 june 2020
 */
#ifndef EX4_DENSE_H
#define EX4_DENSE_H

// ------------------------------ includes ------------------------------
#include "Matrix.h"
#include "Activation.h"
// ------------------------------ class -----------------------------
/**
 * The class represents a layer,
 * and helps it to define and run the various layer operations on the network.
 */
class Dense
{
private:
    Matrix _weights;
    Matrix _bias;
    Activation _activation;

public:

    /**
     * constructor for Dense class
     * @param w
     * @param biasInp
     * @param activationType
     */
    Dense(const Matrix &w, const Matrix &biasInp, ActivationType activationType);

    /**
     * getter function-
     * Returns the weights of this layer
     * forbids modification
     * @return
     */
    const Matrix &getWeights() const;

    /**
     * getter function-
     * Returns the bias of this laye
     * forbids modification
     * @return
     */
    const Matrix &getBias() const;

    /**
     * Returns the activation function of this layer
     * forbids modification
     * @return
     */
    const Activation &getActivation() const;

    /**
     * Applies the layer on input and returns output matrix
     * @param matrix
     * @return
     */
    Matrix operator()(const Matrix &matrix);
};

#endif //EX4_DENSE_H
