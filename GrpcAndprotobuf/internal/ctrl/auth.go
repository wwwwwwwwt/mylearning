/*
 * @Author: zzzzztw
 * @Date: 2023-05-25 16:27:00
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-29 21:32:22
 * @FilePath: /myLearning/GrpcAndprotobuf/internal/ctrl/auth.go
 */
package ctrl

import (
	"GrpcAndprotobuf/proto"
	userProto "GrpcAndprotobuf/proto"
	"GrpcAndprotobuf/service"
	"context"
	"fmt"
	"log"
)

type AuthController struct {
	proto.UnimplementedAuthServiceServer
}

func NewAuthController() *AuthController {
	return &AuthController{}
}

func validLoginRequest(resq *userProto.LoginRequest) error {
	return nil
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

func (a *AuthController) Register(ctx context.Context, req *userProto.RegisterRequest) (*userProto.RegisterResponse, error) {
	userinfo, err := userFromContext(ctx)
	if err != nil {
		return nil, err
	}
	fmt.Println(userinfo)
	return &userProto.RegisterResponse{}, nil
}
