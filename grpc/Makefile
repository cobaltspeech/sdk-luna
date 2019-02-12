# Copyright (2019) Cobalt Speech and Language Inc.

.PHONY: all go

all: go

GO_OUTDIR=go-luna/lunapb

go: ${GO_OUTDIR}/luna.pb.go

${GO_OUTDIR}/luna.pb.go: luna.proto
	mkdir -p ${GO_OUTDIR}
	protoc -I . -I /usr/include/google/protobuf/ --go_out=plugins=grpc:"${GO_OUTDIR}" $<
