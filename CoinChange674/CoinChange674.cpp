// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=8&page=show_problem&problem=615
//

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Vector2D
{
public:
    Vector2D(int numRows, int numCols, const T& initial)
        : Data(numRows * numCols, initial)
        , NumRows(numRows)
        , NumColumns(numCols)
    {
        assert(numRows > 0);
        assert(numCols > 0);
    }

    const T& operator()(int row, int col) const {
        assert(row >= 0 && row < NumRows);
        assert(col >= 0 && col < NumColumns);
        return Data[row * NumColumns + col];
    }

    T& operator()(int row, int col) {
        assert(row >= 0 && row < NumRows);
        assert(col >= 0 && col < NumColumns);
        return Data[row * NumColumns + col];
    }

private:
    vector<T> Data;
    int NumRows;
    int NumColumns;
};


const int64_t UNINITIALIZED = -1;
const int MAX_CENTS = 7489;
Vector2D<int64_t> Memo(MAX_CENTS+1,5, UNINITIALIZED);

static int64_t GetNumberOfWaysToMakeChangeFor(int Amount, int MaxCoin)
{
    if (Amount == 0) {
        return 1;
    }

    int64_t& total = Memo(Amount, MaxCoin);
    if (total != UNINITIALIZED)
    {
        return total;
    }
    total = 0;
    if (Amount >= 50 && MaxCoin >= 4)
    {
        total += GetNumberOfWaysToMakeChangeFor(Amount - 50, 4);
    }
    if (Amount >= 25 && MaxCoin >= 3)
    {
        total += GetNumberOfWaysToMakeChangeFor(Amount - 25, 3);
    }
    if (Amount >= 10 && MaxCoin >= 2)
    {
        total += GetNumberOfWaysToMakeChangeFor(Amount - 10, 2);
    }
    if (Amount >= 5 && MaxCoin >= 1)
    {
        total += GetNumberOfWaysToMakeChangeFor(Amount - 5, 1);
    }
    if (Amount >= 1)
    {
        total += GetNumberOfWaysToMakeChangeFor(Amount - 1, 0);
    }
    return total;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int Amount;
    while (cin >> Amount)
    {
        cout << GetNumberOfWaysToMakeChangeFor(Amount, 4) << '\n';
    }
    return 0;
}