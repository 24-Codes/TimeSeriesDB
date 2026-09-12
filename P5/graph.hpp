#ifndef graph_hpp
#define graph_hpp

#include <string>
#include <vector>
#include <map>

#include "countrydata.hpp"
#include "tree.hpp"

// Stores a single (series_code, threshold, relation) relationship tuple
struct relationship {
    std::string series_code;
    double threshold;
    std::string relation;
};

// Stores countries as nodes and relationships as edges
class graph {
public:
    graph();
    ~graph();

    // Creates a node for every country and clears all edges
    void initialize(countrydata** countries, int count);

    // Prints all countries adjacent to the given country code
    void adjacent(std::string code);

    // Finds all countries satisfying condition, connects every pair
    bool update_edges(std::string series_code, double threshold, std::string relation, countrydata** countries, int country_count);

private:
    // Maps country code to country name
    std::map<std::string, std::string> nodes;

    // Adjacency list: maps country code to list of adjacent country codes
    std::map<std::string, std::vector<std::string>> adjacency;

    /*
        CITATION:

        The following private member variables were suggested with the help of Claude. I wasn't entirely
        comfortable with the C++ standard STL library and the map class. The AI suggested I use these
        member variables alongside the code for the UPDATE_EDGES_P5 command to implement the graph.
    */

    // Edge relationships: edges[code1][code2] stores list of relationship tuples
    // Smaller code always goes first to avoid storing the same edge twice
    std::map<std::string, std::map<std::string, std::vector<relationship>>> edges;

    // Returns normalized edge key with smaller code first
    std::pair<std::string, std::string> edge_key(std::string a, std::string b);

    // Checks if a relationship tuple already exists on an edge
    bool has_relationship(std::string code1, std::string code2, relationship& rel);

    // Adds a bidirectional adjacency if not already present
    void add_adjacency(std::string code1, std::string code2);
};

#endif
