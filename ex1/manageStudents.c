/**
 * @file manageStudents.c
 * @author  Nitzan Rosen
 * @version 1.0
 * @date 19.04.2020
 *
 * @brief ex1 in c course
 *
 * @section this program getting list of students, and after that do one of the follow:
 *      best- cakculate the best student
 *      merge- doing merge-sort by grades
 *      quick- doing quick sort by alphabetical order
 * Input  : best\ merge\ quick by stdin
 * Output : stdout
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// -------------------------- const definitions -------------------------
#define MAXIMAL_GRADE_VALUE 100
#define MINIMAL_GRADE_VALUE 0
#define MINIMAL_AGE_VALUE 18
#define MAXIMAL_AGE_VALUE 120
#define ARR_SIZE 42
#define LINE_SIZE 65
#define STUDENT_ARR_SIZE 5500
#define MERGE_SUB_ARRY_SIZE 2750
#define BEST "best"
#define MERGE "merge"
#define QUICK "quick"
#define USAGE_ERROR_2 "USAGE: run with only one of the following params: best, merge, quick"
#define USAGE_ERROR_1 "USAGE: please insert only one argument"
#define INPUT_MESSAGE "Enter student info. To exit press q, then enter"
#define ERROR_WRONG_INPUT_PARAM_NUMBER "ERROR: wrong number of parameters in input"
#define ERROR_INFO_MUST_MATCH "ERROR: info must match specified format"
#define ERROR_ID_10_DIGITS "ERROR: id must have 10 digits"
#define ERROR_CHARACTERS_OR_WHITESPACE "ERROR: name must contain only english characters or whitespace"
#define ERROR_CHARACTERS_OR_HYPHEN "must contain only english characters or -"
#define ERROR "ERROR:"
#define IN_LINE "in line"
#define BEST_STUDENT_IS "best student info is: "
#define Q "q"
#define HYPHEN '-'
#define RETURN_OK 0
#define RETURN_NOTOK 1

/**
 * struct that represents student.
 * @id char[42]
 * @name char[42]
 * @grade int
 * @age int
 * @country char[42]
 * @city char[42]
 * @bestParam float, calculate by grade/age
 */
typedef struct Student
{
    char name[ARR_SIZE], country[ARR_SIZE], city[ARR_SIZE], id[ARR_SIZE];
    int grade, age;
    float bestParam;
} Student;

//Detailed description later in the file! , before the defining of each function
int checkIdParam(char *id, int linesCounter);

int checkParams(char id[], char name[], char grade[], char age[], char country[], char city[],
                int linesCounter);

void printByFormat(Student student);

int bestStudentPart();

int mergePart();

int quickSortPart();

int inputFunction(Student studentarr[], int *insertedStudentCounter);

int calcBestStudent(Student students[], int insertedStudentCounter);

int checkNameParam(char nameIn[], int linesCounter);

int
checkNumericalParam(char numIn[], int lowerBound, int upperBound, char param[], int linesCounter);

int checkCountryAndCityParam(char nameIn[], char param[], int linesCounter);

Student
createStudent(char id[], char name[], char grade[], char age[], char country[], char city[]);

void merge(Student Array[], int left, int mid, int right);

void mergeSort(Student Array[], int left, int right);

int max(int a, int b);

void swap(Student *a, Student *b);

void quickSort(Student Array[], int lowerIndex, int higherIndex);

int partition(Student Array[], int lowerIndex, int higherIndex);


/**
 * main function of the program.
 * taaking the wanted action from the user, and calls the needed functions
 * @param argc
 * @param argv
 * @return 0 or 1
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf(USAGE_ERROR_1);
        return RETURN_NOTOK; //1
    }

    if (strncmp(argv[1], BEST, 4) == 0 && ((int)strlen(argv[1]) == 4))
    {
        return bestStudentPart();
    }
    else if (strncmp(argv[1], MERGE, 5) == 0 && ((int)strlen(argv[1]) == 5))
    {
        return mergePart();
    }
    else if (strncmp(argv[1], QUICK, 5) == 0 && ((int)strlen(argv[1]) == 5))
    {
        return quickSortPart();
    }
    else
    {
        printf(USAGE_ERROR_2);
        return RETURN_NOTOK; //1
    }
}

/**
 * getting input and calculating the best student.
 * if evertying ends aucessfully- return 0
 * if we got problens- return 1
 */
