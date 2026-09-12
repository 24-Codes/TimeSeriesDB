#include "graph.hpp"
#include <iostream>

graph::graph(){}
graph::~graph(){}

// Sets up a graph node for every country and clears any existing edges
void graph::initialize(countrydata** countries, int count){

    // Clear existing graph data
    nodes.clear();
    adjacency.clear();

    // Create a node for each country
    for (int i = 0; i < count; i++){

        std::string code = countries[i]->g_country_code();
        std::string name = countries[i]->g_country_name();

        // Store country code -> name mapping
        nodes[code] = name;

        // Initialize empty adjacency list for this country
        adjacency[code] = std::vector<std::string>();
    }
}

void graph::adjacent(std::string code){

    // Check if country is in graph
    if (nodes.find(code) == nodes.end()){

        std::cout << "failure" << std::endl;
        return;
    }

    // Checks if country has any adjacent countries
    if (adjacency[code].empty()){

        std::cout << "none" << std::endl;
        return;
    }

    // Print all adjacent country names separated by spaces
    bool first = true;
    for (auto& adj_code : adjacency[code]){

        if (!first) std::cout << " ";
        std::cout << nodes[adj_code];
        first = false;
    }

    std::cout << std::endl;
}

// Refer to citation in graph.hpp for details of following graph class member variables.

// Returns normalized edge key with smaller code first.
// This avoids storing the same edge twice (e.g. AFG-ALB and ALB-AFG)
std::pair<std::string, std::string> graph::edge_key(std::string a, std::string b){
    if (a < b) return {a, b};
    return {b, a};
}

// Checks if a given relationship tuple already exists on the edge between code1 and code2
bool graph::has_relationship(std::string code1, std::string code2, relationship& rel){

    auto key = edge_key(code1, code2);

    // Check if edge exists at all
    if (edges.find(key.first) == edges.end()) return false;
    if (edges[key.first].find(key.second) == edges[key.first].end()) return false;

    // Check if this exact tuple already exists
    for (auto& r : edges[key.first][key.second]){
        if (r.series_code == rel.series_code &&
            r.threshold == rel.threshold &&
            r.relation == rel.relation){
            return true;
        }
    }
    return false;
}

// Adds a bidirectional adjacency between two countries if not already present
void graph::add_adjacency(std::string code1, std::string code2){

    bool already_adjacent = false;
    for (auto& c : adjacency[code1]){
        if (c == code2){
            already_adjacent = true;
            break;
        }
    }

    if (!already_adjacent){
        adjacency[code1].push_back(code2);
        adjacency[code2].push_back(code1);
    }
}

// Builds a temporary tree for the series, finds all countries satisfying the condition, then connects
// every pair with the relationship tuple. Returns true if at least one new relationship was added.
bool graph::update_edges(std::string series_code, double threshold, std::string relation, countrydata** countries, int country_count){

    // Collect all countries with valid data for this series
    std::string valid_names[512];
    double valid_means[512];
    int valid_count = 0;
    double global_min = 0.0;
    double global_max = 0.0;
    bool first = true;

    for (int i = 0; i < country_count; i++){
        for (int j = 0; j < countries[i]->get_series_count(); j++){

            if (countries[i]->get_series_code(j) == series_code){
                double m = countries[i]->get_series_mean(j);

                if (m >= 0.0){

                    valid_names[valid_count] = countries[i]->g_country_name();
                    valid_means[valid_count] = m;
                    valid_count++;

                    if (first || m < global_min) global_min = m;
                    if (first || m > global_max) global_max = m;

                    first = false;
                }

                break;
            }
        }
    }

    // No valid data
    if (valid_count == 0) return false;

    // Build a temp tree for this series
    Tree temp_tree;
    temp_tree.build(valid_names, valid_means, valid_count, global_min, global_max);

    // Find all country names satisfying condition
    std::vector<std::string> matching_names;
    temp_tree.find_list(threshold, relation, matching_names);

    // Need at least 2 countries to form an edge
    if (matching_names.size() < 2) return false;

    // Convert matching country names to codes
    std::vector<std::string> matching_codes;

    for (auto& name : matching_names){
        for (int i = 0; i < country_count; i++){

            if (countries[i]->g_country_name() == name){

                matching_codes.push_back(countries[i]->g_country_code());
                break;
            }
        }
    }

    // Build the relationship tuple
    relationship rel;
    rel.series_code = series_code;
    rel.threshold = threshold;
    rel.relation = relation;

    // Connect every pair of matching countries
    bool added = false;

    for (int i = 0; i < (int)matching_codes.size(); i++){
        for (int j = i + 1; j < (int)matching_codes.size(); j++){

            std::string c1 = matching_codes[i];
            std::string c2 = matching_codes[j];

            // Skip if this exact relationship already exists on this edge
            if (has_relationship(c1, c2, rel)){
                continue;
            }

            // Add adjacency if not already adjacent
            add_adjacency(c1, c2);

            // Add the relationship tuple to the edge
            auto key = edge_key(c1, c2);
            edges[key.first][key.second].push_back(rel);
            added = true;
        }
    }

    return added;
}
