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
    ListNode* mergeTwoLists(ListNode *list1, ListNode *list2) {
        ListNode dummp(-1);
        ListNode *prev = &dummp;
        while(list1 && list2){
            if(list1->val < list2->val){
                prev->next = list1;
                list1 = list1->next;
            }
            else{
                prev->next = list2;
                list2 = list2->next;
            }
            prev = prev->next;
        }
        prev->next = list1?list1:list2;
        return dummp.next;
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) {     
        ListNode *ans = nullptr;
        for(int i = 0;i<lists.size();++i){
            ans = mergeTwoLists(ans,lists[i]);
        }
        return ans;
    }
};

ListNode *createList(const std::vector<int>& vec)
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
    std::cout<<"Á´±íÒÑÊÍ·Å!"<<std::endl;
}

int main()
{
    ListNode* l1 = createList({1,4,5});
    ListNode* l2 = createList({1,3,4});
    ListNode* l3 = createList({2,6});

    std::vector<ListNode*> lists = {l1,l2,l3};
    Solution s;
    ListNode* head =  s.mergeKLists(lists);
    
    // std::cout<<"["<<std::endl;
    // for(ListNode* head:lists){
    //     std::cout<<" ";
    //     printList(head);
    // }
    // for(ListNode* head:lists){
    //     deletList(head);

    // }
    // std::cout<<"]"<<std::endl;
    printList(head);
    deletList(head);

    return 0;
}