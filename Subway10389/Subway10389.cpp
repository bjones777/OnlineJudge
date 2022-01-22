// 10389 - Subway
// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=678&page=show_problem&problem=1330

#include <cassert>
#include <cstdint>
#include <cmath>
#include <functional>
#include <queue>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Point2D
{
    Point2D()
    : X(0)
    , Y(0)
    {}

    Point2D(istream& is)
    {
        Read(is);
    }
    Point2D(int x, int y) : X(x), Y(y) {}

    void Read(istream& is)
    {
        is >> X >> Y;
        assert((bool)is);
    }

    int64_t GetDistanceSquaredTo(const Point2D& to) const
    {
        int dx = X - to.X;
        int dy = Y - to.Y;
        return dx * dx + dy * dy;
    }

    double GetDistanceTo(const Point2D& to) const
    {
        double dx = X - to.X;
        double dy = Y - to.Y;
        return sqrt(dx * dx + dy * dy);
    }

    bool operator==(const Point2D& rhs) const
    {
        return X == rhs.X && Y == rhs.Y;
    }

    int X;
    int Y;
};

struct SubwayLine
{
    SubwayLine(const string& line)
    {
        istringstream iss(line);
        for(;;)
        {
            Point2D last(iss);
            if (last.X == -1 && last.Y == -1)
            {
                break;
            }
            else
            {
                Stations.push_back(last);
            }
        }
        Visited.resize(Stations.size(),false);
    }

    vector<Point2D> Stations;
    vector<bool> Visited;
};

struct Map
{
    Map(istream& is)
    {
        StartLocation.Read(is);
        EndLocation.Read(is);
        string line;
        getline(is,line);
        while (getline(is, line))
        {
            if (line.empty())
            {
                break;
            }
            Lines.emplace_back(SubwayLine(line));
        }
    }

    vector<SubwayLine> Lines;
    Point2D StartLocation;
    Point2D EndLocation;
};

// 10 km/h
const double SPEED_WALKING = 10000;
// 40 km/h
const double SPEED_SUBWAY = 40000;

// 40 km/h
const double SPEED_MAX = 40000;
const int INVALID_INDEX = -1;

struct Entry
{
    Entry(const Point2D& cl, const Point2D& el, double time)
    : CurrentLocation(cl)
    , EndLocation(el)
    , Time(time)
    , Line(nullptr)
    , StationIndex(INVALID_INDEX)
    {}

    Entry(const Point2D& cl, const Point2D& el, double time, SubwayLine* line, int stationIndex)
        : CurrentLocation(cl)
        , EndLocation(el)
        , Time(time)
        , Line(line)
        , StationIndex(stationIndex)
    {}

    Point2D CurrentLocation;
    Point2D EndLocation;
    double Time;
    SubwayLine* Line;
    int StationIndex;
    
    double GetBestTime() const
    {
        return Time + EndLocation.GetDistanceTo(CurrentLocation)/SPEED_MAX;
    }

    bool operator>(const Entry& e) const {
        return GetBestTime() > e.GetBestTime();
    }
};

static void go()
{
    Map subwayMap(cin);
    priority_queue<Entry,vector<Entry>,greater<Entry>> q;
    q.push(Entry(subwayMap.StartLocation, subwayMap.EndLocation, 0));
    while (!q.empty())
    {
        Entry e = q.top();
        q.pop();
        if (e.CurrentLocation == e.EndLocation)
        {
            // convert from hours to minutes
            cout << round(e.Time*60) << '\n';
            return;
        }
        if (e.Line != nullptr)
        {
            if (e.Line->Visited[e.StationIndex])
            {
                continue;
            }
            e.Line->Visited[e.StationIndex] = true;
            // try riding the subway, up or down the line
            if (e.StationIndex != 0 && !e.Line->Visited[e.StationIndex-1])
            {
                Point2D stationLocation = e.Line->Stations[e.StationIndex - 1];
                q.push(Entry(stationLocation
                             , e.EndLocation
                             , e.Time + stationLocation.GetDistanceTo(e.CurrentLocation)/SPEED_SUBWAY
                             , e.Line
                             , e.StationIndex-1));
            }
            if (e.StationIndex+1 != e.Line->Stations.size() && !e.Line->Visited[e.StationIndex + 1])
            {
                Point2D stationLocation = e.Line->Stations[e.StationIndex + 1];
                q.push(Entry(stationLocation
                    , e.EndLocation
                    , e.Time + stationLocation.GetDistanceTo(e.CurrentLocation) / SPEED_SUBWAY
                    , e.Line
                    , e.StationIndex + 1));
            }
        }

        // try just walking to the end-destination
        q.push(Entry(e.EndLocation
            , e.EndLocation
            , e.Time + e.EndLocation.GetDistanceTo(e.CurrentLocation) / SPEED_WALKING
            , e.Line
            , e.StationIndex + 1));

        // try walking to all stations we haven't been to yet
        for (SubwayLine& line : subwayMap.Lines)
        {
            for (int i = 0; i < (int)line.Stations.size(); ++i) 
            {
                if(line.Visited[i]) continue;
                Point2D stationLocation = line.Stations[i];
                q.push(Entry(stationLocation
                            , e.EndLocation
                            , e.Time + stationLocation.GetDistanceTo(e.CurrentLocation) / SPEED_WALKING
                            , &line
                            , i));
            }
        }
    }
    assert(false);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    bool isFirst = true;
    while (N-- > 0)
    {
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            cout << '\n';
        }
        go();
    }
    return 0;
}

