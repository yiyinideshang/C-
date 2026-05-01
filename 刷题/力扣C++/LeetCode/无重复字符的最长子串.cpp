#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution{
public:
//"cbaebabacd"
//"abc"
    vector<int> findAnagrams(string s,string p){
        vector<int> result;
        int slen = s.length();
        int plen = p.length();
        if(slen<plen){
            return result; 
        }
        vector<int> pcount(26,0),windowcount(26,0);

        //初始化对照窗口,即统计p中各各字母出现的次数
        for(char ch:p){
            pcount[ch-'a']++;
        }

        //初始化滑动窗口,大小为p的长度
        for(int i=0;i<plen;i++){
            windowcount[s[i]-'a']++;
        }

        //如果该滑动窗口与对照窗口相同,则将该滑动窗口的起始下标位置加入结果集中
        if(windowcount==pcount){
            result.push_back(0);
        }

        //进行滑动窗口
        for(int i=plen;i<slen;i++){
            //移除窗口的最左侧元素
            windowcount[s[i-plen]-'a']--;
            //加入窗口的最左侧元素
            windowcount[s[i]-'a']++;
            if(windowcount==pcount){
                result.push_back(i-plen+1);
            }
        }
        return result;
    }
};

int main(){

    Solution s;
    vector<int> my_vector;
    my_vector = s.findAnagrams("cba","abc");
    for(int i : my_vector)
    {
        std::cout<<i<<" ";
    }
    return 0;
}