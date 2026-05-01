#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int>ans;
//初始化前缀和为1
//nums:  4    5         1           8           2 
//ans:   1    1*4       1*4*5       1*4*5*1     1*4*5*1*8
//result 1*4  1*4*5     1*4*5*1     1*4*5*1*8   1*4*5*1*8*2(×不需要了)
        int result = 1;
        for(int i = 0; i < nums.size(); i++){//计算前缀积
            ans.push_back(result);//初始化前缀积
            result = result * nums[i];//每一次循环,result记录为下一次的前缀积
        }

//恢复result=1,即后缀和初始化为1;在这里ans[i]表示上一次该i位置的前缀积
//nums:   4                        5                 1                8               2 
//ans:    ans[i]*5*1*8*2*1         ans[i]*1*8*2*1    ans[i]*8*2*1     ans[i]*2*1      ans[i]*1
//result: 4*5*1*8*2*1(x不需要了)    5*1*8*2*1         1*8*2*1          8*2*1           2*1
        result = 1;
        for(int i = nums.size() - 1; i >= 0; i--){
            ans[i] *= result;//当前i的前缀积 * 当前i的后缀积
            result *= nums[i];//计算后缀积
        }
        return ans;
    }
};

int main(){

    vector<int> nums = {4,5,1,8,2};
    Solution s;
    vector<int> answer = s.productExceptSelf(nums);
    for(int i : answer){
        std::cout<<i<<" ";
    }
    
    return 0;
}