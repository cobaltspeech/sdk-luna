---
title: "Installation"
weight: 1
---

Instructions for installing the SDK are language-specific.

### Go
The Go SDK supports go modules.  We recommend using Go 1.11 or later.  You can download the sdk using:

`go get -u github.com/cobaltspeech/sdk-luna/grpc/go-luna`

Alternatively, if you just import `github.com/cobaltspeech/sdk-luna/grpc/go-luna` in your application and run `go mod tidy`, it will be fetched automatically.

### Python
The Python SDK supports installation via pip.  We recommend Python 3.5 or later, and pip 19.0 or later.  You can download the sdk using:

`pip install "git+https://github.com/cobaltspeech/sdk-luna#egg=luna&subdirectory=grpc/py-luna"`