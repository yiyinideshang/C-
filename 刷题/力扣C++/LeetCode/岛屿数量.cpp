#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        //方向数组,代表上下左右四个方向
        std::vector<std::pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
        std::queue<std::pair<int,int>> q;
        int ans = 0;//记录岛屿结果

        for(int i = 0;i<m;i++){
            for(int j = 0;j<n;j++){
                if(grid[i][j] == '1'){
                    ans++;//新岛屿
                    //标记该岛屿,防止环路
                    grid[i][j] = '2';
                    //将该岛屿下标 入队
                    q.push({i,j});

                    //BFS广度优先搜索,扩展整个该岛屿
                    while(!q.empty()){
                        auto [x,y] = q.front();
                        q.pop();
                        //探索四个方向
                        for(auto& d:dirs){
                            int nx = x + d.first;
                            int ny = y + d.second;
                            //如果是每越界,且是未访问的陆地时
                            if(nx >=0 && nx < m && ny >=0 && ny < n && grid[nx][ny] == '1'){
                                grid[nx][ny] = '2';//先标记该陆地为已访问
                                q.push({nx,ny});//再将该陆地下标入队
                            }
                        }
                    }
                }
            }
        }
        return ans;
    }
};

int main(){
    //grid 网格
    vector<vector<char>> grid = 
    {
        {'1','1','1','1','0'},
        {'1','1','0','1','0'},
        {'1','1','0','0','0'},
        {'0','0','0','0','0'}
    };

    Solution s;
    int result = s.numIslands(grid);
    std::cout<<result<<std::endl;

    return 0;
}