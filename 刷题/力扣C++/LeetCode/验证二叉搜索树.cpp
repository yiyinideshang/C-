#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
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

// 根据层序遍历序列构建二叉树（"null" 表示空节点）
TreeNode* buildPreorder(const vector<string>& nodes,int& index){
    if (nodes.empty() || nodes[0] == "null") return nullptr;

    // 创建根节点
    TreeNode* root = new TreeNode(stoi(nodes[0]));
    queue<TreeNode*> q;
    q.push(root);

    int i = 1;  // 从第二个元素开始处理
    while (!q.empty() && i < nodes.size()) {
        TreeNode* current = q.front();
        q.pop();

        // 处理左子节点
        if (i < nodes.size() && nodes[i] != "null") {
            current->left = new TreeNode(stoi(nodes[i]));
            q.push(current->left);
        }
        i++;

        // 处理右子节点
        if (i < nodes.size() && nodes[i] != "null") {
            current->right = new TreeNode(stoi(nodes[i]));
            q.push(current->right);
        }
        i++;
    }
    return root;
}

// 便利包装：隐藏 index 引用
TreeNode* buildTree(const vector<string>& preorder){
    int index = 0;
    return buildPreorder(preorder,index);
}

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        stack<TreeNode*> stk;
        long long prev = LONG_MIN;

        TreeNode* curr = root;
        while (curr || !stk.empty()) {
            // 一路向左，将所有左节点入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            // 弹出栈顶节点，此时即为中序访问的节点
            curr = stk.top(); stk.pop();

            // 检查是否严格大于前驱
            if (curr->val <= prev) 
                return false;
            prev = curr->val;

            // 转向右子树
            curr = curr->right;
        }
        return true;
    }
};

//后序释放
void deleteTree(TreeNode* root){
    if(!root)
        return;//空节点本身就指向nullptr,不用释放
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main()
{
    //通过vector构建二叉树
    //预先将层序遍历序列存入 vector（"null" 表示空节点）
    vector<string> preorder = {"2","1","3"};
    TreeNode* root = buildTree(preorder);

    Solution s;
    bool flag = s.isValidBST(root);
    std::cout<<flag<<std::endl;

    deleteTree(root);
    return 0;   
}