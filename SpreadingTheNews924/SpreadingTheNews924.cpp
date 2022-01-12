// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=11&page=show_problem&problem=865

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <string>

using namespace std;

static const int INVALID_BOOM_DAY = -1;
static const int INF = numeric_limits<int>::max()/2 - 1;

struct Result
{
    Result(int M, int D) : MaximumBoomSize(M), FirstBoomDay(D) {}
    int MaximumBoomSize;
    int FirstBoomDay;
};

class FriendsList
{
public:
    FriendsList(istream& is);

    Result FindBoomDayAndSize(int source) const;
private:
    vector<vector<int>> FriendsForEmployees;
    int NumEmployees;
};

FriendsList::FriendsList(istream& is)
{
    is >> NumEmployees;
    assert((bool)is);
    FriendsForEmployees.resize(NumEmployees);
    for (vector<int>& FriendForEmployee : FriendsForEmployees)
    {
        int N;
        is >> N;
        assert((bool)is);
        FriendForEmployee.reserve(N);
        while (N-- > 0)
        {
            int Friend;
            is >> Friend;
            FriendForEmployee.push_back(Friend);
        }
    }
}

struct Entry
{
    Entry(int E, int D) : Employee(E), Day(D) {}
    int Employee;
    int Day;
};

Result FriendsList::FindBoomDayAndSize(int source) const
{
    vector<int> Counts;
    vector<int> DayHeard(FriendsForEmployees.size(), INF);

    queue<Entry> q;
    q.push(Entry(source, 0));
    Counts.push_back(0);
    DayHeard[source] = 0;
    while(!q.empty())
    {
        Entry e = q.front();
        q.pop();
        for (int employee : FriendsForEmployees[e.Employee])
        {
            if(DayHeard[employee] != INF) continue;
            DayHeard[employee] = e.Day + 1;
            while(e.Day + 1 + 1 > Counts.size()) {
                Counts.push_back(0);
            }
            Counts[(e.Day + 1)] += 1;
            q.push(Entry(employee, e.Day + 1));
        }
    }
    auto iter = max_element(Counts.begin(), Counts.end());
    if (*iter == 0)
    {
        return Result(0,INVALID_BOOM_DAY);
    }
    return Result(*iter, (int)(iter - Counts.begin()));
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    FriendsList Friends(cin);
    int T;
    cin >> T;
    while (T-- > 0)
    {
        int Source;
        cin >> Source;
        Result result = Friends.FindBoomDayAndSize(Source);
        if (result.FirstBoomDay == INVALID_BOOM_DAY)
        {
            cout << "0\n";
        }
        else
        {
            cout << result.MaximumBoomSize << ' ' << result.FirstBoomDay << '\n';
        }
    }
    return 0;
}

