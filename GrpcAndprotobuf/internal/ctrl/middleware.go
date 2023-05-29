/*
 * @Author: zzzzztw
 * @Date: 2023-05-29 21:14:52
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-29 21:44:06
 * @FilePath: /myLearning/GrpcAndprotobuf/internal/ctrl/middleware.go
 */
package ctrl

import (
	"GrpcAndprotobuf/model"
	"context"
	"errors"
	"fmt"
	"log"

	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/metadata"
	"google.golang.org/grpc/status"
)

func (*AuthController) AuthFuncOverride(ctx context.Context, fullMethodName string) (context.Context, error) {
	// 重写这个函数，就会绕过Newserver中的逻辑，根据服务端调用的rpc方法（fullMethodName）来进行判断/
	fmt.Println(fullMethodName)

	if fullMethodName == "/proto.AuthService/Login" { //现在是login方法时就饶过，不验证token
		return ctx, nil
	}
	// 期望： key:value  key = token ,  value = jwt / 123
	// 1. 从ctx取出token信息
	// metadata 作用是承载grpc请求中，请求头中的信息header。请求体是我们之前定义的protobuf中的消息
	// 客户端向服务端传数据，客户端时outgoing， 客户端时incoming
	// 2. 客户端发来的格式：key：val， token， 123， 收到使用md.Get("指定字段")取出后是一个string切片
	// 	md := metadata.Pairs("key", "val", "token", "123")
	//  ctx := metadata.NewOutgoingContext(context.Background(), md)
	md, ok := metadata.FromIncomingContext(ctx)
	// 2. 验证token
	if !ok {
		return ctx, status.Error(codes.Unauthenticated, "noAuthorized")
	}

	token := md.Get("token") // 取出token
	key := md.Get("key")
	log.Println("key -> ", key)
	if len(token) > 0 {
		tk := token[0]
		//验证token， 拦截||放过数据
		if tk == "123" {
			return ctx, nil
		}
		return ctx, status.Error(codes.Unauthenticated, "noAuthorized")
	}
	return ctx, status.Error(codes.Unauthenticated, "noAuthorized")
}

type userInfoKey struct{}

func newUserContext(ctx context.Context, userinfo *model.UserModel) context.Context {
	return context.WithValue(ctx, userInfoKey{}, userinfo)
}

func userFromContext(ctx context.Context) (*model.UserModel, error) {
	um, ok := ctx.Value(userInfoKey{}).(*model.UserModel)
	if !ok {
		return nil, errors.New("not found")
	}
	return um, nil
}
