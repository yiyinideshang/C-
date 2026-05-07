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
    TreeNode* prev = nullptr;   // 全局指针：记录已展开链表的最后一个节点
    void flatten(TreeNode* root) {
        if (root == nullptr) return;

        // 1. 提前保存当前节点原来的左右孩子
        TreeNode* leftChild  = root->left;
        TreeNode* rightChild = root->right;

        // 2. 如果前面已经有节点，就把当前节点接到链尾
        if (prev != nullptr) {
            prev->right = root;
        }

        // 3. 当前节点成为新的链尾，并且清理它的指针
        prev = root;
        prev->left  = nullptr;
        prev->right = nullptr;

        // 4. 递归展开左子树（它会接到当前节点后面）
        flatten(leftChild);

        // 5. 递归展开右子树（它会接到左子树整条链的后面）
        flatten(rightChild);
    }
};

int main(){
    //通过vector构建二叉树
    //预先将层序遍历序列存入 vector（"null" 表示空节点）
    vector<string> levelOrder = 
    {"1","2","5","3","4","null","6"};
    TreeNode* root = build_levelOrder(levelOrder);

    Solution s;
    s.flatten(root);

    vector<string> result = levelOrderSerialize(root);
    for(auto s : result){
        std::cout<<s<<" ";
    }
    return 0;
}