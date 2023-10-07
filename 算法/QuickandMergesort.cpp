/*
 * @Author: zzzzztw
 * @Date: 2023-05-29 14:53:53
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-07 16:30:39
 * @FilePath: /myLearning/算法/QuickandMergesort.cpp
 */
#include <bits/stdc++.h>

using namespace std;

// 开128位
#include<bits/stdc++.h>
using namespace std;
inline __int128 read(){
    __int128 x = 0, f = 1;
    char ch = getchar();
    while(ch < '0' || ch > '9'){
        if(ch == '-')
            f = -1;
        ch = getchar();
    }
    while(ch >= '0' && ch <= '9'){
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}
inline void print(__int128 x){
    if(x < 0){
        putchar('-');
        x = -x;
    }
    if(x > 9)
        print(x / 10);
    putchar(x % 10 + '0');
}
using ll = __int128_t;

int n;
const int N = 100;
int s[N];
void merge_sort(vector<int>&a, int l, int r){
    if(l >= r)return;
    int mid = l + r >> 1;
    int i = l, j = mid + 1, k = 0;
    merge_sort(a, l, mid);
    merge_sort(a, mid + 1, r);

    while(i <= mid && j <= r){
        if(a[i] < a[j]){
            s[k++] = a[i++];
        }else{
            s[k++] = a[j++];
        }
    }
    while(i <= mid)s[k++] = a[i++];
    while(j <= r)s[k++] = a[j++];
    for(int u = l, p = 0; u<=r; u++){
        a[u] = s[p++];
    }
}

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

int main(){
    cin>>n;
    vector<int>a(n);
    for(int i = 0; i<n; i++){
        cin>>a[i];
    }

    //merge_sort(a, 0, n-1);
    quick_sort(a, 0, n-1);
    for(int i = 0; i< n;i++)cout<<a[i]<<" ";
    cout<<"\n";
    return 0;
}