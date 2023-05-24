/*
 * @Author: zzzzztw
 * @Date: 2023-05-24 13:49:56
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-24 13:58:58
 * @FilePath: /myLearning/GrpcAndprotobuf/main.go
 */
package main

import (
	userProto "GrpcAndprotobuf/proto"
	"encoding/json"
	"fmt"

	"google.golang.org/protobuf/proto"
)

func main() {
	var user = &userProto.User{
		Name:  "abc",
		Email: "aa@qq.com",
	}

	data, _ := json.Marshal(user)
	fmt.Println(string(data))

	msg, _ := proto.Marshal(user)
	fmt.Println(string(msg))
}
