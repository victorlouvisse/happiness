#ifndef TRIE_H
#define TRIE_H

#include <iostream>

using namespace std;

class Node;

class Trie
{
public:
    Trie();
    ~Trie();

    void insert( string str, double avgVal );
    bool search( string str, double * avgVal );

private:
    Node* m_root;
};

#endif // TRIE_H
