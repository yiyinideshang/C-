#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ans = 0;
        for(int x:nums){
            ans ^= x;
        }
        return ans;
    }
};

int main(){

    Solution s;
    std::vector<int> nums = {2,2,1};
    std::cout<<s.singleNumber(nums)<<std::endl;
    return 0;
}