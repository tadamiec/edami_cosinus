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

int main(int argc, char *argv[]) {

	const string file = "data.txt";
	ifstream in(file.c_str());
	if (!in) {
		cout << endl << "Error reading " << file << endl;
		exit(EXIT_FAILURE);
	}
	vector<int> nums;
	for (string s; getline(in,s); ) {
		nums.push_back( toInt(s) );
	}
	cout << endl;
	for (size_t i = 0; i < nums.size(); i++) {
		cout << nums.at(i) << endl;
	}    

}