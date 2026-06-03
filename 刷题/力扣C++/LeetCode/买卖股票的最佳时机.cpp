#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        //初始化:
        int minPrice = prices[0];//记录股票最小价格,初始为第0天的价格
        int maxProfit = 0;//记录股票最大利润,初始为0利润

        // 从第i=1天开始每天记录,
        // 当天抛出后的利润(当天的价格-之前的股票最小价格)与之前最大利润比较 以及
        // 当天股票价格和之前股票最小价格
        for(int i = 1;i<prices.size();i++){
            //严格遵循“先买后卖”的约束，使用先更新利润、后更新最低价的写法
            maxProfit = std::max(maxProfit,prices[i]-minPrice);//更新当前最大利润
            minPrice = std::min(minPrice,prices[i]);//更新当前股票最小值
        }
        return maxProfit;
    }
};

int main(){
    std::vector<int> prices = {7,2,1,3,10,4,1,8};
    Solution s;
    std::cout<<s.maxProfit(prices)<<std::endl;

    return 0;
}