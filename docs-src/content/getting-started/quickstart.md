---
title: "Quickstart"
weight: 10
---

Once you have your Luna server up and running, let's see how we can ask it to synthesize speech for some text!

First, you need to know the address (`host`:`port`) where the server is running.  This document will assume the values `127.0.0.1:2727`, but be sure to change those to point to your server instance.  Port 2727 is the default port that the luna server binds to.

The following code snippet queries the server for a list of available voices, and uses the first listed voice to run text to speech.

{{%tabs %}}
{{% tab "Go" %}}

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
{{% /tab %}}
{{% tab "Python" %}}
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
{{% /tab %}}
{{%/tabs %}}

