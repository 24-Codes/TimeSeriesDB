#include "timeseries.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// Constructor definition
timeseries::timeseries(){

    array_capacity = 2;
    array_size = 0;
    years_array = new int[array_capacity];
    data_array = new double[array_capacity];

}

// Destructor definition
timeseries::~timeseries(){
    delete[] years_array;
    delete[] data_array;
}

/*
    CITATION:

    The following two methods were implemented with the help of Claude using the following prompt:
    "There seems to be a memory issue as I'm getting a 'trace trap' error when compiling. Can you find
    the reason for the error and give me pointers on how to fix this issue?"
*/
// Copy constructor: deep copy all heap arrays so two objects don't share memory
timeseries::timeseries(const timeseries& other){
    array_size     = other.array_size;
    array_capacity = other.array_capacity;
    country_name   = other.country_name;
    country_code   = other.country_code;

    years_array = new int[array_capacity];
    data_array  = new double[array_capacity];

    for (int i = 0; i < array_size; i++){
        years_array[i] = other.years_array[i];
        data_array[i]  = other.data_array[i];
    }
}

// Copy assignment operator: deep copy, handle self-assignment
timeseries& timeseries::operator=(const timeseries& other){
    if (this == &other) return *this; // Guard against self-assignment

    // Free existing heap memory
    delete[] years_array;
    delete[] data_array;

    array_size     = other.array_size;
    array_capacity = other.array_capacity;
    country_name   = other.country_name;
    country_code   = other.country_code;

    years_array = new int[array_capacity];
    data_array  = new double[array_capacity];

    for (int i = 0; i < array_size; i++){
        years_array[i] = other.years_array[i];
        data_array[i]  = other.data_array[i];
    }

    return *this;
}

// Allocates new arrays with a larger or smaller capacity and copies existing data into them.
void timeseries::resize(int new_size){

    // Do not shrink below minimum capacity
    if (new_size < 2){
        return;
    }

    // Copy existing elements into new arrays
    int* new_years_array = new int[new_size];
    double* new_data_array = new double[new_size];

    for (int i = 0; i < array_size; i++) {
        new_years_array[i] = years_array[i];
        new_data_array[i] = data_array[i];
    }

    // Delete old arrays from memory
    delete[] years_array;
    delete[] data_array;

    // Update pointers and capacity
    years_array = new_years_array;
    data_array = new_data_array;
    array_capacity = new_size;

    return;
}

// Automatically grows or shrinks the arrays depending on usage
void timeseries::check_and_resize(){

    // Double in size if array is full
    if (array_size >= array_capacity){
        resize(array_capacity * 2);
    }
    // Halve in size if array is less than or equal a quarter to capacity
    else if (array_size <= array_capacity / 4){
        resize(array_capacity / 2);
    }

    return;
}

// Reads a CSV file and stores all valid (non -1) values along with their corresponding years.
void timeseries::load(std::string filename){

    // Clears any previously loaded data if any
    delete[] years_array;
    delete[] data_array;

    array_size = 0;
    array_capacity = 2;
    years_array = new int[array_capacity];
    data_array = new double[array_capacity];

    std::fstream FileData(filename);
    std::string line;

    // Read each line of the CSV file
    while (std::getline(FileData, line)){

        std::stringstream ss(line);
        std::string text;

        std::getline(ss, text, ','); // Country name
        std::getline(ss, text, ','); // Country code
        std::getline(ss, text, ',');
        std::getline(ss, text, ',');

        int year = 1960;

        // Read yearly values
        while (std::getline(ss, text, ',')) {

            double value = std::stod(text);

            // Only store valid (!= -1) data
            if (value != -1) {
                check_and_resize();
                years_array[array_size] = year;
                data_array[array_size] = value;
                array_size++;
            }

            year++;
        }

    }

    return;
}

