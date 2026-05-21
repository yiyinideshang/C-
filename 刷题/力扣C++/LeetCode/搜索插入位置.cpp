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
            if (nums[mid] < target) {
                left = mid + 1;           // 去右半边 [mid+1, right]
            } else if (nums[mid] > target) {
                right = mid - 1;          // 去左半边 [left, mid-1]
            } else {
                return mid;               // 找到直接返回
            }
        }
        return left;                      // 没找到，返回插入位置
    }
};

int main(){
    vector<int> nums = {1,3,5,6};
    int target = 2;
    Solution s;
    std::cout<<s.searchInsert(nums,target)<<std::endl;
    return 0;
}