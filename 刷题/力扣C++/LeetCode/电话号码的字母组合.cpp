#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        vector<string> mapping = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
        vector<string> result = {""};
    }
   
};

int main(){

    Solution s;
    string digits = "23";
    vector<string> result = s.letterCombinations(digits);
    for(string ch:result){
        std::cout<<ch<<" ";
    }
    return 0;
}