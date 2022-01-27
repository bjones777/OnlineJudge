// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=4&page=show_problem&problem=233
//

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

static int NumPixelsAtDepth(int depth)
{
    assert(depth >= 0 && depth <= 5);
    return 1 << (2 * (5-depth));
}

static void Advance(const string& a, int& ai)
{
    char ca = a[ai];
    if (ca != 'p')
    {
        ++ai;
        return;
    }
    ++ai;
    for (int i = 0; i < 4; ++i)
    {
        Advance(a,ai);
    }
}
static int CountBlack(const string& a, int& ai, int depth)
{
    char ca = a[ai];
    if(ca == 'e') 
    {
        ++ai;
        return 0;
    }
    if(ca == 'f') 
    {
        ++ai;
        return NumPixelsAtDepth(depth);
    }
    assert(ca == 'p');
    ++ai;
    int retVal = 0;
    for (int i = 0; i < 4; ++i)
    {
        retVal += CountBlack(a,ai,depth+1);        
    }
    return retVal;
}

static int CountBlack(const string& a, const string& b, int& ai, int& bi, int depth)
{
    int retVal = 0;
    char ca = a[ai];
    char cb = b[bi];
    if (ca == 'f')
    {
        retVal += NumPixelsAtDepth(depth);
        Advance(a, ai);
        Advance(b, bi);
        
    }
    else if (cb == 'f')
    {
        retVal += NumPixelsAtDepth(depth);
        Advance(a, ai);
        Advance(b, bi);
    }
    else if (ca == 'e' && cb == 'e')
    {
        Advance(a, ai);
        Advance(b, bi);
    }
    else if (ca == 'p' && cb == 'p')
    {
        ++ai;
        ++bi;
        for (int i = 0; i < 4; ++i)
        {
            retVal += CountBlack(a, b, ai, bi, depth+1);
        }
    }
    else if (ca == 'e' && cb == 'p')
    {
        ++ai;
        retVal += CountBlack(b,bi,depth);
    }
    else if (ca == 'p' && cb == 'e')
    {
        ++bi;
        retVal += CountBlack(a, ai, depth);
    }
    else
    {
        assert(false);
    }
    return retVal;

}

int go(const string& a, const string& b)
{
    int ai = 0;
    int bi = 0;
    int retVal = CountBlack(a,b,ai,bi,0);
    assert(ai == (int)a.size());
    assert(bi == (int)b.size());
    return retVal;
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
        cout << "There are " << go(a, b) << " black pixels.\n";
    }
    return 0;
}

