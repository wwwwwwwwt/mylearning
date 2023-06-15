/*
 * @Author: zzzzztw
 * @Date: 2023-02-25 16:48:22
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-14 16:09:09
 * @FilePath: /myLearning/threadprint/test.cpp
 */
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
using namespace std;
class test{
private:
    mutex mtx_;
    condition_variable cv_;
  //  char flag;
    enum Flag {A = 0, B, C};
    volatile Flag flag;
    void state(volatile Flag &flag){
        switch (flag)
        {
        case A:
            flag = B;
            break;
        
        case B:
            flag = C;
            break;
        case C:
            flag = A;
            break;
        default:
            break;
        }
    }
public:
    test() {flag = A;}
    ~test(){};
    void printa(){
        unique_lock<mutex>locker(mtx_);
        while(true){
            cv_.wait(locker,[&]{
                return flag == A;
            });
            this_thread :: sleep_for(chrono::seconds(1));
            printf("A\n");
            state(flag);
            cv_.notify_all();
       }
    }
    void printb(){
        unique_lock<mutex>locker(mtx_);
        while(true){
            cv_.wait(locker,[&]{
                return flag == B;
            });
            this_thread :: sleep_for(chrono::seconds(1));
            printf("B\n");
            state(flag);
            cv_.notify_all();
        }

    }
    void printc(){
        unique_lock<mutex>locker(mtx_);
        while(true){
            cv_.wait(locker,[&]{
                return flag == C;
            });
            this_thread :: sleep_for(chrono::seconds(1));
            printf("C\n");
            state(flag);
            cv_.notify_all();
        }

    }
};

int main(){
    test a;
    thread t1(&test::printa,&a);
    thread t2(&test::printb,&a);
    thread t3(&test::printc,&a);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}