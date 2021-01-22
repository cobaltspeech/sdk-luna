---
title: "Batch Synthesis"
description: "Describes how to synthesize audio in batch mode."
weight: 25
---

Batch synthesis generates audio data from text and waits for synthesis to
complete before returning any data to the client. Once synthesis has
completed, all of the generated audio data is returned. The
benefit of using this method is its simplicity - a client submits text
and receives all the generated audio for the text. However, the downsides
to this method include:

* Slower response time - must wait for synthesis to complete before
  receiving audio.
* Data packet size - if the text to synthesize is very large, the
  returned audio will also be very large. All that data must be
  transmitted over the network connection and stored in memory until
  the client application is finished using it.

Once generated, it is the client code’s responsibility to handle the returned
audio, whether it sends it to an output device (e.g., speakers), saves it to a
file, or simply discards it.


## Example
Assuming the synthesis configuration has been
[set up previously](../synthesis-config#example), here is an example
showing how to use the batch synthesis method:

{{< tabs >}}

{{< tab "Go" "go" >}}
// Make the synthesis request.
request := &lunapb.SynthesizeRequest{
	Config: synthConfig,
	Text:   "this is an awesome sentence",
}

response, err := client.Synthesize(context.Background(), request)

// Do something interesting with the returned audio.
fmt.Printf("Received %v bytes of audio.\n", len(response.Audio))
{{< /tab >}}

{{< tab "Python" "python" >}}
# Make the synthesis request.
text = "this is an awesome sentence"
request = SynthesizeRequest(config=synth_config, text=text)
response = client.Synthesize(request)

# Do something interesting with the returned audio.
print("Received {} bytes of audio.".format(len(response.audio)))
{{< /tab >}}

{{< tab "C++" "c++" >}}
// Make the synthesis request.
std::string text = "this is an awesome sentence";
ByteVector audio = client.synthesize(synthConfig, text);

// Do something interesting with the returned audio.
std::cout << "Received " << audio.size() << " bytes of audio." << std::endl;
{{< /tab >}}

{{< /tabs >}}
