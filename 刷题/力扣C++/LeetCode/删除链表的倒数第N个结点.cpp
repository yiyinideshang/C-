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
    ListNode *removeNthFromEnd(ListNode *head, int n)
    {
        ListNode dummp(-1);
        dummp.next = head;
        ListNode *prev = &dummp;
        ListNode *tail = &dummp;

        //tail指针比prev先走n+1个节点
        for(int i = 0;i<=n;++i){
            tail = tail->next;
        }

        //同时移动这两个指针,直到tail指针到达链表末尾的nullptr节点,此时prev指针指向正好是待删除节点的前驱节点
        while(tail != nullptr){
            prev = prev->next;
            tail = tail->next;
        }

        //删除待删除节点
        ListNode *temp = prev->next;
        prev->next = prev->next->next;
        delete temp;

        //返回真正的头结点
        return dummp.next;
    }
};

ListNode* bulidList(std::vector<int>& val){
    ListNode dummp(0);
    ListNode *tail = &dummp;
    for(int i : val){
        tail->next = new ListNode(i);
        tail = tail->next;
    }
    return dummp.next;
}

void printList(ListNode *head){
    ListNode *tail = head;
    while(tail){
        std::cout<<tail->val;
        tail = tail->next;
        if(tail) 
            std::cout<<"->";
    }
    std::cout<<std::endl;
}

void deleteList(ListNode *&head){
    while(head){
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
    std::cout<<"已释放链表"<<std::endl;
}

int main()
{
    Solution s;
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int n = 2;
    ListNode *head = bulidList(vec);

    head = s.removeNthFromEnd(head, n);

    printList(head);
    deleteList(head);
    return 0;
}