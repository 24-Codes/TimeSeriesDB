#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "tree.hpp"

// Constructor to initialize interval bounds, empty arrays for country names and means, and null children
TreeNode::TreeNode(double l, double r){

    L = l;
    R = r;
    count = 0;
    cap = 2;
    left = nullptr;
    right = nullptr;

    // Arrays
    names = new std::string[cap];
    means = new double[cap];

}

// Destructor, recursively deletes children then frees country name/mean arrays
TreeNode::~TreeNode(){
    delete left;
    delete right;
    delete[] names;
    delete[] means;
}

// Adds a country and its mean to this node. Doubles capacity if the arrays are full.
void TreeNode::push(std::string name, double mean){

    // Double capacity if full
    if (count >= cap){
        int new_cap = cap * 2;
        std::string* new_names = new std::string[new_cap];
        double* new_means = new double[new_cap];

        // Copy existing data into new arrays
        for (int i = 0; i < count; i++){
            new_names[i] = names[i];
            new_means[i] = means[i];
        }

        // Free old arrays and update pointers
        delete[] names;
        delete[] means;
        names = new_names;
        means = new_means;
        cap = new_cap;
    }

    // Add the new country at the end
    names[count] = name;
    means[count] = mean;
    count++;
}

/*
    CITATION:

    The following recursive method was partially implemented with the help of Claude using the following
    prompt: "I'm trying to implement a binary tree where it builds its subtrees recursively, where the data
    (name/mean) in each node is passed down to the appropriate child (L or R). However, I'm having issues
    with coding the boundary intervals (L,R) and there seems to be a memory leak in my code. Can you find
    where it is?"
*/
TreeNode* Tree::build_node(double L, double R, std::string* names, double* means, int n){

    // Create node and add all countries to it
    TreeNode* node = new TreeNode(L, R);
    for (int i = 0; i < n; i++){
        node->push(names[i], means[i]);
    }

    // Stopping condition a: only 1 country
    if (n <= 1) return node;

    // Stopping condition b: all means within 1E-3
    bool all_equal = true;
    for (int i = 1; i < n; i++){
        if (std::fabs(means[i] - means[0]) > 1E-3){
            all_equal = false;
            break;
        }
    }
    if (all_equal) return node;

    // Split at midpoint
    double mid = (L + R) / 2.0;

    // Count how many countries go left vs right
    int left_n = 0;
    int right_n = 0;
    for (int i = 0; i < n; i++){
        if (means[i] < mid) left_n++;
        else right_n++;
    }

    // Build left child [L, mid) with countries whose mean is less than mid
    if (left_n > 0){

        std::string* ln = new std::string[left_n];
        double* lm = new double[left_n];
        int index = 0;

        for (int i = 0; i < n; i++){

            if (means[i] < mid){

                ln[index] = names[i];
                lm[index] = means[i];
                index++;
            }
        }

        node->set_left(build_node(L, mid, ln, lm, left_n));
        delete[] ln;
        delete[] lm;
    }

    // Build right child [mid, R] with countries whose mean is >= mid
    if (right_n > 0){

        std::string* rn = new std::string[right_n];
        double* rm = new double[right_n];
        int index = 0;

        for (int i = 0; i < n; i++){

            if (means[i] >= mid){
                rn[index] = names[i];
                rm[index] = means[i];
                index++;
            }

        }

        node->set_right(build_node(mid, R, rn, rm, right_n));

        delete[] rn;
        delete[] rm;
    }
    return node;
}

// Loops through names array and returns true if name is found
bool TreeNode::contains(std::string name){

    int i = 0;
    while (i < count){
        if (names[i] == name) return true;
        i++;
    }

    return false;
}

// Finds the country and overwrites it by shifting everything after it one spot to the left
void TreeNode::remove_entry(std::string name){

    int pos = 0;
    bool found = false;

    // Find position of country
    while (pos < count){

        if (names[pos] == name){
            found = true;
            break;
        }

        pos++;
    }

    if (!found){
        return;
    }
    // Overwrite by shifting left
    int j = pos;
    while (j < count - 1){
        names[j] = names[j + 1];
        means[j] = means[j + 1];
        j++;
    }

    count--;
}

