#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Solution {
    // 判断子串 s[l..r] 是否为回文
    bool is_palindrome(const string& s, int l, int r) {
        while (l < r) {
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }

    // 回溯：当前待分割的字符串区间为 s[start..n-1]
    void dfs(const string& s, int start,
             vector<string>& path, vector<vector<string>>& ans) {
        int n = s.size();
        if (start == n) {          // 已经分割完整个字符串
            ans.push_back(path);
            return;
        }

        // 枚举当前段的结束位置 end
        for (int end = start; end < n; ++end) {
            // 如果 s[start..end] 是回文，就在 end 处切一刀
            if (is_palindrome(s, start, end)) {
                path.push_back(s.substr(start, end - start + 1)); // 做出选择
                dfs(s, end + 1, path, ans);                       // 处理剩余部分
                path.pop_back();                                   // 撤销选择
            }
            // 如果不是回文，这个 end 就不能切，直接跳过（继续循环尝试下一个 end）
        }
    }

public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> ans;
        vector<string> path;
        dfs(s, 0, path, ans);
        return ans;
    }
};

int main(){
    Solution soult;
    string s = "aab";
    vector<vector<string>> result = soult.partition(s);
    std::cout<<"[";
    for(int i = 0;i<result.size();i++){
        std::cout<<"[";
        for(int j = 0;j<result[i].size();j++){
            std::cout<<result[i][j];
            if(j!=result[i].size()-1){
                std::cout<<",";
            }
        }
        std::cout<<"]";
        if(i!=result.size()-1){
            std::cout<<",";
        }
    }
    std::cout<<"]";
    return 0;
}