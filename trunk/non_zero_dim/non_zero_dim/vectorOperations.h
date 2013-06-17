#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <math.h>
#include <algorithm>
#include <ctime>

// returns length of given vector
double length(std::vector<std::pair<int, double>>* v);

// normalize given vector
void normalize(std::vector<std::pair<int, double>>* v);

// returns result of multiplying two vectors given by their index
double multiply (int vector1, int vector2);

// returns indices of vectors in given vector's e-neighborhood
std::vector<int> findNeighborhood(std::set<int>& vectorsIndices, int vectorIndex, double epsilon);

//creating inverted index
int createInvertedIndex();

//sorting popular dimensions
int sortPopularDimensions();

std::vector<int> getPopularDimensions(int v, double epsilon, bool least = false);

struct sort_pred;

struct sort_pred2;

std::vector<int> getDimensions(int v, double epsilon, bool sorted);

std::vector<int> getKDimensions(int v, double epsilon);

std::set<int> getCandidates(int v, double epsilon);

std::pair<int, int> readVectors(std::ifstream &file, bool binary);

std::vector<std::vector<double>> countDimOcurrences();

void saveBinaryVectorsToFile(std::string binaryVectorsFileName);

void sortVectors();

void processVectors(double epsilon, std::ofstream& resultsFile1, std::ofstream& resultsFile2);