// Prints all (year, value) pairs
void timeseries::print(){

    if (array_size == 0){
        std::cout << "failure" << std::endl;
        return;
    }

    for (int i = 0; i < array_size; i++){
        std::cout << "(" << years_array[i] << "," << data_array[i] << ")";
        if (i < array_size - 1){
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

/*
    CITATION:

    The following add method was partially debugged by ChatGPT using the following prompt: "I have two
    dynamically allocated arrays and I'm trying to implement an add function where I can add in values for
    both arrays, but there might be a memory leak somewhere. Can you find it?"
*/
// Adds a (year, value) pair to the time series.
bool timeseries::add(double y, double d){

    // Case 1: empty array
    if (array_size == 0) {

        check_and_resize();
        years_array[0] = y;
        data_array[0] = d;
        array_size = 1;

        return true;
    }

    // Find insertion position
    int index = 0;
    while (index < array_size && years_array[index] < y) {
        index++;
    }
    // Case 2: year already exists -> failure
    if (index < array_size && years_array[index] == y) {
        return false;
    }
    // Case 3: insert new year
    check_and_resize();
    // Shift elements of array to make room
    for (int i = array_size; i > index; i--) {
        years_array[i] = years_array[i - 1];
        data_array[i]  = data_array[i - 1];
    }

    // Insert new values
    years_array[index] = y;
    data_array[index]  = d;
    array_size++;

    return true;

}

void timeseries::update(double y, double d){

    // Loop to search for inputted year
    for (int i = 0; i < array_size; i++) {

        if (years_array[i] == y) {

            // Case 1: If data is negative, remove this year and its value
            if (d < 0) {

                // Shift elements of array to the left due to the deleted year and value
                for (int j = i; j < array_size - 1; j++) {
                    years_array[j] = years_array[j + 1];
                    data_array[j]  = data_array[j + 1];
                }

                array_size--;

                // Resize array if needed
                check_and_resize();

                std::cout << "success" << std::endl;
                return;
            }

            // Case 2: If data is valid, update the value
            data_array[i] = d;
            std::cout << "success" << std::endl;
            return;
        }
    }

    // Case 3: If the year isn't in array, then failure
    std::cout << "failure" << std::endl;

    return;
}

// Computes the average of all stored data values. Returns 0.0 if no data exists.
double timeseries::mean(){

    // If array is empty, then no mean
    if (array_size == 0) {
        return -1.0;
    }

    double sum = 0.0;

    for (int i = 0; i < array_size; i++) {
        sum += data_array[i];
    }

    return sum / array_size;
}

bool timeseries::is_monotonic(){

    // If array is empty (no valid data), then output failure
    if (array_size == 0) {
        std::cout << "failure" << std::endl;
        return false;
    }

    // Edge case where series has only 1 data point which is considered monotonic
    if (array_size == 1) {
        std::cout << "series is monotonic" << std::endl;
        return true;
    }

    int i = 0;

    // Skip over equal consecutive values
    while ((i < array_size - 1 && data_array[i]) == data_array[i + 1]) {
        i++;
    }

    // All values are equal
    if (i == array_size - 1) {
        std::cout << "series is monotonic" << std::endl;
        return true;
    }

    // Determine expected direction
    bool increasing = data_array[i] < data_array[i + 1];
    bool decreasing = !increasing;

    // Check remaining elements follow the direction
    for (; i < array_size - 1; i++) {

        if (increasing && data_array[i] > data_array[i + 1]) {
            std::cout << "series is not monotonic" << std::endl;
            return false;
        }

        if (decreasing && data_array[i] < data_array[i + 1]) {
            std::cout << "series is not monotonic" << std::endl;
            return false;
        }
    }

    std::cout << "series is monotonic" << std::endl;

    return true;
}

bool timeseries::best_fit(double &m, double &b){

    // If don't have at least 2 points, then fail
    if (array_size < 2) {
        std::cout << "failure" << std::endl;
        return false;
    }

    double avg_x = 0.0;
    double avg_y = 0.0;

    // Average computations
    for (int i = 0; i < array_size; i++) {
        avg_x += years_array[i];
        avg_y += data_array[i];
    }

    avg_x /= array_size;
    avg_y /= array_size;

    double numer = 0.0;
    double denom = 0.0;

    // Deviations
    for (int i = 0; i < array_size; i++) {
        double dx = years_array[i] - avg_x;
        double dy = data_array[i] - avg_y;

        numer += dx * dy;
        denom += dx * dx;
    }

    // If invalid slope, then fail
    if (denom == 0) {
        std::cout << "failure" << std::endl;
        return false;
    }

    // Compute slope and intercept
    m = numer / denom;
    b = avg_y - m * avg_x;

    std::cout << "slope is " << m << " intercept is " << b << std::endl;

    return true;
}

void timeseries::s_country_name(std::string& name){
    country_name = name;
}
void timeseries::s_country_code(std::string& code){
    country_code = code;
}

std::string timeseries::g_country_name(){
    return country_name;
}
std::string timeseries::g_country_code(){
    return country_code;
}
