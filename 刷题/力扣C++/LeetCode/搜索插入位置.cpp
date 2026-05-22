#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;      // 闭区间
        while (left <= right) {           // 区间不为空，等号有意义
            int mid = left + (right - left) / 2;
            if (nums[mid] >= target) {
                right = mid - 1;          // 去左半边 [left, mid-1]
            } else /*if (nums[mid] < target)*/{
                left = mid + 1;           // 去右半边 [mid+1, right]
            } 
        }
        // 循环结束时 left 指向第一个 >= target 的元素
        return left;                      // 没找到，返回插入位置
        
        // 本题只是找 >= target 的位置，不要求等于 target，直接返回 left;否则:
        //if (left < nums.size() && nums[left] == target) return left;
        //return -1;
    }
};

int main(){
    vector<int> nums = {1,3,5,6};
    int target = 2;
    Solution s;
    std::cout<<s.searchInsert(nums,target)<<std::endl;
    return 0;
}