vector<int> encode(const string &s);
vector<int> encode(const vector<int> &s);
vector<int> encode(vector<int>::iterator start, vector<int>::iterator end);

int main() {
    map<vector<int>, vector<string> > encodings;
    for(auto i = dict.begin(); i != dict.end(); i++) {
        vector<int> e = encode(*i);
        encodings[e].push_back(*i);
    }

    vector<int> ct_encode = encode(cipher_text.begin(), cipher_text.end());
    for(auto i = ct_encode.begin(); i != ct_encode.end(); i++)
        cout << *i << ',';
    cout << endl;

    vector<string> result = encodings[ct_encode];
    for(auto i = result.begin(); i != result.end(); i++) {
        cout << *i << endl;
    }

    /*
      for(auto i = encodings.begin(), j = next(i, 10); i != j; i++) {
      for(int k : i->first) cout << k << " ";
      cout << ": ";
      for(string s : i->second) cout << s << ", ";
      cout << endl;
      }
    */
}


vector<int> encode(const string &s)
{
    // seen keeps track of letters we saw already and what we mapped them to
    // 0..25 maps to a..z
    vector<int> encoding;
    vector<int> seen(26, -1);
    int mapping = 0;
    for(int i = 0; i < s.size(); i++) {
        if(seen[s[i]-'a'] < 0) {
            seen[s[i]-'a'] = mapping;
            encoding.push_back(mapping);
            mapping++;
        }
        else encoding.push_back(seen[s[i]-'a']);
    }
    return encoding;
}

vector<int> encode(const vector<int> &s)
{
    vector<int> encoding;
    vector<int> seen(103, -1);
    int mapping = 0;
    for(int i = 0; i < s.size(); i++) {
        if(seen[s[i]] < 0) {
            seen[s[i]] = mapping;
            encoding.push_back(mapping);
            mapping++;
        }
        else encoding.push_back(seen[s[i]]);
    }
    return encoding;
}

vector<int> encode(vector<int>::iterator start, vector<int>::iterator end)
{
    vector<int> encoding;
    vector<int> seen(103, -1);
    int mapping = 0;
    for(auto i = start; i != end; i++) {
        if(seen[*i] < 0) {
            seen[*i] = mapping;
            encoding.push_back(mapping);
            mapping++;
        }
        else encoding.push_back(seen[*i]);
    }
    return encoding;
}
