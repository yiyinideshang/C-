#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size();
        int n = matrix[0].size();
        int left = 0;
        int right = m*n-1;
        while(left <= right){
            int mid = left+(right-left)/2;
            int x = matrix[mid/n][mid%n];
            if(x == target){
                return true;
            }
            if(x < target){
                left = mid+1;
            }
            else if(x>target){
                right = mid-1;
            }
        }
        return false;
    }
};

int main(){
    vector<vector<int>> matrix  = {{1,3,5,7},{10,11,16,20},{23,30,34,60}};
    Solution s;
    int target = 3;
    std::cout<<s.searchMatrix(matrix,target)<<std::endl;

    return 0;
}
