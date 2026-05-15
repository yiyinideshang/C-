#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        vector<bool> used(nums.size(),false);
        backtrack(nums,path,used,res);
        return res;
    }
    void backtrack(vector<int>& nums,vector<int>& path,vector<bool>& used,vector<vector<int>>& res){
        if(path.size() == nums.size()){
            res.push_back(path);
            return;
        }
        for(int i = 0;i<nums.size();++i){
            if(used[i]){
                continue;
            }
            path.push_back(nums[i]);
            used[i] = true;
            backtrack(nums,path,used,res);

            path.pop_back();
            used[i] = false;
        }       
    }
};

int main(){

    Solution s;
    vector<int> nums = {1,2,3};
    vector<vector<int>> permute = s.permute(nums);
    // {{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}};
    //
    std::cout<<"[";
    for(int i = 0;i<permute.size();++i){
        std::cout<<"[";
        for(int j = 0;j<permute[0].size();++j){
            std::cout<<permute[i][j];
            if(j!=permute[0].size()-1){
                std::cout<<",";
            }
        }
        std::cout<<"]";
        if(i!=permute.size()-1){
            std::cout<<",";
        }
    }
    std::cout<<"]";

    return 0;
}