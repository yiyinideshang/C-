#include <iostream>
#include <stack>
using namespace std;

class MinStack {
public:
    MinStack() {//初始化堆栈对象
        
    }
    
    void push(int val) {//将元素val推入堆栈
        stk.push(val);
        if(minstk.empty()||val<=minstk.top()){//只有当前元素小于最小值时,压入最小栈并更新最小元素
            minstk.push(val);
        }
    }
    
    void pop() {//删除堆栈顶部的元素
        if(stk.top()==minstk.top()){//只有当最小栈中的最小元素与要删除的元素相同时,才删除最小栈
            minstk.pop();
        }
        stk.pop();//始终删除当前工作栈的元素
    }
    
    int top() {//获取堆栈顶部的元素
        return stk.top();
    }
    
    int getMin() {//获取堆栈中的最小元素
        return minstk.top();
    }
private:
    std::stack<int> stk;
    std::stack<int> minstk;
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

int main(){

    MinStack* obj = new MinStack();
    obj->push(-2);
    obj->push(0);
    obj->push(-3);
    std::cout<<obj->getMin()<<std::endl;
    obj->pop();
    std::cout<<obj->top()<<std::endl;
    std::cout<<obj->getMin()<<std::endl;

    return 0;
}