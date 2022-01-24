// 957 - Popes
// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=661&page=show_problem&problem=898
// 

#include <iostream>
#include <vector>

using namespace std;

static void go(int Y)
{
    int P;
    cin >> P;
    vector<int> Popes;
    Popes.reserve(P);
    while (P-- > 0)
    {
        int YP;
        cin >> YP;
        Popes.push_back(YP);
    }
    int BestNumPopes = 0;
    int StartYear = -1;
    int EndYear = -1;
    for (int i = 0; i + BestNumPopes < (int)Popes.size(); ++i)
    {
        const int MaxY = Popes[i] + Y;
        int lb = i;
        int ub = (int)Popes.size();
        while (lb < ub)
        {
            int mid = (ub + lb)/2;
            if (Popes[mid] < MaxY)
            {
                lb = mid+1;
            }
            else {
                ub = mid;
            }
        }
        int count = ub - i;
        if (count > BestNumPopes)
        {
            BestNumPopes = count;
            StartYear = Popes[i];
            EndYear = Popes[ub-1];
        }
    }
    cout << BestNumPopes << ' ' << StartYear << ' ' << EndYear << '\n';
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int Y;
    while (cin >> Y)
    {
        go(Y);
    }
    return 0;
}
