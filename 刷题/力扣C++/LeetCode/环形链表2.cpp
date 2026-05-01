#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution
{
public:
    ListNode *detectCycle(ListNode *head) {
        unordered_set<ListNode*> myset;
        while(head){
            if(myset.find(head) != myset.end()){
                return head;
            }
            myset.insert(head);
            head = head->next;
        }
        return nullptr;
    }
};

ListNode* bulidList(const std::vector<int> &val){
    ListNode dummy(0);
    ListNode *tail = &dummy;
    for(int i : val){
        tail->next = new ListNode(i);
        tail = tail->next;
    }
    return dummy.next;
}

void printList(ListNode *head){
    while(head){
        std::cout<<head->val;
        head = head->next;
        if(head)
            std::cout<<"->";
    }
    std::cout<<std::endl;  
}

void deleteList(ListNode *&head){
    while (head){
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
    std::cout<<"Á´±íÒÑÊÍ·Å!"<<std::endl;

}

int main()
{
    vector<int> myvetor = {3,2,0,-4};
    ListNode *head = bulidList(myvetor);
    Solution s;
    head = s.detectCycle(head);
    printList(head);
    deleteList(head);
    return 0;
}