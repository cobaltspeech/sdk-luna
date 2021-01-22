---
title: "Including the SDK"
description: "Gives language-specific instructions about how to add the SDK to your project."
weight: 21
---

Language-specific instructions to include the SDK in your project
are given below.


## C++

To use the Luna SDK in a C++ project, you must first download the SDK from
[GitHub](https://github.com/cobaltspeech/sdk-luna).
To help simplify the build process, the C++ SDK uses
[CMake](http://www.cmake.org), although it is possible to use a different
build system with some additional work. Details for building and including
the SDK in a C++ project are described in detail in
[this README file](https://github.com/cobaltspeech/sdk-luna/blob/master/grpc/cpp-luna/README.md).


## Go
The Go SDK supports Go modules and requires Go 1.14 or later. To use the
SDK, import this package into your application:

``` go
import (
	"github.com/cobaltspeech/sdk-luna/grpc/go-luna"
	"github.com/cobaltspeech/sdk-luna/grpc/go-luna/lunapb"
)
```

{{% panel theme="info" header="Note" %}}
When using Go, it is not necessary to download the SDK using git
(unless you want to build the example code).
The `go build` command will automatically fetch the sdk-luna code
from GitHub to use in your Go project.
{{% /panel %}}


## Python
The Python SDK requires Python v3.5.0 or greater. The SDK may be installed
from GitHub using `pip`:

```bash
pip install "git+https://github.com/cobaltspeech/sdk-luna#egg=cobalt-luna&subdirectory=grpc/py-luna"
```

Once installed, simply import the Luna module to use the SDK:

```python
import luna

# Or import the required classes
from luna.client import LunaClient
from luna.luna_pb2 import SynthesizeRequest, SynthesizerConfig
```
