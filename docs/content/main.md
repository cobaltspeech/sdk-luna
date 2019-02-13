---
title: "Cobalt's Luna SDK Documentation"
date: 2019-02-13T12:56:41+05:30
draft: false
weight: 10
---

# Luna SDK
Luna is Cobalt's text-to-speech engine.  It can be bundled into your application as a library, or deployed on-prem and accessed over the network.  This documentation refers to accessing the network-based luna server.

Cobalt will provide you with a package of Luna that contains the engine, your voice objects, and a server application.  This server exports Luna's functionality over the gRPC protocol.  The https://github.com/cobaltspeech/luna-sdk repository contains the SDK that you can use in your application to communicate with the Luna server.  This SDK is currently available for Go and Python languages; and we would be happy to talk to you if you need support for other languages.  Most of the core SDK is generated automatically using the gRPC tools, and Cobalt provides a top level package for more convenient API calls.

# Installation
Instructions for installing the SDK are language-specific.

### Go
The Go SDK supports go modules.  We recommend using Go 1.11 or later.  You can download the sdk using:

`go get -u github.com/cobaltspeech/sdk-luna/grpc/go-luna`

Alternatively, if you just import `github.com/cobaltspeech/sdk-luna/grpc/go-luna` in your application and run `go mod tidy`, it will be fetched automatically.

### Python
The Python SDK supports installation via pip.  We recommend Python 3.5 or later, and pip 19.0 or later.  You can download the sdk using:

`pip install "git+https://github.com/cobaltspeech/sdk-luna#egg=luna&subdirectory=grpc/py-luna"`

# Quickstart

Once you have your Luna server up and running, let's see how we can ask it to synthesize speech for some text!

First, you need to know the address (`host`:`port`) where the server is running.  This document will assume the values `127.0.0.1:2727`, but be sure to change those to point to your server instance.  Port 2727 is the default port that the luna server binds to.

The following code snippet queries the server for a list of available voices, and uses the first listed voice to run text to speech.

``` go
package main

import (
	"context"
	"fmt"

	"github.com/cobaltspeech/sdk-luna/grpc/go-luna"
	"github.com/cobaltspeech/sdk-luna/grpc/go-luna/lunapb"
)

const serverAddress = "127.0.0.1:2727"

func main() {
	// Create a Luna Client
	client, err := luna.NewClient(serverAddress)
	if err != nil {
		// TODO: handle err
	}

	// Clean up at the end
	defer client.Close()

	// Get list of available voices
	pbvoices, err := client.ListVoices(context.Background())
	if err != nil {
		// TODO: handle err
	}

	voices := pbvoices.Voices
	if len(voices) == 0 {
		// TODO: handle err
	}

	// Choose the first voice
	v := voices[0]

	// Request TTS
	pbtts, err := client.Synthesize(
		context.Background(),
		&lunapb.SynthesizeRequest{
			Config: &lunapb.SynthesizerConfig{
				VoiceId: v.Id,
			},
			Text: "hello world!",
		})
	if err != nil {
		// TODO: handle err
	}

	// The generated speech is 32-bit float samples, at 48k samples per second.
	// Do something with the samples.
	fmt.Printf("Received %d samples\n", len(pbtts.Samples))

}

```

``` python
from luna.client import LunaClient
from luna import luna_pb2 as lunapb

SERVER_ADDRESS = "127.0.0.1:2727"

try:
    # Create a Luna Client
    client  = LunaClient(SERVER_ADDRESS)

    # Retrieve a list of available voices
    pbvoices = client.ListVoices()

    # Choose the first voice for synthesis
    v = pbvoice.voices[0]

    # Request TTS
    pbtts = client.Synthesize(
        lunapb.SynthesizeRequest(
            config=lunapb.SynthesizerConfig(voice_id=v.id),
            text="hello world!"))

    #  The generated speech is 32-bit float samples, at 48k samples per second.
    # Do something with the samples.
    print("Received %d samples" % len(pbtts.samples))
except Exception as err:
    # TODO: handle exceptions
    print(err)

```

# Transport Layer Security

Cobalt recommends that your luna server deployment in production be configured with TLS.  The default setting for grpc clients to connect to the server uses TLS and validates the server's SSL certificate before transmitting data to it.

It may sometimes be required to disable TLS in the gRPC setup: perhaps for testing, or perhaps for special deployment needs.  If the server is not using TLS, the client setup needs to be changed in order for the connection to succeed. The code samples in this section show how to create the luna client for connections without TLS.

``` go
package main

import (
	"github.com/cobaltspeech/sdk-luna/grpc/go-luna"
)

const serverAddress = "127.0.0.1:2727"

func main() {
	// Create a Luna Client with TLS disabled
	client, err := luna.NewClientWithInsecure(serverAddress)

	if err != nil {
		// TODO: handle err
	}

	// ...
}

```

``` python
from luna.client import LunaClient

SERVER_ADDRESS = "127.0.0.1:2727"

# create a luna client with TLS disabled
client = LunaClient.with_insecure(SERVER_ADDRESS)
```


# Mutually Authenticated TLS

In our recommended default setup, TLS is enabled in the gRPC setup, and when connecting to the server, clients check the server's SSL certificate to verify that they are talking to an authentic server.  This is similar to how "https" connections work in web browsers.

It may sometimes be required that both the client and server need to authenticate with each other when establishing the connection.  This is possible by configuring the luna client with a valid certificate file, which it presents to the luna server when negotiating TLS connection.  The server validates the certificate and only allows authentic clients to send requests to it.

Please contact us to discuss more details of this setup -- whether to use it and how to configure it.

``` go
package main

import (
	"github.com/cobaltspeech/sdk-luna/grpc/go-luna"
)

const serverAddress = "127.0.0.1:2727"

func main() {
	// Create a Luna Client with specific SSL credentials
	client, err := luna.NewClientWithCertFile(serverAddress, "/path/to/cert.pem")

	if err != nil {
		// TODO: handle err
	}

	// ...
}

```

``` python
from luna.client import LunaClient

SERVER_ADDRESS = "127.0.0.1:2727"

# create a luna client with specific SSL credentials
client = LunaClient.with_cert_file(SERVER_ADDRESS, "/path/to/cert.pem")
```
