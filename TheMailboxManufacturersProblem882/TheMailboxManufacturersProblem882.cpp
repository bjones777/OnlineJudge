// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=10&page=show_problem&problem=823
//

#include <algorithm>
#include <cassert>
#include <limits>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Vector3D
{
public:
    using reference = typename vector<T>::reference;
    using const_reference = typename vector<T>::const_reference;

    Vector3D(int numRows, int numCols, int numStacks, const T& initial)
        : Data(numRows * numCols * numStacks, initial)
        , NumRows(numRows)
        , NumColumns(numCols)
        , NumStacks(numStacks)
    {
        assert(numRows > 0);
        assert(numCols > 0);
        assert(numStacks > 0);
    }

    const_reference operator()(int row, int col, int stack) const {
        assert(row >= 0 && row < NumRows);
        assert(col >= 0 && col < NumColumns);
        assert(stack >= 0 && stack < NumStacks);
        return Data[row * NumColumns*NumStacks + col * NumStacks + stack];
    }

    reference operator()(int row, int col, int stack) {
        assert(row >= 0 && row < NumRows);
        assert(col >= 0 && col < NumColumns);
        assert(stack >= 0 && stack < NumStacks);
        return Data[row * NumColumns*NumStacks + col * NumStacks + stack];
    }

private:
    vector<T> Data;
    int NumRows;
    int NumColumns;
    int NumStacks;
};

const int UNINITIALIZED = -1;

struct Solver882
{
    Solver882(int k, int n)
    : Memo(k+1,n+2,n+2, UNINITIALIZED)
    {
    }

    int FindMinFireCrackers(int k, int lb, int ub);
    
    Vector3D<int> Memo;
};

int Solver882::FindMinFireCrackers(int k, int lb, int ub)
{
    if (lb >= ub)
    {
        return 0;
    }
    if (k == 1)
    {
        int diff = ub - lb - 1;
        return (diff * (diff+1))/2 + lb * (diff+1);
    }
    if (Memo(k, lb, ub) != UNINITIALIZED)
    {
        return Memo(k, lb, ub);
    }
    // seems like there should be single entry to pick, 
    // but it isn't the middle, and dynamic programming is fast
    // enough, so... just try them all
    int retVal = numeric_limits<int>::max();
    for (int toTry = lb; toTry < ub; ++toTry)
    {
        retVal = min(retVal,toTry + max(FindMinFireCrackers(k-1,lb,toTry),
                                        FindMinFireCrackers(k, toTry+1, ub)));
    }
    Memo(k, lb, ub) = retVal;
    return retVal;
}


static int go(int k, int n)
{
    Solver882 solver(k,n);
    return solver.FindMinFireCrackers(k,1,n+1);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int TC;
    cin >> TC;
    while (TC-- > 0)
    {
        int k, n;
        cin >> k >> n;
        cout << go(k,n) << '\n';
    }
    return 0;
}