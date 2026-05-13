#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        //prerequisites{{a1,b1},{a2,b2}},学a1必须先学b1,b1->a1
        //[1,0],[2,0],[3,1],[3,2]   0->1,0->2,1->3,2->3;
        //存储每个顶点的出度信息,从0~numCourses-1,
        //如0->1,0->2则graph[0] = {1,2},表示0课程学完才可以学课程1,2
        //graph[1] = {3}
        //graph[2] = {3}
        vector<vector<int>> graph(numCourses);

        //存储0~numCourses-1每个顶点的入度个数
        //如0:0,
        //1:1,  0->1;
        //2:1,  0->2;
        //3:2;  1->3,2->3
        vector<int> indegree(numCourses,0);

        for(const auto& pre:prerequisites){
            int a = pre[0];//要修的课程
            int b = pre[1];//先修的课程
            graph[b].push_back(a);//存储每个顶点b的出度信息b->a
            indegree[a]++;//存储每个顶点a的入度个数,b出度相当于a入度
        }

        queue<int> q;
        for(int i = 0;i<numCourses;i++){
            if(indegree[i] == 0){//将所有入度为0的顶点入队
                q.push(i);
            }
        }
        
        int visited = 0;
        while(!q.empty()){
            int cur = q.front();
            q.pop();
            visited++;//当前课可修
        
            for(int next:graph[cur]){//当前课在graph保存的出度信息依次记录为next
                //对应的next的入度个数-1;
                indegree[next]--;
                if(indegree[next] == 0){
                    q.push(next);
                }
            }
        }
        //如果所有课都修过,则说明无环
        return visited == numCourses;
    }
};

int main(){

    Solution s;
    int numCoures = 4;
    vector<vector<int>> prerequisited = 
    {
        {1,0},
        {2,0},
        {3,1},
        {3,2}
    };
    int result = s.canFinish(numCoures,prerequisited);
    if(result){
        std::cout<<"true";
    }
    else{
        std::cout<<"false";
    }
    return 0;
}