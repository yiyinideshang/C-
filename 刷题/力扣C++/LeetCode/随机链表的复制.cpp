#include <iostream>
#include <unordered_map>

class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

class Solution {
public:
    std::unordered_map<Node*,Node*> hasNode;
    Node* copyRandomList(Node* head) {
        if(!head)
            return nullptr;
        if(!hasNode.count(head)){
            Node* Newhead = new Node(head->val);
            hasNode[head] = Newhead;
            Newhead->next = copyRandomList(head->next);
            Newhead->random = copyRandomList(head->random);
        }
        return hasNode[head];
    }
};

int main(){


    return 0;
}