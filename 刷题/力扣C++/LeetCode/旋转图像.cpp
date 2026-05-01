#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n=matrix.size(), m=matrix[0].size();

        for(int i=0;i<n;i++)
            for(int j=i;j<m;j++)
                swap(matrix[i][j],matrix[j][i]);

        for(int i=0;i<n;i++)
            reverse(matrix[i].begin(),matrix[i].end());
    }
};

int main(){
    Solution s;
    vector<vector<int>> matrix = {{1,2,3},{4,5,6},{7,8,9}};
    s.rotate(matrix);
    
    //输出打印n x n 的矩阵
    std::cout<<"[";
    for(int i = 0;i<matrix.size();i++){
        std::cout<<"[";
        for(int j = 0;j<matrix.size();j++){
            std::cout<<matrix[i][j];
            if(j!=matrix.size()-1){
                std::cout<<",";
            }
        }
        std::cout<<"]";
        if(i != matrix.size()-1){
            std::cout<<",";
        }
    }
    std::cout<<"]"<<std::endl;
    return 0;
}