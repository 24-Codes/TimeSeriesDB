#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "timeseries.hpp"

//discard after reading in
std::string series_name;
std::string series_code;

//year and data arrays specs
int* years_array;
double* data_array;
int array_size;
int array_capacity;

//constructor and destructor defn's
timeseries::timeseries(){
    array_capacity = 2;
    array_size = 0;
    years_array = new int[array_capacity];
    data_array = new double[array_capacity];
}

timeseries::~timeseries(){
    delete[] years_array;
    delete[] data_array;
}

void load(std::string filename){}
void print(){}
void add(double y, double d){}
void update(double y, double d){}


double timeseries::mean(){
return 0.0;
}

bool timeseries::is_monotonic(){
return 0;
}

bool timeseries::best_fit(double &m, double &b){
return 0;
}