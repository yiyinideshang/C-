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
    bool isSymmetric(TreeNode* root) {
       stack<TreeNode*> s;//显式维护一个栈
       s.push(root->left);//左子树的根节点入栈
       s.push(root->right);//右子树的根节点入栈
       TreeNode* u;
       TreeNode* v;
       while(!s.empty()){
        v = s.top();s.pop();//右子树的根节点此时在栈顶,记录并移除
        u = s.top();s.pop();//左子树的根节点此时在栈顶,记录并移除
        // 两者都为空，结构对称且无需再比较子节点
        if(!u && !v) continue;
        // 一个空一个非空，或值不相等，都不对称
        if((!u || !v) || (u->val != v->val)) return false;

        // 按镜像对应关系入栈
        s.push(u->left);//左子树的左孩子
        s.push(v->right);//右子树的右孩子

        s.push(u->right);//左子树的右孩子
        s.push(v->left);//右子树的左孩子
       }
       return true;//所有对应关系都成立,返回真
    }
};

int main(){
    vector<string> preorder = 
    {"1","2","2","3","4","4","3"};
    TreeNode* root = buildTree(preorder);
    cout<<"生成的二叉树先序遍历为:"<<endl;
    printPreorder(root);
    cout<<endl;

    Solution s;
    int solut = s.isSymmetric(root);
    std::cout<<solut<<std::endl;
    return 0;
}