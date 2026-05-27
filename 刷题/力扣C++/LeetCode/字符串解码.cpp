#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Solution {
public:
    std::string decodeString(const std::string& s) {
        std::stack<int> nums;        // 存放重复次数
        std::stack<std::string> strs; // 存放 '[' 之前的字符串
        std::string curr;            // 当前正在构建的字符串
        int num = 0;                 // 当前正在累积的数字

        for (char c : s) {
            if (std::isdigit(c)) {
                // 1. 数字：累积到 num 中（处理多位数）
                num = num * 10 + (c - '0');
            } 
            else if (c == '[') {
                // 2. 左括号：进入新的一层嵌套
                //    保存当前数字和字符串，然后重置状态
                nums.push(num);
                strs.push(curr);//每次遇到[将上一次,即[前面的字母串压入栈
                num = 0;
                curr.clear();
            } 
            else if (c == ']') {
                // 3. 右括号：一层嵌套结束，解码并拼接
                int times = nums.top();    // 弹出重复次数
                nums.pop();
                std::string prev = strs.top(); // 弹出上一层的字符串
                strs.pop();

                // 将 curr 重复 times 次，接到 prev 后面
                std::string temp;
                for (int i = 0; i < times; ++i) {
                    temp += curr;
                }
                curr = prev + temp;
            } 
            else {
                // 4. 字母：直接拼接到当前字符串
                curr += c;
            }
        }

        return curr;
    }
};


int main(){
    std::string s = "abc3[d2[ef]]g";
    Solution result;
    std::cout<<result.decodeString(s)<<std::endl;
    return 0;
}