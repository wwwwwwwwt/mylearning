/*
 * @Author: zzzzztw
 * @Date: 2023-07-13 19:32:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-24 11:16:52
 * @FilePath: /myLearning/test2.cpp
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode{
    int val;
    ListNode* next;
    ListNode(int v):val(v), next(nullptr){};
};

int main(){
    
 
    vector<int>a = {0,1,2,3,4,5,6};

    vector<int>b = std::move(a);
    cout<<b.size()<<" "<<b.capacity()<<endl;
    cout<<a.size()<<" "<<a.capacity()<<endl;

    ListNode* dummyhead = new  ListNode(-1);
    ListNode* cur = dummyhead;
    for(int i = 0; i < 10; i++){
        ListNode* node = new ListNode(i);
        cur->next = node;
        cur = cur->next;
    }
    cur = dummyhead;
    int k = 2;
    int  loop = 5;
    while(loop--){
        ListNode* temp = cur->next;
        for(int  i = 0; i < k - 1; i++){
            ListNode* node = temp->next;
            temp->next = node->next;
            node->next = cur->next;
            cur->next = node;
        }
        for(int i = 0; i < k; i++){
            cur = cur->next;
        }
    }
    cur = dummyhead;
    while(cur && cur->next){
        cout<<cur->next->val<<" ";
        cur = cur->next;
    }
    cout<<endl;


    return 0;
    
}  