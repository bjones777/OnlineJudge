// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=8&page=show_problem&problem=577
//

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

static bool IsSquare(int64_t t)
{
    int64_t lb = 1;
    int64_t ub = t;
    while (lb <= ub)
    {
        int64_t mid = (ub - lb)/2 + lb;
        int64_t m2 = mid * mid;
        if (m2 == t) 
        {
            return true;
        }
        if (m2 > t)
        {
            ub = mid-1;
        }
        else
        {
            lb = mid+1;
        }
    }
    return false;
}

static int64_t ToNumber(const string& line, int64_t base)
{
    int64_t value = 0;
    int64_t column = 1;
    for (auto iter = line.rbegin(); iter != line.rend(); ++iter, column *= base)
    {
        value += column * (*iter-'0');
    }
    return value;
}

static int go(const string& line)
{
    auto iter = max_element(line.begin(),line.end());
    for (int base = max(2,(*iter-'0')+1); base < 100; ++base)
    {
        if (IsSquare(ToNumber(line, base)))
        {
            return base;
        }
    }
    return -1;
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