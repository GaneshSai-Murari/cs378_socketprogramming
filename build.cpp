#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "build.h"
using namespace std;

bool buildMatrix(int adj[MAX_NODES][MAX_NODES], int& n) {

    ifstream file("oracle-node-config.txt");

    if (!file.is_open()) {
        cerr << "Could not open config file" << endl;
        return false;
    }

    int rows[MAX_NODES - 1][MAX_NODES - 1];
    int row_lengths[MAX_NODES - 1] = {};
    int row_count = 0;

    string line;
    while (getline(file, line)) {

        size_t pos = line.find_first_not_of(" \t\r\n");
        if (pos == string::npos || line[pos] == '#') {
            continue;
        }
        stringstream ss(line);
        int cost;
        int col_count = 0;

        while (ss >> cost) {
            rows[row_count][col_count++] = cost;
        }

        if (col_count > 0) {
            row_lengths[row_count] = col_count;
            ++row_count;
        }
    }

    file.close();

    if (row_count == 0) {
        return false;
    }

    n = row_count + 1;
    
    for (int i = 0; i < row_count; ++i) {
        if (row_lengths[i] != row_count - i) {
            cerr<<"Invalid file"<<endl;
            return false;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            adj[i][j] = (i==j) ? 0 : -1;
        }
    }

    for (int i = 0; i<n-1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            adj[i][j] = rows[i][j - i - 1];
            adj[j][i] = adj[i][j];
        }
    }

    return true;
}
