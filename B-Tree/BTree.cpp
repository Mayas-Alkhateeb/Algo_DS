#include "BTree.h"
#include <iostream>
using namespace std;
// Node methods
bool BTreeNode::searchkey(int _key){
        int i = 0;
        while(i < n && _key > keys[i]){
            i++;
        }
        if(i < n && _key == keys[i])
            return true;
        if(isLeaf)
            return false;
        return C[i]->searchkey(_key);
    }





//!-----------------------------------------------------------
// Constructor
BTree::BTree(int degree){
    root = nullptr;
    t = degree;
}
// Methods
void BTree::search(int _key){
    if(root == nullptr){
        cout << "The tree is empty" << endl;
        return;
    }
    if(root->searchkey(_key))
        cout << "The key "<<_key<<" is found in the tree" << endl;
    else
        cout << "The key "<<_key<<" is NOT found in the tree" << endl;
    return;
}

void BTree::splitChild(BTreeNode* _parent, int _index, BTreeNode* _child){
    BTreeNode *newNode = new BTreeNode;
    newNode->keys = new int[2*t - 1];
    newNode->C = new BTreeNode *[2*t];
    newNode->isLeaf = _child->isLeaf;
    newNode->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        newNode->keys[j] = _child->keys[j + t];
    
    if (!_child->isLeaf) {
        for (int j = 0; j < t; j++)
            newNode->C[j] = _child->C[j + t];
    }
    _child->n = t - 1;

    for (int j = _parent->n; j >= _index + 1; j--)
        _parent->C[j + 1] = _parent->C[j];

    _parent->C[_index + 1] = newNode;

    for (int j = _parent->n - 1; j >= _index; j--)
        _parent->keys[j + 1] = _parent->keys[j];

    _parent->keys[_index] = _child->keys[t - 1];
    _parent->n++;
    return;
}

void BTree::insertNonFull(BTreeNode *node, int _key){
    int i = node->n-1;
    if (node->isLeaf) {
        while (i >= 0 && _key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = _key;
        node->n++;
    }
    else{
        while (i >= 0 && _key < node->keys[i])
            i--;
        i++;

        if (node->C[i]->n == 2 * t - 1) {
            splitChild(node, i, node->C[i]);

            if (_key > node->keys[i])
                i++;
        }

        insertNonFull(node->C[i], _key);
    }
    return;
}

void BTree::insert(int _key){
    if(root == nullptr){ // Make a new root if the tree is empty
        root = new BTreeNode;
        root->keys = new int[2*t - 1];
        root->C = new BTreeNode *[2*t];
        root->isLeaf = true;
        root->n = 0;
    }
    
    if(root->n == 2*t-1){ // Node is full
        BTreeNode *newRoot = new BTreeNode;
        newRoot->keys = new int[2*t - 1];
        newRoot->C = new BTreeNode *[2 * t];
        newRoot->isLeaf = false;
        newRoot->n = 0;
        newRoot->C[0] = root;
        splitChild(newRoot, 0, root);
        
        root = newRoot;
    }
    insertNonFull(root, _key);
}

void BTree::remove(int _key){
    //TODO : implement the remove method
    cout <<"NOT IMPLEMENETED YET"<<endl;
}

void BTree::printTree(){
    if(root == nullptr){
        cout << "The tree is empty"<<endl;
        return;
    }
    printNode(root, "");
}

void BTree::printNode(BTreeNode *node,string indent) {
    cout << indent;

    for (int i = 0; i < node->n; i++)
        cout << node->keys[i] << " ";

    cout << endl;

    if (!node->isLeaf) {
        for (int i = 0; i <= node->n; i++)
            printNode(node->C[i], indent + "    ");
    }
}


//Destructor
void BTree::destroyTree(BTreeNode *node) {
    if (node != nullptr) {
        if (!node->isLeaf) {
            for (int i = 0; i <= node->n; i++)
                destroyTree(node->C[i]);
        }

        delete[] node->keys;
        delete[] node->C;
        delete node;
    }
}
BTree::~BTree(){
    destroyTree(root);
}
