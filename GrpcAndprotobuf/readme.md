<!--
 * @Author: zzzzztw
 * @Date: 2023-05-25 15:55:00
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-29 21:44:04
 * @FilePath: /myLearning/GrpcAndprotobuf/readme.md
-->
# readme

## 编译proto文件的sh脚本

* proto文件中 option字 go_package = 项目顶层文件夹名/proto 
* 建立一个proto文件夹用于存放proto文件，在这个文件使用这个脚本。 修改其中的GrpcAndprotobuf为项目顶层文件夹即可

```sh

protoc -I/usr/local/include -I. --go_out=. --go-grpc_out=. ./*.proto
mv ./GrpcAndprotobuf/proto/*.go . && rm -rf GrpcAndprotobuf

```

## proto文件编写

* 首先syntax="proto3"
* package 一般为proto
* message + 名字 类似结构体
* 其中字段1 2 3 4 自增，使用后不许修改
* 原理是根据1 2 3 4位置进行对应字符解析，效率高，不用想json一样带上key
* example
```proto
syntax = "proto3";

package proto;

import "google/protobuf/timestamp.proto";
option go_package = "GrpcAndprotobuf/proto";

message User{

    enum UserStatus{
        Unkown = 0;
        Active = 1;
        InActive = 2;
    }
    int64 id = 1;
    string name = 2;
    string email = 3;
    string password = 4;
    google.protobuf.Timestamp create_at = 5;
    google.protobuf.Timestamp update_at = 6;
    UserStatus status = 7;
    repeated string mobiles = 8; // repeat file 类似数组
}

```
* 简单的使用例子， 使用proto序列化
```go
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
```

* grpc专用字段service， 内部方法都是rpc属性

```go
syntax="proto3";

package proto;

import "user.proto";
option go_package = "GrpcAndprotobuf/proto";

service AuthService {
    rpc Login(LoginRequest) returns (LoginResponse) ;
    rpc Register(RegisterRequest) returns (RegisterResponse) ;
}

message LoginRequest {
    string username = 1;
    string password = 2;
}


message LoginResponse {
    string token = 1 ;
    User user = 2;
}

message RegisterRequest {
    string username = 1;
    string password = 2;
}

message RegisterResponse{}

```


# 案例讲解

1. 生成grpc的go文件后。我们需要关注里面三个方法，前两个方法由我们编写的proto文件中service中定义

```go
type AuthServiceServer interface {
	Login(context.Context, *LoginRequest) (*LoginResponse, error)
	Register(context.Context, *RegisterRequest) (*RegisterResponse, error)
	mustEmbedUnimplementedAuthServiceServer()
}
```
* 类似一个抽象接口，我们自己的服务端需要实现这三个方法
* grpc提供了一个结构体，已经实现了这三个方法，我们可以直接拿过来，当作占位，比如我们可以先只实现Login方法
  
```go
type AuthController struct {
	proto.UnimplementedAuthServiceServer
}

func (a *AuthController) Login(ctx context.Context, resq *userProto.LoginRequest) (*userProto.LoginResponse, error) {

	log.Println("user login-> ", resq.Username, resq.Password)

	// 验证参数
	if err := validLoginRequest(resq); err != nil {
		return nil, err
	}

	// 具体业务

	user, err := service.NewAuthService().Login(ctx, resq.Username, resq.Password)

	if err != nil {
		// 错误日志
		return nil, err
	}

	// 做完业务组装一些响应数据

	resp := &userProto.LoginResponse{
		Token: "123",
		User:  &userProto.User{Id: user.Id},
	}
	//最终返回自己的参数
	return resp, nil
}

```

2. 把我们的grpc服务先启动起来
* 设置我们服务端监听的端口，返回lis
* 使用grpcNew一个服务端
* 然后将实现了我们编写的grpc需要支持的服务的对象（authctl），注册到服务端（s）上```userProto.RegisterAuthServiceServer(s, authctl)```
* 最后让服务端阻塞监听我们获得的连接lis。会被每个连接创建一个协程使用我们注册的服务去处理。

```go
// grpc/server.go
func StartGRPCServer() {
	lis, err := net.Listen("tcp", ":8000")
	if err != nil {
		panic(err)
	}

	s := grpc.NewServer()

	authctl := ctrl.NewAuthController()

	userProto.RegisterAuthServiceServer(s, authctl)

	err = s.Serve(lis)
	if err != nil {
		panic(err)
	}
}


```


3. 客户端逻辑

