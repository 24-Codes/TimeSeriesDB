#ifndef countrydata_hpp
#define countrydata_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "timeseries.hpp"

class countrydata {
public:
    // Constructor and destructor
    countrydata();
    ~countrydata();

    // Resizing functions from P1
    void resize(int new_size);
    void check_and_resize();

    // Clears all stored data of the countrydata object
    void clear();

    // Helper functions to set series name/code
    void s_country_name(std::string& name);
    void s_country_code(std::string& code);

    // Helper functions to return series name/code
    std::string g_country_name();
    std::string g_country_code();

    // Methods for P2
    int find_series(std::string& series_code);
    void load_p2(const std::string& country_name);
    void list_p2();
    bool add_p2(std::string& series_code, int y, double d);
    bool update_p2(std::string& series_code, int y, double d);
    bool print_p2(std::string& series_code);
    bool delete_p2(std::string& series_code);
    bool biggest_p2();
    bool ts_p2(std::string& series_code);

    // P3 helper functions:

    // Takes one line in the CSV file and appends series to country
    void load_line(std::string& line);

    // Get methods / accessors needed for range
    int get_series_count(){
        return country_array_size;
    }
    std::string get_series_code(int i){
        return country_array[i].g_country_code();
    }
    double get_series_mean(int i){
        return country_array[i].mean();
    }

private:
    // Metadata for P2
    std::string country_name;
    std::string country_code;

    // Dynamic array for storing multiple timeseries (data) for a country
    timeseries* country_array;
    int country_array_size;
    int country_array_capacity;
};

#endif
