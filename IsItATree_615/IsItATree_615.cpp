// IsItATree_615.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

static int GetIndex(unordered_map<int, int>& IdToIndexMap, int vertex)
{
    auto iter = IdToIndexMap.find(vertex);
    if (iter != IdToIndexMap.end())
    {
        return iter->second;
    }
    int index = (int)IdToIndexMap.size();
    IdToIndexMap[vertex] = index;
    return index;
}

static bool CheckPathToRoot(vector<int>& ParentIndices, int toCheck, int rootIndex, vector<bool>& Visited)
{
    if (toCheck == rootIndex)
    {
        return true;
    }
    if (Visited[toCheck] && ParentIndices[toCheck] != rootIndex)
    {
        return false;
    }
    Visited[toCheck] = true;
    if (!CheckPathToRoot(ParentIndices, ParentIndices[toCheck], rootIndex, Visited)) {
        return false;
    }
    ParentIndices[toCheck] = rootIndex;
    return true;
}

static bool IsTree(istream& is, int s, int e)
{
    const int INVALID_INDEX = -1;
    unordered_map<int,int> IdToIndexMap;
    vector<int> ParentIndices;
    int ParentCount = 0;
    bool isTree = true;
    for (; s != 0 && e != 0; is >> s >> e)
    {
        if(!isTree) continue;
        int si = GetIndex(IdToIndexMap, s);
        int ei = GetIndex(IdToIndexMap, e);
        ParentIndices.resize(IdToIndexMap.size(), INVALID_INDEX);
        int parentIndex = ParentIndices[ei];
        if (parentIndex == INVALID_INDEX && ei != si)
        {
            ++ParentCount;
            ParentIndices[ei] = si;
        }
        else 
        {
            isTree = false;            
        }
    }
    if (!isTree)
    {
        return false;
    }
    // is it an empty tree?
    if (ParentIndices.size() == 0)
    {
        return true;
    }
    // do we have one element without a parent?
    if ((ParentCount + 1) != ParentIndices.size())
    {
        return false;
    }
    auto iter = find(ParentIndices.begin(), ParentIndices.end(), INVALID_INDEX);
    assert(iter != ParentIndices.end());
    int rootIndex = (int)(iter - ParentIndices.begin());
    vector<bool> Visited(ParentIndices.size(), false);
    for (int i = 0; i < (int)ParentIndices.size(); ++i)
    {
        if (!CheckPathToRoot(ParentIndices, i, rootIndex, Visited))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int s, e;
    for (int TC = 1;cin >> s >> e;++TC)
    {
        if (s < 0 && e < 0) 
        {
            break;
        }
        bool isTree = IsTree(cin,s,e);
        cout << "Case " << TC << " is ";        if(!isTree)        {            cout << "not ";        }
        cout << "a tree.\n";
    }
    return 0;
}