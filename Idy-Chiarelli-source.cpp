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

class Ciphertext;
class Dictionary;
class Key;

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
template<typename T>
void print_vec(const vector<T> &vec);
void string_to_vector(string ct_string, vector<int> &ct_vec);
string brute(Ciphertext& ct, Dictionary& dict, Key& key);
string bruter(Ciphertext& ct, Dictionary& dict, Key& key, int index);
int naive(string &ct);

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

class Ciphertext{
public:
    Ciphertext(string &ct) {
        stringstream ctss(ct);
        string current;
        length = 0;
        int pos = 0;
        while(ctss >> current) {
            length++;
            stringstream ss(current);
            string num;
            vector<int> word;
            while(getline(ss, num, ',')) word.push_back(stoi(num));
            words.push_back(make_pair(word, pos++));
        }

        // sort in ascending order of word length
        sort(words.begin(), words.end(),
            [](const pair<vector<int>, int> &a, const pair<vector<int>, int> &b) -> bool
            { return a.first.size() < b.first.size(); });

     }

    size_t size() { return length; }
    const vector<int>& operator[](size_t i) const { return words[i].first; }
    void print() {
        for(int i = 0; i < words.size(); i++) {
            for(int j = 0; j < words[i].first.size(); j++) {
                cout << words[i].first[j];
                if(j < words[i].first.size() - 1) cout << ',';
            }
            if(i < words.size() - 1) cout << ' ';
            else cout << '\n';
        }
    }
    void print_in_place() {
        vector<pair<vector<int>, int>> temp = words;
        sort(temp.begin(), temp.end(),
             [](const pair<vector<int>, int> &a, const pair<vector<int>, int> &b) -> bool
             { return a.second < b.second; });
        for(int i = 0; i < temp.size(); i++) {
            for(int j = 0; j < temp[i].first.size(); j++) {
                cout << temp[i].first[j];
                if(j < temp[i].first.size() - 1) cout << ',';
            }
            if(i < temp.size() - 1) cout << ' ';
            else cout << '\n';
        }
    }

    void reorder_guess(string& guess) {
        vector<pair<string, int>> v;
        stringstream ss(guess);
        string word;
        int i = words.size() - 1;
        while(ss >> word) v.push_back(make_pair(word, words[i--].second));
        sort(v.begin(), v.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) -> bool
             { return a.second < b.second; });
        stringstream out;
        for(int i = 0; i < v.size(); i++) out << v[i].first << ' ';
        guess = out.str();
    }

private:
    size_t length;
    vector<pair<vector<int>, int>> words;
    static const int by_amount[29];
};


// this is very space inefficient as it stores 3 copies of the dict.
// a better implementation would allocate space once and use pointers
// for the 3 different containers
class Dictionary {
public:
    Dictionary(const string& path) {
        sorted = vector<vector<string>>(29);
        tries = vector<Trie>(29);
        ifstream ifs(path);
        string line;
        if(ifs) {
            while(getline(ifs, line)) {
                all.insert(line);
                if(line.size() > sorted.size()) {
                    sorted.resize(line.size() + 1);
                    tries.resize(line.size() + 1);
                }
                sorted[line.size()].push_back(line);
                tries[line.size()].addWord(line);
            }
        }
    }

    bool has(const string& word) { return all.find(word) != all.end(); }

    bool next_can_be(const string& prefix, char c, int len) { return tries[len].getNodeAt(prefix)->findChild(c) != NULL; }

    size_t size() { return all.size(); }

    // returns vector containing words of size size
    const vector<string>& get_words(int size) {
        if(size > sorted.size()) return sorted[0];
        return sorted[size];
    }

    // returns subarray of strings that fit criteria,
    // empty array if nonexistant
    // CALLER MUST FREE
    vector<string>* filter(string q, int len) {
        if(len > sorted.size() || len != q.size()) return NULL;
        vector<string>* ret = new vector<string>(sorted[len].size());
        auto it = copy_if(sorted[len].begin(), sorted[len].end(), ret->begin(),
                [q](string m) {
                    for(int i = 0; i < q.size(); i++) {
                        if(q[i] == ' ') continue;
                        if(q[i] != m[i]) return false;
                    }
                    return true;
                });
        ret->resize(distance(ret->begin(), it));
        if(ret->size() == 0) {
            delete ret;
            return NULL;
        }
        return ret;
    }

