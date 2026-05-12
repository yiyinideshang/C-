#include <iostream>
#include <vector>

class Solution {
public:
    int climbStairs(int n) {
        int dp0 = 1;//不对dp[0]进行操作
        int dp1 = 1;//表示爬到第1级台阶,有dp[1] = 1中方法
        int dp2 = 2;//表示爬到第2级台阶,有dp[2] = 2中方法,即 "爬1个台阶+爬一个台阶" 和 "直接爬2个台阶"
        for(int i = 3;i<=n;i++){
            int sum = dp1 + dp2;
            dp1 = dp2;
            dp2 = sum;
        }

        //将判断n是否小于1写在最后也行,当n<=1时返回n,否则返回dp[2];
        //也确保了返回值是正确的
        return n<=1?n:dp2;//返回更新后的dp[2];
    }
};

int main(){

    Solution s;
    int n = 5;
    int result = s.climbStairs(n);
    std::cout<<result<<std::endl;
    return 0;
}