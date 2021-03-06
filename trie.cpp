#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Node {
public:
    Node() { mContent = ' '; mMarker = false; }
    ~Node() { for(Node* n : mChildren) delete n; }
    char content() { return mContent; }
    void setContent(char c) { mContent = c; }
    bool wordMarker() { return mMarker; }
    void setWordMarker() { mMarker = true; }
    Node* findChild(char c);
    void appendChild(Node* child) { mChildren.push_back(child); }
    vector<Node*> children() { return mChildren; }

private:
    char mContent;
    bool mMarker;
    vector<Node*> mChildren;
};

class Trie {
public:
    Trie();
    ~Trie();
    void addWord(string s);
    bool searchWord(string s);
    void deleteWord(string s);
    Node* getNodeAt(const string& s);
private:
    Node* root;
};

Node* Node::findChild(char c)
{
    for (int i = 0; i < mChildren.size(); i++) {
        Node* tmp = mChildren.at(i);
        if (tmp->content() == c) {
            return tmp;
        }
    }
    return NULL;
}

Trie::Trie()
{
    root = new Node();
}

Trie::~Trie()
{
    // Free memory
    delete root;
}

void Trie::addWord(string s)
{
    Node* current = root;

    if (s.length() == 0) {
        current->setWordMarker(); // an empty word
        return;
    }

    for (int i = 0; i < s.length(); i++) {
        Node* child = current->findChild(s[i]);
        if (child != NULL) {
            current = child;
        }
        else {
            Node* tmp = new Node();
            tmp->setContent(s[i]);
            current->appendChild(tmp);
            current = tmp;
        }
        if (i == s.length() - 1)
            current->setWordMarker();
    }
}

bool Trie::searchWord(string s)
{
    Node* current = root;
    while (current != NULL) {
        for (int i = 0; i < s.length(); i++) {
            Node* tmp = current->findChild(s[i]);
            if (tmp == NULL)
                return false;
            current = tmp;
        }
        if (current->wordMarker())
            return true;
        else
            return false;
    }
    return false;
}

Node* Trie::getNodeAt(const string& s) {
    Node* current = root;
    for (int i = 0; i < s.length(); i++) {
        Node* tmp = current->findChild(s[i]);
        current = tmp;
    }
    return current;
}