* 先使用grpc.Dial获得与grpc的连接。withinsecure是默认避开证书检测
* 将我们获得的连接，放进我们编写grpc自动生成的生成客户端的api```cli := userProto.NewAuthServiceClient(cc) ```， 将返回一个我们注册的服务的接口
* 这样我们的cli就可以使用我们注册的服务api进行远程方法调用，来使用protobuf消息的传递。
```go
func main() {
    //authclient/main.go
	cc, err := grpc.Dial("localhost:8000", grpc.WithInsecure())
	if err != nil {
		panic(err)
	}

	cli := userProto.NewAuthServiceClient(cc)

	resp, err := cli.Login(context.Background(), &userProto.LoginRequest{
		Username: "admin",
		Password: "admin",
	})

	if err != nil {
		panic(err)
	}

	fmt.Println(resp.Token, resp.User.Id)

}
```

4. 回到auth服务的逻辑
* 首先验证参数
* 具体的业务逻辑
* 根据proto编写的内容组成响应数据
* 返回响应
```go
func (a *AuthController) Login(ctx context.Context, resq *userProto.LoginRequest) (*userProto.LoginResponse, error) {

	log.Println("user login-> ", resq.Username, resq.Password)

	// 验证参数
	if err := validLoginRequest(resq); err != nil {
		return nil, err
	}

	// 具体业务

	user, err := service.NewAuthService().Login(ctx, resq.Username, resq.Password)

	if err != nil {
		// 错误日志
		return nil, err
	}

	// 做完业务组装一些响应数据

	resp := &userProto.LoginResponse{
		Token: "123",
		User:  &userProto.User{Id: user.Id},
	}
	//最终返回自己的参数
	return resp, nil
}

```

5. 具体的业务逻辑

* demo 目前只是简单的返回一个用户信息id
* 包括一个userModel结构体，单独写出来解耦，包括用户的一些信息。

```go

package service

import (
	"GrpcAndprotobuf/model"
	"context"
)

type Authservice struct{}

func NewAuthService() *Authservice {
	return &Authservice{}
}

func (*Authservice) Login(ctx context.Context, username string, password string) (*model.UserModel, error) {
	return &model.UserModel{Id: 1}, nil
}
```

6. GRPC中间件的使用

* 洋葱模型，一层一层穿透进去，到了最底层进行核心逻辑， 然后再一层一层穿透出去
* 一元拦截器与链式调用
```go
// 在internal/grpc/server.go中
s := grpc.NewServer(
		// grpc.UnaryInterceptor(
		// 	func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
		// 		一元拦截器
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
		grpc.ChainUnaryInterceptor(
			//链式调用
			func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
				// 中间件的使用，洋葱模型
				//前处理
				//t1 := time.Now()
				fmt.Println("login1 before")
				//陷入核心逻辑
				resp, err = handler(ctx, req)
				fmt.Println("login1 after")

				//后处理
				//	fmt.Println("耗时：", time.Since(t1))
				return resp, err
			},
			func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
				fmt.Println("login2 before")
				//陷入核心逻辑
				resp, err = handler(ctx, req)
				fmt.Println("login2 after")
				return resp, err
			},
			func(ctx context.Context, req interface{}, info *grpc.UnaryServerInfo, handler grpc.UnaryHandler) (resp interface{}, err error) {
				fmt.Println("login3 before")
				//陷入核心逻辑
				resp, err = handler(ctx, req)
				fmt.Println("login3 after")
				return resp, err
			},

			// login1 before
			// login2 before
			// login3 before
			// 2023/05/29 18:46:34 user login->  admin admin
			// login3 after
			// login2 after
			// login1 after
		),
	)

```

* 鉴权 auth中间件
* 使用这个包 "github.com/grpc-ecosystem/go-grpc-middleware/v2/interceptors/auth"
* 使用方法：
```go
internal/grpc/server.go
在启动服务时，将中间件作为拦截器注册到NewServer中。
s := grpc.NewServer(
	grpc.UnaryInterceptor(
		auth.UnaryServerInterceptor(func(ctx context.Context) (context.Context, error) {
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

		},
		)),
)

相对应的，客户端需要提供自己的token来进行鉴权
	//ctx := metadata.AppendToOutgoingContext(context.Background(), "token", "123")
	md := metadata.Pairs("key", "val", "token", "123")
	ctx := metadata.NewOutgoingContext(context.Background(), md)
	resp, err := clientpool.Get().Login(ctx, &userProto.LoginRequest{
		Username: "admin",
		Password: "admin",
	})

鉴权模块提供这个函数，如果重写这个函数继承这个接口根据客户端调用的rpc方法名字来进行判断执行哪个鉴权逻辑
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
```
