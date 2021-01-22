---
title: "Streaming Synthesis"
description: "Describes how to stream synthesized audio as it is generated."
weight: 26
---

Unlike [batch synthesis](../batch), streaming synthesis begins to return the
generated audio data to the client as soon as it is ready, sending multiple
data packets until synthesis has completed. While this method is slightly
more complex than batch synthesis, it brings the following benefits:

* Faster response time - a client may begin to use data as soon as it receives.
  This allows a client to begin audio playback even before synthesis is
  complete.
* Smaller data packet size - because the audio is sent in multiple messages over
  the gRPC stream, each packet will carry less data than the batch method.
  * This makes it possible to submit larger sentences to Luna without
    running out of memory.
  * Optionally, a [buffer size](../synthesis-config/#buffer-size-optional)
    may be specified to give more control over the amount of data transmitted
    over the network connection at a time.

Once generated, it is the client code’s responsibility to handle the returned
audio, whether it sends it to an output device (e.g., speakers), saves it to a
file, or simply discards it.


## Example
Assuming the synthesis configuration has been
[set up previously](../synthesis-config/#example), here is an example
showing how to use the streaming synthesis method:

{{< tabs >}}

{{< tab "Go" "go" >}}
// Create the synthesis stream
request := &lunapb.SynthesizeRequest{
	Config: synthConfig,
	Text:   "this is an even better sentence",
}

stream, err := client.SynthesizeStream(context.Background(), request)

// Wait for audio data from the server until the stream is closed, which
// will happen when synthesis is complete.
for {
	// Get the next response from the server
	response, err := stream.Recv()

	// This indicates the stream has finished.
	if err == io.EOF {
		break
	}
	if err != nil {
		return err
	}

	// Do something interesting with the audio
	fmt.Printf("Received %v bytes of audio.\n", len(response.Audio))
}

fmt.Printf("Synthesis complete.\n")
{{< /tab >}}

{{< tab "Python" "python" >}}
# Create the synthesis stream
text = "this is an even better sentence"
request = SynthesizeRequest(config=synth_config, text=text)
stream = client.SynthesizeStream(request)

# Wait for audio data from the server until the stream is closed, which
# will happen when synthesis is complete.
for response in stream:
    # Do something interesting with the audio
    print("Received {} bytes of audio.".format(len(response.audio)))

print("Synthesis complete.")
{{< /tab >}}

{{< tab "C++" "c++" >}}
// Create the synthesis stream
std::string text = "this is an even better sentence";
LunaSynthesizerStream stream = client.synthesizeStream(synthConfig, text);

// Wait for audio data from the server until the stream is closed, which
// will happen when synthesis is complete.
ByteVector audio;
while(stream.receiveAudio(audio))
{
    // Do something interesting with the audio
    std::cout << "Received " << audio.size() << " bytes of audio." << std::endl;
}

stream.close();
std::cout << "Synthesis complete." << std::endl;
{{< /tab >}}

{{< /tabs >}}
