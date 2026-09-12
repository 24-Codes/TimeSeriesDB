#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "timeseries.hpp"
#include "countrydata.hpp"
#include "tree.hpp"
#include "hash.hpp"
#include "graph.hpp"

// P5
graph p5_graph;

// P4
hash p4_hash;

// P3 helper functions/commands:

// Fixed-size array of country pointers and a count
countrydata* countries[512];
int country_count = 0;

// Pointer to binary tree
Tree* p3_tree = nullptr;

// Loads all countries and their time series from lab2_multidata.csv.
// Each line in the CSV file represents one timeseries for one country.
// Countries are stored in the fixed-size array, series are then appended by load_line().
void load_p3(){

    // Free any previously loaded country data
    for (int i = 0; i < country_count; i++){
        delete countries[i];
        countries[i] = nullptr;
    }

    country_count = 0;

    std::ifstream file("lab2_multidata.csv");
    std::string line;

    while (std::getline(file, line)){

        // Read just the country name and code to identify the country
        std::stringstream ss(line);
        std::string cn, cc;
        std::getline(ss, cn, ',');
        std::getline(ss, cc, ',');

        // Check if this country already has a slot in the array
        int index = -1;
        for (int i = 0; i < country_count; i++){
            if (countries[i]->g_country_name() == cn){ index = i; break; }
        }

        // If not found, create a new slot and set its name and code
        if (index == -1){
            countries[country_count] = new countrydata();
            countries[country_count]->s_country_name(cn);
            countries[country_count]->s_country_code(cc);
            index = country_count++;

            // Insert into hash table for P4 lookups
            p4_hash.insert(countries[index]);

        }

        // Append the series from this line to the appropriate country
        countries[index]->load_line(line);
    }
}

void list_p3(const std::string& country_name){

    // Search for the country by name and print its data if found
    for (int i = 0; i < country_count; i++){
        if (countries[i]->g_country_name() == country_name){
            countries[i]->list_p2();
            break;
        }
    }

}

// Finds the min and max mean for a given series code across all countries.
void range_p3(const std::string& series_code){

    double min_mean = 0.0;
    double max_mean = 0.0;
    bool first = true;

    // Loop all countries and find the series by code
    for (int i = 0; i < country_count; i++){

        // Search this country's series for the given code
        for (int j = 0; j < countries[i]->get_series_count(); j++){

            if (countries[i]->get_series_code(j) == series_code){
                double m = countries[i]->get_series_mean(j);

                // Only consider countries with valid data
                if (m >= 0.0){
                    if (first){
                        min_mean = m;
                        max_mean = m;
                        first = false;
                    } else {
                        if (m < min_mean) min_mean = m;
                        if (m > max_mean) max_mean = m;
                    }
                }
                break; // Found the series in this country, move to next
            }
        }
    }

    std::cout << min_mean << " " << max_mean << std::endl;
}

