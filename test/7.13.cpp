#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
using namespace std;


void quicksort(vector<int>& a, int l, int r){
    if(l >= r)return;
    int target = a[l + r >> 1];
    int i = l - 1, j = r +1;
    while(i < j){
        do i++;while(a[i] < target);
        do j--;while(a[j] > target);
        if(i < j)swap(a[i], a[j]);
    } 
    quicksort(a, l, j);
    quicksort(a, j + 1, r);
}
int k[100];
void merge_sort(vector<int>&a, int l, int r){
    if(l >= r)return;
    int mid = l + r >> 1;
    merge_sort(a, l ,mid);
    merge_sort(a, mid + 1, r);
    int i = l, j = mid + 1, idx = 0;
    while(i <= mid && j <= r){
        if(a[i] < a[j])k[idx++] = a[i++];
        else k[idx++] = a[j++];
    }
    while(i <= mid)k[idx++] = a[i++];
    while(j <= r)k[idx++] = a[j++];
    for(int i = l, p = 0; i <= r; i++){
        a[i] = k[p++];
    }
}

template<typename T>
class Single{
public:
    static  shared_ptr<T> GetSingle(){
        static once_flag flag;
        std::call_once(flag, [&]{
            ins = make_shared<T>();
        });
        return ins;
    }    

private:
    static shared_ptr<T> ins;
};


class TEST{
public:
    TEST(){
        cout<<"test construct"<<endl;
    }
    void Print(){
        cout<<"hello"<<endl;
    }
};

template<typename T>
shared_ptr<T> Single<T>::ins = nullptr;


int main(){
    vector<int>a = {100,3,40,58,92};
    nth_element(a.begin(), a.begin() + 2, a.end());
    cout<<a[1]<<endl;
    string str = "1";
    string str2 = str;
    cout<<sizeof (str)<<" "<<&str2<<endl;


    vector<int>b = {1,2,3,4,5};
    cout<<b.size()<<" "<<b.capacity()<<endl;
    //vector<int>().swap(b);
    b.clear();
    b.shrink_to_fit();
    cout<<b.size()<<" "<<b.capacity()<<endl;
   // quicksort(a, 0, (int)a.size());
    merge_sort(a, 0, (int)a.size());
    for(int i = 0; i < a.size(); i++){
        cout<<a[i]<<endl;
    }

    auto test = Single<TEST>::GetSingle();
    test->Print();


    return 0;
}