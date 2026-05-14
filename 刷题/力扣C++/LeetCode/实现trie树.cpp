#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class Trie {
public:
    Trie() = default; //使用默认构造函数
    
    void insert(string word) {
        Trie* node = this;
        for(char c:word){
            if(node->next[c-'a'] == nullptr){

                node->next[c-'a'] = new Trie();
            }
            node = node->next[c-'a'];
        }
        node->isEnd = true;
    }
    
    bool search(string word) {
        Trie* trie = this;
        for(char c:word){
            if(trie->next[c-'a'] == nullptr){
                return false;
            }
            else{
                trie = trie->next[c-'a'];
            }
        }
        return trie->isEnd;
    }
    
    bool startsWith(string prefix) {
        Trie* trie = this;
        for(char c:prefix){
            if(trie->next[c-'a'] == nullptr){
                return false;
            }
            trie = trie->next[c-'a'];
        }
        return true;
        
    }

    ~Trie() {//析构函数
        for (int i = 0; i < 26; ++i)
            delete next[i];
    }
private:
    bool isEnd = false;
    Trie* next[26]{}; //使用C++11的 全零初始化（nullptr）
};

/** 
 * 您的Trie对象将如下所示实例化和调用： * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */

int main(){

    Trie trie = Trie();
    trie.insert("apple");
    std::cout<<trie.search("apple")<<std::endl;
    std::cout<<trie.search("app")<<std::endl;
    std::cout<<trie.startsWith("app")<<std::endl;
    trie.insert("app");
    std::cout<<trie.search("app")<<std::endl;
    return 0;
}