/**
 * @file MlpNetwork.c
 * @author  Nitzan Rosen
 * @date 17 june 2020
 * @brief This class will use us to arrange the different layers into a network structure
 * and enable the input of the network and the corresponding output.
 */
// ------------------------------ includes ------------------------------
#include "MlpNetwork.h"
#include "Matrix.h"
#include "Dense.h"
// -------------------------- const definitions -------------------------
#define INIT_VALUE 0
#define FINAL_VECTOR_SIZE 10
// ------------------------------ methods -----------------------------
/**
 * constructor for class MlpNetwork
 * @param weights
 * @param biases
 */
MlpNetwork::MlpNetwork(Matrix *weights, Matrix *biases) :
        _layer1(Dense(weights[0], biases[0], Relu)),
        _layer2(Dense(weights[1], biases[1], Relu)),
        _layer3(Dense(weights[2], biases[2], Relu)),
        _layer4(Dense(weights[3], biases[3], Softmax))
{
}

/**
     * operator that applies the entire network on input
     * @param mat - input matrix
     * @return
     */
Digit MlpNetwork::operator()(const Matrix &inMatrix)
{
    Digit outputDigit;
    outputDigit.probability = INIT_VALUE;
    outputDigit.value = INIT_VALUE;

    Matrix currentMat = Matrix(inMatrix);
    currentMat = _layer1(currentMat);
    currentMat = _layer2(currentMat);
    currentMat = _layer3(currentMat);
    currentMat = _layer4(currentMat); //currentMat is vector in length of 10

    for (int i = 0; i < FINAL_VECTOR_SIZE; ++i)
    {
        if (currentMat[i] > outputDigit.probability)
        {
            outputDigit.probability = currentMat[i];
            outputDigit.value = i;
        }
    }
    return outputDigit;
}
