/*
 * @Author: zzzzztw
 * @Date: 2023-05-25 19:01:28
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-29 21:33:48
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
	"google.golang.org/grpc/metadata"
)

func main() {

	/*cc, err := grpc.Dial("localhost:8000", grpc.WithInsecure())
	if err != nil {
		panic(err)
	}*/

	//	cli := userProto.NewAuthServiceClient(cc)

	// clientpool := NewuserClientPool("localhost:8000", 2)

	// //ctx := metadata.AppendToOutgoingContext(context.Background(), "token", "123")
	// md := metadata.Pairs("key", "val", "token", "456")
	// ctx := metadata.NewOutgoingContext(context.Background(), md)
	// resp, err := clientpool.Get().Login(ctx, &userProto.LoginRequest{
	// 	Username: "admin",
	// 	Password: "admin",
	// })

	// if err != nil {
	// 	panic(err)
	// }

	// fmt.Println(resp.Token, resp.User.Id)

	// service a ctx
	clientpool := NewuserClientPool("localhost:8000", 2)
	//md := metadata.Pairs("key", "value", "token", "123")
	ctx := metadata.AppendToOutgoingContext(context.Background(), "token", "123")

	{
		resp, err := clientpool.Get().Login(ctx, &userProto.LoginRequest{
			Username: "",
			Password: "admin",
		})
		if err != nil {
			log.Fatal(err)
		}
		fmt.Println(resp.Token, resp.User.Id)
	}

	{
		_, err := clientpool.Get().Register(ctx, &userProto.RegisterRequest{
			Username: "",
			Password: "",
		})
		if err != nil {
			log.Fatal(err)
		}
	}

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
