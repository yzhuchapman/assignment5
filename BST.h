#ifndef __BST_H
#define __BST_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <stdio.h>
#include "TreeNode.h"

#define MARKER "NULL"
#define MAXROLLBACKS 5 // maximum number of database states to roll back to

using namespace std;

template <typename T>
class BST{
public:
    TreeNode<T> *root;
    string fileName;
    deque<string> snapshotQ;  // double ended queue of up to MAXROLLBACKS prior database states used for rollback

    BST(string fileName){
        this->fileName = fileName;
        root = NULL;
    }

    ~BST(){
        root = NULL;
    }

    // Inserts a node into the tree
    bool insert(T data){
        TreeNode<T> * node = new TreeNode<T>(data);
        if(root == NULL) {
            root = node;
            return true;
        }
        TreeNode<T>* current = root;
        TreeNode<T>* parent;
        while (current != NULL) {
            parent = current;
            if(data == current->data) return false; // duplicate node
            else if(data < current->data) current = current->left;
            else current = current->right;
        }
        if(data < parent->data) parent->left = node;
        else parent->right = node;

        return true;
    }

    //Helper method, traverses tree, removes node, and adjusts tree
    bool remove(int ID, TreeNode<T> *node, TreeNode<T> *parent, bool isLeft){
        if(node == NULL) return false;
        if(node->data > ID) return remove (ID, node->left, node, true);
        else if(node->data < ID) return remove (ID, node->right, node, false);
        else{
            if(!node->left && !node->right) {
                delete node;
                node = NULL;

                if (parent != NULL) {
                    if (isLeft)
                        parent->left = NULL;
                    else
                        parent->right = NULL;
                }
            }
            else if(!node->right) { node->data = node->left->data; node->left = node->left->left; }
            else if(!node->left) { node->data = node->right->data; node->right = node->right->right; }
            else {
                TreeNode<T> * parent = node;
                TreeNode<T> * min = node->right;
                bool check = true;

                while(min->left) {
                    if(check) check = false;
                    parent = min;
                    min = min->left;
                }

                node->data = min->data;
                if(check) parent->right = min->right;
                else parent->left = min->right;
            }
            return true;
        }
    }

    //Removes node that has given ID and adjusts tree
    bool remove(int ID){
        bool success = remove (ID, root, NULL, false);
        if (!success)
            popSnapshot();

        return success;
    }

    //Helper Method to Search, Searches tree for node with given ID and returns that node
    TreeNode<T>* search(int ID, TreeNode<T> * node){
        if(node == NULL) return NULL;
        if(node->data == ID) return node;
        if(node->data > ID)
            return search(ID, node->left);
        else
            return search(ID, node->right);
    }

    //Searches tree for node with given ID and returns that node
    T* search(int ID){
        TreeNode<T>* result = search(ID, root);
        if(result == NULL) return NULL;
        return &result->data;
    }

    //creates a string version of the database that can be saved or user to rebuild the tree
    void makeSerializedString(TreeNode<T> *node, ostringstream& serialStream){
        if(node == NULL) {
            serialStream << MARKER << '\n';
            return;
        }

        makeSerializedString(node->left, serialStream);
        serialStream << node->data.ToString();
        makeSerializedString(node->right, serialStream);
    }

    //writes tree to file
    void serialize(){
        ofstream outFile (fileName);
        ostringstream outStream;
        makeSerializedString(root, outStream);
        outFile << outStream.str();
        outFile.close();
    }

    //reads tree from file
    void deserialize(){
        delete root;
        root = NULL;

        ifstream inFile(fileName);
        string textLine;
        while (getline(inFile, textLine)){
            if(textLine != MARKER)
                insert(T(textLine));
        }

        inFile.close();
    }

    // store a snapshot of the database as a string in memory (snapshotQ) in order to rollback changes
    void snapshot(){
        if (snapshotQ.size() >= MAXROLLBACKS)
            snapshotQ.pop_back();

        ostringstream snapStream;
        makeSerializedString(root, snapStream);
        snapshotQ.push_front(snapStream.str());
    }

    void popSnapshot(){
        if (!snapshotQ.empty())
            snapshotQ.pop_front();
    }

    void clearsnapshotQ(){
        snapshotQ.clear();
    }

    // restore the most recent snapshot of the database stored in memory (snapshotQ)
    void rollback(){
        if (snapshotQ.empty()) return;

        string record = snapshotQ.front();

        ofstream outFile (fileName);
        outFile << record;
        outFile.close();

        snapshotQ.pop_front();

        deserialize();
    }

    //Helper Method, prints all nodes
    void printNodes(TreeNode<T> *node){
        if(node == NULL) return;
        printNodes(node->left);
        cout << node->data.ToString();
        printNodes(node->right);
    }

    // Prints all nodes
    void printNodes(){
        printNodes(root);
    }

    // Helper Method, displays all nodes
    void displayAll(TreeNode<T> *node){
        if(node == NULL) return;
        displayAll(node->left);
        node->data.display();
        displayAll(node->right);
    }

   	// Displays all nodes
    void displayAll(){
        displayAll(root);
    }
};

#endif
