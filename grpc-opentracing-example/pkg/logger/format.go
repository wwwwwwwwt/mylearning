/*
 * @Author: zzzzztw
 * @Date: 2023-06-02 16:00:48
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-02 16:06:12
 * @FilePath: /myLearning/grpc-opentracing-example/pkg/logger/format.go
 */
package logger

import (
	"github.com/sirupsen/logrus"
)

func init() {
	logrus.SetFormatter(&logFormatter{
		JSONFormatter: &logrus.JSONFormatter{},
	})
}

type logFormatter struct {
	*logrus.JSONFormatter
}
