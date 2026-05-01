#include <iostream>
#include <vector>
#include <stack>
using namespace std;

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
struct ListNode{
    int val;
    ListNode *next;
    ListNode() : val(0),next(nullptr) {}
    ListNode(int x) : val(x),next(nullptr){}
    ListNode(int x,ListNode *next) : val(x),next(next){}
};

class Solution {
public:
    bool isPalindrome(ListNode* head) {
        stack<int> stack;
        ListNode* p = head;
        while(p!=nullptr){
            stack.push(p->val);
            p = p->next;
        }
        p = head;
        while(p!=nullptr){
            if(p->val != stack.top()){
                return false;
            }
            stack.pop();
            p = p->next;
        }
        return true;
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

void printList(ListNode *&head){
    ListNode* curt = head;
    while(curt){
        std::cout<<curt->val;
        curt = curt->next;
        if(curt) std::cout<<" -> ";
    }
    std::cout<<std::endl;
}

void deletList(ListNode *&head){
    while(head){
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
    std::cout<<"Á´±íÒÑÊÍ·Å!"<<std::endl;
}

int main(){
    Solution s;
    vector<int> myvetor = {1,2,2,1};
    ListNode *head;
    head = bulidList(myvetor);
    std::cout<<s.isPalindrome(head)<<std::endl;
    printList(head);
    deletList(head);
    return 0;
}