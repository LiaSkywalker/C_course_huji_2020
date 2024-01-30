/**
 * @file RailWayPlanner.c
 * @author  Nitzan Rosen
 * @version 1.0
 * as asked, running time equals O(N*L*|A|)
 * @brief ex2 in c course
 * @section this program will calculate for givent input, the minimal cost and print it
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
//#include <math.h>
//#include <assert.h>
//#include <regex.h>
//#include <ctype.h>


// -------------------------- const definitions -------------------------
#define INPUT_LINE_LENGTH 1025
#define USAGE_ERROR_1 "Usage: RailWayPlanner <InputFile>"
#define OPEN_FILE_ERROR "File doesn't exists."
#define EMPTY_FILE_ERROR "File is empty."
#define INVALID_LINE_ERROR "Invalid input in line: %d."
#define LENGTH_MIN_BORDER 0
#define CON_MIN_BORDER 1
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define EOS '\0'
#define OUTPUT_FILE_NAME "railway_planner_output.txt"
#define MINIMAL_MSG "The minimal price is: %d"
#define COMMA ","
#define ALLOC_SIZE 10
#define NUMBER_OF_FIELDS_IN_PART 3 //4-1
#define MINIMAL_VALUE_NOT_FOUND -1

// -------------------------- functions header -------------------------
/**
 * struct to represent Part for the rail.
 */
typedef struct Part
{
    char start; //left connection type
    char end; //right connection type
    int length; //length of the part
    int price; //price of part
    int endIndexInArray; //index in types array of the end part
} Part;
/**
 * struct to represent rail in the price table
 */
typedef struct Rail
{
    char lastType;
    int totalRailPrice;
} Rail;

//for all the functions, description down
int writeStrToOutput(char strToPrint[]);
int writeNumToOutput(char strToPrint[], int numToPrint);
int readFile(FILE *fp, char **TypesArray, Part **PartsArray);
void freeAllocations(Part **PartsArray, char **TypesArray);
int checkIsNumber(char num[], int min);
int checkNumericLines(int minBorder, char *linePtr, int *valueToUpdate);
int isLegalChar(char *tok);
int checkThirdLine(char *line, char *TypesArray);
int checkPartLine(char line[], Part *PartsArray, int partCounter,
                  char *TypesArray, int typesCounter);
void resetTable(int lenInTable, int typesCounter, const char *TypesArray, Rail **Rails);
int buildTable(int railLength, int typesCounter, char *TypesArray,
               Part *PartsArray, int partsCounter);
void fillTable(Part *PartsArray, int lenInTable, int partsCounter, int typesCounter, Rail **Rails);
int getIndexOfType(char type, const char *TypesArray, int typesCounter);
int findMinimalPrice(int railLength, int typesCounter, Rail **Rails);
void tryToAddPart(Part currentPart, Rail ***Rails, int currentLength, int currentType);
int isascii1(char _c);
int isCharInTypesArray(const char *tok, const char *TypesArray, int typesCounter);

// -------------------------- functions  -------------------------
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        writeStrToOutput(USAGE_ERROR_1);
        return EXIT_FAILURE;
    }
    char *TypesArray = NULL;
    Part *PartsArray = NULL;

    FILE *fp = fopen(argv[1], "r"); //open file for reading
    if (fp == NULL) //fail opening file
    {
        writeStrToOutput(OPEN_FILE_ERROR);
        freeAllocations(&PartsArray, &TypesArray);
        return EXIT_FAILURE; //exit(EXIT_FAILURE); //todo
    }
    //read file, and call to functions who calculates the minimal price
    if (readFile(fp, &TypesArray, &PartsArray) == EXIT_FAILURE)
    {
        freeAllocations(&PartsArray, &TypesArray);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);
    freeAllocations(&PartsArray, &TypesArray);
    return EXIT_SUCCESS;
}

/**
 * build table in dimensions of (L+1)*K, and call to functions who initiallize it,
 * and calculate the minimal price
 * @param railLength
 * @param typesCounter
 * @param TypesArray
 * @param PartsArray
 * @param partsCounter
 * @return the minimal value if found OR -1 if there isn't any number like that
 */
