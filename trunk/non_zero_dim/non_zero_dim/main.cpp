#include "vectorOperations.h"
#include "Mode.h"

using namespace std;

clock_t start, finish;
double epsilon = 0.9;
const string outputFileName1 = "results.log";
const string outputFileName2 = "resultsInfo.log";
ofstream resultsFile1;
ofstream resultsFile2;
ofstream resultsFile3;
double time1, time2, time3, time4, time5;

void prepareData(ifstream &dataFile, bool binary){
	pair<int, int> pair;
	cout << "reading vectors...";
	if (binary){
		pair = readVectors(dataFile, true);
		string binaryVectorsFileName = "dataFiles/binaryData.mat";
		saveBinaryVectorsToFile(binaryVectorsFileName);
	} else {
		pair = readVectors(dataFile, false);
	}
	cout << "done" << endl << endl;

	cout << "number of vectors: " << pair.first << endl;
	cout << "number of distinct dimensions: " << pair.second << endl << endl;
	resultsFile2 << "number of vectors: " << pair.first << endl;
	resultsFile2 << "number of distinct dimensions: " << pair.second << endl;

	cout << "creating inverted index...";
	start = clock();
	int dimNumber = createInvertedIndex();
	finish = clock();
	cout << "done" << endl;
	time3 = (double) (finish - start)/CLOCKS_PER_SEC;
	cout << "elapsed time: " << time3 << "s" << endl << endl;

	resultsFile2 << "dimension number: " << dimNumber << endl << endl;
	resultsFile2 << "inverted index creation: " << time3 << "s" << endl << endl;
}

void test (){
	for(int i = 1; i<6; i++){
		mode = (Mode) i;
		cout << endl << "mode: " << modeName(mode) << endl;
		resultsFile2 << "mode: " << modeName(mode) << endl;
		processVectors(epsilon, resultsFile1, resultsFile2);
	}
}

string getTimeString() {
	time_t rawtime;
	struct tm timeinfo;
	char ret[80];

	time (&rawtime);
	localtime_s (&timeinfo, &rawtime);

	strftime (ret, 80, "%Y-%m-%d_%H-%M-%S", &timeinfo);

	return std::string(ret);
}

int main (int argc, const char* argv[]){

	string fileName, dataTypeModeName;

	cout << "COSINE NEIGHBORHOOD" << endl << endl;

	cout << "Please enter the name of the file to process:" << endl;
	cin >> fileName;

	//opening file in working directory
	ifstream dataFile(fileName);
	//if file not found search in dataFiles directory
	if(!dataFile.good())
		dataFile = ifstream("dataFiles/" + fileName);
	if(!dataFile.good()){
		cout << "Could not open file with specified name" << endl;
		system("pause");
		return 0;
	}

	resultsFile1 = ofstream(fileName + "-results-" + getTimeString() + ".log");
	resultsFile2 = ofstream(fileName + "-resultsInfo-" + getTimeString() + ".log");
	resultsFile3 = ofstream(fileName + "-resultsDimData-" + getTimeString() + ".log");

	cout << "Please select type of data on which you want to operate:" << endl;
	cout << "1. Real valued vectors." << endl;
	cout << "2. Binary valued vectors." << endl;

	unsigned int dataTypeMode;
	cin >> dataTypeMode;

	bool binary = false;

	if (dataTypeMode == 2){
		dataTypeModeName = "binary";
		binary = true;
	} else {
		if(dataTypeMode != 1)
			cout << "Wrong input. Assuming real valued vectors." << endl;
		dataTypeModeName = "real";
		binary = false;
	}

	resultsFile2 << "File name: " << endl << fileName << endl << endl;
	resultsFile2 << "Type of vectors' values: " << endl << dataTypeModeName << endl << endl;

	prepareData(dataFile, binary);

	cout << "Please enter epsilon value:" << endl;
	cin >> epsilon;
	resultsFile2 << "Epsilon: " << endl << epsilon << endl << endl;

	cout << "Please select vector selection mode: " << endl;
	int i = 1;
	for(; i<6; i++){
		cout << i << ". " << modeName((Mode) i) << endl;
	}
	cout << i << ". " << "test (run all modes)" << endl;

	unsigned int modeNumber;
	cin >> modeNumber;

	if(modeNumber > 5)
		test();
	else{
		mode = (Mode) modeNumber;
		cout << endl << "mode: " << modeName(mode) << endl;
		resultsFile2 << "mode: " << modeName(mode) << endl;
		processVectors(epsilon, resultsFile1, resultsFile2);
	}

	cout << endl << "Do you want to create file with dimension statistics? (yes/no)" << endl;
	string answer;
	cin >> answer;
	if (answer == "yes"){
		vector<vector<double>> dimOcurrences = countDimOcurrences();
		for(int i = 0; i < dimOcurrences.size(); i++){
			double sum = 0;
			int minDimVal, maxDimVal;
			double avgDimVal;
			vector<double> vector = dimOcurrences[i];
			if (vector.size()==0){
				minDimVal = 0;
				maxDimVal = 0;
				avgDimVal = 0;
			} else {
				for(auto it = vector.begin(); it != vector.end(); it++){
					sum += (*it);
				}

				minDimVal = *min_element(vector.begin(), vector.end());
				maxDimVal = *max_element(vector.begin(), vector.end());
				avgDimVal = sum/vector.size();
			}

			resultsFile3 << i << " : " << maxDimVal << " : " << minDimVal << " : " << avgDimVal << endl;
		}
	}

	system("pause");
}
