#include <bits/stdc++.h>

using namespace std;


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