int buildTable(int railLength, int typesCounter, char *TypesArray,
               Part *PartsArray, int partsCounter)
{
    //memory allocations for the table
    int lenInTable = railLength + 1;
    Rail **Rails;
    Rails = (Rail **)malloc((lenInTable) * sizeof(Rail *));
    for (int i = 0; i < lenInTable; i++)
    {
        Rails[i] = (Rail *)malloc(typesCounter * sizeof(Rail));
    }

    resetTable(lenInTable, typesCounter, TypesArray, Rails); //reset the table
    //fill the table with the wanted values
    fillTable(PartsArray, lenInTable, partsCounter, typesCounter, Rails);

    //find the minimal number from thee table
    int finalMinNumber = findMinimalPrice(railLength, typesCounter, Rails);

    //free the memory allocations :)
    for (int j = 0; j < lenInTable; j++)
    {
        free(Rails[j]);
        Rails[j] = NULL;
    }
    free(Rails);
    Rails = NULL;
    return finalMinNumber;
}

/**
 * get the filled table and find the minimal price
 * @param railLength
 * @param typesCounter
 * @param Rails
 * @return the minimal price if found, or -1
 */
int findMinimalPrice(int railLength, int typesCounter, Rail **Rails)
{
    int min = INT_MAX;
    for (int t = 0; t < typesCounter; t++)
    {
        if (Rails[railLength][t].totalRailPrice < min)
        {
            min = Rails[railLength][t].totalRailPrice;
        }
    }
    if (min == INT_MAX)
    {
        return MINIMAL_VALUE_NOT_FOUND;
    }
    if(min < 0)
    {
        return MINIMAL_VALUE_NOT_FOUND;
    }
    return min;
}

/**
 * fill the table with the minimal prices for every type and length
 * @param PartsArray
 * @param lenInTable
 * @param partsCounter
 * @param typesCounter
 * @param Rails
 * @param TypesArray
 */
void fillTable(Part *PartsArray, int lenInTable, int partsCounter, int typesCounter, Rail **Rails)
{
    //len zero is already initiallize, so we will start from 1
    for (int l = 1; l < lenInTable; l++)
    {
        for (int t = 0; t < typesCounter; t++)
        {
            for (int p = 0; p < partsCounter; p++)
            {
                Part currentPart = PartsArray[p];
                //todo
//                currentCheapestRail(PartsArray[p], &Rails, l, t,TypesArray,typesCounter);
                tryToAddPart(currentPart, &Rails, l, t);
            }
        }
    }
}

/**
 * update the prices in the table
 * @param currentPart
 * @param Rails
 * @param currentLength
 * @param currentType
 * @param TypesArray
 * @param typesCounter
 */
void tryToAddPart(Part currentPart, Rail ***Rails, int currentLength, int currentType)
{
    Part addedPart = currentPart;
    if ((*Rails)[currentLength][currentType].lastType != addedPart.start ||
        addedPart.length > currentLength)
    {
        return;
    }
    int addedPartStartIndex = addedPart.endIndexInArray;
    if (addedPartStartIndex == -1)
    {
        return;
    }
    Rail previousPart = (*Rails)[currentLength - addedPart.length][addedPartStartIndex];
    if (previousPart.totalRailPrice == INT_MAX)
    {
        return;
    }
    int newRailPrice = previousPart.totalRailPrice + addedPart.price;
    if (newRailPrice < (*Rails)[currentLength][currentType].totalRailPrice)
    {
        (*Rails)[currentLength][currentType].totalRailPrice = newRailPrice;
    }
}

/**
 * get table, and initiallize the values inside it-
 * zeroes in the first line, INF in all the rest
 * @param lenInTable
 * @param typesCounter
 * @param TypesArray
 * @param Rails
 */
void resetTable(int lenInTable, int typesCounter, const char *TypesArray, Rail **Rails)
{
    for (int l = 0; l < lenInTable; l++)
    {
        for (int t = 0; t < typesCounter; t++)
        {
            if (l == 0)
            {
                Rails[l][t] = (Rail){TypesArray[t], 0};
            }
            else
            {
                Rails[l][t] = (Rail){TypesArray[t], INT_MAX};
            }
        }
    }
}

/**
 * reead the opened file, and parse the lines due to ex2 given format
 * after that call the build table function to calculate the best price
 * @param fp
 * @param TypesArray
 * @param PartsArray
 * @return EXIT_FAILURE if we had problems, else EXIT_SUCCESS
 */
