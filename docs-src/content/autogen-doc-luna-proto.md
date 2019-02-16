---
title: "API Specification"
draft: false
weight: 100
---

# API Specification

The Luna API is specified as a [proto file](https://github.com/cobaltspeech/sdk-luna/blob/master/grpc/luna.proto).
This section of the documentation is auto-generated from the spec.  It describes
the data types and functions defined in the spec. The "messages" below
correspond to the data structures to be used, and the "service" contains the
methods that can be called.




## luna.proto





### Service: Luna
Service that implements the Cobalt Luna Text-to-Speech API

| Method Name | Request Type | Response Type | Description |
| ----------- | ------------ | ------------- | ------------|
| Version | VersionRequest | VersionResponse | Queries the Version of the Server |
| ListVoices | ListVoicesRequest | ListVoicesResponse | Retrieves a list of available text-to-speech voices |
| Synthesize | SynthesizeRequest | SynthesizeResponse | Performs synchronous text-to-speech generation. |

 <!-- end services -->



### Message: ListVoicesRequest
The top-level message sent by the client for the `ListVoices` method.


This message is empty and has no fields.






### Message: ListVoicesResponse
The message sent by the server for the `ListVoices` method.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| voices | Voice | repeated | List of voices available for use that match the request. |







### Message: SynthesizeRequest
The top-level message sent by the client for the `Synthesize` method.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | SynthesizerConfig |  | Provides configuration for the text-to-speech engine. |
| text | string |  | The text to generate speech for. |







### Message: SynthesizeResponse
The message returned to the client by the `Synthesize` method.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| samples | float | repeated | Audio samples of the entire generated speech, in the 32-bit float encoding, single channel, sampled at 48KHz. |







### Message: SynthesizerConfig
Configuration for setting up the text-to-speech engine.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| voice_id | string |  |  |







### Message: VersionRequest
The top-level message sent by the client for the `Version` method.


This message is empty and has no fields.






### Message: VersionResponse
The message sent by the server for the `Version` method.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| version | string |  |  |







### Message: Voice
Description of a Luna Voice


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| id | string |  | Unique identifier of the voice. This identifier is used to choose the voice during a synthesis request, and is specified in the `SynthesizerConfig` message. |
| name | string |  | Name of the voice. This is a concise name describing the voice, and maybe presented to the end-user, for example, to help which voice to choose for their TTS task. |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->




## Scalar Value Types

| .proto Type | Notes | Go Type | Python Type |
| ----------- | ----- | ------- | ----------- |
| double |  | float64 | float |
| float |  | float32 | float |
| int32 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint32 instead. | int32 | int |
| int64 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint64 instead. | int64 | int/long |
| uint32 | Uses variable-length encoding. | uint32 | int/long |
| uint64 | Uses variable-length encoding. | uint64 | int/long |
| sint32 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s. | int32 | int |
| sint64 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s. | int64 | int/long |
| fixed32 | Always four bytes. More efficient than uint32 if values are often greater than 2^28. | uint32 | int |
| fixed64 | Always eight bytes. More efficient than uint64 if values are often greater than 2^56. | uint64 | int/long |
| sfixed32 | Always four bytes. | int32 | int |
| sfixed64 | Always eight bytes. | int64 | int/long |
| bool |  | bool | boolean |
| string | A string must always contain UTF-8 encoded or 7-bit ASCII text. | string | str/unicode |
| bytes | May contain any arbitrary sequence of bytes. | []byte | str |


