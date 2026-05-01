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

class Solution
{
public:
    ListNode *insertionSortList(ListNode *head)
    {  
        //处理空链表或者只有一个节点的链表,已是有序的
        if(!head ||!head->next){
            return head;
        }

        //1. 创建哑元节点,简化在头部插入的操作
        ListNode dummy(0);
        dummy.next = head;

        //2.初始化指针,prev指向有序链表的尾部,首先指向链表的第一个节点'
        //curr指向待插入的首个节点,首先指向链表第一个节点的后继节点
        ListNode *prev = head;
        ListNode *curr = head->next;

        while(curr){
            //情况1:待插入节点大于等于已排序节点的末尾,直接追加
            if(curr->val >= prev->val){
                prev = prev->next;//追加后,更新已排序节点的末尾指针
            }
            //情况2:待插入节点更小,遍历之前的已排序节点的链表
            else{
                //从链表的头部存储的值开始,寻找第一个大于curr->val的节点的前驱节点
                ListNode *p = &dummy;
                while(p->next->val <= curr->val){
                    p = p->next;
                }
                //最终p节点停在第一个大于curr->val的节点的前驱节点
                //如 0->3->5->4->1时,prev停在5,curr停在4,p停在3
                //执行插入操作
                prev->next = curr->next;
                curr->next = p->next;
                p->next = curr;
                //之后变为0->3->4->5->1
            }
            //curr指针移动到下一个待插入节点
            curr = prev->next;
        }
        return dummy.next;
    }
};

int main()
{
    std::vector<int> vet = {4, 2, 1, 3};
    ListNode *head = bulidList(vet);
    Solution s;
    head = s.insertionSortList(head);

    printList(head);
    deleteList(head);

    return 0;
}