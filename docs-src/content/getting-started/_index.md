---
title: "Getting Started"
weight: 1
---

## Installing the Luna Server Image

The SDK communicates with a Luna server instance using [gRPC](https://grpc.io/).
Cobalt distributes a Docker image that contains the Luna server binary and
model files.

1. Contact Cobalt to get a link to the image file in AWS S3. This link will
   expire in two weeks, so be sure to download the file to your own server.

2. Download with the AWS CLI if you have it, or with curl:

    ```bash
    URL="the url sent by Cobalt"
    IMAGE_NAME="name you want to give the file (should end with the same extension as the url, usually bz2)"
    curl $URL -L -o $IMAGE_NAME
    ```

3. Load the docker image

    ```bash
    docker load < $IMAGE_NAME
    ```

    This will output the name of the image (e.g. `luna-demo-en_us-16`).

4. Start the Luna service

    ```bash
    docker run -p 9001:9001 --name cobalt luna-demo-en_us-16
    ```

    That will start listening for grpc commands on port 9001. You can replace
    `--name cobalt` with any name you want to give the docker container.
    The name simply provides a way to refer back to the running container.


## Client Example
The sdk-luna repo contains [example code](https://github.com/cobaltspeech/sdk-luna/tree/master/examples)
with subdirectories for various languages to build a demo client application.

The demo client creates a command-line interface that allows a user to submit
text to Luna and receive back the synthesized audio. The client applications
require a [config file](https://github.com/cobaltspeech/sdk-luna/tree/master/examples/cli_config.toml.sample)
to specify the server information and an audio playback application. The 
playback application can be anything so long as it accepts formatted audio
data from stdin. To run the application, do the following:

{{< tabs >}}

{{< tab "Go" "bash" >}}
# After building via the `go build` command, run the CLI
cd /path/to/sdk-luna/examples/go
./luna-cli -config /path/to/config.toml
{{< /tab >}}

{{< tab "Python" "bash" >}}
# After setting up an environment to use the Luna module, run the CLI
cd /path/to/sdk-luna/examples/python
python3 luna-cli.py -config /path/to/config.toml
{{< /tab >}}

{{< tab "C++" "bash" >}}
# After building the example
cd /path/to/build/directory
./luna-cli -config /path/to/config.toml
{{< /tab >}}

{{< /tabs >}}

Once the application is running, simply enter the text to synthesize
at the prompt.
