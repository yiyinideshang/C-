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
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        ListNode dummp(0);
        ListNode *tail = &dummp;
        int carry = 0;
        while (l1 != nullptr || l2 != nullptr || carry != 0)
        {
            int sum = 0;
            if (l1 != nullptr)
            {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2 != nullptr)
            {
                sum += l2->val;
                l2 = l2->next;
            }
            if(carry)
                ++sum;
            if(sum/10)
                carry = 1;
            else
                carry = 0;
            tail->next = new ListNode(sum%10);
            tail = tail->next;
        }
        return dummp.next;
    }
};

ListNode *bulidList(std::vector<int> &val)
{
    ListNode dummp(0);
    ListNode *tail = &dummp;
    for (int i : val)
    {
        tail->next = new ListNode(i);
        tail = tail->next;
    }
    return dummp.next;
}

void printList(ListNode *&head)
{
    ListNode *tail = head;
    while (tail)
    {
        std::cout << tail->val;
        tail = tail->next;
        if (tail)
            std::cout << "->";
    }
    std::cout << std::endl;
}

void deletList(ListNode *&head)
{
    while (head)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
    std::cout << "链表已被释放!" << std::endl;
}

int main()
{
    Solution s;
    std::vector<int> vec1 = {2, 4, 3};
    std::vector<int> vec2 = {5, 6, 4};
    ListNode *l1 = bulidList(vec1);
    ListNode *l2 = bulidList(vec2);
    ListNode *l3 = s.addTwoNumbers(l1, l2);

    printList(l3);
    deletList(l1);
    deletList(l2);
    deletList(l3);
    return 0;
}