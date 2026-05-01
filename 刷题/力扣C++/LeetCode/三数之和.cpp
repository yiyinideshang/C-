#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution{
public:
    vector<vector<int>> threeSum(vector<int>& nums){
        std::sort(nums.begin(),nums.end());
        std::vector<vector<int>> my_vector={};
        std::vector<int> vet={};
        int n = nums.size();
       for(int i=0;i<n-2;++i)
       {
            if(i>0 && nums[i]==nums[i-1])
                continue;
            int left = i+1;
            int right = n-1;
            int target = -nums[i];
            while(left < right)
            {
                int sums = nums[left]+nums[right];
                if(sums == target)
                {
                    my_vector.push_back({nums[i],nums[left],nums[right]});
                    while(left<right && nums[left] == nums[left+1])
                        left++;
                    while(left<right && nums[right] == nums[right-1])
                        right--;
                    left++;
                    right--;
                }
                else if(sums<target)
                {
                    left++;
                }
                else
                {
                    right--;
                }
            }
       }
       return my_vector;
    }
};

int main()
{
    vector<int> nums={0,0,1};
    Solution s;
    std::vector<vector<int>> my_vector = s.threeSum(nums);
    std::cout<<"[";
    for(size_t i = 0;i<my_vector.size();++i)
    {
        const auto& v = my_vector[i];
        std::cout<<"[";
        for(size_t j = 0;j<v.size();++j)
        {
            std::cout<<v[j];
            if(j!=v.size()-1)
                std::cout<<",";
        }
        std::cout<<"]";
        if(i!=my_vector.size()-1)
            std::cout<<",";
    }
    std::cout<<"]";
    return 0;
}