protoc -I/usr/local/include -I. --go_out=. --go-grpc_out=. ./*.proto
mv ./GrpcAndprotobuf/proto/*.go . && rm -rf GrpcAndprotobuf