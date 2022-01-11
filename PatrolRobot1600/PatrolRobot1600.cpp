#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
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


struct Entry
{
    Entry(int r, int c, int d, int k) : Row(r), Column(c), Distance(d), K(k) {}
    int Row;
    int Column;
    int Distance;
    int K;
};

static const int INVALID_DISTANCE = -1;

static int DoPatrol()
{
    int m, n;
    int k;
    cin >> m >> n;
    cin >> k;
    Vector2D<int> Area(m,n, 0);
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            cin >> Area(r,c);
        }
    }
    // BFS
    Vector3D<bool> InQ(m, n, k+1, false);
    queue<Entry> q;
    q.push(Entry(0,0,0,k));
    while (!q.empty())
    {
        Entry e = q.front();
        q.pop();
        if (e.Row == m - 1 && e.Column == n - 1)
        {
            return e.Distance;
        }
        for (int i = 0; i < 4; ++i)
        {
            int nr = e.Row;
            int nc = e.Column;
            switch (i)
            {
                case 0:
                    nr -= 1;
                    break;
                case 1:
                    nr += 1;
                    break;
                case 2:
                    nc -= 1;
                    break;
                case 3:
                    nc += 1;
                    break;
            }
            if(nr < 0 || nr >= m) continue;
            if(nc < 0 || nc >= n) continue;
            int nk = k;
            if (Area(nr, nc) == 1)
            {
                nk = e.K - Area(nr, nc);
            }
            if(nk < 0) continue;
            if(InQ(nr,nc,nk)) continue;
            InQ(nr,nc,nk) = true;
            q.push(Entry(nr,nc,e.Distance+1,nk));
        }
    }
    return INVALID_DISTANCE;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int TC;
    cin >> TC;
    while (TC-- > 0)
    {
        cout << DoPatrol() << '\n';
    }
    return 0;
}