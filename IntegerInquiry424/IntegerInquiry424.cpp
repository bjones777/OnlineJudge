// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=6&page=show_problem&problem=365
//

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

struct DigitString
{
    explicit DigitString(const char* inDigits)
    : digits(inDigits)
    {}

    explicit DigitString(string&& inDigits)
        : digits(inDigits)
    {}

    DigitString& operator+=(const DigitString& rhs)
    {
        int endColumn = (int)max(rhs.digits.size(), digits.size());

        string newDigits;
        newDigits.reserve(endColumn+1);
        int carry = 0;
        for (int i = 0; i < endColumn; ++i)
        {
            int dai = ((int)digits.size())-1-i;
            int dbi = ((int)rhs.digits.size()) - 1 - i;
            int da = dai < digits.size() ? digits[dai] - '0' : 0;
            int db = dbi < rhs.digits.size() ? rhs.digits[dbi] - '0' : 0;
            int nds = da + db + carry;
            newDigits.push_back((nds % 10) + '0');
            carry = nds / 10;
        }
        if (carry != 0)
        {
            newDigits.push_back('1');
        }
        reverse(newDigits.begin(),newDigits.end());
        digits.swap(newDigits);
        return *this;
    }

    string digits;
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    DigitString sum("0");
    string line;
    while (getline(cin, line))
    {
        if(line == "0") break;
        sum += DigitString(move(line));
    }
    cout << sum.digits << "\n";
    return 0;
}

