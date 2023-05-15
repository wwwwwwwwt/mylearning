/*
 * @Author: zzzzztw
 * @Date: 2023-05-15 10:02:32
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-15 10:19:15
 * @FilePath: /myLearning/Srting/test.cpp
 */
#include "String.h"

using namespace std;
int main(){
    //String str("123");
    String str = "123";
    String str2(std::move(str));
    cout<<str2[0]<<endl;
    str2[0] = '5';
    cout<<"str2: "<<str2<<endl;
    String res = str2 + "123";
    cout<<res<<endl;
    return 0;
}