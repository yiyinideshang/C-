#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    //查找第一个大于等于target的位置
    //如果都大于等于target,返回0,如果都小于target,返回left=size();
    int find_equal_First(vector<int>& nums, int target) {
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

    //查找最后一个小于等于target的位置
    //如果都小于target,返回right=size()-1,如果都大于target,返回-1;
    int find_equal_Last(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;      // 闭区间
        while (left <= right) {           // 区间不为空，等号有意义
            int mid = left + (right - left) / 2;
            if (nums[mid] <= target) {
                left = mid + 1;           // 去右半边 [mid+1, right]
            } else /*if (nums[mid] > target)*/ {
                right = mid - 1;          // 去左半边 [left, mid-1]
            }
        }
        // 循环结束时 right 指向最后一个 <= target 的元素
        return right;                      // 没找到，返回插入位置
        
        // 本题只是找 ≤ target 的位置，不要求等于 target，直接返回 right;否则:
        //if (right >= 0 && nums[right] == target) return right;
		//return -1;
    }

    //查找第一个大于且等于target的位置,如果没有返回-1:
    //如果都小于target,left=size(),返回-1;
    //如果都大于等于target,判断nums[0] == target,如果成立返回left=0,否则返回-1;
    //如果找到第一个大于等于target的位置,判断nums[left] == target,如果成立返回left,否则返回-1;
    int find_noequal_First(vector<int>& nums, int target) {
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
        
        if (left < nums.size() && nums[left] == target) return left;
        return -1;
    }

    //查找最后一个小于且等于target的位置,如果没有返回-1:
    //如果都大于target,right = -1,返回-1;
    //如果都小于等于target,返回right=size()-1,判断nums[size()-1] == target,如果成立返回right=size()-1,否则返回-1;
    //如果找到最后一个小于等于target的位置,判断nums[left] == target,如果成立返回left,否则返回-1;
    int find_noequal_Last(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;      // 闭区间
        while (left <= right) {           // 区间不为空，等号有意义
            int mid = left + (right - left) / 2;
            if (nums[mid] <= target) {
                left = mid + 1;           // 去右半边 [mid+1, right]
            } else /*if (nums[mid] > target)*/ {
                right = mid - 1;          // 去左半边 [left, mid-1]
            }
        }
        
        if (right >= 0 && nums[right] == target) return right;
		return -1;
    }
};

int main(){
    vector<int> nums = {1,1,3,3,5,6};
    // int target = 0;   //0 -1 -1 -1
    // int target = 7;   //6 4 -1 -1
    int target = 2;   //2 1 -1 -1
    // int target = 3;      //2 3 2 3
    Solution s;

    //查找第一个大于等于target的位置
    std::cout<<s.find_equal_First(nums,target)<<std::endl;//>= target 的位置
    //查找最后一个小于等于target的位置
    std::cout<<s.find_equal_Last(nums,target)<<std::endl;//≤ target 的位置

    //查找第一个大于且等于target的位置
    std::cout<<s.find_noequal_First(nums,target)<<std::endl;//>= target 的位置
    //查找最后一个小于且等于target的位置
    std::cout<<s.find_noequal_Last(nums,target)<<std::endl;//≤ target 的位置
    return 0;
}