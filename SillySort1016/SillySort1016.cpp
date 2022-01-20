// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=245&page=show_problem&problem=3457
//

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Solver
{
    Solver(istream& is, int N)        
    {
        Numbers.reserve(N);
        while (N-- > 0)
        {
            int number;
            cin >> number;
            Numbers.push_back(number);
        }
    }

    int GetMinCostSort();   
private:
    vector<int> Numbers;   
};


int Solver::GetMinCostSort()
{
    const int INVALID_CYCLE = -1;

    // find the correct order
    vector<int> CorrectOrder = Numbers;
    sort(CorrectOrder.begin(), CorrectOrder.end());

    int MinElement = CorrectOrder.front();

    // map the indices
    unordered_map<int,int> FinalPlace;
    FinalPlace.reserve(CorrectOrder.size());
    for (int i = 0; i < (int)CorrectOrder.size(); ++i)
    {
        FinalPlace[CorrectOrder[i]] = i;
    }

    // identify cycles
    int CycleNum = 0;
    vector<int> Cycle(CorrectOrder.size(), INVALID_CYCLE);

    int NumVisited = 0;    
    int Total = 0;
    for (int i = 0; NumVisited < (int)Cycle.size(); ++i)
    {
        if(Cycle[i] != INVALID_CYCLE) continue;
        int CycleLen = 1;
        Cycle[i] = CycleNum++;
        int MinInCycle = Numbers[i];
        int CycleSum = Numbers[i];
        for (int NextIndex = FinalPlace[Numbers[i]]; Cycle[NextIndex] != Cycle[i]; NextIndex = FinalPlace[Numbers[NextIndex]])
        {
            Cycle[NextIndex] = Cycle[i];
            MinInCycle = min(Numbers[NextIndex], MinInCycle);
            CycleSum += Numbers[NextIndex];
            ++CycleLen;
        }

        // Add to the total for the two strategies for solving the cycle
        // Strategy 1: swap with the minimum element in the cycle for N-1 swaps
        // (amin + a1) + (amin + a2)... (amin + an-1) = (amin * (n-1)) + (a1 + ... an-1)
        // since the cycle sum includes amin we can obtain a new sum (amin * (n-2)) + (amin + a1 + ... an-1) 

        // Strategy 2: swap the minimum element in the cycle for the minimum element in
        // the entire array (assuming it isn't the minimum for the entire array)
        // cost = 2 swaps between the min element in array with the min element in cycle
        // an then n-1 swaps between the array min and each element in the cycle = 
        // (min_e + a1) + (min_e + a2) + ... + (min_e + an-1) = 2*(amin + min_e) + (CycleSum - amin) + (CycleLen - 1) * min_e
        // if min_e == amin then the equation becomes
        // 4 * amin + CycleSum - amin + (amin * CycleLen - 1)
        // or CycleSum - (MinInCycle * (CycleLen + 2)), which will always be 
        // greater then strategy 1
        if(CycleLen > 1)
        {
            Total += min(CycleSum + (MinInCycle * (CycleLen - 2)),
                         2 * (MinElement + MinInCycle) + (CycleSum - MinInCycle) + (MinElement * (CycleLen-1)));
        }               
        NumVisited += CycleLen;
    }
    return Total;
}
static int go(int N)
{
    Solver solver(cin, N);
    return solver.GetMinCostSort();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    for (int TC = 1; cin >> N; ++TC)
    {
        if (N == 0)
        {
            break;
        }
        cout << "Case " << TC << ": " << go(N) << "\n\n";
    }
    return 0;
}

