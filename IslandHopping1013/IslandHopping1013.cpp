// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3454
//

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

struct Point2D
{
    Point2D(istream& is)
    {
        is >> X >> Y;
        assert((bool)is);
    }
    Point2D(int x, int y) : X(x), Y(y) {}

    int GetDistanceSquaredTo(const Point2D& to) const
    {
        int dx = X - to.X;
        int dy = Y - to.Y;
        return dx * dx + dy * dy;
    }
    int X;
    int Y;
};

struct Island
{
    Island(istream& is)
        : Location(is)
    {
        is >> Inhabitants;
    }
    Point2D Location;
    int Inhabitants;
};

struct Solver
{
    Solver(istream& is, int N)
    {
        Islands.reserve(N);
        for (int i = 0; i < N; ++i)
        {
            Islands.emplace_back(Island(is));
        }
    }

    double FindAverage();

    vector<Island> Islands;
};

struct Entry
{
    Entry(int i, int distSq, int maxDistanceSq) 
    : CurrentIslandIndex(i)
    , DistanceSquared(distSq) 
    , MaxDistanceSquared(maxDistanceSq)
    {}
    int CurrentIslandIndex;
    int DistanceSquared;
    int MaxDistanceSquared;

    bool operator>(const Entry& e) const {
        return DistanceSquared > e.DistanceSquared;
    }
};

double Solver::FindAverage()
{
    priority_queue<Entry,vector<Entry>,greater<Entry>> pq;
    vector<bool> Visited(Islands.size(), false);
    
    double inhabitantsSum = 0.0;
    double weightedSum = 0.0;

    int VisitedCount = 0;
    pq.push(Entry(0,0,0));
    while (!pq.empty())
    {
        Entry e = pq.top();
        pq.pop();
        if(Visited[e.CurrentIslandIndex]) continue;
        ++VisitedCount;
        Visited[e.CurrentIslandIndex] = true;
        
        Island& island = Islands[e.CurrentIslandIndex];
        inhabitantsSum += island.Inhabitants;

        weightedSum += sqrt(e.MaxDistanceSquared) * island.Inhabitants;

        if(VisitedCount == (int)Islands.size()) break;

        for (int i = 0; i < (int)Islands.size(); ++i)
        {
            if(Visited[i]) continue;
            int distSq = island.Location.GetDistanceSquaredTo(Islands[i].Location);
            pq.push(Entry(i, distSq, max(e.MaxDistanceSquared, distSq)));
        }
    }
    //Sanity Check to make sure we visited every island
    assert(VisitedCount == (int)Islands.size());
    return weightedSum/inhabitantsSum;
}

static double go(int N)
{
    Solver solver(cin, N);
    return solver.FindAverage();
}

int main()
{
    cin.tie(nullptr);
    int N;
    for (int TC = 1;cin >> N;++TC)
    {
        if(N == 0) break;
        printf("Island Group: %d Average %0.2f\n\n",TC, go(N));
    }
    return 0;
}
