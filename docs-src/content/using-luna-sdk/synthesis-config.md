---
title: "Synthesis Configuration"
description: "Specify the TTS configuration for speech synthesis."
weight: 24
---

Before we can run text-to-speech, the client needs to specify the
configuration Luna should use to generate the audio. The Luna options
include the specific voice model, the audio encoding, and the sample
size.

## Voice ID
Voice models are defined in the Luna server config file, and the client
can get a list of available voice models using the `ListVoices` method.

{{% tabs %}}

{{% tab "Go" %}}
``` go
// Get the list of voices from Luna
response, err := client.ListVoices(context.Background())

// Display the list
fmt.Printf("Available voices:\n")
for _, voice := range response.Voices {
    fmt.Printf("  Name: %s  ID: %s\n", voice.Name, voice.Id)
}
```
{{% /tab %}}

{{% tab "Python" %}}
``` python
# Get the list of voices from Luna
response = client.ListVoices()

# Display the list
print("Available voices:")
for v in response.voices:
    print("  Name: {}  ID:  {}".format(v.name, v.id))
```
{{% /tab %}}

{{% tab "C++" %}}
``` c++
// Get the list of voices from Luna
std::vector<LunaVoice> voices = client.listVoices();

// Display the list
std::cout << "Available voices:" << std::endl;
for (const LunaVoice &v : voices)
{
    std::cout << "  Name: " << v.name() << "  ID: " << v.id() << std::endl;
}
```
{{% /tab %}}

{{% /tabs %}}

The API requires the voice ID to be specified as part of the TTS synthesis
request. The voice ID for each model is defined in the server's config file.


## Audio Encoding
The API requires that the audio encoding also be specified as part of the
TTS synthesis request. The supported encodings are listed in the
[Luna API Reference](../../protobuf/#enum-synthesizerconfig-audioencoding).

{{% panel theme="info" header="Note" %}}
Audio encoding does not affect the sample rate of the generated audio.
Sample rate is an intrisic part of the Luna voice model being used.
Make sure your application uses a sample rate that matches the model.
If you are using a custom model, be sure to inform Cobalt of your desired
sample rate.
{{% /panel %}}


## Buffer Size (Optional)
When running [streaming synthesis](../streaming), a client may optionally
specify a buffer size. If set (and not zero), the server will wait until
the buffer is full before sending the audio data to the client. In the
case where the entire generated audio is less than the buffer size, the
samples will be returned when synthesis is complete. If the buffer size
is not set, audio data will be returned as soon as it becomes available
on the server.


## Example
Below is an example showing how client code should set up the synthesis
config.

{{% tabs %}}

{{% tab "Go" %}}
``` go
// Set up the synthesis config with the desired voice and encoding
synthConfig := &lunapb.SynthesizerConfig{
    VoiceId: "1", // As defined in the Luna server config file
    Encoding: lunapb.SynthesizerConfig_RAW_LINEAR16,

    // If setting the buffer size for streaming, include this param
    // with the application's desired buffer size.
    NSamples: 8096,
}
```
{{% /tab %}}

{{% tab "Python" %}}
``` python
# Set up the synthesis config with the desired voice and encoding
synth_config = SynthesizerConfig(
    voice_id="1", # As defined in the Luna server config file
    encoding=SynthesizerConfig.RAW_LINEAR16,
    
    # If setting the buffer size for streaming, include this arg with
    # the application's desired buffer size.
    n_samples=8096)
```
{{% /tab %}}

{{% tab "C++" %}}
``` c++
// Set up the synthesis config with the desired voice and encoding
cobaltspeech::luna::SynthesizerConfig synthConfig;
synthConfig.set_voice_id("1"); // As defined in the Luna server config file
synthConfig.set_encoding(cobaltspeech::luna::SynthesizerConfig::RAW_LINEAR16);

// If setting the buffer size for streaming, include this line with the
// application's desired buffer size.
synthConfig.set_n_samples(8096);
```
{{% /tab %}}

{{% /tabs %}}