int main(){

    timeseries ts;  // P1 commands
    countrydata cd; // Countrydata object holding multiple timeseries of a country

    std::string command;
    std::string country_name;

    // Reads inputted commands by user ("LOAD_P2", "ADD_P2", "BIGGEST_P2", etc.) and executes them until "EXIT" is inputted
    while (std::cin >> command) {

        if (command == "EXIT"){

            break; // Terminate program

        } else if (command == "LOAD_P1"){

            std::string filename;
            std::cin >> filename;
            ts.load(filename);
            std::cout << "success" << std::endl;

        } else if (command == "PRINT_P1"){

            ts.print();

        } else if (command == "ADD_P1"){

            int year; double value;
            std::cin >> year >> value;
            if (ts.add(year, value)){
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }

        } else if (command == "UPDATE_P1"){

            int year; double value;
            std::cin >> year >> value;
            ts.update(year, value);

        } else if (command == "MEAN_P1"){

            double m = ts.mean();
            if (m < 0.0){
                std::cout << "failure" << std::endl;
            } else {
                std::cout << "mean is " << m << std::endl;
            }

        } else if (command == "MONOTONIC_P1"){

            ts.is_monotonic();

        } else if (command == "FIT_P1"){

            double m = 0.0, b = 0.0;
            ts.best_fit(m, b);

        } else if (command == "LOAD_P2"){ // Load all series data for an inputted country of the dataset

            std::getline(std::cin, country_name);
            if (!country_name.empty() && country_name[0] == ' '){
                country_name = country_name.substr(1);
            }
            cd.load_p2(country_name);

            std::cout << "success" << std::endl;

        } else if (command == "LIST_P2"){ // Lists (prints) the country name, its code, and all time series of the country

            cd.list_p2();

        } else if (command == "ADD_P2"){ // Adds a new (year, data) pair to a specific series

            std::string series_code;
            int year;
            double value;
            std::cin >> series_code >> year >> value;

            cd.add_p2(series_code, year, value);

        } else if (command == "UPDATE_P2"){ // Updates data for a specific year in a series

            std::string series_code;
            int year;
            double value;
            std::cin >> series_code >> year >> value;

            cd.update_p2(series_code, year, value);

        } else if (command == "PRINT_P2"){ // Prints all (year, data) pairs for a specific series

            std::string series_code;
            std::cin >> series_code;

            cd.print_p2(series_code);

        } else if (command == "DELETE_P2"){ // Deletes a series from a country's data

            std::string series_code;
            std::cin >> series_code;

            cd.delete_p2(series_code);

        } else if (command == "BIGGEST_P2"){ // Finds the series with the largest mean and outputs it.

            cd.biggest_p2();

        } else if (command == "TS_P2"){ // Outputs the size and capacity of the country array

            std::string series_code;
            std::cin >> series_code;

            cd.ts_p2(series_code);
        }

        ////////// P3 stuff /////////

        else if (command == "LOAD_P3"){

            load_p3();

            std::cout << "success" << std::endl;

        // LIST_P3 country_name
        }
        else if (command == "LIST_P3"){

            // Read country name
            std::getline(std::cin, country_name);

            if (!country_name.empty() && country_name[0] == ' '){
                country_name = country_name.substr(1);
            }

            list_p3(country_name);
        }

        //else if (command == "COUNTRY_MIN_P3"){   // not tested?

        //}

        else if (command == "RANGE_P3"){

            std::string series_code;
            std::cin >> series_code;

            range_p3(series_code);

        }
        else if (command == "BUILD_P3"){

            std::string series_code;
            std::cin >> series_code;

            // Arrays to store the names and means of countries for this series. 512 countries at most in dataset so fixed size is fine
            std::string valid_names[512];
            double valid_means[512];

            int valid_count = 0;
            double global_min = 0.0;
            double global_max = 0.0;

            // Used to initialize global_min and global_max on first valid entry
            bool first = true;

            // Loop through all countries and find the series matching the given code
            for (int i = 0; i < country_count; i++){
                for (int j = 0; j < countries[i]->get_series_count(); j++){

                    if (countries[i]->get_series_code(j) == series_code){

                        double m = countries[i]->get_series_mean(j);

                        // Only include countries with a valid mean
                        if (m >= 0.0){

                            valid_names[valid_count] = countries[i]->g_country_name();
                            valid_means[valid_count] = m;
                            valid_count++;

                            // Track global min and max for root interval
                            if (first || m < global_min) global_min = m;
                            if (first || m > global_max) global_max = m;
                            first = false;

                        }
                        break; // Found the series in this country, move to next country
                    }
                }
            }

            // Discard old tree if one exists
            delete p3_tree;
            // Build a new one
            p3_tree = new Tree();
            p3_tree->build(valid_names, valid_means, valid_count, global_min, global_max);

            std::cout << "success" << std::endl;
        }
        else if (command == "FIND_P3"){

            // Print failure if no tree
            if (!p3_tree){
                std::cout << "failure" << std::endl;

            }
            else {
                double target;
                std::string operation;
                std::cin >> target >> operation;

                p3_tree->find(target, operation);

            }

        }
        else if (command == "DELETE_P3"){

            // Read country name
            std::getline(std::cin, country_name);

            if (!country_name.empty() && country_name[0] == ' '){
                country_name = country_name.substr(1);
            }

            if (!p3_tree){
                std::cout << "failure" << std::endl;
            } else {
                bool result = p3_tree->delete_country(country_name);
                if (result){
                    std::cout << "success" << std::endl;
                } else {
                    std::cout << "failure" << std::endl;
                }
            }

        }
        else if (command == "LIMITS_P3"){

        }

        //else if (command == "TRACE_P3"){   // not tested?

        //}

        //////////////// P4 Commands //////////////////

        else if (command == "LOOKUP_P4"){

            std::string code;
            std::cin >> code;

            int searches = 0;
            int index = p4_hash.lookup(code, searches);

            if (index == -1){
                std::cout << "failure" << std::endl;
            }
            else {
                std::cout << "index " << index << " searches " << searches << std::endl;
            }
        }

        else if (command == "REMOVE_P4"){

            std::string code;
            std::cin >> code;

            // Remove from hash table
            bool removed = p4_hash.remove(code);

            if (!removed){

                std::cout << "failure" << std::endl;

            } else {
                // Also remove from tree if one exists
                if (p3_tree != nullptr){

                    // Find the country name by code first since tree uses names
                    for (int i = 0; i < country_count; i++){

                        if (countries[i]->g_country_code() == code){

                            p3_tree->delete_country(countries[i]->g_country_name());

                            break;
                        }
                    }
                }

                std::cout << "success" << std::endl;
            }

        } else if (command == "INSERT_P4"){

            std::string code;
            std::cin >> code;

            if (p4_hash.insert_from_csv(code, countries, country_count)){

                std::cout << "success" << std::endl;

            } else {
                std::cout << "failure" << std::endl;
            }

        } else if (command == "CLEAN_P4"){

            p4_hash.clean();
            std::cout << "success" << std::endl;

        }

        ///////////////////////// P5 ////////////////////////////

        else if (command == "INITIALIZE_P5"){

            p5_graph.initialize(countries, country_count);
            std::cout << "success" << std::endl;

        } else if (command == "ADJACENT_P5"){

            std::string code;
            std::cin >> code;

            p5_graph.adjacent(code);

        } else if (command == "UPDATE_EDGES_P5"){

            std::string series_code;
            double threshold;
            std::string relation;
            std::cin >> series_code >> threshold >> relation;

            if (p5_graph.update_edges(series_code, threshold, relation, countries, country_count)){
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }

        } else if (command == "PATH_P5"){

        } else if (command == "RELATIONSHIPS_P5"){

        }

    }

    /*
        CITATION:

        The following lines of code (before the return 0; line) were suggested by Claude to check for
        memory leaks / valgrind errors.
    */
    // Clean up country array
    for (int i = 0; i < country_count; i++){ delete countries[i]; countries[i] = nullptr; }
    // Clean up tree
    delete p3_tree;

    return 0;
}