int bestStudentPart()
{
    Student StudentArr[STUDENT_ARR_SIZE];
    int insertedStudentCounter = 0;
    int inputStatus = inputFunction(StudentArr, &insertedStudentCounter); //collect students data
    if (inputStatus == 1)
    { //some failure happen
        return RETURN_NOTOK; //1
    }

    if (insertedStudentCounter == 0) //if we didn't collect any data- just return 0.
    {
        return RETURN_OK; //0
    }
        //if we collected data- calculate the best student and print him
    else
    {
        calcBestStudent(StudentArr, insertedStudentCounter);
    }
    return RETURN_OK; //0
}

/**
 * grtting input, and sort the students my grade withe merge sort
 * @return 0 or 1
 */
int mergePart()
{
    Student StudentArr[STUDENT_ARR_SIZE];
    int insertedStudentCounter = 0;

    int inputStatus = inputFunction(StudentArr, &insertedStudentCounter); //collect students data
    if (inputStatus == 1) //some failure happen
    {
        return RETURN_NOTOK; //1
    }

    if (insertedStudentCounter == 0) //if we didn't collect any data- just return 0.
    {
        return RETURN_OK; //0
    }
        //if we collected data- calculate the best student and print him
    else
    {
        mergeSort(StudentArr, 0, insertedStudentCounter - 1);
        for (int i = 0; i < insertedStudentCounter; i++)
        {
            printByFormat(StudentArr[i]);
        }
    }
    return RETURN_OK; //0
}

/**
 * getting input, and sort by alphabetic order
 * @return 0 or 1
 */
int quickSortPart()
{
    Student StudentArr[STUDENT_ARR_SIZE];
    int insertedStudentCounter = 0;

    int inputStatus = inputFunction(StudentArr, &insertedStudentCounter); //collect students data
    if (inputStatus == 1)
    { //some failure happen
        return RETURN_NOTOK; //1
    }

    if (insertedStudentCounter == 0) //if we didn't collect any data- just return 0.
    {
        return RETURN_OK; //0
    }
        //if we collected data- calculate the best student and print him
    else
    {
        quickSort(StudentArr, 0, insertedStudentCounter - 1);
        for (int i = 0; i < insertedStudentCounter; i++)
        {
            printByFormat(StudentArr[i]);
        }
    }
    return RETURN_OK; //0
}

/**
 *  this function get the input from the user
 * @param studentArr
 * @param insertedStudentCounter
 * @return 0 if Ok, 1 else
 */
int inputFunction(Student studentArr[], int *insertedStudentCounter)
{
    int linesCounter = -1;
    int stopCollectDataFlag = 1; //1 true, 0 false

    while ( stopCollectDataFlag )
    {
        printf("%s\n", INPUT_MESSAGE);
        char line[LINE_SIZE], name[ARR_SIZE], country[ARR_SIZE], city[ARR_SIZE], id[ARR_SIZE],
                grade[ARR_SIZE], age[ARR_SIZE];
        int result;

        if (fgets(line, sizeof(line), stdin))
        {
            linesCounter++; //first- raise the counter by 1
            //check if we got q
            if (strncmp(line, Q, 1) == 0 && (int)strlen(line) == 2)
            {
                stopCollectDataFlag = 0;
                return RETURN_OK; //0
            }
            else //check the input line params
            {
                result = sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
                                id, name, grade, age, country, city);
                if (result != 6)
                {
                    printf("%s\n", ERROR_WRONG_INPUT_PARAM_NUMBER);
                    printf("%s %d\n", IN_LINE, linesCounter);
                }
                else
                {
                    int testLine = checkParams(id, name, grade, age, country, city, linesCounter);
                    if (testLine == 0) //all the params are OK :)
                    {
                        //create student
                        Student currentStudent = createStudent(id, name, grade, age, country, city);

                        //insert the student to the students array
                        studentArr[*insertedStudentCounter] = currentStudent;

                        //raise the counter of inserted students by 1
                        *insertedStudentCounter = *insertedStudentCounter + 1;
                    }

                }
            }
        }
        else
        { //some problems happened
            linesCounter++;
            printf("%s\n", ERROR_WRONG_INPUT_PARAM_NUMBER);
            printf("%s %d\n", IN_LINE, linesCounter);
            return RETURN_NOTOK; //1
        }
    }
    return RETURN_OK; //0
}

