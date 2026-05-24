#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0;
        int right = nums.size()-1;
        while(left<right){
            int mid = left+(right-left)/2;
            if(nums[mid]>nums[right]){//只要中间值大于最右侧值,则表明最小值在右侧
                //[1,2,3,4,5,6,0]
                //[2,3,4,5,6,0,1]
                //[3,4,5,6,0,1,2]
                
                //[2,3,4,5,1]
                //[2,1]
                left = mid + 1;
            }
            else{//中间值小于等于最右侧值,则最小值在左侧
                //[0,1,2,3,4,5,6]
                //[4,5,6,0,1,2,3]
                //[5,6,0,1,2,3,4]
                //[6,0,1,2,3,4,5]
                right = mid;
            }
        }
        return nums[left];//当left==right时,只剩一个元素,该元素即为最小值或写成return[right]
    }
};

int main(){
    vector<int> nums = {11,13,15,17};
    Solution s;
    std::cout<<s.findMin(nums)<<std::endl;  

    return 0;
}