#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <time.h>
#include <ctype.h>
#include <algorithm>
#include <utility>
#include <tuple>

#include "trie.cpp"
using namespace std;

void sort_ciphertext(string ct, vector<vector<pair<string, int>>> &ct_sorted);
void create_key(map<char, vector<int>> &key);
void print_key(map<char, vector<int>> &key);
void print_ciphertext(vector<int> &cipher_text);
void encrypt(string &pt, vector<int> &ct, map<char, vector<int>> &key);
string decrypt(vector<int> &ct, map<char, vector<int>> &key);
void delimit_ciphertext(string &ct, vector<int> &ct_nums);
void index_dict(const string &path, set<string> &dict);
void print_freqs(const vector<int> &cipher_text);
void sort_dict(const string &dict, vector<vector<string>> &words);
void print_vec(const vector<int> &vec);
void string_to_vector(string ct_string, vector<int> &ct_vec);

const vector<string> plaintexts = {
    "sconced pouch bogart lights coastal philip nonexplosive shriller outstripping "
    "underbidding nightshirts colly editorializer trembler unresistant resins anthrax "
    "polypus research parapets gratuitous corespondent pyrometer breveted psychoneurosis "
    "scoutings almightily endoscopes cyanosis kayaker hake william blunted incompressibility "
    "lacer cumquat aniline agileness academe obstacle toothpick nondistribution rebukes concertizes "
    "industrialist plenipotentiary swagmen kevils dredge ostensible atavistic p",

    "revelation revering rightest impersonalize juliennes scientists reemphasizing "
    "propose crony bald pampering discharged lincoln authoresses interacted laked "
    "bedmaker intolerably beltlines warningly worldliness serologic bottom guessed "
    "hangup vitiates snaky polypous manifolding sweatshirt divisiveness decapitation "
    "musketry versers pizzas aperies reorganizes fender presentations thereuntil fly "
    "entrapped causewayed shaped freemasonry nudging efflorescence hydrated zazen exegeses fracas unprogressivel",

    "boca ingestion financed indexer generalships boldfaces boughed tesla videotext "
    "expiation brasil kinglets duality rattlesnakes mailability valvelet whimperingly "
    "corralled stench fatal inapplicably uncourageous bubblers req jesse foetor bulgaria "
    "hueless pickwicks intrans gargles purgations subvarieties pettier caste decongestive "
    "replanned continual bribed pirog learning currier careers rustling swankily onetime "
    "prearranges stowage responder inwrapped coign concubines gyrus delta tripled sleetier m",

    "allocated demonstration cocoanuts imprecisions mikado skewer ennobled cathect universalizes "
    "lucidity soldierly calor narthexes jiggling mutinousness relight mistook electra ogles chirk "
    "unsympathetic indorsed theomania gaper moths aerospace riboflavin sensorium teariest luckiest "
    "dither subparts purslane gloam dictatory conversed confides medullary fatsos barked yank chained "
    "changes magicians movables ravenousness dipsomaniac budded windjammers stayers dixie tepidities desexualization boodled dile",

    "shellackers ballets unselfishly meditatively titaness highballed serenaders ramshorns "
    "bottlenecks clipsheet unscriptural empoisoned flocking kantians ostensibilities heigh "
    "hydrodynamics qualifier million unlading distributed crinkliest conte germ certifier weaklings "
    "nickeled watson cutis prenticed debauchery variously puccini burgess landfalls nonsecular "
    "manipulability easterlies encirclements nescient imperceptive dentally sudsers reediness "
    "polemical honeybun bedrock anklebones brothering narks"
};

