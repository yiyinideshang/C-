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

class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        // 空链表或只有一个节点，直接返回
        if(!head || !head->next)
            return head;

        ListNode dummy(-1);
        dummy.next = head;
        ListNode* prev = &dummy; // 指向待交换节点对的前驱

        while(prev->next && prev->next->next){
            ListNode *first = prev->next;   // 每对的第一个节点
            ListNode *second = first->next; // 每对的第二个节点  

            // 执行交换和更新
            first->next = second->next;
            second->next = first;
            prev->next = second;

            // 移动 prev 到下一对的前驱（即当前的 first）
            prev = first;
        }
        return dummy.next;
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

void printList(ListNode* head){
    while(head){
        std::cout<<head->val;
        head = head->next;
        if(head)
            std::cout<<"->";
    }
    std::cout<<std::endl;
}

void deleList(ListNode *&head){
    while(head){
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
    std::cout<<"链表已释放!";
}

int main(){
    std::vector<int> vec = {1,2,3,4,5};
    Solution s;
    ListNode* head = bulidList(vec);
    head = s.swapPairs(head);
    printList(head);
    deleList(head);
    return 0;
}