protoc -I/usr/local/include -I. --go_out=. --go-grpc_out=. ./*.proto
mv ./go-opentracing-example/pb/*.go . && rm -rf go-opentracing-example