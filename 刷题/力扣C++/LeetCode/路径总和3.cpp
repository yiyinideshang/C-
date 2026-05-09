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
private:
    unordered_map<long long, int> prefix;  // 前缀和 -> 出现次数
    int ans = 0;
    int target = 0;

public:
    void dfs(TreeNode* node, long long curSum) {
        if (!node) return;

        // 1. 更新当前前缀和
        curSum += node->val;

        // 2. 查询：有多少个祖先节点的前缀和等于 curSum - target
        //    这些祖先节点到当前节点的路径和就是 target
        ans += prefix[curSum - target];

        // 3. 记录当前前缀和，供子孙节点使用
        prefix[curSum]++;

        // 4. 递归处理左右子树
        dfs(node->left, curSum);
        dfs(node->right, curSum);

        // 5. 回溯：当前节点处理完毕，从哈希表中移除当前前缀和
        //    避免影响其他分支（因为路径已经结束）
        prefix[curSum]--;
    }

    int pathSum(TreeNode* root, int targetSum) {
        // 初始化：空前缀和为 0，出现 1 次（表示从根节点直接开始的路径）
        prefix[0] = 1;
        target = targetSum;
        dfs(root, 0);
        return ans;
    }
};

int main()
{
    //通过vector构建二叉树
    //预先将层序遍历序列存入 vector（"null" 表示空节点）
    vector<string> levelOrder = 
    // {"4","2","6","1","3","5","7"}; //->`[1, 2, 3, 4, 5, 6, 7]`
    // {"1","2","3","4","5","null","8","null","null","6","7","9"};
    {"10","5","-3","3","2","null","11","3","-2","null","1"};
    
    TreeNode* root = build_levelOrder(levelOrder);
    cout<<"生成的二叉树的先序遍历为:"<<endl;
    printPreorder(root);
    cout<<endl;

    Solution s;
    int targetSum = 8;
    // int targetSum = 6;
    int result = 0;
    result =  s.pathSum(root,targetSum);
    std::cout<<result<<std::endl;

    deleteTree(root);
    return 0;   
}