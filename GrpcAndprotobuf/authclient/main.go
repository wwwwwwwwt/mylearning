/*
 * @Author: zzzzztw
 * @Date: 2023-05-25 19:01:28
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-25 19:40:24
 * @FilePath: /myLearning/GrpcAndprotobuf/authclient/main.go
 */
package main

import (
	userProto "GrpcAndprotobuf/proto"
	"context"
	"fmt"
	"log"
	"sync/atomic"

	"google.golang.org/grpc"
)

func main() {

	/*cc, err := grpc.Dial("localhost:8000", grpc.WithInsecure())
	if err != nil {
		panic(err)
	}*/

	//	cli := userProto.NewAuthServiceClient(cc)

	clientpool := NewuserClientPool("localhost:8000", 2)
	resp, err := clientpool.Get().Login(context.Background(), &userProto.LoginRequest{
		Username: "admin",
		Password: "admin",
	})

	if err != nil {
		panic(err)
	}

	fmt.Println(resp.Token, resp.User.Id)

}

// 客户端连接池化

type userClientPool struct {
	clients []userProto.AuthServiceClient
	// mtx     sync.Mutex
	idex int64
}

func (p *userClientPool) Get() userProto.AuthServiceClient {
	//p.mtx.Lock()
	//defer p.mtx.Unlock()
	//新增idx
	idx := atomic.AddInt64(&p.idex, 1)
	log.Println(idx)
	return p.clients[int(idx)%(len(p.clients))]

}

func NewuserClientPool(addr string, sz int) *userClientPool {
	cc, err := grpc.Dial(addr, grpc.WithInsecure())
	if err != nil {
		panic(err)
	}

	var clients []userProto.AuthServiceClient

	for i := 0; i < sz; i++ {
		clients = append(clients, userProto.NewAuthServiceClient(cc))
	}

	return &userClientPool{clients: clients, idex: 0}
}

func (p *userClientPool) Release(client userProto.AuthServiceClient) {

}
