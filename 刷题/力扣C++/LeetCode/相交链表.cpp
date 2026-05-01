#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

//Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
 
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        unordered_set<ListNode*> myset;

        ListNode* cur = headA;
        while(cur){
            myset.insert(cur);
            cur = cur->next;
        }

        cur = headB;
        while(cur){
            if(myset.find(cur) != myset.end()){
                //cur 是一个 ListNode* 类型的指针，指向相交节点
                return cur;//返回当前链表位置,之后程序通过cur->val得到当前链表对应的值
            }
            cur = cur->next;
        }
        return nullptr;
    }
};

//辅助函数:打印链表
void printList(ListNode *head){
    ListNode* cur = head;
    while(cur){
        cout<<cur->val;
        cur = cur->next;
        if(cur)   cout<<"->";
    }
    cout<<endl;
}

// 构造相交链表测试用例
// 链表 A: 4 -> 1 ->               
// 链表 B: 5 -> 6 -> 1 ->   (公共部分 8 -> 4 -> 5)
// 返回 headA, headB, 以及预期的相交节点（值为8的节点）
void buildIntersectedLists(ListNode* &headA,ListNode* &headB,ListNode* &intersectNode){
    ListNode *common = new ListNode(8);
    common->next = new ListNode(4);
    common->next->next = new ListNode(5);

    // 链表 A：4 -> 1 -> common
    headA = new ListNode(4);
    headA->next = new ListNode(1);
    headA->next->next = common;

    // 链表 B：5 -> 6 -> 1 -> common
    headB = new ListNode(5);
    headB->next = new ListNode(6);
    headB->next->next = new ListNode(1);
    headB->next->next->next = common;

    // 记录相交节点（值为8的节点）
    intersectNode = common;
}

// 构造不相交链表测试用例
// 链表 A: 2 -> 6 -> 4
// 链表 B: 1 -> 5
void buildNonIntersectedLists(ListNode* &headA, ListNode* &headB) {
    headA = new ListNode(2);
    headA->next = new ListNode(6);
    headA->next->next = new ListNode(4);

    headB = new ListNode(1);
    headB->next = new ListNode(5);
}

// deleteLists 函数
void deleteLists(ListNode *headA, ListNode *headB) {
    unordered_set<ListNode*> deleted;
    
    ListNode *cur = headA;
    while (cur) {
        if (deleted.find(cur) == deleted.end()) {
            deleted.insert(cur);
            ListNode *temp = cur;
            cur = cur->next;
            delete temp;
        } else {
            break;
        }
    }
    
    cur = headB;
    while (cur) {
        if (deleted.find(cur) == deleted.end()) {
            deleted.insert(cur);
            ListNode *temp = cur;
            cur = cur->next;
            delete temp;
        } else {
            break;
        }
    }
}


int main(){
    Solution sol;
    // 测试用例 1：相交链表
    cout << "测试用例1：相交链表" << endl;
    ListNode *headA1, *headB1, *intersect1;
    buildIntersectedLists(headA1, headB1, intersect1);
    cout << "链表A: ";
    printList(headA1);
    cout << "链表B: ";
    printList(headB1);
    ListNode *result1 = sol.getIntersectionNode(headA1, headB1);
    if (result1) {
        cout << "相交节点值为: " << result1->val << endl;
        cout << "预期相交节点值为: " << intersect1->val << endl;
        if (result1 == intersect1) {
            cout << "测试通过~" << endl;
        } else {
            cout << "测试失败!" << endl;
        }
    } else {
        cout << "未找到相交节点（预期应相交）" << endl;
    }

    cout << "\n测试用例2：不相交链表" << endl;
    ListNode *headA2, *headB2;
    buildNonIntersectedLists(headA2, headB2);
    cout << "链表A: ";
    printList(headA2);
    cout << "链表B: ";
    printList(headB2);
    ListNode *result2 = sol.getIntersectionNode(headA2, headB2);
    if (result2) {
        cout << "相交节点值为: " << result2->val << "（预期应为nullptr）" << endl;
    } else {
        cout << "未找到相交节点（符合预期）" << endl;
    }
    
    cout << "\n测试用例3：其中一个链表为空" << endl;
    ListNode *headA3 = new ListNode(1);
    ListNode *headB3 = nullptr;
    ListNode *result3 = sol.getIntersectionNode(headA3, headB3);
    cout << "结果: " << (result3 ? "相交节点" : "nullptr（符合预期）") << endl;


    // 释放内存（测试用例1相交链表）
    deleteLists(headA1, headB1);
    // 释放测试用例2（不相交，独立释放）
    deleteLists(headA2, headB2);
    // 释放测试用例3
    delete headA3;
    return 0;
}