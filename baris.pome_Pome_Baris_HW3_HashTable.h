//
// Created by Barış Pome on 14.05.2024.
//

//CS300 – Spring 2023-2024 - Sabancı University Homework 3 – Search Engine V2
//Barış Pome - 31311
//Hash Table Header File to be used in baris.pome_Pome_Baris_HW3.cpp

#pragma once
#ifndef BARIS_POME_BARIS_POME_HW3_HASHTABLE_H
#define BARIS_POME_BARIS_POME_HW3_HASHTABLE_H

//utility funtions for hash table

// The function to check if a number is prime.
bool isPrime( int n ) {
    if ( n == 1 || n % 2 == 0 ) {
        return false;
    }
    if ( n == 2 || n == 3 ) {
        return true;
    }
    for ( int i = 3; i * i <= n; i += 2 ) {
        if ( n % i == 0 ) {
            return false;
        }
    }
    return true;
}
// The function to find the next prime number.
int nextPrime( int n ) {
    if ( n <= 0 ) { // Handle negative numbers
        n = 3;
    }
    if ( n % 2 == 0 ) { // If n is even, make it odd
        n++;
    }
    while ( !isPrime( n ) ) { // Find the next prime number
        n += 2;
    }
    return n;
}

enum Type {ACTIVE, EMPTY, DELETED}; // Enum for the status of the HashEntry.

// HashEntry Class.
template <class Key, class Value>
struct HashEntry {
    Key key;
    Value value;
    Type info;
    HashEntry(const Key& k = Key(), const Value& v = Value(), Type i = EMPTY ) : key(k), value(v), info(i) {} // Constructor with default values.
};

// HashTable Class.
template <class Key, class Value>
class HashTable {
private:
    int (*hashFunction)(const Key&, int); // Hash function pointer to be used in the HashTable. Since it is a pointer, it can be changed to any hash function.
    vector<HashEntry<Key, Value>> array; // Array of HashEntry objects.
    int capacity; // Current size of the HashTable.
    const Value ITEM_NOT_FOUND;

    // Helper functions for the HashTable class.

    // The function to rehash the HashTable.
    void rehash() {
        vector<HashEntry<Key, Value>> array_to_change = array;
        int prev_capacity = array.size();
        int prev_load = capacity / prev_capacity;

        array.resize(nextPrime(2*array_to_change.size()));
        makeEmpty();

        for (int i = 0; i < array_to_change.size(); i++) {
            if (array_to_change[i].info == ACTIVE) {
                insert(array_to_change[i].key, array_to_change[i].value);
            }
        }

        // Print rehashing information in the format given in the homework document.
        cout << "rehashed..."<< endl;
        cout << "previous table size:" << prev_capacity << ", new table size: " << array.size() << ", current unique word count " << capacity + 1 << ", ";
        cout << "current load factor: " << (double)capacity / array.size() << endl;
    }

    // The function to check if the current position is active.
    bool isActive(int currentPos) const {
        return array[currentPos].info == ACTIVE;
    }

    // The function to find the position of a given key.
    int findPos(const Key& key) const {
        int offset = 1;
        int initialIndex = abs(hashFunction(key, array.size())) % array.size();
        int index = initialIndex;

        while (array[index].info != EMPTY && array[index].key != key) {
            index = initialIndex + offset * offset;  // Quadratic probing
            index = index % array.size();  // Ensure the index wraps around.
            offset++;
        }
        return index;
    }

public:

    // Constructors and Destructor for the HashTable class.
    explicit HashTable(const Value& notFound, int size = 101, int (*hashFunc)(const Key&, int) = nullptr) // Constructor with default values.
            : ITEM_NOT_FOUND(notFound), array(nextPrime(size)), capacity(0), hashFunction(hashFunc) {
        makeEmpty();
    }


    HashTable(const HashTable& rhs) : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), capacity(rhs.capacity) {} // Copy constructor.

    ~HashTable() { // Destructor.
        makeEmpty();
    }

    // The functions to be used in the HashTable class.

    // The function to find the value of a given key.
    Value* find(const Key& key) {
        int currentPos = findPos(key);
        if (isActive(currentPos)) {
            return &array[currentPos].value;  // Return a pointer to the value
        }
        return nullptr;  // Return nullptr if the key is not found
    }

    // The function to make the HashTable empty.
    void makeEmpty() { // Make the HashTable empty.
        capacity = 0;
        for (int i = 0; i < array.size(); i++) {
            array[i].info = EMPTY;
        }
    }

    // The function to insert a new key-value pair.
    void insert(const Key& key, const Value& value) {
        int curr = findPos(key);
        if (isActive(curr)) {
            // Directly update the value at the found position if it's active
            array[curr].value = value;
        } else {
            // Otherwise, insert a new entry
            array[curr] = HashEntry<Key, Value>(key, value, ACTIVE);
            capacity++;
            if ((double)capacity / array.size() > 0.9) {
                rehash();
            }
        }
    }

    // The function to remove a key-value pair.
    void remove(const Key& key) {
        int curr = findPos(key);
        if (isActive(curr)) {
            array[curr].info = DELETED;
        }
    }

    // The function to get unique word count and current load factor.
    void getUniqueWordCountAndLoadFactor() {
        cout << endl;
        cout << "After preprocessing, the unique word count is " << capacity  << ". Current load ratio is " << (double)capacity / array.size() << endl;
    }
};


#endif //BARIS_POME_BARIS_POME_HW3_HASHTABLE_H
