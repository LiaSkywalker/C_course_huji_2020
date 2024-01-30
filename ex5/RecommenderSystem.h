/**
 * @file RecommenderSystem.h
 * @author  Nitzan Rosen
 * @date 05.07 2020
 */

#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H
// ------------------------------ includes ------------------------------
#include <string>
#include <vector>
#include <unordered_map>
// ------------------------------ usings -----------------------------
using std::istringstream;
using std::string;

// ------------------------------ class -----------------------------
/**
 * RecommenderSystem class, followed the wanted API
 */
class RecommenderSystem
{
private:

    /**
     *map of the movies data
     * key: movieName , val: vector of Features
     */
    std::unordered_map<string, std::vector<double>> _moviesMap;

    /**
     * Ratings from the clients table
     * key: user, value: vector of movies ratings
     */
    std::unordered_map<string, std::vector<double>> _ratingsMap;

    /**
     * vector that contains the elements order of ratingMap
     */
    std::vector<string> _moviesVector;

    int _loadMovieTable(std::ifstream &is);

    int _loadRatingsMap(std::ifstream &is);

    int _readFirstRatingFileLine(const string &line, std::vector<string> &movies);

    int _readSingleRating(const string &line);

    int _readFile(const string &path, char Token);

    double _calculateAverage(const std::vector<double> &vector);

    bool _normallizeVec(std::vector<double> &vector, double avg);

    double _calculateNorm(const std::vector<double> &vector1);

    double _calculateAngle(const std::vector<double> &vector1, const std::vector<double> &vector2);

    string _findMostSimilarAngle(const string &userName, const std::vector<double> &prefVec);

    double
    _getRateForCurrentMovie(const std::string &movieName, const std::string &userName, int k);

public:

    /**
     * defult constructor of RecommenderSystem class
     */
    RecommenderSystem();

    /**
     * accept as two input strings containing the path of the two input files
     * and upload the information to the recommendation system.
     * @param moviesAttributesFilePath
     * @param userRanksFilePath
     * @return The function returns 0 to success and -1 to failure.
     */
    int loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath);

    /**
     * accept as a string input representing the client name
     * and return a string representing the movie name recommended
     * by algorithm Recommendation by Content.
     * @param userName
     * @return
     */
    string recommendByContent(const string &userName);

    /**
     * Returns a decimal positive number which is the prediction of userName rating for movieName
     * by the collaborative filtering method.
     * @param movieName
     * @param userName
     * @param k
     * @return
     */
    float predictMovieScoreForUser(const string& movieName, const string& userName, int k);

    /**
     * Returns the movie we recommend to userName by a collaborative filtering method
     * @param userName
     * @param k
     * @return
     */
    string recommendByCF(const string& userName, int k);

};


#endif //EX5_RECOMMENDERSYSTEM_H
