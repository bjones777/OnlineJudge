// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=245&page=show_problem&problem=3488
//

#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct SharedArea
{
   SharedArea(istream& is)
   : Areas(0)
   {
        int N;
        is >> N;
        while (N-- > 0)
        {
            int Tower;
            is >> Tower;
            Areas |= 1 << (Tower-1);
        }
        is >> NumCustomers;
   }

   int GetOvercount(uint32_t used)
   {
        int numShared = 0;
        int overCount = 0;
        used = used & Areas;
        for (int index = 0; used != 0; ++index)
        {
            if ((used & 1) == 1)
            {
                ++numShared;
                if (numShared > 1)
                {
                    overCount += NumCustomers;
                }
            }
            used >>= 1;
        }
        return overCount;
   }

   uint32_t Areas;
   int NumCustomers;
};

struct Solver
{
    Solver(istream& is, int inN, int inC);

    void RecommendLocations();
private:
    void Search(int N, int index, uint32_t used);
    void ReplaceIfBest(uint32_t used);
    int GetTotalCustomers(uint32_t used);
    uint32_t ToRank(uint32_t used);
    int N;
    int C;
    vector<int> TotalService;
    vector<SharedArea> SharedAreas;

    int MaxCustomers;
    uint32_t BestTowers;
    uint32_t BestRank;
};

Solver::Solver(istream& is, int inN, int inC) 
: N(inN)
, C(inC) 
{
    TotalService.reserve(inN);
    while (inN-- > 0)
    {
        int Customers;
        is >> Customers;
        TotalService.push_back(Customers);
    }
    int NumShared;
    is >> NumShared;
    while (NumShared-- > 0)
    {
        SharedAreas.push_back(SharedArea(is));
    }
}

void Solver::Search(int NumLeft, int index, uint32_t used)
{
    if (NumLeft == 0)
    {
        ReplaceIfBest(used);
        return;
    }
    if (index + (NumLeft-1) >= N)
    {
        return;
    }
    Search(NumLeft-1, index+1, used | (1 << index));
    Search(NumLeft, index+1, used);
}

void Solver::ReplaceIfBest(uint32_t used)
{
    int NumCustomers = GetTotalCustomers(used);
    if (NumCustomers > MaxCustomers)
    {
        MaxCustomers = NumCustomers;
        BestTowers = used;
        BestRank = ToRank(used);
        return;
    }
    if (NumCustomers == MaxCustomers)
    {
        uint32_t rank = ToRank(used);
        if(rank > BestRank)
        {
            MaxCustomers = NumCustomers;
            BestTowers = used;
            BestRank = rank;
        }
        return;
    }
}

int Solver::GetTotalCustomers(uint32_t used)
{
    uint32_t origUsed = used;
    int Customers = 0;
    for (int index = 0; used != 0; ++index)
    {
        if ((used & 1) == 1)
        {
            Customers += TotalService[index];
        }
        used >>= 1;
    }
    for (SharedArea& sharedArea : SharedAreas)
    {
        Customers -= sharedArea.GetOvercount(origUsed);
    }
    return Customers;
}

uint32_t Solver::ToRank(uint32_t used)
{
    uint32_t rank = 0;
    for (int index = 0; used != 0; ++index)
    {
        if ((used & 1) == 1)
        {
            rank |= 1 << (N - index -1);
        }
        used >>= 1;
    }
    return rank;
}

void Solver::RecommendLocations()
{
    MaxCustomers = -1;
    BestTowers = 0;
    BestRank = 0;

    Search(C,0,0);
    cout << "Number of Customers: " << MaxCustomers << '\n';
    cout << "Locations recommended:";
    for (int index = 0; index < N; ++index)
    {
        if ((BestTowers & (1 << index)) != 0) {
            cout << ' ' << index+1;
        }
    }
    cout << "\n\n";
}

static void go(int N, int C)
{
    Solver solver(cin, N, C);
    solver.RecommendLocations();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, C;
    for (int TC = 1;cin >> N >> C;++TC)
    {
        if(N == 0 && C == 0) 
        {
            break;
        }
        cout << "Case Number  " << TC << '\n';        go(N,C);    }

    return 0;
}