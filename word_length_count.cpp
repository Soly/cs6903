#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

void generate_word_list(vector<vector<string>> &words, string dict)
{
	ifstream ifs(dict);
	string line;
	
	while (getline(ifs, line))
	{
		words[line.length() - 1].push_back(line);
	}
	reverse(words.begin(), words.end());
}

int main()
{
	vector<vector<string>> words(28);
	string dict = "english_words.txt";
	
	generate_word_list(words, dict);
	
	for (auto i : words)
	{
		for (auto j : i) cout << j << endl;
	}
	
	return 0;
}
