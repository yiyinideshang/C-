#include <iostream>
#include <queue>
#include <unordered_map>
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

// 层序遍历输出:
vector<string> levelOrderSerialize(TreeNode *root)
{
    vector<string> result;
    if (!root)
        return result;
    queue<TreeNode *> q;
    q.push(root);
    while (!q.empty())
    {
        TreeNode *curr = q.front();
        q.pop();
        if (curr)
        {
            // 无论子节点是否为空，都入队，以保持占位
            result.push_back(to_string(curr->val));
            q.push(curr->left);
            q.push(curr->right);
        }
        else
        {
            result.push_back("null");
        }
    }
    // 去除末尾多余的 "null"，避免序列过长
    while (!result.empty() && result.back() == "null")
    {
        result.pop_back();
    }
    return result;
}

// 后序释放
void deleteTree(TreeNode *root)
{
    if (!root)
        return; // 空节点本身就指向nullptr,不用释放
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

class Solution
{
private:
    unordered_map<int, int> index; // 存储 中序值 → 索引

public:
    // 实际递归构造
    TreeNode *myBuildTree(const vector<int> &preorder, const vector<int> &inorder,
                          int preLeft, int preRight, int inLeft, int inRight)
    {
        if (preLeft > preRight)
            return nullptr;

        // 1. 取出根节点的值
        int rootVal = preorder[preLeft];
        TreeNode *root = new TreeNode(rootVal);

        // 2. 找到根在中序中的位置
        int inRoot = index[rootVal];

        // 3. 左子树节点个数
        int leftSize = inRoot - inLeft;

        // 4. 递归构造左右子树
        root->left = myBuildTree(preorder, inorder,
                                 preLeft + 1, preLeft + leftSize,
                                 inLeft, inRoot - 1);
        root->right = myBuildTree(preorder, inorder,
                                  preLeft + leftSize + 1, preRight,
                                  inRoot + 1, inRight);
        return root;
    }

    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
    {
        int n = preorder.size();
        for (int i = 0; i < n; ++i)
            index[inorder[i]] = i; // 构建哈希表
        return myBuildTree(preorder, inorder, 0, n - 1, 0, n - 1);
    }
};

int main()
{
    Solution s;
    vector<int> preoreder = {3, 9,5, 20, 15, 7};
    vector<int> inorder = {5,9, 3, 15, 20, 7};
    TreeNode *result;
    result = s.buildTree(preoreder, inorder);

    vector<string> s1 = levelOrderSerialize(result);
    ;
    std::cout << "[";
    for (int i = 0; i < s1.size(); ++i)
    {
        std::cout << s1[i];
        if (i != s1.size() - 1)
        {
            std::cout << ",";
        }
    }
    std::cout << "]";

    return 0;
}