/**
 * create student Struct with the input parameters
 * @param id
 * @param name
 * @param grade
 * @param age
 * @param country
 * @param city
 * @return Student with all the params
 */
Student createStudent(char id[], char name[], char grade[], char age[], char country[], char city[])
{
    char *ptr;
    int gradeInt = (int)strtol(grade, &ptr, 10);
    int ageInt = (int)strtol(age, &ptr, 10);
    float bestParam = ((float)gradeInt) / ((float)ageInt);

    Student student;
    strncpy(student.id, id, sizeof(student.id));
    strncpy(student.name, name, sizeof(student.name));
    student.grade = gradeInt;
    student.age = ageInt;
    student.bestParam = bestParam;
    strncpy(student.country, country, sizeof(student.country));
    strncpy(student.city, city, sizeof(student.city));
    return student;
}

/**
 * calculate the best student
 * @param students
 * @param insertedStudentCounter
 * @return
 */
int calcBestStudent(Student students[], int insertedStudentCounter)
{
    float CurrentBestStudentParam = 0;
    Student currentBestStudent;
    //find the best student (with saving the order)
    for (int i = 0; i < insertedStudentCounter; i++)
    {
        if (students[i].bestParam > CurrentBestStudentParam)
        {
            CurrentBestStudentParam = students[i].bestParam;
            currentBestStudent = students[i];
        }
    }
    //print the maximal
    printf("%s", BEST_STUDENT_IS);
    printByFormat(currentBestStudent);
    return RETURN_OK; //0
}

void printByFormat(Student student)
{
    char *name = student.name;
    char *country = student.country;
    char *city = student.city;
    char *id = student.id;
    int grade = student.grade;
    int age = student.age;
    printf("%s,%s,%d,%d,%s,%s\n", id, name, grade, age, country, city);
}

/**
 * get the splitted line, and check if the line is valid
 * @param id
 * @param name
 * @param grade
 * @param age
 * @param country
 * @param city
 * @param linesCounter
 * @return 0 if ok, and 1 else.
 */
int checkParams(char id[], char name[], char grade[], char age[], char country[], char city[],
                int linesCounter)
{
    if ((checkIdParam(id, linesCounter) == 0) && (checkNameParam(name, linesCounter) == 0) &&
        (checkNumericalParam(grade, MINIMAL_GRADE_VALUE, MAXIMAL_GRADE_VALUE,  "grade", linesCounter) == 0) &&
        (checkNumericalParam(age, MINIMAL_AGE_VALUE, MAXIMAL_AGE_VALUE, "age", linesCounter) == 0) &&
        (checkCountryAndCityParam(country, "country", linesCounter) == 0) &&
        (checkCountryAndCityParam(city, "city", linesCounter) == 0))
    {
        return RETURN_OK; //0
    }
    return RETURN_NOTOK; //1
}

/**
 * check the id param
 * @param idIn
 * @param linesCounter
 * @return  0 if valid, 1 else
 */
