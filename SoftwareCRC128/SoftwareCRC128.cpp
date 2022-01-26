// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=64
//

#include <iostream>
#include <string>

using namespace std;

static const int g = 34943;

void outputHexDigit(int hex)
{
    if (hex < 10)
    {
        cout << (char)(hex + '0');
        return;
    }
    cout << (char)((hex-10) + 'A');
}

void outputHex(int byte)
{
    int high = byte / 16;
    int low = byte % 16;
    outputHexDigit(high);
    outputHexDigit(low);
}   

void go(const string& line)
{
    int remainder = 0;
    for (char ch : line)
    {
        remainder *= 256;
        remainder += ch;
        remainder %= g;
    }
    remainder *= 256;
    remainder %= g;
    remainder *= 256;
    remainder %= g;
    remainder = (g - remainder) % g;
    int top = remainder / 256;
    int bottom = remainder % 256;
    outputHex(top);
    cout << ' ';
    outputHex(bottom);
    cout << '\n';

}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    while (getline(cin, line))
    {
        if(!line.empty() && line.front() == '#') break;
        go(line);
    }
    return 0;
}
