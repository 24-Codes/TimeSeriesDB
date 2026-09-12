#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "timeseries.hpp"
#include "timeseries.cpp"

int main(){

    timeseries ts;
    std::string command;

    while (std::cin >> command) {

        if (command == "EXIT"){
            break;

        } else if (command == "LOAD_P1"){

            std::string filename;
            std::cin >> filename;
            std::cout << "success\n";

        } else if (command == "PRINT_P1"){

            std::cout << "failiure\n";

        } else if (command == "ADD_P1"){

            std::cout << "success\n";

        } else if (command == "UPDATE_P1"){

            std::cout << "success\n";

        } else if (command == "MEAN_P1"){

            double mean {};
            std::cout << "mean is " << mean << "\n";

        } else if (command == "MONOTONIC_P1"){

            std::cout << "series is monotonic\n";

        } else if (command == "FIT_P1"){

            double slope, y_int {};
            std::cout << "slope is " << slope << " intercept is " << y_int << "\n";

        } else if (command == "EXIT"){

        }

    }
    return 0;
}
