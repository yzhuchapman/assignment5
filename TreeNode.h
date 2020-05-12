#include <iostream>
#include <stdio.h>
#ifndef TreeNode_h
#define TreeNode_h

template <typename T>
class TreeNode {
public:
    T data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(T data):data(data), left(NULL), right(NULL){}; //constructor

    ~TreeNode(){ //destructor
        if(left) left = NULL;
        if(right) right = NULL;
    }
};
#endif
