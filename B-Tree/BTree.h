#ifndef BTREE_H
#define BTREE_H
#include <iostream>
using namespace std;
// B-Tree Node
struct BTreeNode{
    int *keys; // array of keys 
    int n; // number of keys in the node
    BTreeNode **C; // array of pointers to the children of the node
    bool isLeaf; // true if the node is a leaf
    //methods
    bool searchkey(int);
};///////////////////////////////////////////////


// B-Tree
class BTree{
private:
    BTreeNode *root; // root node
    int t; // minimum degree of a node
    void insertNonFull(BTreeNode *, int);
    void splitChild(BTreeNode*, int, BTreeNode*);
    void printNode(BTreeNode*, std::string);
    void destroyTree(BTreeNode*); 

public:
    // Constructor
    BTree(int); 

    // Methods
    void search(int);
    void insert(int);
    void remove(int);
    void printTree();
    
    // Destructor
    ~BTree();
};
#endif