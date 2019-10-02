""" cli_config.py

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

import toml


class ServerConfig(object):
    """Stores Luna server settings."""

    def __init__(self, server_dict):
        self.address = server_dict['Address']
        self.insecure = server_dict['Insecure']
        self.voiceID = server_dict['VoiceID']


class AudioConfig(object):
    """Stores settings for the external playback application"""

    def __init__(self, audio_dict):
        self.application = audio_dict['Application']
        self.args = audio_dict['Args']

    def arg_list(self):
        """Return the application's args as a list of strings"""

        return self.args.split()


class LunaCLIConfig(object):
    """Stores settings for the luna-cli application"""

    def __init__(self, toml_dict):
        self.streaming = toml_dict['Streaming']
        self.server = ServerConfig(toml_dict['Server'])
        self.playback = AudioConfig(toml_dict['Playback'])


def read_config_file(filename):
    """Read the given config file (TOML format) and return a new 
    LunaCLIConfig object."""

    toml_dict = toml.load(filename)
    return LunaCLIConfig(toml_dict)
