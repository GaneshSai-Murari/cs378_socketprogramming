#include <iostream>
#include <vector>
#include "build.h"
using namespace std;

int main() {

    // 1
    vector<vector<int>> adj;
    if (!buildMatrix(adj)) {
        return 1;
    }

    // int n = adj.size();
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         cout << adj[i][j] << "\t";
    //     }
    //     cout << '\n';
    // }

    // 7



    return 0;
}