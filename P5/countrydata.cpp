#include "countrydata.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// Constructor definition
countrydata::countrydata(){

    country_array_size = 0;
    country_array_capacity = 2;
    country_array = new timeseries[country_array_capacity];
    country_name = "";
    country_code = "";

}

// Destructor definition
countrydata::~countrydata(){
    delete[] country_array;
}

// Basically same implementation as P1 resize method
void countrydata::resize(int new_country_array_size){

    // Do not shrink below minimum capacity
    if (new_country_array_size < 2){
        return;
    }

    // Copy existing elements into new arrays
    timeseries* new_country_array = new timeseries[new_country_array_size];

    for (int i = 0; i < country_array_size; i++) {
        new_country_array[i] = country_array[i];
    }

    // Delete old arrays from memory
    delete[] country_array;

    // Update pointers and capacity
    country_array = new_country_array;
    country_array_capacity = new_country_array_size;

}

// Method to automatically check if array size needs resizing
void countrydata::check_and_resize(){

    // Double in size if array is full
    if (country_array_size >= country_array_capacity){
        resize(country_array_capacity * 2);
    }
    // Halve in size if array is less than or equal a quarter to capacity
    else if (country_array_size <= country_array_capacity / 4){
        resize(country_array_capacity / 2);
    }

    return;
}

// Clears all stored data. Essentially "resets" object.
void countrydata::clear(){

    delete[] country_array;
    country_array_size = 0;
    country_array_capacity = 2;
    country_array = new timeseries[country_array_capacity];
    country_name = "";
    country_code = "";

}

// Methods to set series name/code
void countrydata::s_country_name(std::string& name){
    country_name = name;
}
void countrydata::s_country_code(std::string& code){
    country_code = code;
}

// Methods to return series name/code
std::string countrydata::g_country_name(){
    return country_name;
}
std::string countrydata::g_country_code(){
    return country_code;
}

// Method to find a specific series via its code
int countrydata::find_series(std::string& series_code){
    for (int i = 0; i < country_array_size; i++) {
        if (country_array[i].g_country_code() == series_code) {
            return i;
        }
    }
    return -1;
}

/*
    CITATION:

    The following load was partially debugged by ChatGPT using the following prompt: "I'm trying to load
    data from a dataset into a dynamic array using a 'load' function, but I'm getting a 'trace trap' error
    after compilation. What does that mean and do you know how to fix it?"
*/
// Loads data for a single country from dataset
void countrydata::load_p2(const std::string& country_name){

    // Clears any previously loaded data if any and reinitializes country array
    clear();

    std::fstream country_data("lab2_multidata.csv");
    std::string line;

    while (std::getline(country_data, line)){

        std::stringstream ss(line);
        std::string text;

        std::getline(ss, text, ','); // Country name

        // Skip if not requested country
        if (text != country_name){
            continue;
        }

        std::getline(ss, text, ','); // Country code
        country_code = text;
        this->country_name = country_name;

        std::getline(ss, text, ','); // Series name
        std::string series_name = text;
        std::getline(ss, text, ','); // Series code
        std::string series_code = text;

        // Create new timeseries object
        timeseries ts;
        ts.s_country_code(series_code);
        ts.s_country_name(series_name);

        int year = 1960;

        while (std::getline(ss, text, ',')){

            double value = std::stod(text);

            if (value >= 0) {
                ts.add(year, value); // Add data to timeseries
            }

            year++;
        }

        check_and_resize();

        country_array[country_array_size] = ts;
        country_array_size++;
    }

    country_data.close();

    return;
}

// Lists the country name, code, and all of its series
void countrydata::list_p2(){

    std::cout << country_name << " " << country_code;

    for (int i = 0; i < country_array_size; i++){
        std::cout << " " << country_array[i].g_country_name();
    }
    std::cout << std::endl;
}

// Adds data to a specific series
bool countrydata::add_p2(std::string& series_code, int y, double d){

    int index = find_series(series_code);
    if (index == -1) {
        std::cout << "failure" << std::endl;
        return false;
    }

    if (country_array[index].add(y, d)) { // Was: country_array[index].add(y, d); return true;
        std::cout << "success" << std::endl;
        return true;
    } else {
        std::cout << "failure" << std::endl;
        return false;
    }

}

// Update data in a series
bool countrydata::update_p2(std::string& series_code, int y, double d){

    int index = find_series(series_code);
    if (index == -1) { // If series not found, then return failure
        std::cout << "failure" << std::endl;
        return false;
    }

    country_array[index].update(y, d); // Calls timeseries::update
    return true;

return 0.0;
}

// Prints a specific series
bool countrydata::print_p2(std::string& series_code){

    int index = find_series(series_code);
    if (index == -1) { // If series not found, then return failure
        std::cout << "failure" << std::endl;
        return false;
    }

    country_array[index].print(); // Calls timeseries::print
    return true;

return 0.0;
}

bool countrydata::delete_p2(std::string& series_code){

    int index = find_series(series_code);
    if (index == -1) { // If series not found, then return failure
        std::cout << "failure" << std::endl;
        return false;
    }

    // Shifts all series after index to the left after deletion
    for (int i = index; i < country_array_size - 1; i++){
        country_array[i] = country_array[i + 1];
    }
    country_array_size--;

    check_and_resize(); // Check to see if array size is too large or small and adjust its size if needed
    std::cout << "success" << std::endl;

    return true;

return 0.0;
}

// Finds and outputs the series with the largest mean. In case of a tie, outputs the first one in order of dataset.
bool countrydata::biggest_p2(){

    std::string best_code = "";
    double best_mean = -1.0;
    bool found = false;

    for (int i = 0; i < country_array_size; i++){
        double m = country_array[i].mean();
        // mean() returns -1.0 if no valid data — skip those
        if (m >= 0.0 && (!found || m > best_mean)){
            best_mean = m;
            best_code = country_array[i].g_country_code();
            found = true;
        }
    }

    if (!found){
        std::cout << "failure" << std::endl;
        return false;
    }

    std::cout << best_code << std::endl;
    return true;
}

// Outputs the size and capacity of the country array
bool countrydata::ts_p2(std::string& series_code){

    int index = find_series(series_code);
    if (index == -1){ // If series not found, then return failure
        std::cout << "failure" << std::endl;
        return false;
    }

    std::cout << "size is " << country_array[index].get_size()
              << " capacity is " << country_array[index].get_capacity() << std::endl;
    return true;

return 0.0;
}

// P3 stuff:

// Called by load_p3() once per line in the dataset
void countrydata::load_line(std::string& line){

    std::stringstream ss(line);
    std::string text;

    // Skip country name and code (already stored by s_country_name/s_country_code)
    std::getline(ss, text, ',');
    std::getline(ss, text, ',');

    // Read series name and code
    std::getline(ss, text, ',');
    std::string series_name = text;
    std::getline(ss, text, ',');
    std::string series_code = text;

    // Create a new timeseries object and set its metadata
    timeseries ts;
    ts.s_country_name(series_name);
    ts.s_country_code(series_code);

    // Read yearly values starting from 1960, and skip any negative/invalid values
    int year = 1960;
    while (std::getline(ss, text, ',')){
        double value = std::stod(text);
        if (value >= 0){
            ts.add(year, value);
        }
        year++;
    }

    // Grow the country array if needed, then append the new timeseries
    check_and_resize();
    country_array[country_array_size] = ts;
    country_array_size++;
}
