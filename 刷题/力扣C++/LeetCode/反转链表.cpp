#include <iostream>
#include <vector>
using namespace std;

/**
 * Definition for singly-linked list.

 */
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
    ListNode *reverseList(ListNode *head)
    {
        ListNode *prev = nullptr;
        ListNode *curt = head;
        while(curt){
            ListNode *next = curt->next;
            curt->next = prev;
            prev = curt;
            curt = next;
        }
        return prev;
    }
};

void printList(ListNode *&head){
    ListNode *curt = head;
    while(curt){
        std::cout<<curt->val;
        curt = curt->next;
        if(curt)
            std::cout<<" -> ";
    }
    std::cout<<std::endl;
}

ListNode* bulidList(const std::vector<int>& vals){//使用虚拟头结点和尾节点。
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for(int i : vals){//引入虚拟头结点,无需关心第一个节点的特殊处理
        tail->next = new ListNode(i);
        tail = tail->next;
    }
    return dummy.next;

    // ListNode * head = nullptr;
    // ListNode * tail = head;
    // for(int i : vals){
    //     ListNode *curt = new ListNode(i);
    //     if(!head){
    //         head = curt;
    //         tail = curt;
    //     }else{
    //         tail->next = new ListNode(i);
    //         tail = tail->next;
    //     }
    // }
    // return head;
}

void deleteList(ListNode *& head){
    while(head){
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
    std::cout<<"已释放链表!"<<std::endl;
}

int main(){
    Solution s;
    vector<int> myvector = {1,2,3,4,5};
    ListNode *head = bulidList(myvector);
    std::cout<<"反转前的链表:";
    printList(head);
    head = s.reverseList(head);
    std::cout<<"反转后的链表:";
    printList(head);

    deleteList(head);

    return 0;
}