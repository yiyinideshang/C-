#include <iostream>
#include <vector>
#include <string>
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
    TreeNode* invertTree(TreeNode* root) {
        if(root == nullptr)
            return nullptr;
        TreeNode* left = invertTree(root->left);
        TreeNode* right = invertTree(root->right);
        root->left = right;
        root->right = left;
        return root;
    }
};

int main()
{
    //通过vector构建二叉树
    //预先将层序遍历序列存入 vector（"null" 表示空节点）
    vector<string> preorder = 
    // {"4","2","6","1","3","5","7"}; //->`[1, 2, 3, 4, 5, 6, 7]`
    // {"1","2","3","4","5","null","8","null","null","6","7","9"};
    // {"1","2","3","null","null","4","5"};
    {"1","2","3","null","6","4","5"};
    
    TreeNode* root = buildTree(preorder);
    cout<<"生成的二叉树先序遍历为:"<<endl;
    printPreorder(root);
    cout<<endl;

    Solution s;
    root = s.invertTree(root);
    cout<<"翻转后的二叉树的先序遍历为:"<<endl;
    printPreorder(root);
    cout<<endl;

    cout<<"翻转后的二叉树的层序遍历为:"<<endl;
    vector<string> result = levelOrderSerialize(root);
    for(const string& s: result){
        std::cout<<s<<" ";
    }

    deleteTree(root);
    return 0;   
}