/*
 * Copyright (2019) Cobalt Speech and Language, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LUNA_CLI_CONFIG_H
#define LUNA_CLI_CONFIG_H

#include <string>

/*
 * The LunaCLIConfig class represents the configuration specified
 * in the user defined .toml file.
 */
class LunaCLIConfig
{
public:
    // Create a new configuration from the given file
    static LunaCLIConfig parseFile(const std::string &file);

    LunaCLIConfig();
    ~LunaCLIConfig();

    // Returns true if the streaming API should be used
    bool streaming() const;

    // Returns the url of the Luna server (with port number)
    const std::string &lunaServerAddress() const;

    // Returns whether the Luna server connection is insecure
    bool lunaServerInsecure() const;

    // Returns the Voice ID to use on the Luna Server
    const std::string &voiceID() const;

    // Returns the playback command (with args)
    const std::string &playbackCmd() const;

private:
    bool mStreaming;
    std::string mLunaAddress;
    bool mLunaInsecure;
    std::string mVoiceID;
    std::string mPlaybackCmd;
};

#endif // LUNA_CLI_CONFIG_H
