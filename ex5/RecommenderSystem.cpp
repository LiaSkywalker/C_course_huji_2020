/**
 * @file RecommenderSystem.h
 * @author  Nitzan Rosen
 * @date 05.07 2020
 */
// ------------------------------ includes ------------------------------
#include "RecommenderSystem.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
// ------------------------------ defines -----------------------------
#define OPEN_FAIL "Unable to open file "
#define MOVIE_TOKEN 'M'
#define RATING_TOKEN 'R'
#define NA -42 //todo because its cool!!!! change to inf
#define USER_NOT_FOUND "USER NOT FOUND"
#define ZERO 0
#define DEFULT_MOVIE "defult movie"
#define SUCCESS 0
#define FAIL -1
#define DEFULT_NAGATIVE -17

// ------------------------------ class methods -----------------------------

/**
 * operator *= overloading
 * @param vector
 * @param scalar
 * @return Scalar and matrix multiplier
 */
std::vector<double> &operator*=(std::vector<double> &vector, double scalar)
{
    for (auto &i: vector)
    {
        i = i * scalar;
    }
    return vector;
}

/**
 * dot product on two vectors
 * @param vector1
 * @param vector2
 * @return result of dot product
 */
double operator*(const std::vector<double> &vector1, const std::vector<double> &vector2)
{
    if (vector1.size() != vector2.size())
    {
        throw;
    }
    double product = 0;
    for (unsigned int i = 0; i < vector1.size(); i++)
    {
        product += (vector1[i] * vector2[i]); //todo
    }
    return product;
}

/**
 * operator += overloading
 * add vector2 to vector1
 * @param vector1
 * @param vector2
 * @return refernce to vector1
 */
std::vector<double> &operator+=(std::vector<double> &vector1, std::vector<double> &vector2)
{
    for (unsigned int i = 0; i < vector1.size(); i++)
    {
        vector1[i] += vector2[i];
    }
    return vector1;
}

/**
 * defult constructor of RecommenderSystem class
 */
RecommenderSystem::RecommenderSystem()
{
    _moviesMap = std::unordered_map<string, std::vector<double >>();
    _ratingsMap = std::unordered_map<string, std::vector<double >>();
    _moviesVector = std::vector<string>();
}

/**
 * get ifstream reference, and add the given movies to the movies map
 * @param is
 * @return EXIT_SUCCESS in success
 */
int RecommenderSystem::_loadMovieTable(std::ifstream &is)
{
    try
    {
        string line;
        while ( getline(is, line))
        {
            int i = 0;
            std::istringstream iss(line);
            string movieName;
            std::vector<double> currentVector;
            while ( iss.peek() != EOF)
            {
                if (i == 0) //first line
                {
                    iss >> movieName;
                }
                else //rest of the file
                {
                    int myInt;
                    if (iss >> myInt)
                    {
                        currentVector.push_back(myInt);
                    }
                }
                i++;
            }
            this->_moviesMap[movieName] = currentVector;
        }
    }
    catch (const std::exception &e)
    {
        throw;
    }
    return SUCCESS;
}

/**
 * read the first line of rating file
 * @param line
 * @param movies
 * @return
 */
int RecommenderSystem::_readFirstRatingFileLine(const string &line, std::vector<string> &movies)
{
    try
    {
        std::istringstream iss(line);
        while ( iss.peek() != EOF)
        {
            string str;
            iss >> str;
            //check there is no movie with this name
            if (this->_moviesMap.count(str) == 0)
            {
                return FAIL;
            }
            movies.push_back(str);
        }
    }
    catch (const std::exception &e)
    {
        throw;
    }
    return SUCCESS;
}

/**
 * read the lines of rating file
 * @param line
 * @param movies
 * @return status of success
 */
int RecommenderSystem::_readSingleRating(const string &line)
{
    try
    {
        int i = 0; //index of words
        std::istringstream iss(line);
        string customerName;
        while ( iss.peek() != '\r' ) //todo
        {
            //first word- the customer name
            if (i == 0)
            {
                iss >> customerName;
            }
            else //all the rest- params of movies
            {
                double currentNum;
                iss >> currentNum;
                if (iss.good())
                {
                    this->_ratingsMap[customerName].push_back(currentNum);
                }
                else //NA
                {
                    iss.clear();
                    string currentStr;
                    iss >> currentStr;
                    this->_ratingsMap[customerName].push_back(NA);
                }
            }
            i++;
        }
    }
    catch (const std::exception &e)
    {
        throw;
    }
    return SUCCESS;
}

