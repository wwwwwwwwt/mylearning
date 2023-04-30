/*
 * @Author: zzzzztw
 * @Date: 2023-04-29 20:35:06
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-29 20:43:57
 * @FilePath: /myLearning/test.go
 */
package main

import (
	"log"
	"reflect"
	"strings"
	"sync"
)

func main() {
	var wg sync.WaitGroup

	typ := reflect.TypeOf(&wg)

	for i := 0; i < typ.NumMethod(); i++ {

		method := typ.Method(i)
		argv := make([]string, 0, method.Type.NumIn())
		returns := make([]string, 0, method.Type.NumOut())

		for j := 1; j < method.Type.NumIn(); j++ {
			argv = append(argv, method.Type.In(j).Name())
		}

		for j := 0; j < method.Type.NumOut(); j++ {
			returns = append(returns, method.Type.Out(j).Name())
		}

		log.Printf("func (w %s)%s(%s)%s", typ.Elem().Name(), method.Name, strings.Join(argv, ","), strings.Join(returns, ","))
	}
}
