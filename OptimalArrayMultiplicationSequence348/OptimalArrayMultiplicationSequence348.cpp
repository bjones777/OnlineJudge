// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=5&page=show_problem&problem=284
//

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
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

struct Dim
{
    Dim(istream& is)
    {
        is >> Rows >> Columns;
    }
    Dim(int64_t r, int64_t c) 
    : Rows(r)
    , Columns(c)
    {}

    int64_t Rows;
    int64_t Columns;
};

static const int64_t INF = numeric_limits<int64_t>::max();

struct Result
{
    Result()
    : NumMultiplications(INF)
    , Dimensions(0,0)
    {
        
    }

    Result(int64_t numMultiplications, const Dim& dim, const string& rep)
    : NumMultiplications(numMultiplications)
    , Dimensions(dim)
    , Rep(rep)
    {}

    Dim Dimensions;

    int64_t NumMultiplications;

    string Rep;
};

struct Solver348
{
    Solver348(const vector<Dim>& dims)
    : Dims(dims)
    , Memo((int)dims.size()+1, (int)dims.size()+1, Result{})
    {}

    Result FindMinMultiplies()
    {
        return FindMinMultiplies(0, (int)Dims.size());
    }

    Result FindMinMultiplies(int startIndex, int endIndex)
    {
        assert(startIndex != endIndex);
        Result& res = Memo(startIndex, endIndex);
        if(res.NumMultiplications != INF)
        {
            return res;
        }
        if (startIndex+1 == endIndex)
        {
            ostringstream oss;
            oss << "A" << (startIndex + 1);
            res = {
                0
                , Dims[startIndex]
                , oss.str()
                };
            return res;
        }
        if (startIndex + 2 == endIndex)
        {
            ostringstream oss;
            oss << "(A" << (startIndex + 1) << " x A" << (startIndex+2) << ")";
            res = { Dims[startIndex].Rows * Dims[startIndex].Columns * Dims[startIndex+1].Columns
            , {Dims[startIndex].Rows,  Dims[startIndex + 1].Columns}
            , oss.str()
            };
            return res;
        }
        for (int i = startIndex+1; i < endIndex; ++i)
        {
            Result a = FindMinMultiplies(startIndex,i);
            Result b = FindMinMultiplies(i,endIndex);
            int64_t numMultiplications = a.NumMultiplications
                + b.NumMultiplications
                + a.Dimensions.Rows * a.Dimensions.Columns * b.Dimensions.Columns;
            if (numMultiplications < res.NumMultiplications)
            {
                res = {
                    numMultiplications
                    , { a.Dimensions.Rows, b.Dimensions.Columns}
                    , string("(") + a.Rep + " x " + b.Rep + ")"
                };
            }
        }
        return res;
    }

    const vector<Dim>& Dims;
    Vector2D<Result> Memo;
};

static void go(const vector<Dim>& dims)
{
    Solver348 solver(dims);
    cout << solver.FindMinMultiplies().Rep;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    for (int TC = 1;cin >> N;++TC)
    {
        if(N == 0) break;
        vector<Dim> Dims;
        while (N-- > 0)
        {
            Dims.emplace_back(cin);
        }
        cout << "Case " << TC << ": ";
        go(Dims);
        cout << '\n';
    }
    return 0;
}