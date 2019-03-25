---
title: "Cobalt's Luna SDK Documentation"
date: 2019-02-13T12:56:41+05:30
draft: false
weight: 10
---

# Luna SDK
Luna is Cobalt's text-to-speech engine.  It can be bundled into your application as a library, or deployed on-prem and accessed over the network.  This documentation refers to accessing the network-based luna server.

Cobalt will provide you with a package of Luna that contains the engine, your voice objects, and a server application.  This server exports Luna's functionality over the gRPC protocol.  The https://github.com/cobaltspeech/luna-sdk repository contains the SDK that you can use in your application to communicate with the Luna server.  This SDK is currently available for Go and Python languages; and we would be happy to talk to you if you need support for other languages.  Most of the core SDK is generated automatically using the gRPC tools, and Cobalt provides a top level package for more convenient API calls.
