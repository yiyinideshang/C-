#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty()) return {-1, -1};   //为空,直接返回
        int first = findFirst(nums, target);
        //如果都小于target,first=size,如果都大于等于target,则进行nums[left]==target判断
        if(first == nums.size()||nums[first]!=target){
            return {-1, -1};// first不存在,直接返回
        }    
        // 如果 start 存在，那么 end 必定存在,直接找大于等于target+1的位置,-1即可
        // findFirst(nums, target+1) 返回第一个 >= target+1 的索引（即 upper_bound）
        // 减 1 得到最后一个 <= target 的索引，因为 target 已存在，该位置的值就是 target

        int last = findFirst(nums,target+1)-1;
        return {first, last};
    }
private:
    int findFirst(vector<int>& nums,int target){
        int left = 0,right = nums.size()-1;
        while(left<=right){
            int mid = left + (right-left)/2;
            if(nums[mid]>=target){
                right = mid -1;
            }
            else/*if(nums[mid]<target)*/{
                left = mid + 1;
            }
        }

        return left;
    }
};

int main(){
    vector<int> nums = {1,1,3,3,5,6};
    // int target = 0;   // -1 -1
    // int target = 7;   // -1 -1
    // int target = 2;   // -1 -1
    int target = 3;      // 2 3

    Solution s;
    std::vector<int> result =  s.searchRange(nums,target);
    for(int i :result){
        std::cout<<i<<" ";
    }

    return 0;
}