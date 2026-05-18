#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string cur;
        backtrack(res,cur,n,0,0);
        return res;
    }
private:
    void backtrack(vector<string>& res,string& cur,int n,int left,int right){
        if(cur.size() == 2*n){
            res.push_back(cur);
            return;
        }
        if(left<n){
            cur.push_back('(');
            backtrack(res,cur,n,left+1,right);
            cur.pop_back();
        }
        if(right<left){
            cur.push_back(')');
            backtrack(res,cur,n,left,right+1);
            cur.pop_back();
        }
    }
};    

int main(){

    Solution s;
    int n = 3;
    vector<string> str = s.generateParenthesis(n);
    for(string ch:str){
        std::cout<<ch<<" ";
    }
    std::cout<<std::endl;
   
    return 0;
}