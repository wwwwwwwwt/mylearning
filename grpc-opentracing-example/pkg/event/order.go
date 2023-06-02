/*
 * @Author: zzzzztw
 * @Date: 2023-06-02 16:29:45
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-02 16:33:07
 * @FilePath: /myLearning/grpc-opentracing-example/pkg/event/order.go
 */
package event

import (
	"log"
	"opentracing/pb"
)

type CreateOrderPublisher interface {
	Notify(order *pb.Order)
}

func New() CreateOrderPublisher {
	return &createOrderPublisher{}
}

type createOrderPublisher struct {
}

func (c *createOrderPublisher) Notify(order *pb.Order) {
	log.Printf("推送创建订单事件：%v \n", order)
}
