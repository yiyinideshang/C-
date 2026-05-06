#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
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
TreeNode* build_levelOrder(const vector<string>& nodes){
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

class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> stk;
        while(root != nullptr || !stk.empty()){
            // 一路向左，将所有左节点入栈
            while(root!=nullptr){
                stk.push(root);
                root = root->left;
            }
            // 弹出栈顶节点，此时即为中序访问的节点
            root = stk.top();
            stk.pop();
            //取出栈顶节点后,k的值-1
            --k;
            if(k == 0){//如果k的值为0,则表示当前节点的元素值就是原第K小的元素
                break;
            }
            //转向右子树
            root = root->right;
        }
        return root->val;//函数返回为该节点的元素值
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
    vector<string> levelOrder = {"3","1","4","null","2"};
    TreeNode* root = build_levelOrder(levelOrder);

    int k = 2;
    int result = 0;
    Solution s;
    result = s.kthSmallest(root,k);
    std::cout<<result<<std::endl;

    deleteTree(root);
    return 0;   
}