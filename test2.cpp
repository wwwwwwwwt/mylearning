#include <memory>
#include <mutex>
#include <iostream>
#include <vector>
using namespace std;

template<typename T>

class Single{
public:
    static shared_ptr<T> GetInstance(){
        static once_flag flag;
        call_once(flag, [&]{
            ins = shared_ptr<T>(new T);
        });
        return ins;
    }

private:
    static shared_ptr<T>ins;
};

template<typename T>
shared_ptr<T> Single<T>::ins = nullptr;


class Test{
public:
    Test(){
        cout<<"test construct"<<endl;
    }
    void Print(){
        cout<<"hello!"<<endl;
    }
};


void quick_sort(vector<int>&a, int l, int r){
    if(l >= r)return;
    int x = a[l + r >> 1];
    int i = l - 1, j = r + 1;
    while(i < j){
        do i++; while(a[i] < x);
        do j--; while(a[j] > x);
        if(i < j)swap(a[i], a[j]);
    }
    quick_sort(a, l, j);
    quick_sort(a, j + 1, r);
}

int s[100];

void merge_sort(vector<int>&a, int l, int r){
    if(l >= r)return;
    int mid = l + r >> 1;
    merge_sort(a, l, mid);merge_sort(a, mid + 1, r);
    int i = l, j = mid + 1, k = 0;
    while(i <= mid && j <= r){
        if(a[i] < a[j])s[k++] = a[i++];
        else s[k++] = a[j++];
    }
    while(i <= mid)s[k++] = a[i++];
    while(j <= r)s[k++] = a[j++];
    for(int q = l , p = 0; q <= r; q++){
        a[q] = s[p++];
    }
    return;
}   

int main(){
    auto single = Single<Test>::GetInstance();
    single->Print();

    vector<int>a = {0,5,1,4,76,12};
    //quick_sort(a, 0, (int)a.size() - 1);
    merge_sort(a, 0, (int)a.size() - 1);
    for(int i = 0; i < (int)a.size(); i++)cout<<a[i]<<" ";
    printf("\n");
    return 0;
}