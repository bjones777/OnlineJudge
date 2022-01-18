// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=9&page=show_problem&problem=737
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

// needed to be moved out of class otherwise gcc 5.3 crashed
const int UNVISITED = -1;

struct Edge
{
    Edge(int s, int e) : Start(s), End(e) {}
    int Start;
    int End;

    bool operator<(const Edge& rhs) const {
        if(Start < rhs.Start) return true;
        if(Start > rhs.Start) return false;
        return End < rhs.End;
    }
};

struct Solver
{
    Solver(istream& is, int N);
    vector<Edge> FindCriticalLinks();

private:
    void Dfs(int Server);
    vector<vector<int> > AdjList;
    vector<int> DfsId;
    vector<int> DfsLow;
    int NextDfsId;
    vector<Edge> CriticalLinks;
    vector<int> DfsParent;
};

Solver::Solver(istream& is, int N)
: AdjList(N)
{
    while (N-- > 0)
    {
        int Server;
        int NumDirect;
        char ch;
        is >> Server;
        is >> ch;
        assert(ch == '(');
        is >> NumDirect;
        is >> ch;
        assert(ch == ')');
        vector<int>& List = AdjList.at(Server);
        List.reserve(NumDirect);
        while (NumDirect-- > 0)
        {
            int OtherServer;
            is >> OtherServer;
            List.push_back(OtherServer);
        }
    }
}

vector<Edge> Solver::FindCriticalLinks()
{
    CriticalLinks.clear();
    NextDfsId = 0;
    DfsId.assign(AdjList.size(), UNVISITED);
    DfsLow.assign(AdjList.size(), 0);
    DfsParent.assign(AdjList.size(), 0);

    for (int Server = 0; Server < (int)AdjList.size(); ++Server)
    {
        if (DfsId[Server] == UNVISITED)
        {
            Dfs(Server);
        }
    }
    sort(CriticalLinks.begin(), CriticalLinks.end());
    return CriticalLinks;
}

void Solver::Dfs(int Server)
{
    DfsId[Server] = NextDfsId++;
    DfsLow[Server] = DfsId[Server];

    vector<int>& List = AdjList[Server];
    for (int OtherServer : List)
    {
        if (DfsId[OtherServer] == UNVISITED)
        {
            DfsParent[OtherServer] = Server;
            Dfs(OtherServer);
            DfsLow[Server] = min(DfsLow[Server], DfsLow[OtherServer]);
            if (DfsLow[OtherServer] > DfsId[Server])
            {
                int minLink = min(Server, OtherServer);
                int maxLink = max(Server, OtherServer);
                CriticalLinks.push_back(Edge(minLink, maxLink));
            }
        }
        else if (DfsParent[Server] != OtherServer)
        {
            DfsLow[Server] = min(DfsLow[Server], DfsLow[OtherServer]);
        }  
    }
}

static vector<Edge> go(int N)
{
    Solver s(cin, N);
    return s.FindCriticalLinks();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    while (cin >> N)
    {
        vector<Edge> edges = go(N);
        cout << edges.size() << " critical links\n";
        for (Edge& edge : edges)
        {
            cout << edge.Start << " - " << edge.End << '\n';
        }
        cout << '\n';
    }
    return 0;
}