int readFile(FILE *fp, char **TypesArray, Part **PartsArray)
{

    int lineCounter = 0; //counter of the lines number
    char line[INPUT_LINE_LENGTH] = {EOS};

    int railLength = 0; // length of the rail
    int *railLengthPtr = &railLength;

    int typesCounter = 0; // number of possibole connectors
    int *typesCounterPtr = &typesCounter;
    int partsCounter1 = 0;

    *TypesArray = (char *)calloc(sizeof(char), 1);
    if (*TypesArray == NULL)
    {
        return EXIT_FAILURE;
    }

    int partsCapacity = ALLOC_SIZE;
    *PartsArray = (Part *)calloc(sizeof(Part), partsCapacity);
    if (*PartsArray == NULL)
    {
        return EXIT_FAILURE;
    }

    while ( fgets(line, sizeof(line), fp) != NULL)
    {
//        line[strlen(line) - 1] = EOS;
        lineCounter++;
        switch (lineCounter)
        {
            case 1:
                if (checkNumericLines(LENGTH_MIN_BORDER, line, railLengthPtr) == false)
                {
                    writeNumToOutput(INVALID_LINE_ERROR, lineCounter);
                    return EXIT_FAILURE;
                }
                break;

            case 2:
                if (checkNumericLines(CON_MIN_BORDER, line, typesCounterPtr) == false)
                {
                    writeNumToOutput(INVALID_LINE_ERROR, lineCounter);
                    return EXIT_FAILURE;
                }
                break;

            case 3:

                *TypesArray = (char *)realloc(*TypesArray, typesCounter);
                if (*TypesArray == NULL)
                {
                    return EXIT_FAILURE;
                }
                if (checkThirdLine(line, *TypesArray) == false)
                {
                    writeNumToOutput(INVALID_LINE_ERROR, lineCounter);
                    return EXIT_FAILURE;
                }
                break;

            default:
                if (partsCounter1 == partsCapacity) //change the array size if needed
                {
                    partsCapacity += ALLOC_SIZE;
                    *PartsArray = (Part *)realloc(*PartsArray, sizeof(Part) * partsCapacity);
                    if (*PartsArray == NULL)
                    {
                        return EXIT_FAILURE;
                    }
                }
                //check the line and add it to the array
                if (checkPartLine(line, *PartsArray, partsCounter1, *TypesArray, typesCounter) ==
                    false)
                {
                    writeNumToOutput(INVALID_LINE_ERROR, lineCounter);
                    return EXIT_FAILURE;
                }
                partsCounter1++;
        }
    }
    if (lineCounter == 0 && (feof(fp) == true)) //empty file case
    {
        writeStrToOutput(EMPTY_FILE_ERROR);
        return EXIT_FAILURE;
    }
    //now find the minimal price:
    int finalAns = buildTable(railLength, typesCounter, *TypesArray, *PartsArray, partsCounter1);
    writeNumToOutput(MINIMAL_MSG, finalAns);
    return EXIT_SUCCESS;
}

/**
 * get line that represents Part, check itws params, and add it's details into Parts array
 * @param line
 * @param PartsArray
 * @param partCounter
 * @param TypesArray
 * @param typesCounter
 * @return
 */
int checkPartLine(char line[], Part *PartsArray, int partCounter,
                  char *TypesArray, int typesCounter)
{
    int counter = 0;

    char *tok;
    tok = strtok(line, COMMA);
    while ( tok != NULL)
    {
        if (counter > NUMBER_OF_FIELDS_IN_PART)
        {
            return false;
        }
        else if (counter == 0) //start part
        {
            if (isLegalChar(tok) == false ||
                isCharInTypesArray( tok, TypesArray, typesCounter) == false )
            {
                return false;
            }
            PartsArray[partCounter].start = tok[0];
        }
        else if (counter == 1)
        {
            if (isLegalChar(tok) == false ||
                isCharInTypesArray(tok, TypesArray, typesCounter) == false )
            {
                return false;
            }
            PartsArray[partCounter].end = tok[0];
        }
        else if (counter == 2)
        {
            if (checkNumericLines(1, tok, &PartsArray[partCounter].length) == false)
            {
                return false;
            }
        }
        else if (counter == 3)
        {
            if (checkNumericLines(0, tok, &PartsArray[partCounter].price) == false)
            {
                return false;
            }
        }
        counter++;
        tok = strtok(NULL, COMMA);
    }
    PartsArray[partCounter].endIndexInArray = getIndexOfType(PartsArray[partCounter].end,
                                                             TypesArray, typesCounter);
    if (counter != NUMBER_OF_FIELDS_IN_PART + 1)
    {
        return false;
    }
    return true;

}

