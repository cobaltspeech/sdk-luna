# Copyright (2019) Cobalt Speech and Language Inc.

.PHONY: all go py docs

all: go py docs

GO_OUTDIR=go-luna/lunapb
PY_OUTDIR=py-luna/luna
DOCS_OUTDIR=../docs/content

go: ${GO_OUTDIR}/luna.pb.go
py: ${PY_OUTDIR}/luna_pb2.py ${PY_OUTDIR}/luna_pb2_grpc.py
docs: ${DOCS_OUTDIR}/autogen-doc-luna-proto.md

${GO_OUTDIR}/luna.pb.go: luna.proto
	mkdir -p ${GO_OUTDIR}
	protoc -I . -I /usr/include/google/protobuf/ --go_out=plugins=grpc:"${GO_OUTDIR}" $<

${PY_OUTDIR}/luna_pb2.py: luna.proto
	mkdir -p ${PY_OUTDIR}
	python -m grpc_tools.protoc -I. --python_out=${PY_OUTDIR} $<

${PY_OUTDIR}/luna_pb2_grpc.py: luna.proto
	mkdir -p ${PY_OUTDIR}
	python -m grpc_tools.protoc -I. --grpc_python_out=${PY_OUTDIR} $<

${DOCS_OUTDIR}/autogen-doc-luna-proto.md: luna.proto doc.md.tmpl
	protoc --doc_out=${DOCS_OUTDIR} --doc_opt=doc.md.tmpl,autogen-doc-luna-proto.md luna.proto
