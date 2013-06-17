#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <math.h>
#include <algorithm>
#include <ctime>
#include "Mode.h"

std::vector<std::vector<std::pair<int, double>>*> vectors;
std::vector<std::vector<std::pair<int, double>>*> notNormalizedVectors;
std::vector<std::vector<std::pair<int, double>>*> sortedVectors;
std::vector<std::vector<double>> dimCountVector;
std::vector<std::set<int>*> invertedIndex;
std::vector<int> popularDimensions;

Mode mode = DEFAULT;

using namespace std;

// returns length of given vector
double length(vector<pair<int, double>>* v){
	double sum = 0;
	for(auto it = v->begin(); it != v->end(); it++)
	{
		sum += it->second * it->second;
	}
	return sqrt(sum);
}

// normalize given vector
void normalize(vector<pair<int, double>>* v){
	double sum = 0;
	double l = length(v);
	for(auto it = v->begin(); it != v->end(); it++)
	{
		it->second /= l;
	}
}

// returns result of multiplying two vectors given by their index
double multiply (int vector1, int vector2){
	double sum = 0;

	auto it2 = vectors[vector2]->begin();
	for (auto it = vectors[vector1]->begin(); it != vectors[vector1]->end(); )
	{
		if(it2 == vectors[vector2]->end())
			break;
		if(it->first == it2->first){
			sum += it->second * it2->second;
			it++;
			it2++;
		}
		else if(it->first > it2->first)
			it2++;
		else
			it++;
	}
	return sum;
}

// returns indices of vectors in given vector's e-neighborhood
set<int> findNeighborhood(set<int>& vectorsIndices, int vectorIndex, double epsilon){
	set<int> results;
	for(auto it = vectorsIndices.begin(); it != vectorsIndices.end(); it++){
		if(multiply(*it, vectorIndex) >= epsilon)
			results.insert(*it);
	}
	results.insert(vectorIndex);
	return results;
}

//creating inverted index
int createInvertedIndex()
{
	for(int i = 0; i < vectors.size(); i++)
	{
		for(int j = 0; j < (*vectors[i]).size(); j++){
			int dim = (*vectors[i])[j].first;
			if(dim+1 > invertedIndex.size()){
				invertedIndex.resize(dim+1);
			}
			if(invertedIndex[dim] == NULL)
			{
				invertedIndex[dim] = new set<int>();
			}
			invertedIndex[dim]->insert(i);
		}
	}
	return invertedIndex.size()-1;
}

//sorting popular dimensions
int sortPopularDimensions()
{
	popularDimensions.clear();
	vector<pair<int, int>> dims;
	dims.resize(invertedIndex.size());
	for(int i = 0; i < dims.size(); i++){
		dims[i].first = 0;
		dims[i].second = i;
	}
	for(auto it = vectors.begin(); it != vectors.end(); it++){
		for(auto it2 = (*it)->begin(); it2 != (*it)->end(); it2++){
			int dim = it2->first;
			dims[dim].first++;
		}
	}
	sort(dims.begin(), dims.end());
	for(auto it = dims.begin(); it != dims.end(); it++){
		popularDimensions.push_back(it->second);
	}
	return popularDimensions.size();
}

vector<int> getPopularDimensions(int v, double epsilon, bool least)
{
	double t = 1 - epsilon * epsilon;
	double sum = 0;
	vector<int> dims;

	int it = 0;
	int inc = 1;
	if(!least){
		it = popularDimensions.size() - 1;
		inc = -1;
	}
	for(int i = 0; i < popularDimensions.size(); i++, it+=inc){
		if(invertedIndex[popularDimensions[it]] == NULL)
			continue;
		if(invertedIndex[popularDimensions[it]]->count(v) > 0){
			for(auto it2 = vectors[v]->begin(); it2 != vectors[v]->end(); it2++){
				if(popularDimensions[it] == it2->first){
					dims.push_back(it2->first);
					sum += it2->second * it2->second;
					if(sum > t){
						return dims;
					}
				}
			}
		}
	}
	return dims;
}

struct sort_pred {
	bool operator()(const std::pair<int,double> &left, const std::pair<int,double> &right) {
		return left.second > right.second;
	}
};

vector<int> getDimensions(int v, double epsilon, bool sorted)
{
	double t = 1 - epsilon * epsilon;
	double sum = 0;
	vector<int> dims;
	vector<pair<int, double>> * vp = vectors[v];
	// maximal dimensions until condition
	if(sorted){
		vp = sortedVectors[v];
	} 
	for(auto it = vp->begin(); it != vp->end(); it++){
		dims.push_back(it->first);
		sum += it->second * it->second;
		if(sum > t){
			return dims;
		}
	}
	return dims;
}

vector<int> getKDimensions(int v, double epsilon)
{
	double t = 1 - epsilon * epsilon;
	vector<int> dims;
	vector<pair<int, double>> * vp = sortedVectors[v];
	// k - maximal dimensions (without condition)
	int k = floor(t*vp->size()+1);
	for(int i = 0; i < k; i++){
		dims.push_back((*vp)[i].first);
	}
	return dims;
}

set<int> getCandidates(int v, double epsilon)
{
	set<int> candidates;

	vector<int> dims;
	switch(mode){
	case DEFAULT:
		dims = getDimensions(v, epsilon, false);
		break;
	case MAX:
		dims = getDimensions(v, epsilon, true);
		break;
	case KMAX:
		dims = getKDimensions(v, epsilon);
		break;
	case MOSTPOPULAR:
		dims = getPopularDimensions(v, epsilon, false);
		break;
	case LEASTPOPULAR:
		dims = getPopularDimensions(v, epsilon, true);
		break;
	}

	for(int i = 0; i < dims.size(); i++)
	{
		for(auto it = invertedIndex[dims[i]]->begin(); it != invertedIndex[dims[i]]->end(); it++)
		{
			candidates.insert(*it);
		}
	}
	return candidates;
}

