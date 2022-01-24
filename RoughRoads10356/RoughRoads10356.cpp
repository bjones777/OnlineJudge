// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=679&page=show_problem&problem=1297
//

#include <functional>
#include <queue>
#include <iostream>
#include <vector>

using namespace std;

static int INVALID_RESULT = -1;

struct Edge
{
    Edge(int s, int e, int c) : Start(s), End(e), Cost(c) {}

    int Start;
    int End;
    int Cost;
};

struct Graph10356
{
    Graph10356(istream& is, int n, int r);

    int FindMinPath(int start, int end);
private:
    vector<vector<Edge>> AdjList;
    int N;
    int R;
};

Graph10356::Graph10356(istream& is, int n, int r)
: AdjList(n) 
, N(n)
, R(r) 
{
    while (r-- > 0)
    {
        int s;
        int e;
        int c;
        is >> s >> e >> c;
        AdjList.at(s).push_back(Edge(s,e,c));
        AdjList.at(e).push_back(Edge(e, s, c));
    }
}

struct Entry
{
    Entry(bool walking, int d, int cl) 
    : IsWalking(walking)
    , Distance(d)
    , CurrentLocation(cl)
    {}

    bool IsWalking;
    int Distance;
    int CurrentLocation;

    bool operator>(const Entry& rhs) const {
        return Distance > rhs.Distance;
    }
};

int Graph10356::FindMinPath(int start, int end)
{
    const int VISITED_WALKING = 1;
    const int VISITED_RIDING = 2;
    vector<int> Visited(N, 0);
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;
    pq.push(Entry(false,0,0));
    while(!pq.empty())
    {
        Entry e = pq.top();
        pq.pop();
        if (e.CurrentLocation == end && !e.IsWalking) 
        {
            return e.Distance;
        }
        int mask = e.IsWalking ? VISITED_WALKING : VISITED_RIDING;
        if((Visited[e.CurrentLocation] & mask) != 0) continue;
        Visited[e.CurrentLocation] |= mask;
        
        for (const Edge& edge : AdjList[e.CurrentLocation])
        {
            pq.push(Entry(!e.IsWalking, e.Distance + edge.Cost, edge.End));
        }
    }
    return INVALID_RESULT;
}

static int go(int n, int r)
{
    Graph10356 graph(cin, n,r);
    return graph.FindMinPath(0, n-1);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, r;
    for (int TC = 1;cin >> n >> r;++TC)
    {
        cout << "Set #" << TC << '\n';
        int Result = go(n,r);
        if (Result != INVALID_RESULT)
        {
            cout << Result << '\n';
        }
        else
        {
            cout << "?\n";
        }
    }
    return 0;
}

