/*
 * @Author: zzzzztw
 * @Date: 2023-05-25 16:40:16
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-25 16:49:54
 * @FilePath: /myLearning/GrpcAndprotobuf/service/auth.go
 */
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
