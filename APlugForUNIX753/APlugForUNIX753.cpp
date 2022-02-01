// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=690&page=show_problem&problem=694
//

#include <algorithm>
#include <cassert>
#include <limits>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

template<typename T>
class Vector2D
{
public:
    Vector2D()
    {}

    void Resize(int numRows, int numCols, const T& initial)
    {
        Data.clear();
        NumRows = numRows;
        NumColumns = numCols;
        Data.assign(numRows * numCols, initial);
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

struct Device
{
    explicit Device(istream& is)
    {
        is >> Name;
        is >> Plug;
    }

    string Name;
    string Plug;
};

struct Adapter
{
    explicit Adapter(istream& is)
    {
        is >> Receptacle;
        is >> Plug;
    }

    string Receptacle;
    string Plug;
};

static const int INF = numeric_limits<int>::max()/2-1;
static const int INVALID_VERTEX = -1;

struct Solver753
{
    Solver753(istream& is);
    int FindMaxFlow();

    vector<int> FindAugmentingPath(int start, int end);

    void AddEdge(int start, int end, int capacity)
    {
        Capacity(start, end) += capacity;        
    }

    int NumVertices;
    Vector2D<int> Capacity;
    Vector2D<int> Flow;
    int NumDevices;
};

Solver753::Solver753(istream& is)
{   
    vector<string> Receptacles;
    int NR;
    is >> NR;
    Receptacles.reserve(NR);
    while (NR-- > 0)
    {
        string s;
        is >> s;
        Receptacles.push_back(move(s));
    }
    vector<Device> Devices;
    int ND;
    is >> ND;
    Devices.reserve(ND);
    while (ND-- > 0)
    {
        Devices.emplace_back(is);
    }
    vector<Adapter> Adapters;
    int NA;
    is >> NA;
    Adapters.reserve(NA);
    while (NA-- > 0)
    {
        Adapters.emplace_back(is);
    }
    NumDevices = (int)Devices.size();
    NumVertices = 2 // source and sink
                  + (int)Receptacles.size()
                  + (int)Devices.size()
                  + (int)Adapters.size();
    Capacity.Resize(NumVertices,NumVertices,0);
    Flow.Resize(NumVertices,NumVertices,0);

    // fill in capacities
    {
        // source to Receptacles
        int Source = 0;
        int Sink = NumVertices - 1;
        const int StartReceptacle = 1;
        for (int i = 0; i < (int)Receptacles.size(); ++i)
        {
            AddEdge(Source,StartReceptacle + i, 1);
        }
        // Receptacles to Adapters
        const int StartAdapter = 1 + (int)Receptacles.size();
        for (int i = 0; i < (int)Adapters.size(); ++i)
        {
            const Adapter& adapter = Adapters[i];
            for (int j = 0; j < (int)Receptacles.size(); ++j)
            {
                if (Receptacles[j] == adapter.Plug)
                {
                    AddEdge(StartReceptacle + j, StartAdapter + i, INF);
                }
            }
        }

        // Adapters to other adapters
        for (int i = 0; i < (int)Adapters.size(); ++i)
        {
            const Adapter& a1 = Adapters[i];
            for(int j = 0;j < (int)Adapters.size();++j) {
                const Adapter& a2 = Adapters[j];
                if(a1.Receptacle == a2.Plug)
                {
                    AddEdge(StartAdapter + i, StartAdapter + j, INF);
                }
            }
        }

        // Receptacles to devices
        int StartDevice = 1 + (int)Receptacles.size() + (int)Adapters.size();
        for (int i = 0; i < (int)Devices.size(); ++i)
        {
            const Device& device = Devices[i];
            for (int j = 0; j < (int)Receptacles.size(); ++j)
            {
                if (device.Plug == Receptacles[j])
                {
                    AddEdge(StartReceptacle + j, StartDevice + i, 1);
                }
            }
        }

        // Adapters to devices
        for (int i = 0; i < (int)Devices.size(); ++i)
        {
            const Device& device = Devices[i];
            for (int j = 0; j < (int)Adapters.size(); ++j)
            {
                if (device.Plug == Adapters[j].Receptacle)
                {
                    AddEdge(StartAdapter + j, StartDevice + i, 1);
                }
            }
        }

        // Devices to Sink
        for (int i = 0; i < (int)Devices.size(); ++i)
        {
            AddEdge(StartDevice + i, Sink, 1);
        }
    }
}

int Solver753::FindMaxFlow()
{
    int totalFlow = 0;
    for (;;)
    {
        vector<int> path = FindAugmentingPath(0, NumVertices-1);
        if (path.empty())
        {
            return totalFlow;
        }
        int flowForPath = INF;
        for (int i = (int)path.size() - 1; i > 0; --i) {
            flowForPath = min(flowForPath, Capacity(path[i], path[i - 1]) - Flow(path[i], path[i - 1]));
        }
        totalFlow += flowForPath;
        for (int i = (int)path.size() - 1; i > 0; --i) {
            Flow(path[i], path[i - 1]) += flowForPath;
            Flow(path[i - 1], path[i]) -= flowForPath;
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

std::vector<int> Solver753::FindAugmentingPath(int start, int end)
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
                        q.push(Entry(i, e.Distance + 1, e.Vertex));
                        inQ[i] = true;
                    }
                }
            }
        }
    }
    return vector<int>();
}

static int go()
{
    Solver753 solver(cin);
    return solver.NumDevices - solver.FindMaxFlow();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    bool isFirst = true;
    while(N-- > 0)
    {
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            cout << '\n';
        }
        cout << go() << '\n';
    }
    return 0;
}
