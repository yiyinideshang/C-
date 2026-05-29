#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        //temperatures = {73,74,75,71,69,72,76,73};
        int n = temperatures.size();
        std::vector<int> result(n,0);   // 默认 0，表示没有更高的温度
        //单调栈,从栈底到栈顶是单调递增,从栈顶到栈底是单调递减
        std::stack<int> stk;            //存放teme下标位置(天数位置,从0开始)

        for(int i = 0;i<n;i++){
            //当栈不为空并且当前温度大于栈顶的温度时,
            while(!stk.empty()&&temperatures[i]>temperatures[stk.top()]){
                result[stk.top()]=i-stk.top();//将栈顶(teme下标位置)位置需要等待的天数存放到result中
                //i-stk.top()即为天数差值
                stk.pop();//删除栈顶元素,更新了栈顶元素
            }
            stk.push(i);//将当前天入栈,等待遍历未来天数,查找未来更高的温度
        }
        return result;//如果某天的温度在未来没有出现高于该温度,则该天的结果默认是0
    }
};

int main(){
    std::vector<int> temperatures = {73,74,75,71,69,72,76,73};
    //30,40,50,60
    Solution s;
    std::vector<int> result = s.dailyTemperatures(temperatures);
    for(int i:result){
        std::cout<<i<<" ";
    }

    return 0;
}