---
title: "Protobuf Reference"
weight: 10
---

The Luna API is specified as a [proto file](https://github.com/cobaltspeech/sdk-luna/blob/master/grpc/luna.proto). This section of the documentation is auto-generated from the spec. It describes the data types and functions defined in the spec. The “messages” below correspond to the data structures to be used, and the “service” contains the methods that can be called. luna.proto Service: Luna Service that implements the Cobalt Luna Text-to-Speech API Method Name Request Type Response Type Description Version VersionRequest VersionResponse Queries the Version of the Server ListVoices ListVoicesRequest ListVoicesResponse Retrieves a list of available text-to-speech voices Synthesize SynthesizeRequest SynthesizeResponse Performs synchronous text-to-speech generation.

