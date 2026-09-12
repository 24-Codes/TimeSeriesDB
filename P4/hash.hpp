#ifndef hash_hpp
#define hash_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "countrydata.hpp"

// Hash class, stores countries using double hashing
class hash {
public:
    // Constructor
    hash();
    // Destructor
    ~hash();

    // Insert a country into the hash table
    void insert(countrydata* c);

    // Look up a country by code, return index if found, -1 if not. Sets searches to num of hashing steps taken
    int lookup(std::string code, int& searches);

    // Removes a country by marking its slot as a tombstone
    bool remove(std::string code);

    // Reads CSV, creates countrydata object and inserts into hash table
    bool insert_from_csv(std::string code, countrydata** countries, int& country_count);

    void clean();

private:
    countrydata* table[512];
    int state[512]; // 0 = empty, 1 = occupied

    int compute_W(std::string code);
    int h1(int W);
    int h2(int W);
};

#endif