    // randomizes order of strings within sublist
    // might be better than alphabetical
    // makes overall cryptanalysis less deterministic
    void randomize() {
        for(int i = 0; i < sorted.size(); i++) {
            random_shuffle(sorted[i].begin(), sorted[i].end());
        }
    }

    // prioritizes certain words by putting them at the front of their appropriate sublist
    // good for known / chosen plaintexts.
    // would be more efficient with lists, but lists make sorting harder.
    void prime(const string& word) {
        int size = word.size();
        if(size > sorted.size()) return;
        auto it = find(sorted[size].begin(), sorted[size].end(), word);
        if(it == sorted[size].end()) return;
        string found = *it;
        sorted[size].erase(it);
        sorted[size].insert(sorted[size].begin(), found);
    }

private:
    set<string> all;
    vector<Trie> tries;
    vector<vector<string>> sorted;
};

class Key {
public:
    Key() : key(103, -1) {}
    Key(vector<int> v) : key(v) { if(key.size() < 103) key.resize(103,-1); }

    void randomize() {
        vector<int> rand_nums;
        for (int i = 0; i < 103; i++) rand_nums.push_back(i);
        random_shuffle(rand_nums.begin(), rand_nums.end());
        key = rand_nums;
    }

    void clear() {
        key = vector<int>(102, -1);
    }

    // inserts guess number at index representing letter c
    // returns true on success, false on failure
    // fails if no more room for that letter
    bool insert(char c, int n) {
        if(c < 'a' || c > 'z') return false;
        if(n < 0 || n > 102) return false;
        int index = c - 'a';
        int limit = index < 25 ? offsets[index + 1] : 103;
        for(int i = offsets[index]; i < limit; i++) {
            if(key[i] < 0 || key[i] == n) {
                key[i] = n;
                return true;
            }
        }
        return false;
    }

    bool insert(string s, vector<int> v) {
        for(int i = 0; i < s.size(); i++) {
            if(!insert(s[i], v[i])) {
                remove(vector<int>(v.begin(), v.begin()+i));
                return false;
            }
        }
        return true;
    }

    // returns vector of values corresponding to c
    vector<int> get_values(char c) {
        if(c < 'a' || c > 'z') return vector<int>();
        int index = c - 'a';
        int start = offsets[index];
        int end = index < 25 ? offsets[index + 1] : 103;
        return vector<int>(key.begin() + start, key.begin() + end);
    }

    char get_letter(int n) {
        if(n < 0 || n > 102) return 0;
        int index = find(key.begin(), key.end(), n) - key.begin();
        if (index > 102) return 0;
        for(int i = 0; i < 26; i++) {
            if(index >= offsets[i] && i < 25 && index < offsets[i + 1]) return letters[i];
        }
        return 'z';
    }

    // removes n from key vector
    void remove(int n) {
        auto it = find(key.begin(), key.end(), n);
        if(it != key.end()) *it = -1;
    }

    // batch remove
    void remove(const vector<int>& v) {
        for(int i = 0; i < v.size(); i++) remove(v[i]);
    }

    void print() {
        for(int i = 0; i < 26; i++) {
            int limit = i < 25 ? offsets[i+1] : 103;
            cout << letters[i] << ": ";
            for(int j = offsets[i]; j < limit; j++) {
                cout << key[j];
                if(j < limit - 1) cout << ',';
                else cout << endl;
            }
        }
    }

    Ciphertext encrypt(string s) {
        string ct;
        for(int i = 0; i < s.size(); i++) {
            if(isspace(s[i])) {
                ct += ' ';
                continue;
            }
            int index = s[i] - 'a';
            int l = freqs[index];
            ct += to_string(key[(i % l) + offsets[index]]);
            if(i < s.size() - 1) ct += ",";
        }
        return Ciphertext(ct);
    }

