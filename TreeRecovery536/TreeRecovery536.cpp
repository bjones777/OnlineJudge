// TreeRecovery536.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

using namespace std;

struct Tree
{
    Tree(char ch) : Ch(ch)
    {
    }

    ~Tree()
    {
        delete Left;
        delete Right;
    }
    
    void BuildPostOrder(string& builder)
    {
        if(Left != nullptr) Left->BuildPostOrder(builder);
        if(Right != nullptr) Right->BuildPostOrder(builder);
        builder += Ch;
    }

    char Ch;
    Tree* Left = nullptr;
    Tree* Right = nullptr;
};

struct Solver536
{
    Solver536(const string& p, const string& i) 
    : Preorder(p)
    , Inorder(i) 
    {
    }

    ~Solver536()
    {
        delete Root;
    }


    string GetPostOrder();
private:
    Tree* BuildTree(int lb, int ub);
    string Preorder;
    string Inorder;
    Tree* Root = nullptr;
    int PreorderIndex = -1;
};

string Solver536::GetPostOrder()
{
    PreorderIndex = 0;
    Root = BuildTree(0,(int)Preorder.size());
    string builder;
    builder.reserve(Preorder.size());
    if (Root != nullptr)
    {
        Root->BuildPostOrder(builder);
    }
    return builder;
}

Tree* Solver536::BuildTree(int lb, int ub)
{
    if (ub <= lb)
    {
        return nullptr;
    }
    char ch = Preorder.at(PreorderIndex);
    int index = lb;
    for (; index < ub && Inorder[index] != ch; ++index)
    {
    }
    if (index >= ub)
    {
        return nullptr;
    }
    PreorderIndex++;
    Tree* retVal = new Tree(ch);
    retVal->Left = BuildTree(lb,index);
    retVal->Right = BuildTree(index+1, ub);
    return retVal;
}

string go(const string& p, const string& i)
{
    Solver536 s(p,i);
    return s.GetPostOrder();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string preorder, inorder;
    while (cin >> preorder >> inorder)
    {
        cout << go(preorder, inorder) << '\n';
    }
    return 0;
}

