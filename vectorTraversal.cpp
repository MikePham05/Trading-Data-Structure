#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <thread>
#include <chrono>

using namespace std;

/*
    Very easy example of memory locality. 2d array traversal
*/

int main() {
    int n = 5000;
    const int rows = 5000;
    const int cols = 5000;

    // Dynamically allocate a 2D array using std::vector
    std::vector<std::vector<int>> a(rows, std::vector<int>(cols));


    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            a[i][j] = 0;
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> row_first = end - start;


    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            a[j][i] = 0;
    end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> col_first = end - start;

    cout << "Row first: " << row_first.count() << '\n';
    cout << "Col first: " << col_first.count() << '\n';
    return 0;
}