// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=666&page=show_problem&problem=1057
//

#include <cassert>
#include <iostream>
#include <limits>
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

    int GetNumRows() const { return NumRows; }
    int GetNumColumns() const { return NumColumns; }

private:
    vector<T> Data;
    int NumRows;
    int NumColumns;
};


static void traverse(const Vector2D<char>& Map, int startColumn)
{
    const int INF = numeric_limits<int>::max() / 2 - 1;
    Vector2D<int> PathLength(Map.GetNumRows(), Map.GetNumColumns(), INF);
    int currentLength = 0;
    int currentRow = 0;
    int currentCol = startColumn;
    for(;;)
    {
        if(currentRow < 0 
           || currentRow >= Map.GetNumRows()
           || currentCol < 0
           || currentCol >= Map.GetNumColumns())
           { 
            cout << currentLength << " step(s) to exit\n";
            return;
           }
        int pathLength = PathLength(currentRow, currentCol);
        if(pathLength < currentLength)
        {
            cout << pathLength << " step(s) before a loop of " << currentLength - pathLength << " step(s)\n";
            return;
        }
        PathLength(currentRow, currentCol) = currentLength++;
        char dir = Map(currentRow, currentCol);
        switch(dir)
        {
            case 'N':
                currentRow -= 1;
                break;
            case 'S':
                currentRow += 1;
                break;
            case 'E':
                currentCol += 1;
                break;
            case 'W':
                currentCol -= 1;
                break;
            default:
                assert(false);
                break;
        }
    }
}

static void go(int R, int C, int S)
{
    // read in the map
    Vector2D<char> Map(R,C,' ');
    for (int r = 0; r < R; ++r)
    {
        for (int c = 0; c < C; ++c) {
            cin >> Map(r,c);
        }
    }

    traverse(Map, S-1);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int R, C, S;
    while (cin >> R >> C >> S)
    {
        if (R == 0 && C == 0 && S == 0)
        {
            break;
        }
        go(R,C,S);
    }
    return 0;
}

