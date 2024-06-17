//
// Created by Barış Pome on 14.05.2024.
//

//CS300 – Spring 2023-2024 - Sabancı University Homework 3 – Search Engine V2
//Barış Pome - 31311
//AVL Header File to be used in baris.pome_Pome_Baris_HW3.cpp

#pragma once

#ifndef BARIS_POME_BARIS_POME_HW3_AVL_H
#define BARIS_POME_BARIS_POME_HW3_AVL_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


// Templated AVL Tree Node
template <class Key, class Value>
class AVLNode {
public:
    Key key; // Key of the node
    Value value; // Value of the node
    AVLNode* left; // Left child
    AVLNode* right; // Right child
    int height; // Height of the node
    AVLNode(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {} // Constructor
};

// Templated AVL Search Tree Class
template <class Key, class Value>
class AVLSearchTree {
private:
    AVLNode<Key, Value>* root; // Root of the AVL Tree
    Key notFoundKey; // Key for not found value
    Value notFoundValue; // Value for not found value

    // the function to get the height of the node
    int height(AVLNode<Key, Value>* node) { //the function to get the height of the node
        if (node == nullptr) { //if the node is null, return 0
            return 0; //return 0
        }
        return node->height; //return the height of the node
    }

    // the function to get the balance factor of the node
    int balanceFactor(AVLNode<Key, Value>* node) { //the function to get the balance factor of the node
        if (node == nullptr) { //if the node is null, return 0
            return 0;
        }
        return height(node->left) - height(node->right); //return the difference between the height of the left and right child
    }
    // the function to rotate the node to the right
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* y) { // y is the root of the subtree
        AVLNode<Key, Value>* x = y->left; //set x as the left child of y
        AVLNode<Key, Value>* T2 = x->right; //set T2 as the right child of x

        // Perform rotation
        x->right = y; //set the right child of x as y
        y->left = T2; //set the left child of y as T2

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1; //update the height of y
        x->height = max(height(x->left), height(x->right)) + 1; //update the height of x

        // Return new root
        return x;
    }
    // the function to rotate the node to the left
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* x) {  // x is the root of the subtree
        AVLNode<Key, Value>* y = x->right; //set y as the right child of x
        AVLNode<Key, Value>* T2 = y->left; //set T2 as the left child of y

        // Perform rotation
        y->left = x; //set the left child of y as x
        x->right = T2; //set the right child of x as T2

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1; //update the height of x
        y->height = max(height(y->left), height(y->right)) + 1; //update the height of y

        // Return new root
        return y;
    }
    // the function to balance the node
    AVLNode<Key, Value>* balance(AVLNode<Key, Value>* node) {
        if (node == nullptr) { //if the node is null, return null
            return node;
        }
        // Update height
        node->height = 1 + max(height(node->left), height(node->right)); //update the height of the node
        // Get balance factor
        int balance = balanceFactor(node); //get the balance factor of the node
        // If node is unbalanced, then try 4 cases

        // Left Left Case
        if (balance > 1 && balanceFactor(node->left) >= 0)
            return rotateRight(node);

        // Left Right Case
        if (balance > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Right Case
        if (balance < -1 && balanceFactor(node->right) <= 0)
            return rotateLeft(node);

        // Right Left Case
        if (balance < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    // the function to insert a node
    AVLNode<Key, Value>* insert(AVLNode<Key, Value>* node, Key key, Value value) {
        if (node == nullptr) { // if the node is null, create a new node
            return new AVLNode<Key, Value>(key, value);
        }
        // Insert the node
        if (key < node->key) { // if the key is less than the key of the node
            node->left = insert(node->left, key, value);
        } else if (key > node->key) { // if the key is greater than the key of the node
            node->right = insert(node->right, key, value);
        } else { // if the key is equal to the key of the node, update the value
            node->value = value; // Update the existing node's value with the new value
        }
        // Balance the node
        return balance(node); // balance the node
    }
    // the function to find the minimum node
    AVLNode<Key, Value>* findMin(AVLNode<Key, Value>* node) { //find the minimum node
        AVLNode<Key, Value>* current = node; //set the current node as the node
        while (current->left != nullptr) {
            current = current->left; //set the current node as the left child of the current node
        }
        return current;
    }
    //the function to find the maximum node
    AVLNode<Key, Value>* findMax(AVLNode<Key, Value>* node) { //find the maximum node
        AVLNode<Key, Value>* current = node; //set the current node as the node
        while (current->right != nullptr) {
            current = current->right; //set the current node as the right child of the current node
        }
        return current;
    }
    // the function to remove a node
    AVLNode<Key, Value>* remove(AVLNode<Key, Value>* node, Key key) {
        if (node == nullptr) { //if the node is null, return null
            return node;
        }
        if (key < node->key) { //if the key is less than the key of the node
            node->left = remove(node->left, key); //remove the left child of the node recursively
        }
        else if (key > node->key) { //if the key is greater than the key of the node
            node->right = remove(node->right, key);  //remove the right child of the node recursively
        }
        else { //if the key is equal to the key of the node
            if ((node->left == nullptr) || (node->right == nullptr)) { //if the left or right child of the node is null
                AVLNode<Key, Value>* temp; //create a temporary node
                if (node->left != nullptr) { //if the left child of the node is not null
                    temp = node->left;
                }
                else { //if the right child of the node is not null
                    temp = node->right;
                }
                if (temp == nullptr) { //if the temporary node is null
                    temp = node;
                    node = nullptr;
                }
                else { //if the temporary node is not null
                    *node = *temp;
                }
                delete temp;
            }
            else { //if the left and right child of the node are not null
                AVLNode<Key, Value>* temp = findMin(node->right);  //find the minimum node of the right child of the node
                node->key = temp->key; //set the key of the node as the key of the minimum node
                node->right = remove(node->right, temp->key); //remove the minimum node of the right child of the node
            }
        }
        if (node == nullptr) { //if the node is null, return null
            return node;
        }
        // Balance the node
        return balance(node);
    }
    // the function to find a node
    AVLNode<Key, Value>* find(AVLNode<Key, Value>* node, const Key& key) { //find the node with the key
        if (node == nullptr || node->key == key) { //if the node is null or the key of the node is equal to the key
            return node; //return the node
        }
        if (node->key < key) { //if the key is greater than the key of the node
            return find(node->right, key); //find the node in the right child of the node
        }
        return find(node->left, key); //find the node in the left child of the node
    }
    // the function to copy the tree
    AVLNode<Key, Value>* copyTree(const AVLNode<Key, Value>* node) { //copy the tree
        if (node == nullptr) { //if the node is null, return null
            return nullptr;
        }
        AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(node->key, node->value); //create a new node
        newNode->left = copyTree(node->left); //copy the left child of the node recursively
        newNode->right = copyTree(node->right); //copy the right child of the node recursively
        newNode->height = node->height; //set the height of the new node as the height of the node
        return newNode; //return the new node
    }

public:
    AVLSearchTree(Key notFoundKey, Value notFoundValue) : root(nullptr), notFoundKey(notFoundKey), notFoundValue(notFoundValue) {} //constructor

    ~AVLSearchTree() { //destructor
        destroyTree(root);
    }
    // the function to destroy the tree - destructor
    void destroyTree(AVLNode<Key, Value>* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    //the function to find a node
    AVLNode<Key, Value>* find(const Key& key) { //find the node with the key
        AVLNode<Key, Value>* result = find(root, key); //find the node with the key
        if (result == nullptr) { //if the node is null
            return new AVLNode<Key, Value>(notFoundKey, notFoundValue); //return a new node with the not found key and value
        }
        return result; //return the node
    }
    //the function to copy the tree
    AVLSearchTree<Key, Value>& operator=(const AVLSearchTree<Key, Value>& other) {
        if (this != &other) { //if the tree is not the same as the other tree
            this->~AVLSearchTree(); //delete the tree
            root = copyTree(other.root); //copy the tree
        }
        return *this; //return the tree
    }
    //the function to insert a node
    void insert(Key key, Value value) { //insert a node
        root = insert(root, key, value); //insert a node
    }
    //the function to remove a node
    void remove(Key key) { //remove a node
        root = remove(root, key); //remove a node
    }

    void printInOrder(AVLNode<Key, Value>* node) { //print the tree in order
        //new line
        if (node == nullptr) { //if the node is null
            return;
        }
        printInOrder(node->left); //print the left child of the node recursively
        cout << node->key << " "; //print the key of the node
        printInOrder(node->right); //print the right child of the node recursively
    }
};


#endif //BARIS_POME_BARIS_POME_HW3_AVL_H