    bool complete() {
        return find(key.begin(), key.end(), -1) == key.end();
    }
private:
    vector<int> key;
    static const int offsets[26]; //  = {0, 8, 9, 12, 16, 29, 31, 33, 39, 46, 47, 48, 52, 54, 61, 69, 71, 72, 78, 84, 93, 96, 97, 99, 100, 102};
    static const int freqs[26]; // = {8, 1, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2, 1, 6, 6, 9, 3, 1, 2, 1, 2, 1};
    static const string letters; // = "abcdefghijklmnopqrstuvwxyz";
};

const int Key::offsets[26] = {0, 8, 9, 12, 16, 29, 31, 33, 39, 46, 47, 48, 52, 54, 61, 69, 71, 72, 78, 84, 93, 96, 97, 99, 100, 102};
const int Key::freqs[26] = {8, 1, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2, 1, 6, 6, 9, 3, 1, 2, 1, 2, 1};
const string Key::letters = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, char** argv)
{
    if(argc < 2)
        cout << "Specify option -n at cmdline to enable naive guessing" << endl << endl;

    string cipher_text;
    cout << "Enter ciphertext: " << endl;
    getline(cin, cipher_text);

    Ciphertext ct = Ciphertext(cipher_text);

    int nguess = naive(cipher_text);
    if(argc == 2 && string(argv[1]) == "-n") {
        cout << "Naive guess is: " << endl;
        cout << plaintexts[nguess] << endl << endl;
    }

    Dictionary dict = Dictionary("english_words.txt");
    dict.randomize();
    for(int i = 0; i < plaintexts.size(); i++) {
        stringstream text(plaintexts[i]);
        string word;
        while(text >> word) dict.prime(word);
    }

    // Key ekey = Key();
    // string message = plaintexts[3];
    // ekey.randomize();

    // cout << endl << "Message is: " << endl << message << endl;
    // cout << endl << "Key is: " << endl;
    // ekey.print();

    // Ciphertext ct = ekey.encrypt(message);



    cout << endl << "Ciphertext is: " << endl;
    ct.print_in_place();

    cout << endl << "----------------------------------------" << endl;

    Key key = Key();
    string dec = brute(ct, dict, key);

    ct.reorder_guess(dec);

    cout << "Plaintext guess is: " << endl;
    cout << dec << endl;

    cout << endl << "Key guess is: " << endl;
    key.print();
    if(!key.complete()) cout << "Note: Key is not complete." << endl;

    cout << endl << "everything went better than expected." << endl;
    return 0;
}

// naively solves part1 by looking at the length of the first words
int naive(string &ct) {
    stringstream ss(ct);
    string word;
    vector<int> words;
    int len = 0;
    ss >> word;
    stringstream sss(word);
    string word2;
    while(getline(sss, word2, ',')) len++;
    switch(len) {
    case 8:
        return 0;
    case 10:
        return 1;
    case 4:
        return 2;
    case 9:
        return 3;
    case 11:
        return 4;
    }
    return -1;
}

string brute(Ciphertext& ct, Dictionary& dict, Key& key) {
    return bruter(ct, dict, key, ct.size() - 1);
}

string bruter(Ciphertext& ct, Dictionary& dict, Key& key, int index) {
    vector<int> word = ct[index];
    int size = word.size();
    string guess;
    string match;
    bool found = false;
    for(int i = 0; i < size; i++) {
        char c;
        if(c = key.get_letter(word[i])) match += c;
        else match += ' ';
    }
    vector<string>* matches = dict.filter(match, size);
    if(matches) {
        for(int i = 0; i < matches->size(); i++) {
            Key nkey = key;
            if(nkey.insert(matches->at(i), word)) {
                guess = matches->at(i);
            }
            else continue;

            string r;
            if(index > 0) {
                r = bruter(ct, dict, nkey, index - 1);
                if(r == "") continue;
                else {
                    guess += ' ' + r;
                    key = Key(nkey);
                    found = true;
                    break;
                }
            }
        }
        delete matches;
        if(!found) guess = "";
    }
    else if(index == 0 && match.find(' ') == string::npos) guess = match;
    else guess = "";

    return guess;
}

template<typename T>
void print_vec(const vector<T> &vec)
{
    for(auto i = vec.begin(); i != vec.end(); i++)
        cout << *i << ',';
    cout << endl;
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

