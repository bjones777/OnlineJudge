// Anagram195.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

struct AlphabeticFunctor
{
    bool operator()(char a, char b) {
        char upperA = toupper(a);
        char upperB = toupper(b);
        if (upperA == upperB)
        {
            return a < b;
        }
        return upperA < upperB;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    AlphabeticFunctor af;
    string line;
    getline(cin, line);
    int N = atoi(line.c_str());
    while (N-- > 0)
    {
        
        getline(cin, line);
        sort(line.begin(), line.end(), af);
        do 
        {
            cout << line << '\n';
        } while (next_permutation(line.begin(), line.end(), af));
    }
    return 0;
}

