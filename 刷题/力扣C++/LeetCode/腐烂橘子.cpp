#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        std::queue<std::pair<int,int>> q;
        int m = grid.size();//行数,列最大值
        int n = grid[0].size();//列数,行最大值
        int fresh = 0;
        //初始化队列,并统计新鲜橘子
        for(int i = 0;i<m;i++){
            for(int j = 0;j<n;j++){
                if(grid[i][j]==2){
                    q.push({i,j});
                }
                else if(grid[i][j] == 1){
                    fresh++;
                }
            }
        }
        if(fresh == 0)  return 0;//如果一开始就没有新鲜橘子,直接返回0分钟后所有橘子都会腐烂

        //四个方向感染
        std::vector<std::pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
        int minute = 0;

        while(!q.empty()){
            //记录本次队列的个数,避免之后向队列里添加元素时破坏一轮感染的次数
            // 表示本次需要同时size个橘子开始向四周腐败
            int size = q.size();
            bool infeceted = false;
            for(int i = 0;i< size;++i){
                auto [x,y] = q.front();
                q.pop();
                for(auto& d:dirs){
                    int nx = x + d.first;
                    int ny = y + d.second;
                    if(nx>=0&&nx<m&&ny>=0&&ny<n&&grid[nx][ny]==1){
                        grid[nx][ny] = 2;
                        fresh--;
                        q.push({nx,ny});
                        infeceted = true;
                    }
                }
            }
            if(infeceted) minute++;
        }

        return fresh?-1:minute;
    }
};

int main(){
    Solution s;
    vector<vector<int>> grid = {
        {2,1,1},
        {1,1,0},
        {0,1,1}
    };
    int result = s.orangesRotting(grid);
    std::cout<<result<<" 分钟后所有新鲜橘子都会腐烂";

    return 0;
}