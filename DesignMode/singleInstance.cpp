#include <iostream>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <chrono>
using namespace std;
// 饿汉模式，线程安全，共用一个静态对象。
/*
class SingleInstance{
public:
    static SingleInstance* GetInstance(){
        static SingleInstance ins(5);
        return &ins;
    }
    ~SingleInstance(){}
    int Getcnt()const{
        return count;
    }
    void Addcnt(){
        count++;
    }
private:
    SingleInstance(int cnt){
        count = cnt;
        std::cout<<"饿汉"<<std::endl;
    };
    SingleInstance(SingleInstance&&) = default;
    SingleInstance(const SingleInstance&) = default;
    SingleInstance& operator=(const SingleInstance& other){return *this;}

    int count;

};

int main(){
    SingleInstance* ins = SingleInstance::GetInstance();

    SingleInstance* ins2 = SingleInstance::GetInstance();

    std::cout<<ins->Getcnt()<<std::endl;
    std::cout<<ins2->Getcnt()<<std::endl;
    ins->Addcnt();
    std::cout<<ins->Getcnt()<<std::endl;
    std::cout<<ins2->Getcnt()<<std::endl;

    return 0;
}

输出:
饿汉
5
5
6
6
*/

//懒汉模式，线程不安全需要上锁，懒初始化，直到第一次使用时才进行构造静态对象。

class SingleInstance{
public:
    static SingleInstance* GetInstance(){
        if(ins == nullptr){
            pthread_mutex_lock(&mutex);
            if(ins == nullptr){
                ins = new SingleInstance(5);
            }
            pthread_mutex_unlock(&mutex);
        }
        return ins;
    }

    int Getcnt()const{
        return count;
    }
    void Addcnt(){
        count++;
    }

private:
    SingleInstance(int cnt){
        count = cnt;
        std::cout<<"懒汉"<<std::endl;
    };
    SingleInstance(SingleInstance&&) = default;
    SingleInstance(const SingleInstance&) = default;
    SingleInstance& operator=(const SingleInstance& other){return *this;}
    static SingleInstance* ins;
    static pthread_mutex_t mutex;
    int count;
};

SingleInstance* SingleInstance::ins = nullptr;
pthread_mutex_t SingleInstance::mutex;

void work(){
   SingleInstance* ins = SingleInstance::GetInstance();
   this_thread::sleep_for(chrono::seconds(1));
   ins->Addcnt();
    this_thread::sleep_for(chrono::seconds(1));
    cout<<ins->Getcnt()<<endl;
}

int main(){

    std::thread t1(work);
    std::thread t2(work);


    t1.join();
    t2.join();
 


    return 0;
}