#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void string_to_vector(string ct_string, vector<int> &ct_vec) {
	string letter;
	stringstream ss(ct_string);
	
	while (getline(ss, letter, ',')) {
		ct_vec.push_back(stoi(letter));
	}
}

int main() {
	vector<int> ct;
	string ct_string;
	
	cout << "Provide ciphertext: " << endl;
	getline(cin, ct_string);
	string_to_vector(ct_string, ct);
	
	for (auto i : ct) cout << i << endl;
}
