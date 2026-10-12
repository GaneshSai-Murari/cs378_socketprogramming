#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "build.h"
using namespace std;

bool buildMatrix(vector<vector<int>>& adj){

    ifstream file("oracle-node-config.txt");
    string line;
    vector<vector<int>> rows;

    while (getline(file, line)) {
        size_t pos = line.find_first_not_of(" \t\r\n");
        if (pos == string::npos || line[pos] == '#') {
            continue;
        }
        stringstream ss(line);
        vector<int> costs;
        int cost;
        while (ss >> cost) {
            costs.push_back(cost);
        }
        if (!costs.empty()) {
            rows.push_back(costs);
        }
    }

    file.close();
    int n = rows.size() + 1;

    if (rows.empty()) {
        return false;
    }

    adj.assign(n, vector<int>(n, -1));

    for (int i = 0; i < n; i++) {
        adj[i][i] = 0;
    }

    for (int i=0;i<n-1;i++) {
        for (int j=i+1;j<n;j++) {
            adj[i][j] = rows[i][j-i-1];
            adj[j][i] = adj[i][j];
        }
    }
    return true;
}
