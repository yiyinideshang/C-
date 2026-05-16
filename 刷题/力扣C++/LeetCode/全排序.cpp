#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res = {{}};
        for(int num:nums){
            int size = res.size();
            for(int i = 0;i<size;i++){
                vector<int> subset = res[i];
                subset.push_back(num);
                res.push_back(subset);
            }
        }
        return res;
    }   
};

int main(){

    Solution s;
    vector<int> nums = {1,2,3,4};
    vector<vector<int>> permute = s.subsets(nums);
    // {{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}};

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