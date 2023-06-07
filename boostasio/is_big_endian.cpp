#include <iostream>
using namespace std;
bool is_big_endian(){
    int num = 0x00000001;
    cout<<hex<<(int)*((char*)&num)<<endl;
    if(*(char*)&num == 1){
        return false;
    }else return true;
}

int main(){

    int num = 0x12345678;
    char* p = (char*)&num;
    cout<<sizeof(num)<<endl;
    cout<<"原始数据 "<<hex<<num<<endl;
    for(int i = 0; i < sizeof(num);i++){
        cout<<hex<<(int)*(p + i)<<" ";
    }
    cout<<endl;
    if(is_big_endian()){
        cout<<"大端序"<<endl;
    }else cout<<"小端序"<<endl;

    return 0;
}