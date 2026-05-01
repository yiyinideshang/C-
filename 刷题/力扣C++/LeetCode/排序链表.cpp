#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
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

void printList(ListNode *head)
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

void deleteList(ListNode *&head)
{
    while (head)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
    std::cout << "链表已销毁!" << std::endl;
}

class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        ListNode dummyNew(0);          // 新链表的哨兵头
        ListNode* tailNew = &dummyNew; // 新链表尾指针

        ListNode dummyOld(0);          // 原链表的哨兵头（方便删除操作）
        dummyOld.next = head;

        while (dummyOld.next) {
            // 寻找最小值节点的前驱
            ListNode* prev = &dummyOld;
            ListNode* minPrev = &dummyOld;
            int minVal = INT_MAX;

            while (prev->next) {
                if (prev->next->val < minVal) {
                    minVal = prev->next->val;
                    minPrev = prev;
                }
                prev = prev->next;
            }

            // 摘下最小值节点
            ListNode* minNode = minPrev->next;
            minPrev->next = minNode->next;
            minNode->next = nullptr;   // 断开与原链表的连接

            // 接到新链表尾部
            tailNew->next = minNode;
            tailNew = minNode;
        }

        return dummyNew.next;
    }
};

int main()
{
    std::vector<int> vet = {3,2,4,6,5,1,7,8};
    ListNode *head = bulidList(vet);
    Solution s;
    s.sortList(head);

    printList(head);
    deleteList(head);

    return 0;
}