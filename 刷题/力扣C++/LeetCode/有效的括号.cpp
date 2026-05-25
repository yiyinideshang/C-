#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
using namespace std;

class Solution {
public:
    bool isValid(string s) {
        std::stack<char> stk;
        unordered_map<char,char> pairs = 
        {
            {')','('},
            {']','['},
            {'}','{'}
        };
        for(char ch:s){
            if(ch=='('||ch=='['||ch=='{'){
                stk.push(ch);
            }
            else{
                if(stk.empty()||stk.top()!=pairs[ch]){
                    return false;
                }
                stk.pop();
            }
        }
        return stk.empty();
    }
};

int main(){
    std::string s = "{([])}";
    Solution result;
    std::cout<<result.isValid(s)<<std::endl;
    return 0;
}