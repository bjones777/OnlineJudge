// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=12&page=show_problem&problem=989
//

#include <algorithm>
#include <cassert>
#include <limits>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Vector2D
{
public:
    Vector2D(int numRows, int numCols, const T& initial)
        : NumRows(numRows), NumColumns(numCols), Data(numRows * numCols, initial)
    {
    }

    T& operator()(int r, int c) {
        assert(r >= 0 && r < NumRows);
        assert(c >= 0 && c < NumColumns);
        return Data.at(r * NumColumns + c);
    }

    const T& operator()(int r, int c) const {
        assert(r >= 0 && r < NumRows);
        assert(c >= 0 && c < NumColumns);
        return Data.at(r * NumColumns + c);
    }

private:
    int NumRows;
    int NumColumns;
    vector<T> Data;
};

static const int INF = numeric_limits<int>::max();

struct Solver10048
{
    Solver10048(istream& is, int C, int S)
    : MinDecibels(C,C,INF)
    {
        for (int i = 0; i < C; ++i)
        {
            MinDecibels(i,i) = 0;
        }
        while (S-- > 0)
        {
            int Start, End, Cost;
            is >> Start >> End >> Cost;
            MinDecibels(Start-1, End-1) = Cost;
            MinDecibels(End - 1, Start - 1) = Cost;
        }

        bool updated = false;
        do 
        {
            updated = false;
            for (int i = 0; i < C; ++i)
            {
                for (int j = 0; j < C; ++j) 
                {
                    for (int k = 0; k < C; ++k) 
                    {
                        int oldValue = MinDecibels(i, j);
                        int newValue = min(MinDecibels(i, j), max(MinDecibels(i,k), MinDecibels(k, j)));
                        if (oldValue != newValue)
                        {
                            MinDecibels(i, j) = newValue;
                            updated = true;
                        }
                    }
                }
            }

        } while (updated);
        
    }

    Vector2D<int> MinDecibels;
};


static void go(int C, int S, int Q)
{
    Solver10048 solver(cin,C,S);
    while (Q-- > 0)
    {
        int Start, End;
        cin >> Start >> End;
        int Result = solver.MinDecibels(Start - 1, End - 1);
        if (Result == INF)
        {
            cout << "no path\n";
        }
        else
        {
            cout << Result << '\n';
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int C, S, Q;
    for(int TC = 1;(cin >> C >> S >> Q);++TC)
    {
        if(C == 0 && S == 0 && Q == 0) break;
        if(TC != 1) cout << '\n';
        cout << "Case #" << TC << '\n';
        go(C,S,Q);
    }
    return 0;
}

