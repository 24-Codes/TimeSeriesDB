#ifndef timeseries_hpp
#define timeseries_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class timeseries {
public:
    // Constructor and destructor
    timeseries();
    ~timeseries();

    // Check citation in timeseries.cpp
    timeseries(const timeseries& other);            // Copy constructor
    timeseries& operator=(const timeseries& other);  // Copy assignment

    // Methods for statistical operations
    double mean();
    bool is_monotonic();
    bool best_fit(double &m, double &b);

    // Data operations
    void load(std::string filename);
    void print();
    bool add(double y, double d);
    void update(double y, double d);
    void resize(int new_size);
    void check_and_resize();

    // Methods to set series name/code (P2)
    void s_country_name(std::string& name);
    void s_country_code(std::string& code);

    // Methods to return series name/code (P2)
    std::string g_country_name();
    std::string g_country_code();

    // Methods to get size and capacity (used by ts_p2)
    int get_size()     { return array_size; }
    int get_capacity() { return array_capacity; }

private:
    // Metadata for P2
    std::string country_name;
    std::string country_code;

    // Dynamic arrays for timeseries years and values
    int* years_array;
    double* data_array;
    int array_size;
    int array_capacity;
};

#endif
