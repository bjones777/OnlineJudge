// AddingReversedNumbers713.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

static string AddReversed(const string& a, const string& b)
{
    string result;
    int carry = 0;
    result.reserve(max(a.size(),b.size())+1);
    for (int i = 0, e = max((int)a.size(), (int)b.size()); i < e; ++i)
    {
        int da = i < (int)a.size() ? (a[i] - '0') : 0;
        int db = i < (int)b.size() ? (b[i] - '0') : 0;
        int sum = da + db + carry;
        result.push_back((char)(sum % 10 + '0'));
        carry = sum / 10;
    }
    if (carry != 0)
    {
        result.push_back('1');
    }
    // trim leading 0's
    int nonLeadingZeroIndex = 0;
    while (nonLeadingZeroIndex < (int)result.size() && result[nonLeadingZeroIndex] == '0')
    {
        ++nonLeadingZeroIndex;
    }
    return result.substr(nonLeadingZeroIndex,result.size() - nonLeadingZeroIndex);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    while (N-- > 0)
    {
        string a, b;
        cin >> a >> b;
        cout << AddReversed(a,b) << '\n';
    }
    return 0;
}

