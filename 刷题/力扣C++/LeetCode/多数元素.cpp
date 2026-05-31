#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0;
        int count = 0;
        for(int num:nums){
            if(count==0){//如果个数为0,则更新候选人为num;
                candidate = num;
            }
            count +=(num==candidate)?1:-1;
        }
        return candidate;
    }
};

//2222111
int main(){
    std::vector<int> nums = {2,2,1,1,1,2,2};
    Solution s;
    std::cout<<s.majorityElement(nums)<<std::endl;
    return 0;
}