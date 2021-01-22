---
title: "Luna SDK Documentation"
---

# Luna API Overview
Luna is Cobalt’s [text to speech](https://en.wikipedia.org/wiki/Speech_synthesis)
(TTS) engine. It can be deployed on-prem and accessed over the network or on your
local machine via an API. We currently support Go, Python, and C++ and are adding 
support for more languages.

From the API's perspective, the process of generating audio data is fairly
straightforward. After establishing a connection with the Luna server, a client
submits text to Luna, and Luna returns synthesized audio data that mimics human
speech.

For information about installing and running the Luna server, see the
[Getting Started](./getting-started/) section. For details about using the SDK,
see [Using Luna SDK](./using-luna-sdk/).

