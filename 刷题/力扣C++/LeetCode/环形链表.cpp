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
    bool hasCycle(ListNode *head)
    {
        if(head == nullptr || head->next == nullptr){
            return false;
        }
        ListNode *slow = head;
        ListNode *fast = head->next;
        while(slow != fast){
            if(fast == nullptr || fast->next==nullptr){
                return false;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
        return true;
    }
};

ListNode* bulidList(const std::vector<int> &val){
    ListNode dummy(0);
    ListNode *tail = &dummy;
    for(int i : val){
        //tail->next等价于 (*tail).next;tail->next是指针指向的变量的next属性
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
    std::cout<<"链表已释放!"<<std::endl;

}

int main()
{
    vector<int> myvetor = {3,2,0,-4};
    ListNode *head = bulidList(myvetor);
    Solution s;
    std::cout<<s.hasCycle(head)<<std::endl;
    printList(head);
    deleteList(head);
    return 0;
}