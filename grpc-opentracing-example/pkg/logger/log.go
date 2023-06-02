/*
 * @Author: zzzzztw
 * @Date: 2023-06-02 15:57:59
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-02 16:01:30
 * @FilePath: /myLearning/grpc-opentracing-example/pkg/logger/log.go
 */
package logger

import (
	"context"

	"github.com/sirupsen/logrus"
)

type Logger interface {
	logrus.FieldLogger
}

func WithContext(ctx context.Context, module ...string) Logger {
	if len(module) > 0 {
		return logrus.StandardLogger().WithContext(ctx).WithField("module", module[0])
	}
	return logrus.StandardLogger().WithContext(ctx)
}
