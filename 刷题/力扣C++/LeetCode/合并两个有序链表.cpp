#include <iostream>
#include <vector>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
public:
    ListNode *mergeTwoLists(ListNode *list1, ListNode *list2)
    {
        // 哨兵头 + 尾插法：每次取两链表当前节点值较小者接入尾部，最后拼接剩余链表。
        ListNode prevhead = ListNode(-1);//哨兵节点prevhead
        ListNode *prev = &prevhead;//尾指针prev动态构建结果链表
        while(list1 != nullptr && list2 != nullptr){
            if(list1->val < list2->val){//选择较小节点接入prev->next,对应节点的链表指针后移
                prev->next = list1;
                list1 = list1->next;
            }
            else{
                prev->next = list2;
                list2 = list2->next;
            }
            prev = prev->next;//尾指针prev同步后移
        }
        prev->next = list1==nullptr?list2:list1;//拼接剩余部分
        return prevhead.next;
    }
};

ListNode *bulidList(std::vector<int>& vec)
{
    ListNode dummp(0);
    ListNode *tail = &dummp;
    for(int i :vec){
        tail->next = new ListNode(i);
        tail = tail->next;
    }
    return dummp.next;
}

void printList(ListNode *& head){
    ListNode *tail = head;
    while(tail){
        std::cout<<tail->val;
        tail = tail->next;
        if(tail)
            std::cout<<"->";
    }
    std::cout<<std::endl;
}

void deletList(ListNode *&head){
    while(head){
        ListNode *tail = head;
        head = head->next;
        delete tail;
    }
    std::cout<<"链表已释放!"<<std::endl;
}

int main()
{
    Solution s;
    std::vector<int> vec1 = {1, 2, 4};
    std::vector<int> vec2 = {1,3,4};
    ListNode *list1 = bulidList(vec1);
    ListNode *list2 = bulidList(vec2);
    ListNode *list3 = s.mergeTwoLists(list1, list2);
    
    printList(list3);
    deletList(list3);
    return 0;
}