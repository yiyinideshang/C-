#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> current={};
        backtrack(nums,0,current,res);
        return res;
    }
    void backtrack(vector<int>& nums,int start,vector<int>& current,vector<vector<int>>& res){
        res.push_back(current);
        for(int i = start;i<nums.size();++i){
            current.push_back(nums[i]);
            backtrack(nums,i+1,current,res);
            current.pop_back();
        }
    }
};

int main(){

    Solution s;
    //vector<int> nums = {1,2,3};
    vector<int> nums = {1,2,3,4};
    vector<vector<int>> permute = s.subsets(nums);
    //[[],[1],[1,2],[1,2,3],[1,3],[2],[2,3],[3]]
    //[[],[1],[1,2],[1,2,3],[1,2,3,4],[1,2,4],[1,3],[1,3,4],[1,4],[2],[2,3],[2,3,4],[2,4],[3],[3,4],[4]]
    
    std::cout<<"[";
    for(int i = 0;i<permute.size();++i){
        std::cout<<"[";
        for(int j = 0;j<permute[i].size();++j){
            std::cout<<permute[i][j];
            if(j!=permute[i].size()-1){
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