// Constructor: initializes root node
Tree::Tree(){
    root = nullptr;
}

// Destructor: deletes root node
Tree::~Tree(){
    delete root;
}

// Builds the tree via BUILD_P3. Deletes any existing tree first.
void Tree::build(std::string* names, double* means, int n, double global_min, double global_max){

    delete root;

    root = build_node(global_min, global_max, names, means, n);
}

// Helper for FIND_P3. Only prints from leaf nodes since countries are stored in every node from root to
// leaf. If we printed from every node, we'd get duplicates.
void Tree::find_helper(TreeNode* node, double target, std::string operation, bool& printed_something){

    if (node == nullptr) {
        return;
    }

    // Check if node is a leaf node
    if (node->get_left() == nullptr && node->get_right() == nullptr){

        for (int i = 0; i < node->get_count(); i++){

            double m = node->get_mean(i);

            // Check if this country matches the operation
            bool matches = false;

            if (operation == "less"){
                matches = (m < target);
            }
            else if (operation == "greater"){
                matches = (m > target);

            }
            else if (operation == "equal"){
                matches = (std::fabs(m - target) <= 1E-3);
            }

            if (matches){
                if (printed_something) std::cout << " ";
                std::cout << node->get_name(i);
                printed_something = true;
            }
        }

        return;
    }

    // Not a leaf so recurse into children
    find_helper(node->get_left(), target, operation, printed_something);
    find_helper(node->get_right(), target, operation, printed_something);
}

// Finds and prints all countries whose mean satisfies operation. Prints a newline at the end
void Tree::find(double target, std::string operation){

    bool printed_something = false;
    find_helper(root, target, operation, printed_something);

    std::cout << std::endl;
}

// Recursively removes country from node and all descendants. Returns nullptr if node becomes an empty
// leaf, otherwise returns the node.
TreeNode* Tree::delete_helper(TreeNode* node, std::string name){

    // Base case
    if (node == nullptr){
        return nullptr;
    }

    // Skip nodes that don't have this country
    bool has_country = node->contains(name);
    if (!has_country) return node;

    // Remove from current node
    node->remove_entry(name);

    // Update left and right children recursively
    TreeNode* updated_left = delete_helper(node->get_left(), name);
    TreeNode* updated_right = delete_helper(node->get_right(), name);
    node->set_left(updated_left);
    node->set_right(updated_right);

    // Check if node has become an empty leaf
    bool no_left = (node->get_left() == nullptr);
    bool no_right = (node->get_right() == nullptr);
    bool empty = (node->get_count() == 0);

    if (no_left && no_right && empty){
        delete node;
        return nullptr;
    }

    return node;
}

// Public method to delete a country from the tree
bool Tree::delete_country(std::string name){

    // Can't delete if tree is empty
    if (root == nullptr){
        return false;
    }

    // Can't delete if country isn't even in the root
    bool in_tree = root->contains(name);
    if (!in_tree){
        return false;
    }

    root = delete_helper(root, name);

    return true;
}

// P5 helper, basically same logic as find_helper but stores results in a vector instead of printing.
void Tree::find_list(double target, std::string operation, std::vector<std::string>& results){

    if (!root) return;

    // Use a stack to traverse leaf nodes
    std::vector<TreeNode*> stack;
    stack.push_back(root);

    while (!stack.empty()){

        TreeNode* node = stack.back();
        stack.pop_back();

        bool is_leaf = (node->get_left() == nullptr && node->get_right() == nullptr);

        if (is_leaf){

            // Check each country in this leaf
            for (int i = 0; i < node->get_count(); i++){

                double m = node->get_mean(i);
                bool matches = false;

                if (operation == "less"){
                    matches = (m < target);
                }
                else if (operation == "greater"){
                    matches = (m > target);
                }
                else if (operation == "equal"){
                    matches = (std::fabs(m - target) <= 1E-3);
                }

                if (matches){
                    results.push_back(node->get_name(i));
                }
            }

        } else {
            if (node->get_left()) stack.push_back(node->get_left());
            if (node->get_right()) stack.push_back(node->get_right());
        }
    }
}
