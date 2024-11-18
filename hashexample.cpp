#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

int main() {
    string s = "some random string!";
    float a = 1.03f;
    char endl = '\n';
    auto h1 = hash<string>{}(s);
    auto h2 = hash<float>{}(a);
    cout << h1 << endl;
    cout << h2 << endl;
    return 0;
}