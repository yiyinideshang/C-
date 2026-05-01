#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <string>

// 您的LRUCache对象将如下所示实例化和调用：
// LRUCache* obj = new LRUCache(capacity);
// int param_1 = obj->get(key);
// obj->put(key,value);
class LRUCache {
public:
    LRUCache(int capacity):capacity(capacity) {
        
    }
    
    int get(int key) {
        auto umap_iter = key_to_iter.find(key);
        if(umap_iter == key_to_iter.end()){//没有这本书
            return -1;
        }
        auto list_iter = umap_iter->second;// 有这本书
        //把这本书(list_iter) 从书堆里(cache_list)中抽出来,放到最上面(cache_list.begin())
        //将链表 x 中迭代器 i 指向的元素移动到当前链表的 position 位置。
        cache_list.splice(cache_list.begin(),cache_list,list_iter);
        return list_iter->second;// 返回这本书的 value
    }
    
    void put(int key, int value) {
        auto umap_iter = key_to_iter.find(key);
        if(umap_iter != key_to_iter.end()){//有这本书
            auto list_iter = umap_iter->second;
            list_iter->second = value;//更新value值
            //把这本书(list_iter) 从书堆里(cache_list)中抽出来,放到最上面(cache_list.begin())
            cache_list.splice(cache_list.begin(),cache_list,list_iter);
            return ;
        }
        //新书,放到最上面(emplace_front),存入哈希表中
        cache_list.emplace_front(key,value);
        key_to_iter[key] = cache_list.begin();
        //书太多了,超过capacity容量
        if(key_to_iter.size() > capacity){
            //去掉最后一本书,删除对应的哈希表
            //.back返回最后一个元素的引用;首先通过cache_list.back()方法得到最后一个节点,
            //再使用.first方法,得到该节点的key值
            //最后从哈希表中删除该key,随之即删除了该key->双链表的链表节点迭代器
            key_to_iter.erase(cache_list.back().first);
            //从链表中删除最后一本书
            cache_list.pop_back();
        }
    }
private:
    int capacity;
    //pair里面存的是key和value;
    // cache_list   key->value
    std::list<std::pair<int,int>> cache_list;
    //key -> 双链表的链表节点迭代器
    //key_to_iter key ->list<int,int>::iterator
    std::unordered_map<int,std::list<std::pair<int,int>>::iterator> key_to_iter;
};

int main(){
 // 操作序列（仅用于展示）
    std::vector<std::string> output;  // 存放结果（字符串形式）

    LRUCache* obj = new LRUCache(2);

    // 1. put(1, 1) → null
    obj->put(1, 1);
    output.push_back("null");

    // 2. put(2, 2) → null
    obj->put(2, 2);
    output.push_back("null");

    // 3. get(1) → 1
    output.push_back(std::to_string(obj->get(1)));

    // 4. put(3, 3) → null
    obj->put(3, 3);
    output.push_back("null");

    // 5. get(2) → -1
    output.push_back(std::to_string(obj->get(2)));

    // 6. put(4, 4) → null
    obj->put(4, 4);
    output.push_back("null");

    // 7. get(1) → -1
    output.push_back(std::to_string(obj->get(1)));

    // 8. get(3) → 3
    output.push_back(std::to_string(obj->get(3)));

    // 9. get(4) → 4
    output.push_back(std::to_string(obj->get(4)));

    // 打印结果，格式为 [null, null, null, 1, ...]
    std::cout << "[";
    for (size_t i = 0; i < output.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << output[i];
    }
    std::cout << "]" << std::endl;

    delete obj;  // 不要忘记释放内存
    return 0;
}