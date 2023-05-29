/*
 * @Author: zzzzztw
 * @Date: 2023-05-25 16:21:53
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-29 21:39:24
 * @FilePath: /myLearning/GrpcAndprotobuf/internal/grpc/server.go
 */
package grpc

import (
	"GrpcAndprotobuf/internal/ctrl"
	userProto "GrpcAndprotobuf/proto"
	"context"
	"net"

	"github.com/grpc-ecosystem/go-grpc-middleware/v2/interceptors/auth"
	"github.com/grpc-ecosystem/go-grpc-middleware/v2/interceptors/validator"
	"google.golang.org/grpc"
)

func StartGRPCServer() {
	lis, err := net.Listen("tcp", ":8000")
	if err != nil {
		panic(err)
	}

	s := grpc.NewServer(
		// 		一元拦截器
		// grpc.UnaryInterceptor(
		// 	func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
		// 		// 中间件的使用，洋葱模型
		// 		//前处理
		// 		t1 := time.Now()

		// 		//陷入核心逻辑
		// 		resp, err = handler(ctx, req)

		// 		//后处理
		// 		fmt.Println("耗时：", time.Since(t1))
		// 		return resp, err
		// 	},
		// )
		//链式调用
		// grpc.ChainUnaryInterceptor(
		//
		// 	func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
		// 		// 中间件的使用，洋葱模型
		// 		//前处理
		// 		//t1 := time.Now()
		// 		fmt.Println("login1 before")
		// 		//陷入核心逻辑
		// 		resp, err = handler(ctx, req)
		// 		fmt.Println("login1 after")

		// 		//后处理
		// 		//	fmt.Println("耗时：", time.Since(t1))
		// 		return resp, err
		// 	},
		// 	func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
		// 		fmt.Println("login2 before")
		// 		//陷入核心逻辑
		// 		resp, err = handler(ctx, req)
		// 		fmt.Println("login2 after")
		// 		return resp, err
		// 	},
		// 	func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
		// 		fmt.Println("login3 before")
		// 		//陷入核心逻辑
		// 		resp, err = handler(ctx, req)
		// 		fmt.Println("login3 after")
		// 		return resp, err
		// 	},

		// 	// login1 before
		// 	// login2 before
		// 	// login3 before
		// 	// 2023/05/29 18:46:34 user login->  admin admin
		// 	// login3 after
		// 	// login2 after
		// 	// login1 after
		// ),
		// grpc.UnaryInterceptor(
		// 	auth.UnaryServerInterceptor(func(ctx context.Context) (context.Context, error) {
		// 		// // 期望： key:value  key = token ,  value = jwt / 123
		// 		// // 1. 从ctx取出token信息
		// 		// // metadata 作用是承载grpc请求中，请求头中的信息header。请求体是我们之前定义的protobuf中的消息
		// 		// // 客户端向服务端传数据，客户端时outgoing， 客户端时incoming
		// 		// // 2. 客户端发来的格式：key：val， token， 123， 收到使用md.Get("指定字段")取出后是一个string切片
		// 		// // 	md := metadata.Pairs("key", "val", "token", "123")
		// 		// //  ctx := metadata.NewOutgoingContext(context.Background(), md)
		// 		// md, ok := metadata.FromIncomingContext(ctx)
		// 		// // 2. 验证token
		// 		// if !ok {
		// 		// 	return ctx, status.Error(codes.Unauthenticated, "noAuthorized")
		// 		// }

		// 		// token := md.Get("token") // 取出token
		// 		// key := md.Get("key")
		// 		// log.Println("key -> ", key)
		// 		// if len(token) > 0 {
		// 		// 	tk := token[0]
		// 		// 	//验证token， 拦截||放过数据
		// 		// 	if tk == "123" {
		// 		// 		return ctx, nil
		// 		// 	}
		// 		// 	return ctx, status.Error(codes.Unauthenticated, "noAuthorized")
		// 		// }
		// 		// return ctx, status.Error(codes.Unauthenticated, "noAuthorized")

		// 	}),
		grpc.ChainUnaryInterceptor(
			auth.UnaryServerInterceptor(func(ctx context.Context) (context.Context, error) {
				return ctx, nil
			}),
			validator.UnaryServerInterceptor(validator.WithFailFast()), // 如果有错快速失败，不向下进行调用了
		),
	)

	authctl := ctrl.NewAuthController()

	userProto.RegisterAuthServiceServer(s, authctl)

	err = s.Serve(lis)
	if err != nil {
		panic(err)
	}
}
