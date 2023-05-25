/*
 * @Author: zzzzztw
 * @Date: 2023-05-25 16:21:53
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-25 18:59:40
 * @FilePath: /myLearning/GrpcAndprotobuf/internal/grpc/server.go
 */
package grpc

import (
	"GrpcAndprotobuf/internal/ctrl"
	userProto "GrpcAndprotobuf/proto"
	"net"

	"google.golang.org/grpc"
)

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
