#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

// 二叉树节点的定义:
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

//中序遍历输出:
void printInorder(TreeNode* root){
    if(!root){
        cout<<"null"<<" ";//可以输出时不加null
        return ;
    }
    printInorder(root->left);
    cout<<root->val<<" ";
    printInorder(root->right);
}

//层序遍历输出:
vector<string> levelOrderSerialize(TreeNode* root){
    vector<string> result;
    if(!root)   
        return result;
    queue<TreeNode*> q;
    q.push(root);
    while(!q.empty()){
        TreeNode* curr = q.front();
        q.pop();
        if(curr){
            // 无论子节点是否为空，都入队，以保持占位
            result.push_back(to_string(curr->val));
            q.push(curr->left);
            q.push(curr->right);
        }
        else{
            result.push_back("null");
        }
    }
    // 去除末尾多余的 "null"，避免序列过长
    while(!result.empty() && result.back() == "null"){
        result.pop_back();
    }
    return result;
}

//后序释放
void deleteTree(TreeNode* root){
    if(!root)
        return;//空节点本身就指向nullptr,不用释放
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return build(nums,0,nums.size()-1);
    }
    TreeNode* build(const vector<int>& nums,int left,int right){
        if(left > right) return nullptr;

        int mid = (left + right + rand() % 2)/2;
        TreeNode* root = new TreeNode(nums[mid]);

        root->left = build(nums,left,mid-1);
        root->right = build(nums,mid+1,right);

        return root;
    }
};

int main(){
    Solution s;
    vector<int> nums = {1,2,3,4,5};
    TreeNode* root = s.sortedArrayToBST(nums);

    cout<<"生成的二叉树中序遍历为:"<<endl;
    printInorder(root);
    cout<<endl;


    cout<<"生成的二叉树层序遍历为:"<<endl;
    vector<string> result = levelOrderSerialize(root);
    for(const string& s: result){
        std::cout<<s<<" ";
    }
    cout<<endl;

    return 0;
}