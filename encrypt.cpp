#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <time.h>
#include <ctype.h>
#include <algorithm>
#include <utility>
using namespace std;

void create_key(map<char, vector<int>> &key)
{
  static string letters = "abcdefghijklmnopqrstuvwxyz";
  static int freqs[] = {8, 1, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2, 1, 6, 6, 9, 3, 1, 2, 1, 2, 1};
  vector<int> rand_nums;
  for (int i = 0; i <= 102; i++) rand_nums.push_back(i);
  random_shuffle(rand_nums.begin(), rand_nums.end());
  int rand_index = 0;

  for (int i = 0; i < letters.size(); i++)
	{
		vector<int> key_list;
		for (int j = 0; j < freqs[i]; j++)
		{
			key_list.push_back(rand_nums[rand_index++]);
		}

    key[letters[i]] = key_list;
  }
}


//Print out key
void print_key(map<char, vector<int>> &key) {
	for (auto i : key)
	{
		cout << i.first << ": ";
		for (int j : i.second) cout << j << " ";
		cout << endl;
	}
}

void encrypt(string &pt, vector<int> &ct, map<char, vector<int>> &key) {
  int string_counter = 0;
  for (char c : pt)
    {
      if (isspace(c)) ct.push_back(-1);
      else
        {
          vector<int> temp = key.find(c)->second;

          // This is the scheduling algo
          ct.push_back(temp[string_counter % temp.size()]);
        }
      string_counter++;
    }
}

string decrypt(vector<int> &ct, map<char, vector<int>> &key) {
  vector<char> rkey(102);
  for(char i = 'a'; i <= 'z'; i++) {
    vector<int> curr = key.find(i)->second;
    for(int j = 0; j < curr.size(); j++) {
      rkey[curr[j]] = i;
    }
  }
  string pt = "";
  for(int i = 0; i < ct.size(); i++) {
    if(ct[i] < 0) {
      pt += ' ';
      continue;
    }
    pt += rkey[ct[i]];
  }
  return pt;
}

void delimit_ciphertext(string ct, vector<int> &ct_nums)
{
	istringstream ss(ct);
	string word;
	while(getline(ss, word, ' '))
    {
      istringstream ss_token(word);
      string character;
      while (getline(ss_token, character, ','))
        {
          ct_nums.push_back(stoi(character));
        }
      if (!ss.eof()) ct_nums.push_back(-1);
    }
}

void index(const string &path, set<string> &dict) {
  ifstream ifs(path);
  if(!ifs) {
    cerr << "Error opening " << path << endl;
    exit(1);
  }
  string line;
  while(getline(ifs, line)) dict.insert(line);
}

int main()
{
	string message;
	vector<int> cipher_text;
	map<char, vector<int>> key;
	srand(time(NULL));
	create_key(key);
  // print_key(key);
  // cout << "Provide a message: ";
  // getline(cin, message);
  // delimit_ciphertext(message, cipher_text);

  cout << "Top 10 words" << endl;
 set<string> dict;
  index("english_words.txt", dict);
  for(auto i = dict.begin(), j = next(dict.begin(), 10); i != j; i++) {
    cout << *i << endl;
  }
  cout << "There are " << dict.size() << " words in the dictionary" << endl;





  /*
   message = "sconced pouch bogart lights coastal philip nonexplosive shriller outstripping underbidding nightshirts colly editorializer trembler unresistant resins anthrax polypus research parapets gratuitous corespondent pyrometer breveted psychoneurosis scoutings almightily endoscopes cyanosis kayaker hake william blunted incompressibility lacer cumquat aniline agileness academe obstacle toothpick nondistribution rebukes concertizes industrialist plenipotentiary swagmen kevils dredge ostensible atavistic p";
   // encrypt(message, cipher_text, key);
  for (int i = 0; i < cipher_text.size(); i++)
	{
    if (cipher_text[i] != -1)
		{
			cout << cipher_text[i];
      if (i != cipher_text.size() - 1 && cipher_text[i+1] != -1) cout << ",";
    }
		else cout << " ";
  }

	cout << endl;

  vector<pair<int, int> > freqs(103);
  for(int i = 0; i < freqs.size(); i++) {
    freqs[i].first = i;
    freqs[i].second = 0;
  }
  for(int i = 0; i < cipher_text.size(); i++) {
    if(cipher_text[i] > 0) freqs[cipher_text[i]].second++;
  }

  sort(freqs.begin(), freqs.end(), [](pair<int,int> x, pair<int, int> y){return x.second > y.second;});
  for(int i = 0; i < freqs.size(); i++) cout << freqs[i].first << ": " << freqs[i].second << endl;
  */
  // cout << decrypt(cipher_text, key) << endl;
	return 0;
}
