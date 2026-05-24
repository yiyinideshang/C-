#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

class Solution {
public:
    int search(vector<int>& nums, int tar{        int left = 0;
        int right = nums.size()-1;
        while(left<=right){
            int mid = left + (right-left)/2;
            if(nums[mid]==target)   return mid;
            if(nums[left]<=nums[mid]){
                //nums[left]=nums[mid];
                if(nums[mid]>target&&nums[left]<=target){
                    //nums[left<=target]
                    right = mid-1;//4 5 6 7 0 1 2 mid=7,target=5;
                }
                else if(nums[mid]>target&&nums[left]>target){
                    left = mid + 1;//4 5 6 7 0 1 2 mid=7,target=0;
                }
                else if(nums[mid]<target){
                    left = mid + 1;//4 5 6 7 8 1 2 mid=7,target=8;
                }
            }
            else if(nums[mid]<nums[right]){
                if(nums[mid]<target&&nums[right]>=target){
                    //nums[right]=target;
                    left = mid + 1;//4 5 6 0 1 2 3 mid=0,target=2
                }
                else if(nums[mid]<target&&nums[right]<target){
                    right = mid -1;//4 5 6 0 1 2 3 mid=0,target=4
                }
                else if(nums[mid]>target){
                    right = mid -1;//9 0 1 2 3 4 7 mid=2,target=0
                }
            }
        }
        return -1;
    }
};

int main(){
    vector<int> nums = {4,5,6,7,0,1,2};
    int target = 0;
    Solution s;
    std::cout<<s.search(nums,target)<<std::endl;
    return 0;
}