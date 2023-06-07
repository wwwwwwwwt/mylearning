// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.28.1
// 	protoc        v3.12.4
// source: good.proto

package pb

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type Good struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Id    int64   `protobuf:"varint,1,opt,name=id,proto3" json:"id,omitempty"`
	Name  string  `protobuf:"bytes,2,opt,name=name,proto3" json:"name,omitempty"`
	Stoke int64   `protobuf:"varint,3,opt,name=stoke,proto3" json:"stoke,omitempty"` // 库存.
	Price float64 `protobuf:"fixed64,4,opt,name=price,proto3" json:"price,omitempty"`
}

func (x *Good) Reset() {
	*x = Good{}
	if protoimpl.UnsafeEnabled {
		mi := &file_good_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Good) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Good) ProtoMessage() {}

func (x *Good) ProtoReflect() protoreflect.Message {
	mi := &file_good_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Good.ProtoReflect.Descriptor instead.
func (*Good) Descriptor() ([]byte, []int) {
	return file_good_proto_rawDescGZIP(), []int{0}
}

func (x *Good) GetId() int64 {
	if x != nil {
		return x.Id
	}
	return 0
}

func (x *Good) GetName() string {
	if x != nil {
		return x.Name
	}
	return ""
}

func (x *Good) GetStoke() int64 {
	if x != nil {
		return x.Stoke
	}
	return 0
}

func (x *Good) GetPrice() float64 {
	if x != nil {
		return x.Price
	}
	return 0
}

type GetGoodsByIDsRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Ids []int64 `protobuf:"varint,1,rep,packed,name=ids,proto3" json:"ids,omitempty"`
}

func (x *GetGoodsByIDsRequest) Reset() {
	*x = GetGoodsByIDsRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_good_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *GetGoodsByIDsRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*GetGoodsByIDsRequest) ProtoMessage() {}

func (x *GetGoodsByIDsRequest) ProtoReflect() protoreflect.Message {
	mi := &file_good_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use GetGoodsByIDsRequest.ProtoReflect.Descriptor instead.
func (*GetGoodsByIDsRequest) Descriptor() ([]byte, []int) {
	return file_good_proto_rawDescGZIP(), []int{1}
}

func (x *GetGoodsByIDsRequest) GetIds() []int64 {
	if x != nil {
		return x.Ids
	}
	return nil
}

type GetGoodsByIDsResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Goods []*Good `protobuf:"bytes,1,rep,name=goods,proto3" json:"goods,omitempty"`
}

func (x *GetGoodsByIDsResponse) Reset() {
	*x = GetGoodsByIDsResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_good_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *GetGoodsByIDsResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*GetGoodsByIDsResponse) ProtoMessage() {}

func (x *GetGoodsByIDsResponse) ProtoReflect() protoreflect.Message {
	mi := &file_good_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use GetGoodsByIDsResponse.ProtoReflect.Descriptor instead.
func (*GetGoodsByIDsResponse) Descriptor() ([]byte, []int) {
	return file_good_proto_rawDescGZIP(), []int{2}
}

func (x *GetGoodsByIDsResponse) GetGoods() []*Good {
	if x != nil {
		return x.Goods
	}
	return nil
}

type StokeRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	GoodId int64 `protobuf:"varint,1,opt,name=goodId,proto3" json:"goodId,omitempty"` // 商品id
	Stoke  int64 `protobuf:"varint,2,opt,name=stoke,proto3" json:"stoke,omitempty"`   // 库存数量
}

func (x *StokeRequest) Reset() {
	*x = StokeRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_good_proto_msgTypes[3]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *StokeRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*StokeRequest) ProtoMessage() {}

func (x *StokeRequest) ProtoReflect() protoreflect.Message {
	mi := &file_good_proto_msgTypes[3]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use StokeRequest.ProtoReflect.Descriptor instead.
func (*StokeRequest) Descriptor() ([]byte, []int) {
	return file_good_proto_rawDescGZIP(), []int{3}
}

func (x *StokeRequest) GetGoodId() int64 {
	if x != nil {
		return x.GoodId
	}
	return 0
}

func (x *StokeRequest) GetStoke() int64 {
	if x != nil {
		return x.Stoke
	}
	return 0
}

type UpdateGoodsStokeRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Requests []*StokeRequest `protobuf:"bytes,1,rep,name=requests,proto3" json:"requests,omitempty"`
}

func (x *UpdateGoodsStokeRequest) Reset() {
	*x = UpdateGoodsStokeRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_good_proto_msgTypes[4]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *UpdateGoodsStokeRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*UpdateGoodsStokeRequest) ProtoMessage() {}

