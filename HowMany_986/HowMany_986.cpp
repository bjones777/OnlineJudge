// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=11&page=show_problem&problem=927
//

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Entry
{
    Entry(int x, int height, int peaks, bool wasPrevUp) 
    : X(x)
    , Height(height)
    , Peaks(peaks)
    , WasPrevUp(wasPrevUp) {}
    int X;
    int Height;
    int Peaks;
    bool WasPrevUp;

    bool operator==(const Entry &rhs) const
    {
        return (X == rhs.X
            && Height == rhs.Height
            && Peaks == rhs.Peaks
            && WasPrevUp == rhs.WasPrevUp);
    }
};

namespace std {

    template <>
    struct hash<Entry>
    {
        std::size_t operator()(const Entry& k) const
        {
            return (hash<int>()(k.X)
                ^ ((hash<int>()(k.Height) << 1) >> 1)
                ^ ((hash<int>()(k.Peaks) << 2) >> 2)
                ^ ((hash<bool>()(k.WasPrevUp) << 3) >> 3));
        }
    };
}

struct Solver
{
    Solver(int n, int r, int k) : N(n), R(r), K(k)
    {}

    int FindNumPaths(int x, int height, int peaks, bool wasPrevUp)
    {
        if (height < 0)
        {
            return 0;
        }
        // If we can't hit (2*N, 0) no point continuing
        int diff = 2 * N - x;
        if (height > diff)
        {
            return 0;
        }

        // are we at the end?
        if (diff == 0)
        {
            if (height == 0)
            {
                return (peaks == R) ? 1 : 0;
            }
            return 0;
        }

        Entry memoKey = Entry(x, height, peaks, wasPrevUp);
        auto iter = Memo.find(memoKey);
        if(iter != Memo.end())
        {
            return iter->second;
        }
        int Result = FindNumPaths(x+1, height+1, peaks, true);
        if (wasPrevUp && height == K)
        {
            peaks += 1;
            if (peaks > R)
            {
                Memo[memoKey] = Result;
                return Result;
            }
        }
        Result += FindNumPaths(x + 1, height - 1, peaks, false);
        Memo[memoKey] = Result;
        return Result;    
    }


    int N;
    int R;
    int K;
    unordered_map<Entry,int> Memo;
};


static int go(int n, int r, int k)
{
    Solver solver(n,r,k);
    return solver.FindNumPaths(0,0,0,false);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, r, k;
    while (cin >> n >> r >> k)
    {
        cout << go(n, r, k) << '\n';
    }
    return 0;
}