int checkIdParam(char idIn[], int linesCounter)
{
    if ((int)strlen(idIn) != 10) //id is 10 numbers int
    {
        printf("%s\n", ERROR_ID_10_DIGITS);
        printf("%s %d\n", IN_LINE, linesCounter);
        return RETURN_NOTOK; //1
    }

    // check there are only numbers
    for (int i = 0; i < (int)strlen(idIn); i++)
    {
        unsigned char currentChar = (unsigned char)idIn[i];
        unsigned int isDigit = (unsigned int)isdigit(currentChar);

        if (!isDigit)
        {
            printf("%s\n", ERROR_INFO_MUST_MATCH);
            printf("in line %d\n", linesCounter);
            return RETURN_NOTOK; //1
        }
        if (i == 0 && (strncmp(idIn, "0", 1) == 0))
        {
            printf("%s\n", ERROR_INFO_MUST_MATCH);
            printf("in line %d\n", linesCounter);
            return RETURN_NOTOK; //1
        }
    }
    //if we arrive here- the number is OK :)
    return RETURN_OK; //0
}

/**
 * cheak the name param
 * @param nameIn
 * @param linesCounter
 * @return
 */
int checkNameParam(char nameIn[], int linesCounter)
{
    //check we contaion only english characters or whitespace
    for (int i = 0; i < (int)strlen(nameIn); i++)
    {
        unsigned char currentChar = (unsigned char)nameIn[i];
        unsigned int isAlpha = (unsigned int)isalpha(currentChar);

        if (((nameIn[i] == '-') == 0) && ((nameIn[i] == ' ') == 0) && (isAlpha == 0))
        {
            printf("%s\n", ERROR_CHARACTERS_OR_WHITESPACE);
            printf("%s %d\n", IN_LINE, linesCounter);
            return RETURN_NOTOK; //1
        }
    }
    //if we arrive here- the param is OK :)
    return RETURN_OK; //0
}

/**
 * this function check the country/city params
 * @param nameIn the string to check
 * @param param city/country. uses inside print statement
 * @param linesCounter
 * @return 0 if all ok, 1 else
 */
int checkCountryAndCityParam(char nameIn[], char param[], int linesCounter)
{
    //check we contain only english characters or -
    for (int i = 0; i < (int)strlen(nameIn); i++)
    {
        unsigned char currentChar = (unsigned char)nameIn[i];
        unsigned int isAlpha = (unsigned int)isalpha(currentChar);

        if (((nameIn[i] == HYPHEN) == 0) && (isAlpha == 0))
        {
            printf("%s %s %s\n", ERROR, param, ERROR_CHARACTERS_OR_HYPHEN);
            printf("%s %d\n", IN_LINE, linesCounter);
            return RETURN_NOTOK; //1
        }
    }
    //if we arrive here- the param is OK :)
    return RETURN_OK; //0
}

/**
 * this function get "number" input, make sure it's number, and check if it between the bounds.
 * @param numIn
 * @param lowerBound
 * @param upperBound
 * @param param
 * @param linesCounter
 * @return 0 if ok, 1 else
 */
int
checkNumericalParam(char numIn[], int lowerBound, int upperBound, char param[], int linesCounter)
{

    //grade\age are up to 3 digits
    if ((int)strlen(numIn) > 3)
    {
        printf("ERROR: %s must be an integer between %d and %d\n", param, lowerBound, upperBound);
        printf("%s %d\n", IN_LINE, linesCounter);
        return RETURN_NOTOK; //1
    }

    // check there are only numbers
    for (int i = 0; i < (int)strlen(numIn); i++)
    {
        unsigned char currentChar = (unsigned char)numIn[i];
        unsigned int isDigit = (unsigned int)isdigit(currentChar);

        if (!isDigit)
        {
            printf("%s\n", ERROR_INFO_MUST_MATCH);
            printf("%s %d\n", IN_LINE, linesCounter);
            return RETURN_NOTOK; //1
        }
    }
    //transform to int and check the bounds
    char *ptr;
    int num = (int)strtol(numIn, &ptr, 10);

    if ((lowerBound > num) || (num > upperBound))
    {
        printf("ERROR: %s must be an integer between %d and %d\n", param, lowerBound, upperBound);
        printf("%s %d\n", IN_LINE, linesCounter);
        return RETURN_NOTOK; //1
    }
    //if we arrive here- the param is OK :)
    return RETURN_OK; //0
}