/**
 * load ratings to class
 * @param is
 * @return
 */
int RecommenderSystem::_loadRatingsMap(std::ifstream &is)
{
    try
    {
        int j = 0;
        string line;
        std::vector<string> movies;
        while ( getline(is, line))
        {
            std::istringstream iss(line);
            if (j == 0)
            {
                _readFirstRatingFileLine(line, movies);
                this->_moviesVector = movies;
            }
            else
            {
                _readSingleRating(line);
            }
            j++;
        }
    }
    catch (const std::exception &e)
    {
        throw;
    }
    return SUCCESS;
}

/**
 * this function get a path, and token of type-processing, and read the data to the class
 * @param path
 * @param Token
 * @return int due to the success\ fail status
 */
int RecommenderSystem::_readFile(const string &path, char Token)
{
    //first open the file
    std::ifstream is;
    try
    {
        is.open(path, std::ifstream::in);
    }
    catch (const std::exception &e) //opening went wrong
    {
        std::cerr << OPEN_FAIL << path << std::endl;
        throw;
    }

    //secondlly- read the file!
    try
    {
        if (Token == MOVIE_TOKEN) //movie
        {
            this->_loadMovieTable(is);
        }
        else //rating
        {
            this->_loadRatingsMap(is);
        }
        if (is.peek() != EOF)
        {
            is.close();
            throw;
        }
    }
    catch (const std::exception &e)
    {
        is.close();
        throw;
    }
    is.close();
    return SUCCESS;
}

/**
 * get Input two strings that contain the path of the two input files
 * Upload the information to the recommendation system
 * if path is wrong- print errorMsg
 * @param moviesAttributesFilePath
 * @param userRanksFilePath
 * @return Return 0 to success and -1 to failure
 */
int
RecommenderSystem::loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath)
{
    try
    {
        _readFile(moviesAttributesFilePath, MOVIE_TOKEN);
        _readFile(userRanksFilePath, RATING_TOKEN);
    }
    catch (const std::exception &e)
    {
        return FAIL;
    }
    return SUCCESS;
}

/**
 * get vector, and return it's avreage
 * @param vector
 * @return
 */
double RecommenderSystem::_calculateAverage(const std::vector<double> &vector)
{
    int counter = 0;
    double sum = 0;
    for (auto &i : vector)
    {
        if (i != NA)
        {
            sum += i;
            counter++;
        }
    }
    return sum / counter;
}

/**
 * normallize the given vector with the average
 * @param vector
 * @param avg
 * @return
 */
bool RecommenderSystem::_normallizeVec(std::vector<double> &vector, double avg)
{
    for (auto &i : vector)
    {
        if (i != NA)
        {
            i = i - avg;
        }
    }
    return true;
}

/**
 * calculte the norm of the two vectors
 * @param vector1
 * @param vector2
 * @return the norm that calculated
 */
double RecommenderSystem::_calculateNorm(const std::vector<double> &vector1)
{
    double sum = 0;

    for (auto &i: vector1)
    {
        //sum += pow(vec[i], SQUARE);
        sum += (i * i);
    }
    double norm = std::sqrt(sum);
    return norm;
}

/**
 *
 * @param vector1
 * @param vector2
 * @return angle bwtween the two vectors
 */
double RecommenderSystem::_calculateAngle(const std::vector<double> &vector1,
                                          const std::vector<double> &vector2)
{
    double angle = (vector1 * vector2) / (_calculateNorm(vector1) * _calculateNorm(vector2));
    return angle;
}

/**
 * return the movie with the maximal angle
 * @param userName
 * @param prefVec
 * @return //todo check that function
 */
string
RecommenderSystem::_findMostSimilarAngle(const string &userName, const std::vector<double> &prefVec)
{
    string movieNameToReturn = DEFULT_MOVIE;
    double myAngle = -2;
    double currentAngle;
    for (unsigned int i = 0; i < _moviesVector.size(); i++) //for every index in given user vector
    {
        double value = _ratingsMap[userName][i];
        if (value == NA) //if there was movie the user didnt rated
        {
            string currMovie = this->_moviesVector[i];
            //calculate angle between movieVec and prefernce vector
            currentAngle = _calculateAngle(prefVec, this->_moviesMap[currMovie]);
            if (currentAngle > myAngle)
            {
                myAngle = currentAngle;
                movieNameToReturn = currMovie;
            }
        }
    }
    return movieNameToReturn;
}

