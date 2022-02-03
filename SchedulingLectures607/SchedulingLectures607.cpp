// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=548
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

struct Result607
{
    Result607()
    : NumLectures(numeric_limits<int>::max())
    , DissatisfactionIndex(numeric_limits<int>::max())
    {}

    Result607(int inNumLectures,int inDissatisfactionIndex)
    : NumLectures(inNumLectures), DissatisfactionIndex(inDissatisfactionIndex)
    {}

    bool operator<(const Result607& rhs) const
    {
        if(NumLectures < rhs.NumLectures) return true;
        if (NumLectures > rhs.NumLectures) return false;
        return DissatisfactionIndex < rhs.DissatisfactionIndex;
    }

    operator bool() const
    {
        return NumLectures != numeric_limits<int>::max() ||
               DissatisfactionIndex != numeric_limits<int>::max();
    }

    int NumLectures;
    int DissatisfactionIndex;
};

struct Solver607
{
    Solver607(int inL, int inC, const vector<int>& inTs)
    : L(inL), C(inC), Ts(inTs), Memo((int)inTs.size(),inL+1,Result607())
    {}

    Result607 FindMin()
    {
        return FindMin(0,L);
    }

    Result607 FindMin(int index,int TimeLeft)
    {
        if (index >= (int)Ts.size())
        {
            if(TimeLeft == L)
            {
                return Result607(0, 0);
            }
            return Result607(1, GetDissatisfactionIndex(TimeLeft));
        }
        Result607& result = Memo(index,TimeLeft);
        if (result)
        {
            return result;
        }
        // try to cover it this lecture
        if (Ts[index] <= TimeLeft)
        {
            result = FindMin(index+1, TimeLeft - Ts[index]);
        }
        // try to cover it next lecture
        if (TimeLeft != L)
        {
            Result607 resultNext = FindMin(index, L);
            resultNext.NumLectures += 1;
            resultNext.DissatisfactionIndex += GetDissatisfactionIndex(TimeLeft);
            result = min(result, resultNext);
        }
        return result;
    }

    int GetDissatisfactionIndex(int TimeLeft)
    {
        if (TimeLeft == 0)
        {
            return 0;
        }
        if (TimeLeft <= 10)
        {
            return -C;
        }
        return (TimeLeft - 10) * (TimeLeft - 10);
    }

    int L;
    int C; 
    const vector<int>& Ts;

    Vector2D<Result607> Memo;
};

static void go(int L, int C, const vector<int>& ts)
{
    Solver607 solver(L,C,ts);
    Result607 result = solver.FindMin();
    cout << "Minimum number of lectures: " << result.NumLectures << '\n';
    cout << "Total dissatisfaction index: " << result.DissatisfactionIndex << '\n';
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    for (int TC = 1;cin >> N;++TC)
    {
        if(N == 0) break;
        if (TC != 1)
        {
            cout << '\n';
        }
        cout << "Case " << TC << ":\n";
        int L, C;
        cin >> L >> C;
        vector<int> ts;
        ts.reserve(N);
        int nt = N;
        while (nt-- > 0)
        {
            int t;
            cin >> t;
            ts.push_back(t);
        }
        go(L, C, ts);
    }
    return 0;
}

