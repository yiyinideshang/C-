#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> current = {};
        sort(candidates.begin(),candidates.end());
        backtrack(candidates,target,0,result,current);
        return result;
    }
    void backtrack(vector<int>& candidates,int target,int index,vector<vector<int>>& result,vector<int>& current){
        if(target == 0){
            result.push_back(current);
            return;
        }
        for(int i = index;i<candidates.size();++i){
            if(target<candidates[i]){
                break;
            }
            int tar = target - candidates[i];
            current.push_back(candidates[i]);
            backtrack(candidates,tar,i,result,current);
            current.pop_back();
        }
    }
};

int main(){

    vector<int> candidates = {2,3,6,7};
    // vector<int> candidates = {2,3,5};

    int target = 7;
    Solution s;
    vector<vector<int>> result =  s.combinationSum(candidates,target);

    std::cout<<"[";
    for(int i = 0;i<result.size();++i){
        std::cout<<"[";
        for(int j = 0;j<result[i].size();++j){
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