/**
 * check if the given char is in TypeArray
 * @param tok
 * @param TypesArray
 * @param typesCounter
 * @return true if yes, false else
 */
int isCharInTypesArray( const char *tok, const char *TypesArray, int typesCounter)
{

    for (int i = 0; i < typesCounter; i++)
    {
        if (TypesArray[i] == tok[0])
        {
            return true;
        }
    }
    return false;
}

/**
 * got char of type, and check what is itws index in the Types array
 * @param type
 * @param TypesArray
 * @param typesCounter
 * @return the index of the char, -1 if the wanted index not found
 */
int getIndexOfType(char type, const char *TypesArray, int typesCounter)
{
    for (int i = 0; i < typesCounter; i++)
    {
        if (TypesArray[i] == type)
        {
            return i;
        }
    }
    return -1;
}

/**
 * get the third line with the legal types, and add them to Types array
 * @param line
 * @param TypesArray
 * @return
 */
int checkThirdLine(char *line, char *TypesArray)
{
    //check the types of connector
    line[strlen(line) - 1] = '\0';
    int counter = 0;
    char *tok = NULL;
    tok = strtok(line, COMMA);
    while ( tok != NULL)
    {
        if ((isLegalChar(tok)) == false) //check if tok is legal char
        {
            return false;
        }
        TypesArray[counter] = tok[0];
        tok = strtok(NULL, COMMA);
        counter++;
    }
    return true;
}

/**
 * check if the given char is valid number.
 * if the number is valid, cast it into valueToUpdate pointer
 * @param minBorder
 * @param linePtr
 * @param valueToUpdate
 * @return true if the checking and the casting ends successfully, false else
 */
int checkNumericLines(int minBorder, char *linePtr, int *valueToUpdate)
{
    if (checkIsNumber(linePtr, minBorder) == false)
    {
        return false;
    }
    else
    {
        int num = (int)strtod(linePtr, NULL);
        *valueToUpdate = num;
        return true;
    }
}

/**
 * check if the given char is number
 * @param num
 * @param min
 * @return true if the char is number, false else
 */
int checkIsNumber(char num[], int min)
{
    char *literal;
    double digitNum = strtod(num, &literal);
    if (min > digitNum || (int)digitNum != digitNum ||
        ((strncmp(literal, "\n", INPUT_LINE_LENGTH) != 0) &&
        (strncmp(literal, "" , INPUT_LINE_LENGTH) != 0)))
    {
        return false;
    }
    return true;
}

/**
 * check if the given pointer is char
 * @param tok
 * @return true if it's char, false else
 */
int isLegalChar(char *tok)
{
    if (((strlen(tok) == 1) && isascii1(tok[0])) == false)
    {
        return false;
    }
    return true;
}

/**
 * check if char is ascii
 * @return
 */
int isascii1( char __c)
{
    return (unsigned)(__c) <= 0177;
}

/**
 * open output file to write, and write inside it the given str
 * @param strToPrint
 * @return EXIT_SUCCESS if the process ends successfully
 */
int writeStrToOutput(char strToPrint[])
{
    FILE *fp = fopen(OUTPUT_FILE_NAME, "w");
    if (fp == NULL)
    {
        return EXIT_FAILURE;
    }
    fprintf(fp, "%s", strToPrint);
    fclose(fp);
    return EXIT_SUCCESS;
}

/**
 * open output file to write, and write inside it the given str, and the given number
 * @param strToPrint
 * @param numToPrint
 * @return EXIT_SUCCESS if the process ends successfully
 */
int writeNumToOutput(char strToPrint[], int numToPrint)
{
    FILE *fp = fopen(OUTPUT_FILE_NAME, "w");
    if (fp == NULL)
    {
        return EXIT_FAILURE;
    }
    fprintf(fp, strToPrint, numToPrint);
    fclose(fp);
    return EXIT_SUCCESS;
}

/**
 * free the memory allocations of the input pointers
 * @param PartsArray
 * @param TypesArray
 */
void freeAllocations(Part **PartsArray, char **TypesArray)
{
    free(*PartsArray);
    free(*TypesArray);
    *PartsArray = NULL;
    *TypesArray = NULL;
}