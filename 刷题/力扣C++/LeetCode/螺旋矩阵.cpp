#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> myvector = {};
        //设置上下左右边界
        if(matrix.empty())
            return myvector;
        int up = 0;//上边界
        int down = matrix.size()-1;//下边界
        int left = 0;//左边界
        int right = matrix[0].size()-1;//右边界
        while(true){
            for(int i = left;i<=right;i++)  
                myvector.push_back(matrix[up][i]);//向右移动
            if(++up > down)  break;//更新上边界

            for(int i = up;i<=down;i++)
                myvector.push_back(matrix[i][right]);//向下移动
            if(--right < left)  break;//更新右边界

            for(int i = right;i>=left;i--)
                myvector.push_back(matrix[down][i]);//向左移动
            if(--down < up) break;//更新下边界

            for(int i = down;i>=up;i--)
                myvector.push_back(matrix[i][left]);//向上移动
            if(++left > right) break;//更新左边界
        }
        return myvector;
    }
};

int main(){
    vector<vector<int>> matrix = {{1,2,3},{4,5,6},{7,8,9}};

    // std::cout<<"[";
    // for(int i = 0;i<matrix.size();i++){
    //     std::cout<<"[";
    //     for(int j = 0;j<matrix[0].size();j++){
    //         std::cout<<matrix[i][j];
    //         if(j!=matrix[0].size()-1){
    //             std::cout<<",";
    //         }
    //     }
    //     std::cout<<"]";
    //     if(i!=matrix.size()-1){
    //         std::cout<<",";
    //     }
    // }
    // std::cout<<"]"<<std::endl;

    Solution s;
    vector<int> myvector;
    myvector = s.spiralOrder(matrix);
    std::cout<<"[";
    for(int i : myvector){
        std::cout<<i<<" ";
    }
    std::cout<<"]"<<std::endl;
    return 0;
}
