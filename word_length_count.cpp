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
		words[line.length()].push_back(line);
	}
	reverse(words.begin(), words.end());
}

int main()
{
	vector<vector<string>> words(29);
	string dict = "english_words.txt";
	
	generate_word_list(words, dict);

  cout << "{";
	for (int i = words.size() - 1; i >= 0 ; i--)
	{
		// for (auto j : i) cout << j << endl;
      cout << words[i].size();
      if(i > 0) cout << ", ";
      else cout << "}" << endl;
	}

	return 0;
}
