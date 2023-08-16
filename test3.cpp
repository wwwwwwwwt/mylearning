/*
 * @Author: zzzzztw
 * @Date: 2023-07-24 12:57:46
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-06 16:19:52
 * @FilePath: /myLearning/test3.cpp
 */
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <memory>
#include <vector>

extern "C"
using namespace std;


class test{
public:
    test(){
        flag = A;
    }

    void PrintA(){
        unique_lock<mutex>locker(mtx);
        while(true){
            cv.wait(locker, [&](){
                return flag == A;
            });
            cout<<"A"<<endl;
            this_thread::sleep_for(chrono::seconds(1));
            state(flag);
            cv.notify_all();
        }
    }
    void PrintB(){
        unique_lock<mutex>locker(mtx);
        while(true){
            cv.wait(locker, [&](){
                return flag == B;
            });
            cout<<"B"<<endl;
            this_thread::sleep_for(chrono::seconds(1));
            state(flag);
            cv.notify_all();
        }
    }
    void PrintC(){
        unique_lock<mutex>locker(mtx);
        while(true){
            cv.wait(locker, [&](){
                return flag == C;
            });
            cout<<"C"<<endl;
            this_thread::sleep_for(chrono::seconds(1));
            state(flag);
            cv.notify_all();
        }
    }

private:
    enum Flag{A,B,C};
    volatile Flag flag;
    void state(volatile enum Flag &flag){
        switch(flag){
            case A:
                flag = B;
                break;
            case B:
                flag = C;
                break;
            case C:
                flag = A;
                break;
        }
    }
    mutex mtx;
    condition_variable cv;
};


template <typename T>
class Single{
public:
    static shared_ptr<T> GetInstance(){
        static once_flag flag;
        call_once(&flag, [&](){
            ins = shared_ptr<T>(new T);
        });
        return ins;
    }
private:
    static shared_ptr<T> ins;
};

template<typename T>
shared_ptr<T> Single<T>::ins = nullptr;

__attribute__((constructor)) void hello(){
    printf("hello\n");
}

__attribute__((destructor)) void bye(){
    printf("bye\n");
}



bool is_big_endian(){
    int a = 0x00000001;
    if(*(char*)(&a) == 1)return false;
    return true;
}

int main(){

   /* test A;
    thread t1(&test::PrintA, &A);
    thread t2(&test::PrintB, &A);
    thread t3(&test::PrintC, &A);

    t1.join();
    t2.join();
    t3.join();*/
   /* vector<int>a = {1,2,3,4,5};
    a.reserve(a.capacity() + 1);
    for(int i = 0; i < a.size(); i++){
        cout<<a[i]<<" "<< &a[i]<<endl;
    }
    cout<<"-------------------------"<<endl;
    a.insert(a.begin() + 2, 10);
    for(int i = 0; i < a.size(); i++){
        cout<<a[i]<<" "<< &a[i]<<endl;
    }*/


    if(!is_big_endian())cout<<"xiaoduan"<<endl;
    else cout<<"daduan"<<endl;

    return 0;
}