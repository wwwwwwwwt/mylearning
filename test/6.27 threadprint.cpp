/*
 * @Author: zzzzztw
 * @Date: 2023-06-27 12:50:14
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-27 13:42:06
 * @FilePath: /myLearning/test2.cpp
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class test{
public:
    test(){
        flag = A;
        cnt = 0;
    }
    ~test() = default;

    void printA(){
        unique_lock<mutex>locker(mtx);
        while(true){
            cv.wait(locker, [&](){
                return flag == A;
            });
            if(cnt >= 6){
                state(flag);
                cv.notify_all();
                break;
            }
            ++cnt;
            cout<<"A"<<endl;
            this_thread::sleep_for(std::chrono::seconds(1));
            state(flag);
            cv.notify_all();
        }
    }
    void printB(){
        unique_lock<mutex>locker(mtx);
        while(true){
            cv.wait(locker, [&](){
                return flag == B;
            });
            if(cnt >= 6){
                state(flag);
                cv.notify_all();
                break;
            }
            ++cnt;
            cout<<"B"<<endl;
            this_thread::sleep_for(std::chrono::seconds(1));
            state(flag);
            cv.notify_all();
        }
    }
    void printC(){
        unique_lock<mutex>locker(mtx);
        while(true){
            cv.wait(locker, [&](){
                return flag == C;
            });
            if(cnt >= 6){
                state(flag);
                cv.notify_all();
                break;
            }
            ++cnt;
            cout<<"C"<<endl;
            this_thread::sleep_for(std::chrono::seconds(1));
            state(flag);
            cv.notify_all();
        }
    }

private:
    enum Flag{A,B,C};
    volatile Flag flag;
    volatile int cnt;
    mutex mtx;
    condition_variable cv;
    void state(volatile enum Flag& flag){
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
};


int main(){
    test A;
    thread t1(&test::printA, &A);
    thread t2(&test::printB, &A);
    thread t3(&test::printC, &A);

    t1.join();
    t2.join();
    t3.join();
    cout<<"all done"<<endl;
    return 0;
}