/**
 * nerge two subarrays- Array[l..m] and Array[m+1..r]
 * @param Array
 * @param left Array[left..mid]
 * @param mid
 * @param right Array[mid...right]
 */
void merge(Student Array[], int left, int mid, int right)
{

    int leftSize = mid - left + 1;
    int rightSize = right - mid;
    Student L[MERGE_SUB_ARRY_SIZE];
    Student R[MERGE_SUB_ARRY_SIZE]; //create temporary arrays for the left and the right subarray

    //Copy data to temp arrays L[] and R[]
    for (int i = 0; i < leftSize; i++)
    {
        L[i] = Array[left + i];
    }
    for (int j = 0; j < rightSize; j++)
    {
        R[j] = Array[mid + 1 + j];
    }

    //counters for the items who already inserted
    int i = 0; // Initial index of left subarray
    int j = 0; // Initial index of right subarray
    int k = left; // Initial index of merged subarray

    //Merge the temp arrays back into arr[l..r]
    while ( i < leftSize && j < rightSize )
    {
        if (L[i].grade <= R[j].grade)
        {
            Array[k] = L[i];
            i++;
        }
        else
        {
            Array[k] = R[j];
            j++;
        }
        k++;
    }

    //copy the remain elements of L[]
    while ( i < leftSize )
    {
        Array[k] = L[i];
        i++;
        k++;
    }

    //copy the remain elements of R[]
    while ( j < rightSize )
    {
        Array[k] = R[j];
        j++;
        k++;
    }
}

/**
 * sorting the given array with merge-sort, in place!
 * @param Array
 * @param left
 * @param right
 */
void mergeSort(Student Array[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(Array, left, mid); //sort subArray- Array[left....mid]
        mergeSort(Array, mid + 1, right); //sort subArray- Array[mid....right]
        merge(Array, left, mid, right);
    }
}

/**
 * return the max element of the two
 * @param a
 * @param b
 * @return
 */
int max(int a, int b)
{
    if (a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/**
 * function to swap two elements
 * @param a
 * @param b
 */
void swap(Student *a, Student *b)
{
    Student t = *a;
    *a = *b;
    *b = t;
}

/**
 * sorting the given array using QuickSort
 * @param Array
 * @param low
 * @param high
 */
void quickSort(Student Array[], int lowerIndex, int higherIndex)
{
    if (lowerIndex < higherIndex)
    {
        int partitionIndex = partition(Array, lowerIndex, higherIndex);

        quickSort(Array, lowerIndex, partitionIndex - 1); //sort the element before pivot
        quickSort(Array, partitionIndex + 1, higherIndex); //sort the elemnt after pivot
    }
}

/**
 * takes last element as pivot, places
 *the pivot element at its correct position in sorted
 *array, and places all smaller (smaller than pivot)
 *to left of pivot and all greater elements to right
 *of pivot
 * @param Array
 * @param lowerIndex
 * @param higherIndex
 * @return
 */
int partition(Student Array[], int lowerIndex, int higherIndex)
{
    // the pivot is the higher index char pivotElement[] = Array[higherIndex].name;
    int i = (lowerIndex - 1);  // Index of smaller element

    for (int j = lowerIndex; j <= higherIndex - 1; j++)
    {
        int maxLengh = max(((int)strlen(Array[j].name)), ((int)strlen(Array[higherIndex].name)));
        int result = strncmp(Array[j].name, Array[higherIndex].name, maxLengh);
        // If current element is smaller than the pivot (Array[j] < pivotIndex)
        if (result < 0)
        {
            i++;    // increment index of smaller element
            swap(&Array[i], &Array[j]);
        }
    }
    swap(&Array[i + 1], &Array[higherIndex]);
    return (i + 1);
}