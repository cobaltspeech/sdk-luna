#!/usr/bin/env python3

""" luna-cli.py

Copyright (2019) Cobalt Speech and Language Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import argparse
import time
from cli_config import read_config_file
from luna.client import LunaClient
from luna import luna_pb2 as lunapb
from player import Player


def batch_synthesis(text, client, synth_config, player):
    """Run the batch synthesis method for Luna client (i.e., wait
    until all audio is generated before playing)"""

    start_time = time.time()

    # Call the synthesis method
    request = lunapb.SynthesizeRequest(config=synth_config, text=text)
    response = client.Synthesize(request)
    synth_duration = time.time() - start_time
    print("batch synthesis took {:f} seconds".format(synth_duration))

    # Play the result
    start_time = time.time()
    player.batch_play(response.audio)

    # Print some statistics
    play_duration = time.time() - start_time
    print("playback took {:f} seconds".format(play_duration))

    rtf = play_duration / synth_duration
    print("real time factor: {:f}\n".format(rtf))


def stream_synthesis(text, client, synth_config, player):
    """Run the streaming synthesis method for Luna client (i.e., play
    audio as it is generated)"""

    start_time = time.time()

    # Start the player application
    player.start()

    # Setup the synthesis stream
    request = lunapb.SynthesizeRequest(config=synth_config, text=text)
    stream = client.SynthesizeStream(request)

    # Play responses as they come
    first_response = True
    for response in stream:
        if first_response:
            first_response = False

            # Display some statistics for the first response
            time_to_first = time.time() - start_time
            print("time to first samples: {:f} seconds".format(time_to_first))

        player.push_audio(response.audio)

    # Close the stream and stop the player application
    player.stop()

    # Print how long the entire method took
    total_time = time.time() - start_time
    print("streaming synthesis took {:f} seconds\n".format(total_time))


if __name__ == "__main__":
    banner = "Luna CLI - Copyright (2019) Cobalt Speech and Language, Inc.\n\n"
    banner += "Luna is Cobalt's text-to-speech (TTS) engine.\n\n"

    # Get the config file from cli args
    parser = argparse.ArgumentParser(description=banner)
    parser.add_argument('-config', '--config',
                        action='store', default='config.toml', type=str)
    args = parser.parse_args()

    # Parse the config file
    config = read_config_file(args.config)

    # Print the banner
    print(banner)

    # Create the Luna client for TTS
    if config.server.insecure:
        # Create an insecure client (not recommended for production)
        client = LunaClient.with_insecure(config.server.address)
    else:
        client = LunaClient(config.server.address)

    # Print the Luna version
    response = client.Version()
    print(response.version)
    print("Connected to {}".format(config.server.address))

    # Print the available models
    print("Available voices:")
    response = client.ListVoices()
    for v in response.voices:
        print("  Name: {}  ID: {}".format(v.name, v.id))

    print("")

    # Setup the synthesis config with the desired voice and encoding
    synth_config = lunapb.SynthesizerConfig(
        voice_id="1", encoding=lunapb.SynthesizerConfig.RAW_LINEAR16)

    # Setup the playback application
    player = Player(config.playback)

    # Run the main loop
    print("Enter text to synthesize at the prompt. To exit, use Ctrl+D.\n")
    while True:
        try:
            # Display the prompt (with color using ANSI escape codes)
            # and get the user input
            text = input("\x1B[94mLuna> \x1B[0m")
            if config.streaming:
                stream_synthesis(text, client, synth_config, player)
            else:
                batch_synthesis(text, client, synth_config, player)

        except EOFError:
            break

        except Exception as err:
            print("synthesis error: {}".format(err))
            break

    print("\nExiting...")
