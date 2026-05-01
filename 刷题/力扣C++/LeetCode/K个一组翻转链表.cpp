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
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if(k == 1)
            return head;
        int count = 0;
        ListNode *tail = head;
        while(tail){
            count++;
            tail = tail->next;
        }   
        int group_count  = count/k;
       
        ListNode *curt = head;
        ListNode *new_head  = nullptr;
        ListNode* prev_group_tail = nullptr; // 上一组翻转后的尾节点
        int i = 0,j = 0;
        while(curt && i<group_count){
            ListNode *prev = nullptr;
            ListNode* cur_group_tail = curt; 
            j = 0;
            while(curt && j<k){
                ListNode *next = curt->next;
                curt->next = prev;
                prev = curt;
                curt = next;
                ++j;
            }
            if(prev_group_tail){
                prev_group_tail->next = prev;
            }
            else{
                new_head = prev;
            }
            cur_group_tail->next = curt;
            prev_group_tail = cur_group_tail;
            ++i;
        }
        return new_head;
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
    int k = 2;
    head = s.reverseKGroup(head,k);

    printList(head);
    deleteList(head);

    return 0;
}