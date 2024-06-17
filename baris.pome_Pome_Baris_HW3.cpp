//CS300 – Spring 2023-2024 - Sabancı University Homework 3 – Search Engine V2
//Barış Pome - 31311
//Hash Table Header File to be used in baris.pome_Pome_Baris_HW3.cpp

//main function for the homework named baris.pome_Pome_Baris_HW3.cpp

#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "baris.pome_Pome_Baris_HW3_AVL.h"
#include "baris.pome_Pome_Baris_HW3_HashTable.h"

using namespace std;

// Structs for the homework document
struct DocumentItem { // Document item for each word given in the homework document
    string documentName; // Document name
    int count; // Count of the word in the document
};

struct WordItem { // Word item for each word given in the homework document
    string word; // Word
    vector<DocumentItem> documents; // Documents containing the word
};

//helper functions for the main function
// the function to convert a string to lowercase and to process the string
vector<string> processString(const string& input) { //process the string to get the words in the string
    string output = input;
    transform(output.begin(), output.end(), output.begin(), ::tolower); // Convert to lowercase
    replace_if(output.begin(), output.end(), [](unsigned char c){ return !isalpha(c); }, ' '); // Replace non-alphabetical characters with space

    vector<string> words;
    stringstream ss(output);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

// Function to merge a WordItem's document counts when the word is encountered again
void mergeWordItem(WordItem& existingItem, const string& documentName) {
    auto it = find_if(existingItem.documents.begin(), existingItem.documents.end(),
                      [&documentName](const DocumentItem& di) { return di.documentName == documentName; });
    if (it != existingItem.documents.end()) {
        // Document already exists for this word, increment count
        it->count++;
    } else {
        // New document for this word, add new document item
        existingItem.documents.push_back(DocumentItem{documentName, 1});
    }
}


// Function to process the documents and build the AVL Tree and HashTable
void processDocuments(const vector<string>& documents, AVLSearchTree<string, WordItem>& avlTree, HashTable<string, WordItem>& hashTable) {
    for (const auto& documentName: documents) {
        ifstream file(documentName); // Open the file
        if (file.is_open()) { // If the file is open
            string line;
            while (getline(file, line)) { // Read the file line by line
                vector<string> words = processString(line); // Process the line to get the words
                for (const auto& word: words) {
                    if (word.empty()) { // If the word is empty, skip
                        continue;
                    }
                    // AVL Tree
                    AVLNode<string, WordItem>* foundNode = avlTree.find(word); // Find the word in the AVL Tree
                    if (foundNode != nullptr && foundNode->key != "") { // If the word is found
                        mergeWordItem(foundNode->value, documentName); // Merge the word item
                    } else { // If the word is not found
                        WordItem wordItem; // Create a new WordItem
                        wordItem.word = word; // Set the word
                        wordItem.documents.push_back(DocumentItem{documentName, 1}); // Add the document item
                        avlTree.insert(word, wordItem); // Insert the word into the AVL Tree
                    }
                    // Hash Table
                    WordItem* foundItem = hashTable.find(word);
                    if (foundItem != nullptr) {
                        mergeWordItem(*foundItem, documentName); // Dereference the pointer to pass by reference
                    } else {
                        WordItem wordItem; // Create a new WordItem
                        wordItem.word = word;
                        wordItem.documents.push_back(DocumentItem{documentName, 1});
                        hashTable.insert(word, wordItem);
                    }


                }
            }
            file.close(); // Close the file
        } else { // If the file is not open
            cout << "Unable to open file " << documentName << endl; // Print error message
        }
    }
    hashTable.getUniqueWordCountAndLoadFactor(); // Get the unique word count and load factor of the HashTable
}


// Function to read document names from the user
void readDocumentNames(vector<string>& documents) {
    int numDocuments; // Number of documents
    cout << "Enter number of input files: "; // Prompt for number of documents
    cin >> numDocuments; // Read the number of documents
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < numDocuments; ++i) {
        string documentName; // Document name
        cout << "Enter " << i + 1 << ". file name: ";
        cin >> documentName;
        documents.push_back(documentName); // Add the document name to the vector
    }
}


// Hash function for the HashTable for the key value of string - since my hashtable is a template class, I need to pass the hash function as a parameter
int hash_function(const string& key, int table_size) { //hash function for the hash table
    int hash = 0; //initialize the hash value as 0
    for (int i = 0; i < key.length(); i++) { //iterate over the key
        hash = 37 * hash + key[i]; //calculate the hash value
    }
    return hash % table_size; //return the hash value
}

