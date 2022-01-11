#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <string>

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

class Graph
{
public:
    Graph(int numVertices)
    : Flow(numVertices, numVertices, 0)
    , Capacity(numVertices, numVertices, 0)
    , NumVertices(numVertices)
    {}

    void AddEdge(int start,int end, int capacity);

    int FindMaxFlow(int start, int end);

    // returns path from end to start
    vector<int> FindAugmentingPath(int start,int end);    
private:
    Vector2D<int> Flow;
    Vector2D<int> Capacity;
    int NumVertices;

    static const int INF = numeric_limits<int>::max();
    static const int INVALID_VERTEX = -1;
};

void Graph::AddEdge(int start, int end, int capacity)
{
    Capacity(start, end) += capacity;
    Capacity(end, start) += capacity;
}

// Ford-Fulkerson
int Graph::FindMaxFlow(int start, int end)
{
    int totalFlow = 0;
    for (;;)
    {
        vector<int> path = FindAugmentingPath(start, end);
        if (path.empty())
        {
            return totalFlow;
        }
        int flowForPath = INF;
        for (int i = (int)path.size()-1; i > 0; --i) {
            flowForPath = min(flowForPath, Capacity(path[i], path[i-1]) - Flow(path[i], path[i-1]));
        }
        totalFlow += flowForPath;
        for (int i = (int)path.size() - 1; i > 0; --i) {
            Flow(path[i], path[i-1]) += flowForPath;
            Flow(path[i-1], path[i]) -= flowForPath;
        }
    }
}

struct Entry
{
    Entry(int v, int d, int f) : Vertex(v), Distance(d), From(f) {}
    int Vertex;
    int Distance;
    int From;
};

vector<int> Graph::FindAugmentingPath(int start, int end)
{
    // BFS - Edmonds-Karp
    queue<Entry> q;
    vector<bool> inQ(NumVertices, false);
    vector<int> dist(NumVertices, INF);
    vector<int> prev(NumVertices, INVALID_VERTEX);
    inQ[start] = true;
    q.push(Entry(start, 0, INVALID_VERTEX));
    while (!q.empty())
    {
        Entry e = q.front();
        q.pop();
        if (e.Distance < dist[e.Vertex])
        {
            dist[e.Vertex] = e.Distance;
            prev[e.Vertex] = e.From;
            if (e.Vertex == end)
            {
                vector<int> path;
                for (int v = end; v != INVALID_VERTEX; v = prev[v]) {
                    path.push_back(v);
                }
                return path;
            }
            else
            {
                for (int i = 0; i < NumVertices; ++i)
                {
                    if (Capacity(e.Vertex, i) - Flow(e.Vertex, i) > 0
                        && !inQ[i]) 
                    {
                        q.push(Entry(i, e.Distance+1, e.Vertex));
                        inQ[i] = true;
                    }
                }
            }
        }
    }
    return vector<int>();
}

void AddEdge(Graph& g)
{
    int s, e, cap;
    cin >> s >> e >> cap;
    assert((bool)cin);
    g.AddEdge(s-1,e-1, cap);
}

void DoNetwork(int numVerts, int networkNumber)
{
    Graph g(numVerts);
    int s, t, c;
    cin >> s >> t >> c;
    while (c-- > 0)
    {
        AddEdge(g);
    }
    cout << "Network " << networkNumber << '\n';
    cout << "The bandwidth is " << g.FindMaxFlow(s-1,t-1) << ".\n\n";
}

void DoNetworks()
{
    for (int networkNumber = 1;; ++networkNumber)
    {
        int numVerts;
        cin >> numVerts;
        if (numVerts == 0)
        {
            return;
        }
        DoNetwork(numVerts,networkNumber);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    DoNetworks();
    return 0;
}