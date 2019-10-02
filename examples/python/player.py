""" player.py

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

import subprocess


class Player(object):
    """Player represents the external playback executable
    with corresponding args
    """

    def __init__(self, audio_config):
        self.args = [audio_config.application]
        self.args.extend(audio_config.arg_list())
        self.process = None

    def batch_play(self, audio):
        """Starts the playback app, plays all the given audio,
        then closes the playback app. Audio should be binary data.
        """

        self.start()
        self.push_audio(audio)
        self.stop()

    def start(self):
        """Launches the external playback application."""

        # Ignore if we already started
        if self.process is not None:
            return

        # Start the subprocess
        self.process = subprocess.Popen(args=self.args, stdin=subprocess.PIPE)
        return

    def stop(self):
        """Stops the external playback application."""

        # Ignore if we already stopped
        if self.process is None:
            return

        # Stop the subprocess by closing stdin
        self.process.stdin.close()
        self.process.wait()
        self.process = None
        return

    def push_audio(self, audio):
        """Pushes audio data to the playback application. start() should
        be called prior to using this function. Audio should be binary data.
        """

        # Raise an error if we haven't started the app
        if self.process is None:
            raise RuntimeError("playback application is not running")

        self.process.stdin.write(audio)
