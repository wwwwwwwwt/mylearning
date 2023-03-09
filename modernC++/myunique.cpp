/*
 * @Author: zzzzztw
 * @Date: 2023-03-06 10:10:43
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-06 11:24:05
 * @FilePath: /cpptest/modernC++/myunique.cpp
 */
#include <iostream>

using namespace std;

template<typename T>
class Munique_ptr{
public:
    explicit Munique_ptr(T* ptr = nullptr):ptr_(ptr){}
    Munique_ptr(Munique_ptr && rhs){
        ptr_ = rhs.release();
    }
    ~Munique_ptr(){ delete ptr_; }
    Munique_ptr& operator=(Munique_ptr rhs)noexcept
    {
        rhs.Swap(*this);
        return *this;
    }
    template<typename U>
    Munique_ptr(Munique_ptr<U>&& rhs){
        ptr_ = rhs.release();
    }
    template<typename U>
    Munique_ptr& operator=(Munique_ptr<U> rhs){
        
        rhs.Swap(*this);
        return *this;
    }
    void Swap(Munique_ptr &rhs) noexcept
    {
        swap(ptr_, rhs.ptr_);
    }

    T* release() noexcept
    {
        T* ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    T* operator->()const noexcept
    {
        return ptr_;
    }

    T& operator*()const noexcept
    {
        return *ptr_;
    }

    T* get()const noexcept
    {
        return ptr_;
    }
private:
    T* ptr_;
};

class Base
{
public:
    explicit Base(int v = 1):val_(v){};
    virtual ~Base(){cout<<"delete base"<<endl;}
    virtual void print() const {
        cout<<"Base"<<" "<<val_<<endl;
    }
    int get()const {
        return val_;
    }
private:
    int val_;
};
class der:public Base
{
public:
    explicit der(int v,int n):Base(v),node_(n){}
     ~der(){cout<<"delete der"<<endl;}
    void print()const
    {
        cout<<"Base"<<" "<<Base::get()<<" "<<"der"<<" "<<node_<<endl;
    }
private:
    int node_;

};

int main(){
    Munique_ptr<Base>ptr1(new der(1,2));
    Munique_ptr<Base>ptr2;
    ptr2 = move(ptr1);

    /*if(ptr1.get() == nullptr)cout<<"delete ptr1"<<endl;
    ptr2->print();*/

    return 0;
}