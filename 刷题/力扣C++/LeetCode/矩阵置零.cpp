#include <iostream>
#include <vector>
#include <set>
#include <utility>
using namespace std;

class Solution {
public:
// [ [1, 2, 3],
//   [4, 0, 6],
//   [7, 8, 9] ]
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size();     //m行,有m行
        int n = matrix[0].size();  //n列,有n列
        bool firstRowzero = false;
        bool firstColzero = false;

        //判断第0行和第0列有没有0元素
        for(int j = 0;j<n;j++){    //第0行,一行有n列个元素
            if(matrix[0][j]==0){
                firstRowzero = true;//如果第零行有元素值为0,则标记需要处理
            }
        }
        for(int i = 0;i<m;i++){     //第0列,一列有m行个元素
            if(matrix[i][0]==0){ //如果第零列有元素值为0,则标记需要处理
                firstColzero = true;
            }
        }
        
        //用第0行和第0列作为标记,从第1行和第1列开始,如果有元素为0
        //则将其对应的 行首 和 列首 归0
        for(int i = 1;i<m;i++){
            for(int j = 1;j<n;j++){
                if(matrix[i][j]==0){ //如果该元素为0
                    matrix[i][0]=0;  //将该行首的元素归0
                    matrix[0][j]=0;  //将该列首的元素归0
                }
            }
        }

        //根据标记进行矩阵置零
        for(int i = 1;i<m;i++){
            for(int j = 1;j<n;j++){
                if(matrix[i][0]==0 || matrix[0][j]==0){//如果该元素的行首或列首值为0
                    matrix[i][j] = 0;//将该元素归0
                }
            }
        }

        //处理第0行和第0列
        if(firstRowzero){//处理第0行,如果
            for(int j = 0;j<n;j++){
                matrix[0][j] = 0;
            }
        }
        if(firstColzero){//处理第0列
            for(int i = 0;i<m;i++){
                matrix[i][0] = 0;
            }
        }
    }
};

int main(){
    Solution s;
    vector<vector<int>> matrix={{0,1,2,0},{3,4,5,2},{1,3,1,5}};
    s.setZeroes(matrix);
    std::cout<<"[";
    for(int i = 0;i<matrix.size();i++){
        std::cout<<"[";
        for(int j = 0;j<matrix[0].size();j++){
            std::cout<<matrix[i][j];
            if(j != matrix[0].size()-1){
                std::cout<<",";
            }
        }
        std::cout<<"]";
        if(i != matrix.size()-1){
            std::cout<<",";
        }
    }
    std::cout<<"]";

    return 0;
}