#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // 声明一个最小堆：priority_queue<int, 底层容器, 比较器>
        // greater<int> 意味着小的元素优先级高 → 堆顶最小
        std::priority_queue<int,std::vector<int>,std::greater<int>> minHeap;
        for(int num:nums){
            if(minHeap.size()<k){
                minHeap.push(num);
            }
            else if(num>minHeap.top()){
                minHeap.pop();
                minHeap.push(num);
            }
        }

        return minHeap.top();
    }
};

int main(){
    vector<int> nums = {3,2,3,1,2,4,5,5,6};
    Solution s;
    std::cout<<s.findKthLargest(nums,4)<<std::endl;

    return 0;
}