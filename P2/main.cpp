#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "timeseries.cpp"
#include "countrydata.cpp"

int main(){

    timeseries ts;   // Only used for testing.
    countrydata cd;  // Countrydata object holding multiple timeseries of a country
    std::string command;
    std::string country_name;

    // Reads inputted commands by user ("LOAD_P2", "ADD_P2", "BIGGEST_P2", etc.) and executes them until "EXIT" is inputted
    while (std::cin >> command) {

        if (command == "EXIT"){

            break; // Terminate program

        } else if (command == "LOAD_P2"){ // Load all series data for an inputted country of the dataset

            std::cin >> country_name;
            cd.load_p2(country_name);

            std::cout << "success" << std::endl;

        } else if (command == "LIST_P2"){ // Lists (prints) the country name, its code, and all time series of the country

            cd.list_p2();

        } else if (command == "ADD_P2"){ // Adds a new (year, data) pair to a specific series

            std::string series_code;
            int year;
            double value;
            std::cin >> series_code >> year >> value;

            if (!cd.add_p2(series_code, year, value)) {
                std::cout << "failure" << std::endl;
            }

            std::cout << "success" << std::endl;

        } else if (command == "UPDATE_P2"){ // Updates data for a specific year in a series

            std::string series_code;
            int year;
            double value;
            std::cin >> series_code >> year >> value;

            if (!cd.update_p2(series_code, year, value)) {
                std::cout << "failure" << std::endl;
            }

            std::cout << "success" << std::endl;

        } else if (command == "PRINT_P2"){ // Prints all (year, data) pairs for a specific series

            std::string series_code;
            std::cin >> series_code;

            if (!cd.print_p2(series_code)) {
                std::cout << "failure" << std::endl;
            }

            std::cout << "success" << std::endl;

        } else if (command == "DELETE_P2"){ // Deletes a series from a country's data

            std::cout << "success" << std::endl;

        } else if (command == "BIGGEST_P2"){ // Finds the series with the largest mean and outputs it.

            std::cout << "failure" << std::endl;

        } else if (command == "TS_P2"){ // Outputs the size and capacity of the country array

            std::cout << "failure" << std::endl;

        }

    }

    return 0;
}
