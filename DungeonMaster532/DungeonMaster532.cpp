// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=7&page=show_problem&problem=473
//

#include <cassert>
#include <queue>
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

struct Point3D
{
    int X, Y, Z;
    Point3D(int inX,int inY, int inZ)
    : X(inX), Y(inY), Z(inZ)
    {}

    Point3D() : X(-1), Y(-1), Z(-1) {}

    bool operator==(const Point3D& rhs) const
    {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
    }
};

const int TRAPPED = -1;

static int GetNumDirections()
{
    return 6;
}

static Point3D AdjustForDirction(const Point3D& p, int dir)
{
    if (dir == 0)
    {
        return Point3D(p.X-1,p.Y,p.Z);
    }
    else if (dir == 1)
    {
        return Point3D(p.X + 1, p.Y, p.Z);
    }
    else if (dir == 2)
    {
        return Point3D(p.X, p.Y - 1, p.Z);
    }
    else if (dir == 3)
    {
        return Point3D(p.X, p.Y + 1, p.Z);
    }
    if (dir == 4)
    {
        return Point3D(p.X, p.Y, p.Z - 1);
    }
    assert(dir == 5);
    return Point3D(p.X, p.Y, p.Z + 1);
}

struct Entry
{
    Entry(int inNumMoves, const Point3D& inLocation)
    : NumMoves(inNumMoves), Location(inLocation)
    {}

    int NumMoves;
    Point3D Location;
};

static bool IsInBox(const Point3D& p, const Point3D& minP, const Point3D& maxP)
{
    if (p.X < minP.X || p.X >= maxP.X)
    {
        return false;
    }
    if (p.Y < minP.Y || p.Y >= maxP.Y)
    {
        return false;
    }
    if (p.Z < minP.Z || p.Z >= maxP.Z)
    {
        return false;
    }
    return true;
}

static int go(int L, int R, int C)
{
    Vector3D<bool> Maze(L,R,C,false);
    Point3D MinP(0,0,0);
    Point3D MaxP(L,R,C);
    Point3D Start, End;
    for (int l = 0; l < L; ++l)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                char ch;
                cin >> ch;
                if (ch == 'S')
                {
                    Start = Point3D(l,r,c);
                }
                else if (ch == 'E')
                {
                    End = Point3D(l,r,c);
                }
                else if (ch == '#')
                {
                    Maze(l,r,c) = true;
                }
            }
        }
    }

    Vector3D<bool> WillVisit(L, R, C, false);
    queue<Entry> q;
    WillVisit(Start.X, Start.Y, Start.Z) = true;
    q.push(Entry(0,Start));
    while (!q.empty())
    {
        Entry e = q.front();
        q.pop();
        if (e.Location == End)
        {
            return e.NumMoves;
        }
        for (int d = 0, ed = GetNumDirections(); d < ed; ++d)
        {
            Point3D np = AdjustForDirction(e.Location,d);
            if(!IsInBox(np, MinP, MaxP)) continue;
            if(Maze(np.X,np.Y,np.Z)) continue;
            auto v = WillVisit(np.X, np.Y, np.Z);
            if (v) continue;
            v = true;
            q.push(Entry(e.NumMoves+1,np));
        }
    }
    return TRAPPED;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int L, R, C;
    while (cin >> L >> R >> C)
    {
        if(L == 0 && R == 0 && C == 0) break;
        int val = go(L, R, C);
        if (val == TRAPPED)
        {
            cout << "Trapped!\n";
        }
        else
        {
            cout << "Escaped in " << val << " minute(s).\n";
        }
    }
    return 0;
}