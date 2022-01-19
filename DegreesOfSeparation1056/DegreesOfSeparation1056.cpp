// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=245&page=show_problem&problem=3497
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

static const int DISCONNECTED = numeric_limits<int>::max()/2-1;

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

    int GetNumRows() const { return NumRows; }
    int GetNumColumns() const { return NumColumns; }

private:
    vector<T> Data;
    int NumRows;
    int NumColumns;
};


struct Solver
{
    Solver(istream& is, int inP, int R) 
    : AdjMatrix(inP, inP, DISCONNECTED)
    , P(inP)
    {
        for (int i = 0; i < P; ++i)
        {
            AdjMatrix(i,i) = 0;
        }
        unordered_map<string, int> nameToId;
        while (R-- > 0)
        {
            string a, b;
            is >> a >> b;
            int aId = GetId(nameToId, a);
            int bId = GetId(nameToId, b);
            AdjMatrix(aId,bId) = 1;
            AdjMatrix(bId, aId) = 1;
        }
    }
    
    int GetId(unordered_map<string, int>& idMap, const string& s)
    {
        auto iter = idMap.find(s);
        if (iter != idMap.end())
        {
            return iter->second;
        }
        int id = (int)idMap.size();
        idMap.insert(iter,make_pair(s,id));
        return id;
    }

    int GetMaxDegreeOfSeparation()
    {
        bool updated = false;
        int Max = 0;
        do 
        {
            updated = false;
            Max = 0;
            for(int k = 0;k < P;++k) 
            {
                for (int i = 0; i < P; ++i)
                {
                    for (int j = 0; j < P; ++j) 
                    {
                        int oldValue = AdjMatrix(i,j);
                        int newValue = min(oldValue, AdjMatrix(i,k) + AdjMatrix(k,j));
                        if (oldValue != newValue)
                        {
                            AdjMatrix(i, j) = newValue;
                            updated = true;
                        }
                        Max = max(newValue, Max);
                    }
                }
            }
        } while(updated);

        return Max;
    }

    Vector2D<int> AdjMatrix;
    int P;
};

static int go(int P, int R)
{
    Solver s(cin, P, R);
    return s.GetMaxDegreeOfSeparation();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int P, R;
    for(int TC = 1;cin >> P >> R;++TC)
    {
        if(P == 0 && R == 0) 
        {
            break;
        }
        cout << "Network " << TC << ": ";
        int result = go(P, R);
        if (result == DISCONNECTED)
        {
            cout << "DISCONNECTED";
        }
        else
        {
            cout << result;
        }
        cout << "\n\n";   
    }
    return 0;
}