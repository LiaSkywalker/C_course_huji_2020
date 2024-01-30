// implementation of Structs.h
//we will do this part on vectors
// ------------------------------ includes ------------------------------
#include "RBTree.h"
#include "Structs.h"
#include <stdlib.h>
#include <string.h>

// -------------------------- const definitions -------------------------
#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)
#define BACKSLASH_N "\n"

// ----------------------- functions implementation -----------------------

/**
 * return the minimal int from the given input
 * @param a
 * @param b
 * @return
 */
int min(int a, int b)
{
    if (a >= b)
    {
        return b;
    }
    else
    {
        return a;
    }
}

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    char *stringA = (char *)a;
    char *stringB = (char *)b;
    size_t lenA = strlen(stringA);
    size_t lenB = strlen(stringB);
    size_t min = lenA > lenB ? lenB : lenA;
    return strcmp(a, b);
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (strcat(pConcatenated, word) == NULL)
    {
        return EXIT_FAILURE;
    }
    if (strcat(pConcatenated, BACKSLASH_N) == NULL)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    free(s);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{


    Vector *aVec = (Vector *)a;
    Vector *bVec = (Vector *)b;
    int minLen = min(aVec->len, bVec->len);
    for (int i = 0; i < minLen; i++)
    {
        double currentA = aVec->vector[i];
        double currentB = bVec->vector[i];
        if (currentA > currentB)
        {
            return GREATER;
        }
        else if (currentA < currentB)
        {
            return LESS;
        }
        else if (currentA == currentB)
        {
            return EQUAL;
        }
    }
    //they equal until one is ends-the shorter is smaller
    int lenA = aVec->len;
    int lenB = bVec->len;

    if (lenA > lenB)
    {
        return GREATER;
    }
    if (lenA < lenB)
    {
        return LESS;
    }
    return EQUAL; //the last option- they are totally equal
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    Vector *myVector = (Vector *)pVector;
    free(myVector->vector);
    free(myVector);
}

/**
 * helper function- calculate norm of given vector pointer
 * @param pVector
 * @return
 */
double calcNorm(const void *pVector)
{
    Vector *myVec = (Vector *)pVector;
    double myNorm = 0;
    for (int i = 0; i < myVec->len; i++)
    {
        double elemToAdd = myVec->vector[i] * myVec->vector[i];
        myNorm += elemToAdd;
    }
    return myNorm;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector that will hold a copy of the data of pVector.
 * @return 1 on success, 0 on failure (if pVector == NULL || pMaxVector==NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{

    if (pVector == NULL || pMaxVector == NULL)
    {
        return EXIT_FAILURE;
    }
    double norm1 = calcNorm(pVector);
    double norm2 = calcNorm(pMaxVector);
    Vector *myVec1 = (Vector *)pVector;
    Vector *myVec2 = (Vector *)pMaxVector;

    if (norm1 > norm2)
    {
        //deep copy of 1st vector to 2nd
        myVec2->len = myVec1->len;
        myVec2->vector = realloc(myVec2->vector, sizeof(double) * myVec2->len);
        if (myVec2->vector == NULL)
        {
            return EXIT_FAILURE;
        }
        for (int i = 0; i < myVec2->len; i++)
        {
            myVec2->vector[i] = myVec1->vector[i];
        }
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}

/**
 * This function allocates memory it does not free.
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm), NULL on failure.
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector *myVec = (Vector *)malloc(sizeof(Vector));
    if (myVec == NULL)
    {
        return NULL;
    }
    myVec->len = 0;
    myVec->vector = NULL;
    forEachRBTree(tree, copyIfNormIsLarger, myVec);
    return myVec;
}
