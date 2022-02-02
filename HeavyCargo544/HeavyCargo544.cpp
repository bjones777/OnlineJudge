// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=7&page=show_problem&problem=485
//

#include <cassert>
#include <algorithm>
#include <limits>
#include <iostream>
#include <string>
#include <unordered_map>
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

static const int INF = numeric_limits<int>::max()/2-1;

static int FindOrAssignIndex(unordered_map<string, int>& nameToIndex, const string& name)
{
    auto iter = nameToIndex.find(name);
    if (iter != nameToIndex.end())
    {
        return iter->second;
    }
    int index = (int)nameToIndex.size();
    nameToIndex.insert(iter,make_pair(name,index));
    return index;
}

static int go(int n, int r)
{
    unordered_map<string,int> nameToIndex;
    Vector2D<int> MaxLoads(n,n,0);
    for (int i = 0; i < n; ++i)
    {
        MaxLoads(i,i) = INF;
    }
    while (r-- > 0)
    {
        string start, end;
        cin >> start >> end;
        int load;
        cin >> load;
        int si = FindOrAssignIndex(nameToIndex,start);
        int ei = FindOrAssignIndex(nameToIndex,end);
        MaxLoads(si,ei) = MaxLoads(ei, si) = load;
    }
    bool wasUpdated = false;
    do
    {
        wasUpdated = false;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j) 
            {
                for (int k = 0; k < n; ++k)
                {
                    int& oldValue = MaxLoads(i,j);
                    int newValue = min(MaxLoads(i,k),MaxLoads(k,j));
                    if (newValue > oldValue)
                    {
                        wasUpdated = true;
                        oldValue = newValue;
                    }
                }
            }
        }
    } while(wasUpdated);
    string start, end;
    cin >> start >> end;
    return MaxLoads(FindOrAssignIndex(nameToIndex,start), FindOrAssignIndex(nameToIndex,end));
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, r;
    for (int TC = 1; cin >> n >> r; ++TC)
    {
        if(n == 0 && r == 0) break;
        cout << "Scenario #" << TC << '\n';
        cout << go(n,r) << " tons\n\n";
    }
    return 0;
}

