//https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=245&page=show_problem&problem=3466
//

#include <algorithm>
#include <cassert>
#include <limits>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Vector2D
{
public:
    Vector2D()
    {
    }

    void Initialize(int numRows, int numCols, const T& initial)
    {
        assert(numRows > 0);
        assert(numCols > 0);
        NumRows = numRows;
        NumColumns = numCols;
        Data.assign(numRows * numCols, initial);
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
    int NumRows = -1;
    int NumColumns = -1;
};

const int INVALID_TIME = numeric_limits<int>::max()/2-1;
const int UNINITIALIZED_TIME = numeric_limits<int>::min();

enum class Direction
{
    INCREASING,
    DECREASING,
};

static int GetNumDirections()
{
    return static_cast<int>(Direction::DECREASING)+1;
}

static Direction GetDirectionAtIndex(int i)
{
    return static_cast<Direction>(i);
}

int GetStationInDirection(int Station, Direction dir)
{
    switch (dir)
    {
        case Direction::INCREASING:
            return Station+1;
    }
    assert(dir == Direction::DECREASING);
    return Station - 1;
}

struct Solver
{
    Solver(istream& is, int N)
    : NumStations(N)
    {
        is >> RequiredArrivalTime;
        assert((bool)is);

        Memo.Initialize(N, RequiredArrivalTime+1, UNINITIALIZED_TIME);
        TravelTimes.reserve(NumStations-1);
        for (int i = 0; i < NumStations-1; ++i)
        {
            int tt;
            is >> tt;
            assert((bool)is);
            TravelTimes.push_back(tt);
        }

        {
            int ND;
            is >> ND;
            DepartureTimes1.reserve(ND);
            while (ND-- > 0)
            {
                int dt;
                is >> dt;
                assert((bool)is);
                DepartureTimes1.push_back(dt);
            }
        }

        {
            int ND;
            is >> ND;
            DepartureTimes2.reserve(ND);
            while (ND-- > 0)
            {
                int dt;
                is >> dt;
                assert((bool)is);
                DepartureTimes2.push_back(dt);
            }
        }
        

        {
            CumulativeTravelTimesIncreasing.reserve(NumStations);
            int time = 0;
            for(int station = 0;station < NumStations;++station)
            {
                CumulativeTravelTimesIncreasing.push_back(time);
                if(station != NumStations-1)
                {
                    time += GetTravelTime(station,station+1);
                }
            }
        }
        
        {
            CumulativeTravelTimesDecreasing.assign(NumStations, 0);
            int time = 0;
            for(int station = NumStations-1;station >= 0;--station)
            {
                CumulativeTravelTimesDecreasing[station] = time;
                if(station != 0)
                {
                    time += GetTravelTime(station,station-1);
                }
            }
         }
    }

    int FindMinWaitTime()
    {
        return FindMinWaitTime(0, 0);
    }

    int FindNextDeparture(int Station, int Time, Direction dir);

private:
    int GetTravelTime(int StartStation, int DestStation)
    {
        assert(abs(StartStation - DestStation) == 1);
        if (StartStation < DestStation)
        {
            return TravelTimes.at(StartStation);
        }
        assert(StartStation > DestStation);
        return TravelTimes.at(DestStation);
    }

    int FindNextDeparture(int Station, int Time, const vector<int>& DepartureTimes, int TravelTimeToStation);

    int FindMinWaitTime(int Station, int Time);
    int NumStations;
    int RequiredArrivalTime;
    vector<int> TravelTimes;
    vector<int> DepartureTimes1;
    vector<int> DepartureTimes2;
    vector<int> CumulativeTravelTimesIncreasing;
    vector<int> CumulativeTravelTimesDecreasing;
    Vector2D<int> Memo;
};

int Solver::FindMinWaitTime(int Station, int Time)
{
    if (Time > RequiredArrivalTime)
    {
        return INVALID_TIME;
    }
    int Result = Memo(Station, Time);
    if(Result != UNINITIALIZED_TIME)
    {
        return Result;
    }
    Result = (Station == NumStations - 1) ? RequiredArrivalTime - Time : INVALID_TIME;
    for (int i = 0, e = GetNumDirections(); i < e; ++i)
    {
        Direction dir = GetDirectionAtIndex(i);
        int DestStation = GetStationInDirection(Station, dir);
        if(DestStation < 0 || DestStation >= NumStations) continue;

        for (int NextDepartureTime = FindNextDeparture(Station, Time, dir); NextDepartureTime < RequiredArrivalTime; NextDepartureTime = FindNextDeparture(Station, NextDepartureTime + 1, dir))
        {
            int DepartureWaitTime = NextDepartureTime - Time;
            Result = min(Result, DepartureWaitTime + FindMinWaitTime(DestStation, NextDepartureTime + GetTravelTime(Station, DestStation)));
        }
    }
    Memo(Station, Time) = Result;
    return Result;    
}

int Solver::FindNextDeparture(int Station, int Time, Direction dir)
{
    if (dir == Direction::INCREASING)
    {
        return FindNextDeparture(Station, Time, DepartureTimes1, CumulativeTravelTimesIncreasing.at(Station));
    }
    assert(dir == Direction::DECREASING);
    return FindNextDeparture(Station, Time, DepartureTimes2, CumulativeTravelTimesDecreasing.at(Station));
}

int Solver::FindNextDeparture(int Station, int Time, const vector<int>& DepartureTimes, int TravelTimeToStation)
{
    int lb = 0;
    int ub = (int)DepartureTimes.size();
    while(lb < ub)
    {
        int mid = (ub + lb)/2;
        int Depart = TravelTimeToStation + DepartureTimes[mid];
        if (Depart < Time)
        {
            lb = mid + 1;
        }
        else
        {
            ub = mid;
        }
    }
    if (lb == DepartureTimes.size())
    {
        return INVALID_TIME;
    }
    return TravelTimeToStation + DepartureTimes[lb];
}

static int DoTestCase(int N)
{
    Solver solver(cin, N);
    return solver.FindMinWaitTime();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    for (int TC = 1; cin >> N; ++TC)
    {
        if(N == 0) break;
        cout << "Case Number "<< TC << ": ";
        int Result = DoTestCase(N);
        if (Result != INVALID_TIME)
        {
            cout << Result << '\n';
        }
        else
        {
            cout << "impossible\n";
        }
    }
    return 0;
}