#include <vector>
#include <iostream>
#include <queue>
#include <cassert>
using namespace std;

/**
 * Definition for a binary tree node.
**/
struct TreeNode {
 int val;
 TreeNode *left;
 TreeNode *right;
 TreeNode(int x) : val(x), left(nullptr), right(nullptr) {};
 };

class Solution {
public:
    void helper(TreeNode* root,int& sum, int depth, TreeNode* gpar, TreeNode* par)
    {
        if(!root) return;
        if(gpar && gpar->val%2==0) sum+=root->val;
        if(root->left) helper(root->left,sum,depth+1,par,root);
        if(root->right) helper(root->right,sum,depth+1,par,root);
    }
    int sumEvenGrandparent(TreeNode* root) {
        int sum=0;
        helper(root,sum,0,nullptr,nullptr);
        return sum;
    }
};

TreeNode* insertValue(struct TreeNode* root, int value, int which, queue<TreeNode *>& q)
{

    TreeNode* node = nullptr;
    if(value!=0) node=new TreeNode(value);

    if(root==nullptr)
    {
        root=node;
        q.push(node);
        return root;
    } 


    if(which ==-1 && q.front()->left==nullptr)
    {
        q.front()->left=node;
        if(node) q.push(node);
        return root;
    }

    if(which==1 && q.front()->right==nullptr)
    {
        q.front()->right=node;
        if(node)q.push(node);
        q.pop();
        return root;
    }

    return root;
}
 

void printLevelOrder(TreeNode* root)
{
    if (root == nullptr)
        return;
    queue<TreeNode*> n;
    n.push(root);
    while (!n.empty()) {
        cout << n.front()->val << " ";
        if (n.front()->left != nullptr)
            n.push(n.front()->left);
        else cout<<"[null ";
        if (n.front()->right != nullptr)
            n.push(n.front()->right);
        else cout<<"null] ";
        n.pop();
    }
}

TreeNode* buildTree(const vector<int>& nodes)
{
    TreeNode* root=nullptr;
    queue<TreeNode*> q; 
    int which = -1;
    for(auto i=0;i<nodes.size();++i){        
        root = insertValue(root,nodes[i],which,q);
        which=(i==0?which:-which);
    }
    return root;
}

int main()
{
    vector<int> treelayers={6,7,8,2,7,1,3,9,0,1,4,0,0,0,5};
    TreeNode* tree=buildTree(treelayers);
    printLevelOrder(tree);
    Solution solver;
    assert(solver.sumEvenGrandparent(tree)==18);

    return 0;
}