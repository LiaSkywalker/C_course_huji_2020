/**
 * @file Dense.c
 * @author  Nitzan Rosen
 * @date 17 june 2020
 * The class represents a layer,
 * and helps it to define and run the various layer operations on the network.
 */
// ------------------------------ includes ------------------------------
#include "Dense.h"
// -------------------------- const definitions -------------------------
// ---------------------- methods implementations-------------------------
/**
 * Inits a new layer with given parameters
 * note: c’tor accepts 2 matrices and activation type
 * @param w
 * @param bias
 * @param activationType
 */
Dense::Dense(const Matrix &w, const Matrix &biasInp, ActivationType activationType) :
        _weights(w), _bias(biasInp),
        _activation(Activation(activationType))
{

}

/**
 * getter function-
 * Returns the weights of this layer
 * forbids modification
 * @return
 */
const Matrix &Dense::getWeights() const
{
    return _weights;
}

/**
 * getter function-
 * Returns the bias of this laye
 * forbids modification
 * @return
 */
const Matrix &Dense::getBias() const
{
    return _bias;
}

/**
     * Returns the activation function of this layer
     * forbids modification
     * @return
     */
const Activation &Dense::getActivation() const
{
    return _activation;
}

/**
 * Applies the layer on input and returns output matrix
 * Matrix output = layer(input)
 * @param mat
 * @return
 */
Matrix Dense::operator()(const Matrix &matrix)
{
    Matrix result = Matrix(matrix);
    result = (_weights * result) + _bias; // W1*x +b1
    result = _activation(result);  // Relu/Softmax(W1*x +b1)
    return result;
}

