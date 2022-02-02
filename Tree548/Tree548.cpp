// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=7&page=show_problem&problem=489
//

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Tree
{
    Tree(int v) : Value(v) {}

    int Value;

    pair<int,int> FindMin()
    {        
        if (Left == nullptr && Right == nullptr)
        {
            return make_pair(Value,Value);
        }

        if (Right != nullptr && Left == nullptr)
        {
            auto p = Right->FindMin();
            return make_pair(p.first+Value,p.second);
        }

        if (Right == nullptr && Left != nullptr)
        {
            auto p = Left->FindMin();
            return make_pair(p.first + Value, p.second);
        }
        auto pl = Left->FindMin();
        auto pr = Right->FindMin();
        if (pl.first < pr.first)
        {
            return make_pair(pl.first + Value, pl.second);
        }
        if (pr.first < pl.first)
        {
            return make_pair(pr.first + Value, pr.second);
        }
        return make_pair(pr.first, min(pl.second, pr.second));
    }

    unique_ptr<Tree> Left;
    unique_ptr<Tree> Right;
};

static unique_ptr<Tree> BuildTree(const vector<int>& inorderV, vector<int>& postorderV,  int lb, int ub)
{
    if (lb >= ub)
    {
        return nullptr;
    }
    unique_ptr<Tree> tree(new Tree(postorderV.back()));
    postorderV.pop_back();
    auto iter = find(inorderV.begin()+lb, inorderV.begin() + ub, tree->Value);
    tree->Right = BuildTree(inorderV, postorderV, (int)distance(inorderV.begin(), iter)+1, ub);
    tree->Left = BuildTree(inorderV, postorderV, lb, (int)distance(inorderV.begin(), iter));
    return tree;
}

static vector<int> ToVector(const string& s)
{
    vector<int> retVal;
    istringstream iss(s);
    int v;
    while (iss >> v)
    {
        retVal.push_back(v);
    }
    return retVal;
}

static int go(const string& inorder, const string& postorder)
{
    vector<int> inorderV = ToVector(inorder),
                postorderV = ToVector(postorder);
    unique_ptr<Tree> tree = BuildTree(inorderV, postorderV, 0,(int)postorderV.size());
    return tree->FindMin().second;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string inorder, postorder;
    while (getline(cin, inorder) && getline(cin, postorder))
    {
        cout << go(inorder, postorder) << '\n';
    }
    return 0;
}

