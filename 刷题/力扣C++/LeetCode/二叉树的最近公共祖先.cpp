#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
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
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // 递归终止条件：空节点、或找到了 p/q 之一
        if (root == nullptr || root == p || root == q) {
            return root;
        }

        // 后序遍历：先左、再右
        TreeNode* left  = lowestCommonAncestor(root->left,  p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        // 处理“根”：左右都找到了 → 当前 root 就是 LCA
        if (left != nullptr && right != nullptr) {
            return root;
        }

        // 否则，哪个非空就向上传递哪个（可能是已找到的 LCA，或 p/q，或 nullptr）
        return left != nullptr ? left : right;
    }

    //获取p,q节点
    TreeNode* dfsnode(TreeNode* node,int target){
        if(!node)   return nullptr;
        //如果自己节点值是target则直接返回自己节点
        if(node->val == target)  return node;//返回自己节点

        //递归调用左子树,并用leftresult节点接收结果,
        //如果直接用node接收结果,node将发生改变1.目标节点2.nullptr,会导致无法进行右子树的递归
        TreeNode* leftresult =  dfsnode(node->left,target);
        //如果左子树递归结果leftresult的节点值是target则直接返回左子树递归结果
        if(leftresult) return leftresult;//返回左子树节点
        //(只需检查 leftresult 是否非空即可，因为递归返回的节点值一定等于 target（如果非空）。)

        //递归调用右子树
        return dfsnode(node->right,target);//返回右子树节点
    }
};

int main()
{
    //通过vector构建二叉树
    //预先将层序遍历序列存入 vector（"null" 表示空节点）
    vector<string> levelOrder = 
    {"3","5","1","6","2","0","8","null","null","7","4"};
    
    TreeNode* root = build_levelOrder(levelOrder);
    cout<<"生成的二叉树的先序遍历为:"<<endl;
    printPreorder(root);
    cout<<endl;

    Solution s;
    TreeNode *p = s.dfsnode(root,5);
    TreeNode* q = s.dfsnode(root,4);
    TreeNode* result = s.lowestCommonAncestor(root,p,q);

    std::cout<<p->val<<"节点和"<<q->val<<"节点的最近公共祖先为:"<<result->val<<std::endl;

    deleteTree(root);
    return 0;   
}