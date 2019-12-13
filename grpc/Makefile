# Copyright (2019) Cobalt Speech and Language Inc.

.PHONY: all go py gendoc

all: go py gendoc

ifeq ($(PROTOINC),)
$(error "env variable PROTOINC must be defined to the location of googleapis proto files")
endif

GO_OUTDIR=go-luna/lunapb
PY_OUTDIR=py-luna/luna
DOCS_OUTDIR=../docs-src/content/protobuf

go: ${GO_OUTDIR}/luna.pb.go
py: ${PY_OUTDIR}/luna_pb2.py ${PY_OUTDIR}/luna_pb2_grpc.py

${GO_OUTDIR}/luna.pb.go: luna.proto
	mkdir -p ${GO_OUTDIR}
	protoc -I . -I /usr/include/google/protobuf/ --go_out=plugins=grpc:"${GO_OUTDIR}" $<

${PY_OUTDIR}/luna_pb2.py: luna.proto
	mkdir -p ${PY_OUTDIR}
	python -m grpc_tools.protoc -I. --python_out=${PY_OUTDIR} $<

${PY_OUTDIR}/luna_pb2_grpc.py: luna.proto
	mkdir -p ${PY_OUTDIR}
	python -m grpc_tools.protoc -I. --grpc_python_out=${PY_OUTDIR} $<

gendoc:
	protoc \
		-I . \
		-I ${PROTOINC} \
		--doc_out=${DOCS_OUTDIR} \
		--doc_opt=doc.md.tmpl,${DOCS_OUTDIR}/_index.md \
		luna.proto

	# Remove any TODO items from the generated doc
	sed -i 's/TODO.*//g' ${DOCS_OUTDIR}/_index.md
