#include <iostream>

template<typename T>
class unique_ptr_{
public:
    unique_ptr_(T* ptr_):ptr(ptr_){};
    ~unique_ptr_(){delete ptr;}
    unique_ptr_(unique_ptr_&& ohter){
        ptr = ohter.release();
    }
    unique_ptr_& operator=(unique_ptr_ ohter){
        unique_ptr_(ohter).swap(*this);
        return *this;
    }
    T* operator->(){
        return ptr;
    }
    T& operator*(){
        return *ptr;
    }
    T* release(){
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
    void swap(unique_ptr_& other){
        using std::swap;
        swap(ptr, other.ptr);
    }

private:
    T* ptr;
};

class test{
public:
    test(){
        a = 0;
        std::cout<<"construct test"<<std::endl;
    }
    ~test(){
        std::cout<<"destruct test"<<std::endl;
    }
    void add(){
        a++;
    }
    int get(){
        return a;
    }
private:
    int a;

};

int main(){
    unique_ptr_<test>ptr = new test();
    ptr->add();
    int res = ptr->get();
    auto a = std::move(ptr);
    a->add();
    std::cout<<a->get()<<std::endl;
    return 0;
}