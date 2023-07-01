#include <iostream>
#include "6.27 testpool.h"

int main(){
    ThreadPool thread_(4);
    thread_.init();

    this_thread::sleep_for(std::chrono::seconds(2));
    cout<<"----------------------------------------------"<<endl;
    vector<future<int>>vec;
    for(int i = 0; i < 20; i++){
        printf("add [%d] task\n", i);
        vec.push_back(thread_.Addtask([i](){
            this_thread::sleep_for(std::chrono::microseconds(rand()%500 + 100));
            printf("task [%d] is done", i);
            return i;
        }));
    }

    this_thread::sleep_for(std::chrono::seconds(2));

    for(int i = 0; i < 20; i++){
        printf("result is %d\n", vec[i].get());
    }
    cout<<"----------------------------------------------"<<endl;
    return 0;
}