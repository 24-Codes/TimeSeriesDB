#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "hash.hpp"

// Constructor: initialize all slots as empty
hash::hash(){
    for (int i = 0; i < 512; i++){
        table[i] = nullptr;
        state[i] = 0;
    }
}

hash::~hash(){
    for (int i = 0; i < 512; i++){
        table[i] = nullptr;
    }
}

int hash::h1(int W){
    return W % 512;
}

int hash::h2(int W){

    int val = (W / 512) % 512;

    if (val % 2 == 0){
        val += 1;
    }

    return val;
}

// Convert 3-letter country code to base-26 number W
int hash::compute_W(std::string code){

    int d0 = code[0] - 'A';
    int d1 = code[1] - 'A';
    int d2 = code[2] - 'A';

    return (d0 * 676) + (d1 * 26) + d2;
}

// Insert a country into hash table via double hashing
void hash::insert(countrydata* c){

    int W = compute_W(c->g_country_code());
    int i = 0;

    while (i < 512){
        int index = (h1(W) + i * h2(W)) % 512;

        if (state[index] == 0){
            table[index] = c;
            state[index] = 1;
            return;
        }

        i++;
    }
}

// Look up a country by code. searches is always at least 1 since we always compute the primary hash
int hash::lookup(std::string code, int& searches){

    int W = compute_W(code);
    int i = 0;
    searches = 0;

    while (i < 512){

        int index = (h1(W) + i * h2(W)) % 512;
        searches++;

        // Empty slot, country is not in table
        if (state[index] == 0){
            return -1;
        }

        // Skip tombstones
        if (state[index] == 2){

            i++;
            continue;
        }

        // Found
        if (table[index]->g_country_code() == code){
            return index;
        }

        // Wrong country, continue
        i++;
    }

    return -1;
}

// Marks the slot as previously occupied (tombstone) instead of empty. Returns true if found and removed, false if not found.
bool hash::remove(std::string code){

    int searches = 0;
    int index = lookup(code, searches);

    if (index == -1){

        return false;
    }

    // Mark as a tombstone
    state[index] = 2;
    table[index] = nullptr;

    return true;
}

bool hash::insert_from_csv(std::string code, countrydata** countries, int& country_count){

    // Check if already in hash table, if already in table, false
    int searches = 0;
    if (lookup(code, searches) != -1){
        return false;
    }

    // First pass: find the country name for this code
    std::string country_name = "";
    std::string country_code = "";
    std::ifstream file("lab2_multidata.csv");
    std::string line;

    while (std::getline(file, line)){

        std::stringstream ss(line);
        std::string tmp_name, tmp_code;
        std::getline(ss, tmp_name, ',');
        std::getline(ss, tmp_code, ',');

        if (tmp_code == code){
            country_name = tmp_name;
            country_code = tmp_code;

            break;
        }
    }

    file.close();

    // Create new countrydata object and set its name and code
    countries[country_count] = new countrydata();
    countries[country_count]->s_country_name(country_name);
    countries[country_count]->s_country_code(country_code);

    // Second pass: load all series for this country
    std::ifstream file2("lab2_multidata.csv");
    while (std::getline(file2, line)){

        std::stringstream ss(line);
        std::string tmp_name, tmp_code;
        std::getline(ss, tmp_name, ',');
        std::getline(ss, tmp_code, ',');

        if (tmp_code == code){
            countries[country_count]->load_line(line);
        }
    }
    file2.close();

    // Insert into hash table and increment country count
    insert(countries[country_count]);
    country_count++;

    return true;

}

// Re-hashes table by: 1. collecting all currently occupied countries, 2. sorting them alphabetically by
// country code, 3. clearing the entire table, 4. re-inserting in sorted order.
void hash::clean(){

    // 1: collect all occupied countries
    countrydata* temp[512];
    int count = 0;

    for (int i = 0; i < 512; i++){

        if (state[i] == 1){

            temp[count] = table[i];
            count++;
        }
    }

    // 2: sort alphabetically by country code via bubble sort
    for (int i = 0; i < count - 1; i++){
        for (int j = 0; j < count - i - 1; j++){

            if (temp[j]->g_country_code() > temp[j + 1]->g_country_code()){

                countrydata* tmp = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = tmp;
            }
        }
    }

    // 3: clear entire table
    for (int i = 0; i < 512; i++){

        table[i] = nullptr;
        state[i] = 0;
    }

    // 4: re-insert in sorted order
    for (int i = 0; i < count; i++){

        insert(temp[i]);
    }

}
