#ifndef tree_hpp
#define tree_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// Class for nodes of the binary tree. Each node stores an interval (L, R) and the names/means of all countries in that interval.
class TreeNode {
public:
    // Constructor and destructor
    TreeNode(double l, double r);
    ~TreeNode(); // Recursively deletes children

    // Setters for left and right children
    void set_left(TreeNode* n) { left = n; }
    void set_right(TreeNode* n) { right = n; }

    // Getters for FIND_P3
    int get_count() { return count; }
    std::string get_name(int i) { return names[i]; }
    double get_mean(int i) { return means[i]; }
    TreeNode* get_left() { return left; }
    TreeNode* get_right() { return right; }

    // Helper methods for DELETE_P3
    bool contains(std::string name);     // Checks if this node contains the given country name
    void remove_entry(std::string name); // Removes country from this node

    // Adds a country and its mean to this node
    void push(std::string name, double mean);

private:
    std::string* names; // Dynamic array of country names
    double* means;      // Means for each country
    int count;          // Number of countries currently stored
    int cap;            // Current capacity of the dynamic arrays

    double L, R;         // Interval bounds for this node
    TreeNode* left;       // Left child
    TreeNode* right;      // Right child
};

// Class for binary tree
class Tree {
public:
    // Constructor and destructor
    Tree();
    ~Tree();

    // Method to build binary tree from arrays of country names and means. If binary tree already exists, deletes that tree first (BUILD_P3)
    void build(std::string* names, double* means, int n, double global_min, double global_max);

    // Method to search the tree and print all countries whose mean falls under the operation (<=, ==, >=) (FIND_P3)
    void find(double target, std::string operation);

    // Method to remove a country from the tree. Returns true if found, false if not (DELETE_P3)
    bool delete_country(std::string name);

    // Helper for P5 graph: collects countries matching operation into a vector instead of printing
    void find_list(double target, std::string operation, std::vector<std::string>& results);

private:
    TreeNode* root;

    // Method to recursively build subtree for n countries over an interval (L, R)
    TreeNode* build_node(double L, double R, std::string* names, double* means, int n);

    // Recursive helper for find
    void find_helper(TreeNode* node, double target, std::string operation, bool& printed_something);

    // Recursive helper for delete
    TreeNode* delete_helper(TreeNode* node, std::string name);
};

#endif