func (x *UpdateGoodsStokeRequest) ProtoReflect() protoreflect.Message {
	mi := &file_good_proto_msgTypes[4]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use UpdateGoodsStokeRequest.ProtoReflect.Descriptor instead.
func (*UpdateGoodsStokeRequest) Descriptor() ([]byte, []int) {
	return file_good_proto_rawDescGZIP(), []int{4}
}

func (x *UpdateGoodsStokeRequest) GetRequests() []*StokeRequest {
	if x != nil {
		return x.Requests
	}
	return nil
}

type UpdateGoodsStokeResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields
}

func (x *UpdateGoodsStokeResponse) Reset() {
	*x = UpdateGoodsStokeResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_good_proto_msgTypes[5]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *UpdateGoodsStokeResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*UpdateGoodsStokeResponse) ProtoMessage() {}

func (x *UpdateGoodsStokeResponse) ProtoReflect() protoreflect.Message {
	mi := &file_good_proto_msgTypes[5]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use UpdateGoodsStokeResponse.ProtoReflect.Descriptor instead.
func (*UpdateGoodsStokeResponse) Descriptor() ([]byte, []int) {
	return file_good_proto_rawDescGZIP(), []int{5}
}

var File_good_proto protoreflect.FileDescriptor

var file_good_proto_rawDesc = []byte{
	0x0a, 0x0a, 0x67, 0x6f, 0x6f, 0x64, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x05, 0x70, 0x72,
	0x6f, 0x74, 0x6f, 0x22, 0x56, 0x0a, 0x04, 0x47, 0x6f, 0x6f, 0x64, 0x12, 0x0e, 0x0a, 0x02, 0x69,
	0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x02, 0x69, 0x64, 0x12, 0x12, 0x0a, 0x04, 0x6e,
	0x61, 0x6d, 0x65, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09, 0x52, 0x04, 0x6e, 0x61, 0x6d, 0x65, 0x12,
	0x14, 0x0a, 0x05, 0x73, 0x74, 0x6f, 0x6b, 0x65, 0x18, 0x03, 0x20, 0x01, 0x28, 0x03, 0x52, 0x05,
	0x73, 0x74, 0x6f, 0x6b, 0x65, 0x12, 0x14, 0x0a, 0x05, 0x70, 0x72, 0x69, 0x63, 0x65, 0x18, 0x04,
	0x20, 0x01, 0x28, 0x01, 0x52, 0x05, 0x70, 0x72, 0x69, 0x63, 0x65, 0x22, 0x28, 0x0a, 0x14, 0x47,
	0x65, 0x74, 0x47, 0x6f, 0x6f, 0x64, 0x73, 0x42, 0x79, 0x49, 0x44, 0x73, 0x52, 0x65, 0x71, 0x75,
	0x65, 0x73, 0x74, 0x12, 0x10, 0x0a, 0x03, 0x69, 0x64, 0x73, 0x18, 0x01, 0x20, 0x03, 0x28, 0x03,
	0x52, 0x03, 0x69, 0x64, 0x73, 0x22, 0x3a, 0x0a, 0x15, 0x47, 0x65, 0x74, 0x47, 0x6f, 0x6f, 0x64,
	0x73, 0x42, 0x79, 0x49, 0x44, 0x73, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x12, 0x21,
	0x0a, 0x05, 0x67, 0x6f, 0x6f, 0x64, 0x73, 0x18, 0x01, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x0b, 0x2e,
	0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x47, 0x6f, 0x6f, 0x64, 0x52, 0x05, 0x67, 0x6f, 0x6f, 0x64,
	0x73, 0x22, 0x3c, 0x0a, 0x0c, 0x73, 0x74, 0x6f, 0x6b, 0x65, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73,
	0x74, 0x12, 0x16, 0x0a, 0x06, 0x67, 0x6f, 0x6f, 0x64, 0x49, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28,
	0x03, 0x52, 0x06, 0x67, 0x6f, 0x6f, 0x64, 0x49, 0x64, 0x12, 0x14, 0x0a, 0x05, 0x73, 0x74, 0x6f,
	0x6b, 0x65, 0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x05, 0x73, 0x74, 0x6f, 0x6b, 0x65, 0x22,
	0x4a, 0x0a, 0x17, 0x55, 0x70, 0x64, 0x61, 0x74, 0x65, 0x47, 0x6f, 0x6f, 0x64, 0x73, 0x53, 0x74,
	0x6f, 0x6b, 0x65, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x12, 0x2f, 0x0a, 0x08, 0x72, 0x65,
	0x71, 0x75, 0x65, 0x73, 0x74, 0x73, 0x18, 0x01, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x13, 0x2e, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x73, 0x74, 0x6f, 0x6b, 0x65, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73,
	0x74, 0x52, 0x08, 0x72, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x73, 0x22, 0x1a, 0x0a, 0x18, 0x55,
	0x70, 0x64, 0x61, 0x74, 0x65, 0x47, 0x6f, 0x6f, 0x64, 0x73, 0x53, 0x74, 0x6f, 0x6b, 0x65, 0x52,
	0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x32, 0xad, 0x01, 0x0a, 0x0b, 0x47, 0x6f, 0x6f, 0x64,
	0x53, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x12, 0x49, 0x0a, 0x0c, 0x47, 0x65, 0x74, 0x47, 0x6f,
	0x6f, 0x64, 0x73, 0x42, 0x79, 0x49, 0x44, 0x12, 0x1b, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2e,
	0x47, 0x65, 0x74, 0x47, 0x6f, 0x6f, 0x64, 0x73, 0x42, 0x79, 0x49, 0x44, 0x73, 0x52, 0x65, 0x71,
	0x75, 0x65, 0x73, 0x74, 0x1a, 0x1c, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x47, 0x65, 0x74,
	0x47, 0x6f, 0x6f, 0x64, 0x73, 0x42, 0x79, 0x49, 0x44, 0x73, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e,
	0x73, 0x65, 0x12, 0x53, 0x0a, 0x10, 0x55, 0x70, 0x64, 0x61, 0x74, 0x65, 0x47, 0x6f, 0x6f, 0x64,
	0x73, 0x53, 0x74, 0x6f, 0x6b, 0x65, 0x12, 0x1e, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x55,
	0x70, 0x64, 0x61, 0x74, 0x65, 0x47, 0x6f, 0x6f, 0x64, 0x73, 0x53, 0x74, 0x6f, 0x6b, 0x65, 0x52,
	0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x1a, 0x1f, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x55,
	0x70, 0x64, 0x61, 0x74, 0x65, 0x47, 0x6f, 0x6f, 0x64, 0x73, 0x53, 0x74, 0x6f, 0x6b, 0x65, 0x52,
	0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x42, 0x1b, 0x5a, 0x19, 0x67, 0x6f, 0x2d, 0x6f, 0x70,
	0x65, 0x6e, 0x74, 0x72, 0x61, 0x63, 0x69, 0x6e, 0x67, 0x2d, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c,
	0x65, 0x2f, 0x70, 0x62, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_good_proto_rawDescOnce sync.Once
	file_good_proto_rawDescData = file_good_proto_rawDesc
)

func file_good_proto_rawDescGZIP() []byte {
	file_good_proto_rawDescOnce.Do(func() {
		file_good_proto_rawDescData = protoimpl.X.CompressGZIP(file_good_proto_rawDescData)
	})
	return file_good_proto_rawDescData
}

var file_good_proto_msgTypes = make([]protoimpl.MessageInfo, 6)
var file_good_proto_goTypes = []interface{}{
	(*Good)(nil),                     // 0: proto.Good
	(*GetGoodsByIDsRequest)(nil),     // 1: proto.GetGoodsByIDsRequest
	(*GetGoodsByIDsResponse)(nil),    // 2: proto.GetGoodsByIDsResponse
	(*StokeRequest)(nil),             // 3: proto.stokeRequest
	(*UpdateGoodsStokeRequest)(nil),  // 4: proto.UpdateGoodsStokeRequest
	(*UpdateGoodsStokeResponse)(nil), // 5: proto.UpdateGoodsStokeResponse
}
var file_good_proto_depIdxs = []int32{
	0, // 0: proto.GetGoodsByIDsResponse.goods:type_name -> proto.Good
	3, // 1: proto.UpdateGoodsStokeRequest.requests:type_name -> proto.stokeRequest
	1, // 2: proto.GoodService.GetGoodsByID:input_type -> proto.GetGoodsByIDsRequest
	4, // 3: proto.GoodService.UpdateGoodsStoke:input_type -> proto.UpdateGoodsStokeRequest
	2, // 4: proto.GoodService.GetGoodsByID:output_type -> proto.GetGoodsByIDsResponse
	5, // 5: proto.GoodService.UpdateGoodsStoke:output_type -> proto.UpdateGoodsStokeResponse
	4, // [4:6] is the sub-list for method output_type
	2, // [2:4] is the sub-list for method input_type
	2, // [2:2] is the sub-list for extension type_name
	2, // [2:2] is the sub-list for extension extendee
	0, // [0:2] is the sub-list for field type_name
}

func init() { file_good_proto_init() }
func file_good_proto_init() {
	if File_good_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_good_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Good); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_good_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*GetGoodsByIDsRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_good_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*GetGoodsByIDsResponse); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_good_proto_msgTypes[3].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*StokeRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_good_proto_msgTypes[4].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*UpdateGoodsStokeRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_good_proto_msgTypes[5].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*UpdateGoodsStokeResponse); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_good_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   6,
			NumExtensions: 0,
			NumServices:   1,
		},
		GoTypes:           file_good_proto_goTypes,
		DependencyIndexes: file_good_proto_depIdxs,
		MessageInfos:      file_good_proto_msgTypes,
	}.Build()
	File_good_proto = out.File
	file_good_proto_rawDesc = nil
	file_good_proto_goTypes = nil
	file_good_proto_depIdxs = nil
}