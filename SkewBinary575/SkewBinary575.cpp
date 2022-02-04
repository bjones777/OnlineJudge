// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=7&page=show_problem&problem=516
//

#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

static int64_t go(const string& line)
{
    int64_t value = 0;
    int k = (int)line.size();
    for(int i = 0;i < (int)line.size();++i)
    {
        int digit = line[i] - '0';
        value += digit * ((1 << (k-i)) - 1);
    }
    return value;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    while (getline(cin, line))
    {
        if(line == "0") break;
        cout << go(line) << '\n';
    }
    return 0;
}