// Main function
int main() {
    vector<string> documents; // Vector to store document names
    readDocumentNames(documents); // Read the document names from the user

    AVLSearchTree<string, WordItem> avlTree("", WordItem()); // Create an AVL Tree with string key and WordItem value // Create an AVL Tree with string key and WordItem value
    HashTable<string, WordItem> hashTable(WordItem(), 53, hash_function); // Create a HashTable with string key and WordItem value

    processDocuments(documents, avlTree, hashTable); // Process the documents and build the AVL Tree and HashTable

    // Clearing input buffer before reading user input for queries
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    vector<string> QueryWords; // Vector to store queried words
    string input;
    cout << "Enter queried words in one line: "; // Prompt for query
    getline(cin, input); // Reads the actual query line
    QueryWords = processString(input); // Process the query line to get the words

    int k = 20;
    int found_avl_count = 0;
    int found_ht_count = 0;
    bool is_ok = false;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments (with AVL Tree);
        for (const auto& word: QueryWords) {
            AVLNode<string, WordItem>* foundNode = avlTree.find(word); // Find the word in the AVL Tree
            if (foundNode != nullptr && foundNode->key != "") { // If the word is found
                found_avl_count++;
            }
        }
    }

    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() -
             start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments (with Hash Table);
        for (const auto& word: QueryWords) { // Check if the word is found in the Hash Table
            WordItem* foundItem = hashTable.find(word); // Find the word in the Hash Table
            if (foundItem != nullptr && !foundItem->word.empty()) { // If the word is found
                found_ht_count++; // Increment the count
            }
        }
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);


    if (found_avl_count > 0 && found_ht_count > 0) // If the word is found in both AVL Tree and Hash Table
    {


        // Querying and outputting results from AVL Tree
        for (const auto& documentName : documents) {
            bool allWordsFound = true;
            stringstream output;
            output << "in Document " << documentName << ", "; // Output format
            for (const auto& qWord :QueryWords ) { // Check if all queried words are found in the document
                AVLNode<string, WordItem>* foundNode = avlTree.find(qWord); // Find the word in the AVL Tree
                bool wordFoundInDocument = false; // Flag to check if word is found in the document
                if (foundNode != nullptr && foundNode->key != "") {  // If word is found
                    for (const auto& docItem : foundNode->value.documents) {
                        if (docItem.documentName == documentName) { // Check if the document name matches
                            output << qWord << " found " << docItem.count << " times, "; // Output format
                            wordFoundInDocument = true; // Set flag to true
                            break;
                        }
                    }
                }
                if (!wordFoundInDocument) { // If word is not found in the document
                    allWordsFound = false; // Set flag to false
                    break;
                }
            }
            if (allWordsFound) { // If all words are found in the document
                string outputStr = output.str(); // Convert stringstream to string
                outputStr = outputStr.substr(0, outputStr.size() - 2); // Removing trailing comma and space
                cout << outputStr << "." << endl; // Print the output
                is_ok = true; // Set flag to true
            }
        }

        // Querying and outputting results from Hash Table
        for (const auto& documentName : documents) {
            bool allWordsFound = true;
            stringstream output;
            output << "in Document " << documentName << ", "; // Output format
            for (const auto& qWord : QueryWords) { // Check if all queried words are found in the document
                WordItem* foundItem = hashTable.find(qWord); // Find the word in the Hash Table
                bool wordFoundInDocument = false; // Flag to check if word is found in the document
                if (foundItem != nullptr && !foundItem->word.empty()) { // If word is found
                    for (const auto& docItem : foundItem->documents) {
                        if (docItem.documentName == documentName) { // Check if the document name matches
                            output << qWord << " found " << docItem.count << " times, "; // Output format
                            wordFoundInDocument = true; // Set flag to true
                            break;
                        }
                    }
                }
                if (!wordFoundInDocument) { // If word is not found in the document
                    allWordsFound = false; // Set flag to false
                    break;
                }
            }
            if (allWordsFound) { // If all words are found in the document
                string outputStr = output.str(); // Convert stringstream to string
                outputStr = outputStr.substr(0, outputStr.size() - 2); // Removing trailing comma and space
                cout << outputStr << "." << endl; // Print the output
                is_ok = true; // Set flag to true
            }
        }

        if (!is_ok) {
            cout << "No document contains the given query\n"; // Print error message
            cout << "No document contains the given query\n"; // Print error message
        }
    }
    cout << "\nTime: " << BSTTime.count() / k; // Print the time taken for AVL Tree
    cout << "\nTime: " << HTTime.count() / k << "\n"; // Print the time taken for Hash Table
    cout << "Speedup: " << double(BSTTime.count()) / double(HTTime.count()) << "\n"; // Print the speedup

    return 0;
}