pair<int, int> readVectors(ifstream &file, bool binary){
	string line;

	int i = 0;
	int j = 0;

	getline(file, line);
	while(getline(file, line))	
	{
		vectors.push_back(new vector<pair<int, double>>());
		notNormalizedVectors.push_back(new vector<pair<int, double>>());
		istringstream iss(line);
		int dim;
		double value;
		while (iss >> dim) {
			iss >> value;
			if (binary){
				(*vectors[i]).push_back(pair<int, double>(dim, 1));
			}
			else{
				(*vectors[i]).push_back(pair<int, double>(dim, value));
				(*notNormalizedVectors[i]).push_back(pair<int, double>(dim, value));
			}
			j++;
		}
		normalize(vectors[i]);
		i++;
	}
	return pair<int, int>(i, j);
}

vector<vector<double>> countDimOcurrences()
{
	for(int i = 0; i < notNormalizedVectors.size(); i++)
	{
		for(int j = 0; j < (*notNormalizedVectors[i]).size(); j++){
			int dim = (*notNormalizedVectors[i])[j].first;
			if(dim+1 > dimCountVector.size()){
				dimCountVector.resize(dim+1);
			}
			dimCountVector[dim].push_back((*notNormalizedVectors[i])[j].second);
		}
	}
	return dimCountVector;
}

void saveBinaryVectorsToFile(string binaryVectorsFileName){
	ofstream outputFile;
	outputFile.open (binaryVectorsFileName);
	outputFile.clear();
	//list of vectors
	for(auto it = vectors.begin(); it != vectors.end(); it++){
		vector<pair<int, double>>* v = new vector<pair<int, double>>(**it);
		//vector == one text
		for(auto it2 = v->begin(); it2 != v->end(); it2++){
			//pair
			outputFile << it2[0].first;
			outputFile << " ";
			outputFile << 1;
			outputFile << " ";
		}
		outputFile << "\n";
	}
	outputFile.close();
}

void sortVectors(){
	sortedVectors.clear();
	for(auto it = vectors.begin(); it != vectors.end(); it++){
		vector<pair<int, double>>* v = new vector<pair<int, double>>(**it);
		sort(v->begin(), v->end(), sort_pred());
		sortedVectors.push_back(v);
	}
}

void processVectors(double epsilon, ofstream& resultsFile1,  ofstream& resultsFile2){
	clock_t start, finish;
	double sortingTime, processingTime = 0;

	if(mode != DEFAULT){
		if(mode == MAX || mode == KMAX){
			cout << "sorting vectors...";
			resultsFile2 << "vectors sorting time: ";
			start = clock();
			sortVectors();
			finish = clock();
		}
		else if(mode == LEASTPOPULAR || mode == MOSTPOPULAR){
			cout << "sorting dimensions by popularity...";
			resultsFile2 << "dimensions sorting time: ";
			start = clock();
			sortPopularDimensions();
			finish = clock();
		}
		sortingTime = (double) (finish - start)/CLOCKS_PER_SEC;
		resultsFile2 << sortingTime << "s" << endl;
		cout << "done" << endl;
		cout << "elapsed time: " << sortingTime << endl;
	}

	vector<int> candidatesVectors;
	vector<int> neighborsVectors;
	for(int i = 0; i < vectors.size(); i++){
		set<int> candidateVector = getCandidates(i, epsilon);
		candidatesVectors.push_back(candidateVector.size());

		start = clock();
		set<int> neighbors = findNeighborhood(candidateVector, i, epsilon);
		neighborsVectors.push_back(neighbors.size());
		finish = clock();
		processingTime += (double) (finish - start);

		resultsFile1 << i << ";";
		resultsFile1 <<  candidateVector.size() << ";";
		resultsFile1 << neighbors.size() << ":";
		for(auto j = neighbors.begin(); j != neighbors.end(); j++){
			resultsFile1 << *j << " ";
		}
		resultsFile1 << endl;
	}

	processingTime = processingTime/CLOCKS_PER_SEC;

	double sum2 = 0;
	for(auto it2 = candidatesVectors.begin(); it2 != candidatesVectors.end(); it2++){
		sum2 += (*it2);
	}

	int minCandidateVal = *min_element(candidatesVectors.begin(), candidatesVectors.end());
	int maxCandidateVal = *max_element(candidatesVectors.begin(), candidatesVectors.end());
	double avgCandidateVal = sum2/candidatesVectors.size();

	double sum3 = 0;
	for(auto it3 = neighborsVectors.begin(); it3 != neighborsVectors.end(); it3++){
		sum3 += (*it3);
	}

	int minNeighborsVal = *min_element(neighborsVectors.begin(), neighborsVectors.end());
	int maxNeighborsVal = *max_element(neighborsVectors.begin(), neighborsVectors.end());
	double avgNeighborsVal = sum3/neighborsVectors.size();

	cout << "processing time: " << processingTime << "s" << endl;
	resultsFile2 << "processing time: " << processingTime << "s" << endl;
	if(mode != DEFAULT){
		cout << "overall time: " << (sortingTime + processingTime) << "s" << endl;
		resultsFile2 << "overall time: " << (sortingTime + processingTime) << "s" << endl;
	}
	resultsFile2 << "max candidate number: " << maxCandidateVal << endl << "min candidate number: " << minCandidateVal << endl << "average candidate number: " << avgCandidateVal << endl;
	resultsFile2 << "max number of neighbors: " << maxNeighborsVal << endl << "min number of neighbors: " << minNeighborsVal << endl << "average number of neighbors: " << avgNeighborsVal << endl << endl;
}