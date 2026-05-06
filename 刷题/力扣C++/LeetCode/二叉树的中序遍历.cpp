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

class Solution
{
public:
    vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> vec;
        stack<TreeNode *> stk;
        while(root != nullptr || !stk.empty()){
            while(root != nullptr){// 一路向左走到底
                stk.push(root);
                root = root->left;
            }
            root = stk.top();// 弹出栈顶节点并访问
            stk.pop();
            vec.push_back(root->val);
            root = root->right;// 转向右子树
        }
        return vec;
    }
};

int main()
{
    //通过vector构建二叉树
    //预先将层序遍历序列存入 vector（"null" 表示空节点）
    vector<string> levelOrder = 
    // {"4","2","6","1","3","5","7"}; //->`[1, 2, 3, 4, 5, 6, 7]`
    // {"1","2","3","4","5","null","8","null","null","6","7","9"};
    {"1","2","3","null","6","4","5"};
    TreeNode* root = build_levelOrder(levelOrder);
    cout<<"生成的二叉树先序遍历为:"<<endl;
    printPreorder(root);
    cout<<endl;

    Solution s;
    vector<int> vec = s.inorderTraversal(root);

    std::cout<<"生成的二叉树中序遍历为:"<<std::endl;
    //将树序列化回 vector<int>
    std::cout<<"[";
    for(int i = 0;i<vec.size();++i){
        std::cout<<vec[i];
        if(i != vec.size()-1){
            std::cout<<",";
        }
    }
    std::cout<<"]";
    cout<<endl;

    cout<<"生成的二叉树的层序遍历为:"<<endl;
    vector<string> result = levelOrderSerialize(root);
    for(const string& s: result){
        std::cout<<s<<" ";
    }

    deleteTree(root);
    return 0;   
}