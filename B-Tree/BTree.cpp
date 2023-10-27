#include "BTree.h"
#include <iostream>
using namespace std;
// Node methods
bool BTreeNode::searchKey(int _key){
        int i = 0;
        while(i < n && _key > keys[i]){
            i++;
        }
        if(i < n && _key == keys[i])
            return true;
        if(isLeaf)
            return false;
        return C[i]->searchKey(_key);
    }

int BTreeNode::findKeyIndex(int _key) {
    int idx = 0;
    while (idx < n && _key > keys[idx]) {
        idx++;
    }
    return idx;
}

void BTreeNode::deleteKey(int _key) {
    int idx = findKeyIndex(_key);

    if (idx < n && keys[idx] == _key) {
        if (isLeaf) {
            deleteKeyFromLeaf(idx);
        } else {
            deleteKeyFromNonLeaf(idx);
        }
    } else {
        if (isLeaf) {
            cout << "Key " << _key << " does not exist in the tree." << endl;
        } else {
            cout << "Key " << _key << " does not exist in the tree. Please check the input." << endl;
        }
    }
}

void BTreeNode::deleteKeyFromLeaf(int idx) {
    for (int i = idx + 1; i < n; i++) {
        keys[i - 1] = keys[i];
    }
    n--;
}

void BTreeNode::deleteKeyFromNonLeaf(int idx) {
    int key = keys[idx];

    if (C[idx]->n >= t) {
        int predecessor = getPredecessor(idx);
        keys[idx] = predecessor;
        C[idx]->deleteKey(predecessor);
    } else if (C[idx + 1]->n >= t) {
        int successor = getSuccessor(idx);
        keys[idx] = successor;
        C[idx + 1]->deleteKey(successor);
    } else {
        mergeChildren(idx);
        C[idx]->deleteKey(key);
    }
}

int BTreeNode::getPredecessor(int idx) {
    BTreeNode* currentNode = C[idx];
    currentNode->t = t;
    while (!currentNode->isLeaf) {
        currentNode = currentNode->C[currentNode->n];
    }
    return currentNode->keys[currentNode->n - 1];
}

int BTreeNode::getSuccessor(int idx) {
    BTreeNode* currentNode = C[idx + 1];
    currentNode-> t = t;
    while (!currentNode->isLeaf) {
        currentNode = currentNode->C[0];
    }
    return currentNode->keys[0];
}

void BTreeNode::mergeChildren(int idx) {
    BTreeNode* child = C[idx];
    BTreeNode* sibling = C[idx + 1];
    sibling-> t = t;
    child->t = t;
    child->keys[t - 1] = keys[idx];

    for (int i = 0; i < sibling->n; i++) {
        child->keys[i + t] = sibling->keys[i];
    }

    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->n; i++) {
            child->C[i + t] = sibling->C[i];
        }
    }

    for (int i = idx + 1; i < n; i++) {
        keys[i - 1] = keys[i];
    }

    for (int i = idx + 2; i <= n; i++) {
        C[i - 1] = C[i];
    }

    child->n += sibling->n + 1;
    n--;
    delete sibling;
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
    if(root->searchKey(_key))
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
    newNode->t = t;

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
        root->t = t;
    }
    
    if(root->n == 2*t-1){ // Node is full
        BTreeNode *newRoot = new BTreeNode;
        newRoot->keys = new int[2*t - 1];
        newRoot->C = new BTreeNode *[2 * t];
        newRoot->isLeaf = false;
        newRoot->n = 0;
        newRoot->C[0] = root;
        newRoot->t = t;
        splitChild(newRoot, 0, root);
        
        root = newRoot;
    }
    insertNonFull(root, _key);
}

void BTree::remove(int _key){
    if (root == nullptr) {
        cout << "The tree is empty" << endl;
        return;
    }

    root->deleteKey(_key);

    if (root->n == 0) {
        BTreeNode* temp = root;
        if (root->isLeaf) {
            root = nullptr;
        } else {
            root = root->C[0];
        }
        delete temp;
    }
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
