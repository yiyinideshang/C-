#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>   // std::ranges::nth_element
//#include <functional>  // std::ranges::greater
using namespace std;

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        int maxFreq = 0;
        for (int num : nums) {
            maxFreq = max(maxFreq, ++freq[num]);
        }

        // bucket[i] 存放所有出现次数为 i 的数字
        vector<vector<int>> buckets(maxFreq + 1);
        for (auto& [num, cnt] : freq) {
            buckets[cnt].push_back(num);
        }

        vector<int> result;
        for (int i = maxFreq; i >= 0 && result.size() < k; --i) {
            for (int num : buckets[i]) {
                result.push_back(num);
                if (result.size() == k) break;
            }
        }
        return result;
    }
};

int main(){
    Solution s;
    std::vector<int> nums = {1,1,1,2,2,2,3,3,3,3,4};
    int k = 2;
    std::vector<int> result = s.topKFrequent(nums,k);
    for(int i:result){
        std::cout<<i<<" ";
    }

    return 0;
}