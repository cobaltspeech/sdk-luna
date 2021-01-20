module github.com/cobaltspeech/sdk-luna/examples/go

go 1.12

require (
	github.com/BurntSushi/toml v0.3.1
	github.com/c-bata/go-prompt v0.2.3
	github.com/cobaltspeech/sdk-luna/grpc/go-luna v0.0.0-00010101000000-000000000000
	github.com/mattn/go-colorable v0.1.8 // indirect
	github.com/mattn/go-tty v0.0.3 // indirect
	github.com/pkg/term v0.0.0-20190109203006-aa71e9d9e942 // indirect
)

replace github.com/cobaltspeech/sdk-luna/grpc/go-luna => ../../grpc/go-luna
