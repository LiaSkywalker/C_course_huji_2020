/**
 * @file MlpNetwork.h
 * @author  Nitzan Rosen
 * @date 17 june 2020
 */
#ifndef MLPNETWORK_H
#define MLPNETWORK_H

// ------------------------------ includes ------------------------------
#include "Matrix.h"
#include "Dense.h"
#include "Digit.h"
// -------------------------- const definitions -------------------------
#define MLP_SIZE 4
const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
                                  {64,  128},
                                  {20,  64},
                                  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
                               {64,  1},
                               {20,  1},
                               {10,  1}};

// ------------------------------ class -----------------------------
/**
 * This class will use us to arrange the different layers into a network structure
 * and enable the input of the network and the corresponding output
 */
class MlpNetwork
{
private:
    Dense _layer1;
    Dense _layer2;
    Dense _layer3;
    Dense _layer4;
public:

    /**
     * constructor for MlpNetwork class
     * @param weights
     * @param biases
     */
     MlpNetwork(Matrix weights[], Matrix biases[]);

    /**
     * operator that applies the entire network on input
     * @param mat
     * @return
     */
     Digit operator()(const Matrix &mat);
};

#endif // MLPNETWORK_H
