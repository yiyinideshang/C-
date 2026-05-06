#include <iostream>
#include <queue>
#include <vector>
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

void printPreorder(TreeNode* root){
    if(!root){
        cout<<"null"<<" ";//可以输出时不加null
        return ;
    }
       
    cout<<root->val<<" ";
    printPreorder(root->left);
    printPreorder(root->right);
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
    int diameterOfBinaryTree(TreeNode* root) {
        int maxDiameter = 0;//记录最大直径(边数)
        maxDepth(root,maxDiameter);//后序遍历,过程中更新最大值
        return maxDiameter;
    }
private:
    int maxDepth(TreeNode* node,int& maxDiamter){
        if(node == nullptr) return 0;//空节点深度为0
        int leftDepth = maxDepth(node->left,maxDiamter);//左子树的深度
        int rightDepth = maxDepth(node->right,maxDiamter);//右子树的深度
        //如果 经过当前节点的路径长度(边数)leftDepth+rightDepth 大于 标记值,则更新
        maxDiamter = std::max(maxDiamter,leftDepth+rightDepth); 
        return std::max(leftDepth,rightDepth) + 1;//返回当前节点的深度(左/右子树中最大的深度+1)
    }
};

int main(){
    //通过vector构建二叉树
    //预先将层序遍历序列存入 vector（"null" 表示空节点）
    vector<string> levelOrder = 
    {"1","2","3","4","5"};
    TreeNode* root = build_levelOrder(levelOrder);
    cout<<"生成的二叉树先序遍历为:"<<endl;
    printPreorder(root);
    cout<<endl;

    Solution s;
    int solut = s.diameterOfBinaryTree(root);
    std::cout<<solut<<std::endl;
    return 0;
}