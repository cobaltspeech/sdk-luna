---
title: "Mutually Authenticated TLS"
weight: 20
---

In our recommended default setup, TLS is enabled in the gRPC setup, and when connecting to the server, clients check the server's SSL certificate to verify that they are talking to an authentic server.  This is similar to how "https" connections work in web browsers.

It may sometimes be required that both the client and server need to authenticate with each other when establishing the connection.  This is possible by configuring the luna client with a valid certificate file, which it presents to the luna server when negotiating TLS connection.  The server validates the certificate and only allows authentic clients to send requests to it.

Please contact us to discuss more details of this setup -- whether to use it and how to configure it.

{{%tabs %}}
{{% tab "Go" %}}
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
{{% /tab %}}
{{% tab "Python" %}}
``` python
from luna.client import LunaClient

SERVER_ADDRESS = "127.0.0.1:2727"

# create a luna client with specific SSL credentials
client = LunaClient.with_cert_file(SERVER_ADDRESS, "/path/to/cert.pem")
```
{{% /tab %}}
{{%/tabs %}}