/**
 *
 * @param userName
 * @return
 */
string RecommenderSystem::recommendByContent(const string &userName)
{
    if (_ratingsMap.empty() || this->_ratingsMap.count(userName) == 0)
    {
        return USER_NOT_FOUND;
    }
    std::vector<double> userVec = this->_ratingsMap[userName];
    double avg = this->_calculateAverage(userVec); //calculate averages
    _normallizeVec(userVec, avg); //do normalization

    std::vector<double> preferenceVec;
    for (unsigned int i = 0; i < (_moviesMap[_moviesVector[ZERO]]).size(); ++i)
    {
        preferenceVec.push_back(ZERO);
    }
    //create prefernce vector
    for (unsigned int i = 0; i < _moviesVector.size(); i++) //for every index in given user vector
    {
//        double value = _ratingsMap[userName][i]; //check if the user have rating for this movie
        double value = userVec[i];
        if (value != NA)
        {
            string movie = this->_moviesVector[i]; //get the current movie name
            std::vector<double> movieVec = this->_moviesMap[movie];
            preferenceVec += ((movieVec) *= value); //add movieVec*value to prefVec
        }
    }
    return _findMostSimilarAngle(userName, preferenceVec); //calculate angles, and find the best movie
}

/**
 * return the rating for the given movie and user
 * @param movieName
 * @param userName
 * @param k
 * @return
 */
double
RecommenderSystem::_getRateForCurrentMovie(const string &movieName, const string &userName, int k)
{
    if (this->_moviesMap.count(movieName) == 0 || this->_ratingsMap.count(userName) == 0)
    {
        return FAIL;
    }
    const std::vector<double>& givenMovieVector = this->_moviesMap[movieName]; //get current movie Vector
    std::vector<std::pair<double, int>> movieAndAngle; //vec of angles, movie index!

    for (unsigned int i = 0;
         i < _moviesVector.size(); i++)
    {
        if (this->_ratingsMap[userName][i] != NA) //for every movie that the user saw
        {
            string currentMovie = _moviesVector[i]; //current movie
            const std::vector<double> &currMovieVec = this->_moviesMap[currentMovie]; //get it's vector
            double currentAngle = _calculateAngle(currMovieVec, givenMovieVector); //calculate angle
            movieAndAngle.emplace_back(currentAngle, i);
        }
    }
    //find the k max rated
    std::vector<std::pair<double, int>> kElements;
    for (int i = 0; i < k; i++)
    {
        std::pair<double, int> maxPair = {0, 0};
        for (auto &j : movieAndAngle)
        {
            if (j.first > maxPair.first)
            {
                maxPair = {j.first, j.second};
                j.first = DEFULT_NAGATIVE;
            }
        }
        kElements.push_back(maxPair);
    }

    double sum = 0;
    double divider = 0;
    for (auto &kElement : kElements)
    {
        double userInd = _ratingsMap[userName][kElement.second];
        sum += (kElement.first * userInd); //movieIndex!
        divider += kElement.first;
    }
    return (sum / divider); // return the rating!
}

/**
 * return the score for the given movie and user
 * @param movieName
 * @param userName
 * @param k
 * @return
 */
float
RecommenderSystem::predictMovieScoreForUser(const string &movieName, const string &userName, int k)
{
    return (float)_getRateForCurrentMovie(movieName, userName, k);
}

/**
 * Returns the movie we recommend to userName by a collaborative filtering method
 * @param userName: The name of the customer for whom you want to make a recommendation
 * @param k: k Integer and positive number (parameter In the collaborative filtering algorithm)
 * @return: The movie we recommend to userName by a collaborative filtering method
 */
string RecommenderSystem::recommendByCF(const string &userName, int k)
{
    if (this->_ratingsMap.count(userName) == 0)
    {
        return USER_NOT_FOUND;
    }

    std::string result = std::string();
    double score = -1;

    //for every movie
    for (unsigned int i = 0;
         i < _moviesVector.size(); i++)
    {
        if (this->_ratingsMap[userName][i] == NA) //movie was not rated
        {
            double currentRate = _getRateForCurrentMovie(this->_moviesVector[i], userName, k);
            if (currentRate > score)
            {
                score = currentRate;
                result = this->_moviesVector[i];
            }
        }
    }
    return result;
}








