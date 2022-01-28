// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=5&page=show_problem&problem=269
//

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

string trim(const string& s)
{
    int startIndex = 0;
    while (startIndex < (int)s.size() && isspace(s[startIndex]))
    {
        ++startIndex;
    }
    int endIndex = (int)s.size();
    while (endIndex - 1 >= startIndex && isspace(s[endIndex - 1]))
    {
        --endIndex;
    }
    return s.substr(startIndex, endIndex-startIndex);
}

bool validate(const string& s)
{
    int NumDigits = 0;
    int RunningSum = 0;
    int PartialSum = 0;
    for (char ch : s)
    {
        if(ch == '-') continue;
        if (isdigit(ch) || ch == 'X')
        {
            ++NumDigits;
            if (NumDigits > 10)
            {
                return false;
            }
            if (NumDigits < 10 && ch == 'X')
            {
                return false;
            }
            int value = 10;
            if (isdigit(ch))
            {
                value = ch - '0';
            }
            PartialSum += value;
            RunningSum += PartialSum;
            if(NumDigits == 10 && RunningSum % 11 != 0)
            { 
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return NumDigits == 10;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    while (getline(cin,line))
    {
        string t = trim(line);
        if (validate(t))
        {
            cout << t << " is correct.\n";
        }
        else
        {
            cout << t << " is incorrect.\n";
        }
    }
    return 0;    
}