int main()
{
    string message;
    vector<int> cipher_text;
    map<char, vector<int>> key;
    set<string> dict;
    vector<vector<string>> words(29);

    srand(time(NULL));
    create_key(key);

    vector<int> key_guess(103, -1);


    index_dict("english_words.txt", dict);
    cout << "There are " << dict.size() << " words in the dictionary" << endl;
    sort_dict("english_words.txt", words);

    vector<vector<pair<string, int>>> ct_sorted(28);
    string cipher_text_2;
    cout << "Enter ciphertext: " << endl;
    getline(cin, cipher_text_2);
    sort_ciphertext(cipher_text_2, ct_sorted);

    vector<Trie> tries(words.size());
    for(int i = 0; i < tries.size(); i++) {
        for(int j = 0; j < words[i].size(); j++) {
            tries[i].addWord(words[i][j]);
        }
    }

    auto curr_len = ct_sorted.begin();
    while(curr_len->size() == 0) curr_len++;
    vector<int> curr_word;
    string_to_vector((curr_len->begin())->first, curr_word);
    auto curr_guess = words[curr_word.size()].begin();

    static int freqs[] = {8, 1, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2, 1, 6, 6, 9, 3, 1, 2, 1, 2, 1};
    static int offsets[] = {0, 8, 9, 12, 16, 29, 31, 33, 39, 46, 47, 48, 52, 54, 61, 69, 71, 72, 78, 84, 93, 96, 97, 99, 100, 102};
    bool sat = false;
    bool exists = false;
    for(int i = 0; i < curr_guess->size(); i++) {
		char c = (*curr_guess)[i];
		int index = c - 97;
		int off = offsets[index];
		while(key_guess[off] >= 0 && off < 103 && off < offsets[index+1]) off++;
		if(key_guess[off] >= 0) {
			sat = true;
			break;
		}
		
		// only inserts into the key if that ciphertext number hasn't been
		// used already e.g. '23,23,23' only will put one '23' in the key
		for (auto j : key_guess) 
			if (j == curr_word[i]) exists = true;
		if (!exists) key_guess[off] = curr_word[i];
    }

    print_vec(curr_word);
    cout << endl;
    print_vec(key_guess);
    cout << endl;

    /*
     * sort_dict test code
    string string_dict = "english_words.txt";
    sort_dict(string_dict, words);
    for (auto i: words) for (auto j : i) cout << j << endl;
    */

    
    /*message = plaintexts[0];
    encrypt(message, cipher_text, key);
    cout << decrypt(cipher_text, key) << endl;
    print_ciphertext(cipher_text);
    */

}

void print_vec(const vector<int> &vec)
{
  for(auto i = vec.begin(); i != vec.end(); i++) cout << *i << ',';
}

void string_to_vector(string ct_string, vector<int> &ct_vec) {
	string letter;
	stringstream ss(ct_string);

	while (getline(ss, letter, ',')) {
		ct_vec.push_back(stoi(letter));
	}
}

void sort_ciphertext(string ct, vector<vector<pair<string, int>>> &ct_sorted)
{
	int line_counter = 0;
	stringstream ss(ct);
	string line;
	while (getline(ss, line, ' ')) {
		pair<string, int> word(line, line_counter);
		ct_sorted[count(line.begin(), line.end(), ',') + 1].push_back(word);
		line_counter++;
	}
	reverse(ct_sorted.begin(), ct_sorted.end());
}

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

// Print out key
void print_key(map<char, vector<int>> &key)
{
    for (auto i : key) {
        cout << i.first << ": ";
        for (int j : i.second) cout << j << " ";
        cout << endl;
    }
}

void print_ciphertext(vector<int> &cipher_text)
{
    for (int i = 0; i < cipher_text.size(); i++) {
        if (cipher_text[i] != -1) {
            cout << cipher_text[i];
            if (i != cipher_text.size() - 1 && cipher_text[i+1] != -1) cout << ",";
        }
        else cout << " ";
    }
    cout << endl;
}

void encrypt(string &pt, vector<int> &ct, map<char, vector<int>> &key)
{
    int string_counter = 0;
    for (char c : pt) {
        if (isspace(c)) ct.push_back(-1);
        else {
            vector<int> temp = key.find(c)->second;
            // This is the scheduling algo
            ct.push_back(temp[string_counter % temp.size()]);
        }
        string_counter++;
    }
}

string decrypt(vector<int> &ct, map<char, vector<int>> &key)
{
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

void delimit_ciphertext(string &ct, vector<int> &ct_nums)
{
    istringstream ss(ct);
    string word;
    while(getline(ss, word, ' ')) {
        istringstream ss_token(word);
        string character;
        while (getline(ss_token, character, ',')) {
            ct_nums.push_back(stoi(character));
        }
        if (!ss.eof()) ct_nums.push_back(-1);
    }
}

void index_dict(const string &path, set<string> &dict)
{
    ifstream ifs(path);
    if(!ifs) {
        cerr << "Error opening " << path << endl;
        exit(1);
    }
    string line;
    while(getline(ifs, line)) dict.insert(line);
}

void print_freqs(const vector<int> &cipher_text)
{
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
}

void sort_dict(const string &dict, vector<vector<string>> &words)
{
	ifstream ifs(dict);
	string line;

	while (getline(ifs, line))
	{
		words[line.length()].push_back(line);
	}
	// reverse(words.begin(), words.end());
}

