#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        //思路:遍历整个二维数组,让二维数组的每个位置i,j作为起点进行dfs函数
        int m = board.size();
        int n = board[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dfs(board, word, i, j, 0))//index表示从二维数组中开始匹配 word的下标位置
                    return true;
            }
        }
        return false;
    }
private:
    bool dfs(vector<vector<char>>& board,const string& word,int x,int y,int index){
        int m = board.size();
        int n = board[0].size();
        //越界或不匹配,返回false
        if(x<0||x>=m||y<0||y>=n||board[x][y]!=word[index]){
            return false;
        }
        //如果匹配且匹配到最后一个字符时,返回true
        if(index ==word.size()-1){
            return true;
        }
        //临时标记当前格子为:已访问,防止重复访问
        char temp = board[x][y];
        board[x][y]='#';

        //向四个方向进行递归
        bool found = dfs(board,word,x-1,y,index+1) ||
        dfs(board,word,x+1,y,index+1)||
        dfs(board,word,x,y-1,index+1)||
        dfs(board,word,x,y+1,index+1);

        //回溯,恢复原始字符
        board[x][y]=temp;
        return found;//返回四个方向的递归结果,只要有一个方向为真则为真
    }
};

int main(){

    vector<vector<char>> board = {
    {'A','B','C','E'},
    {'S','F','C','S'},
    {'A','D','E','E'},
    };
    string word = "SEE";
    Solution s;
    std::cout<<s.exist(board,word)<<std::endl;

    return 0;
}