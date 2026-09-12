#ifndef p1_hpp
#define p1_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class timeseries {
    public: 

        //constructor and destructor
        timeseries();
        ~timeseries();

        double mean();
        bool is_monotonic();
        bool best_fit(double &m, double &b);
    
        void load(std::string filename);
        void print();
        void add(double y, double d);
        void update(double y, double d);

    private: 

        //discard after reading in
        std::string series_name;
        std::string series_code;

        //year and data arrays specs
        int* years_array;
        double* data_array;
        int array_size;
        int array_capacity;
};

#endif