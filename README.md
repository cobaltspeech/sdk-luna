# SDK for Luna (Cobalt's TTS Engine)

This repository contains the SDK for Cobalt's Luna TTS Engine.

## Network API (using GRPC)

The `grpc` folder at the top level of this repository contains code for Luna's
GRPC API.  The `grpc/luna.proto` file is the authoritative service definition of
the API and is used for auto generating SDK code in multiple languages.


### Auto-generated code
The `grpc` folder contains auto-generated code in several languages.  In order
to generate the code again, you should run `cd grpc && make`.  Generated code is
checked in, and you must make sure it is up to date when you push commits to
this repository.

Code generation has the following dependencies:
  - The protobuf compiler itself.  On ubuntu, this package is `protobuf-compiler`.
  - The golang plugins:
    - `go get -u google.golang.org/grpc`
    - `go get -u github.com/golang/protobuf/protoc-gen-go`
  - The python plugins:
    - `python -m pip install grpcio`
    - `python -m pip install grpcio-tools`
  - The documentation generation plugin:
    - `go get -u github.com/pseudomuto/protoc-gen-doc/cmd/protoc-gen-doc`

### Generating Documentation
The documentation here is generated using the excellent static-site generator,
[Hugo](https://gohugo.io). The content is authored in the
`docs-src/content` folder. The hugo-generated static website should not be
committed back to this repository.

You can download the latest hugo binary from the [release
page](https://github.com/gohugoio/hugo/releases).  Version 0.69.0 or later is
recommended.

To generate the static documentation content, run:
```
# first make sure the generated code is up to date.  This also generates the latest auto-docs.
pushd grpc && make && popd

# then build the static documentation pages
pushd docs-src && hugo -d <path/to/build/dir> && popd
```
