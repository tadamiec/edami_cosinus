#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

int toInt(const string& s) {
	stringstream convert;
	convert << s;
	int i;
	convert >> i;
	return i;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}    
	return elems;
}


//dzielenie stringa
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

vector<int> parseFileToVector(){
	const string file = "data.txt";
	ifstream in(file.c_str());
	if (!in) {
		cout << endl << "Error reading " << file << endl;
		exit(EXIT_FAILURE);
	}
	vector<int> nums;
	for (string s; getline(in,s); ) {
		std::vector<std::string> x = split(s, ' ');
		for (size_t i = 0; i < x.size(); i++) {
			nums.push_back( toInt(x.at(i)) );
		}
	}
	return nums;
}

//int main(int argc, char *argv[]) {
//
//	vector<int> nums;
//	nums = parseFileToVector();
//
//	ofstream myfile;
//	myfile.open ("parseData.txt");
//	for (size_t i = 0; i < nums.size(); i++) {
//		myfile << nums.at(i) << endl;
//	}
//	myfile.close();
//
//
//	/*cout << endl;
//	for (size_t i = 0; i < nums.size(); i++) {
//	cout << nums.at(i) << endl;
//	}*/    
//
//}