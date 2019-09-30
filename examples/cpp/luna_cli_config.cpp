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

#include "luna_cli_config.h"

#include <cpptoml.h>
#include <stdexcept>

LunaCLIConfig LunaCLIConfig::parseFile(const std::string &file)
{
    LunaCLIConfig config;

    // Parse the toml file
    std::shared_ptr<cpptoml::table> toml = cpptoml::parse_file(file);

    // Check whether the app should be streaming or not
    config.mStreaming = *(toml->get_qualified_as<bool>("Streaming"));

    // Get server values
    config.mLunaAddress =
        *(toml->get_qualified_as<std::string>("Server.Address"));
    if (config.mLunaAddress.empty())
    {
        throw std::runtime_error("missing Luna server url in config file");
    }

    config.mLunaInsecure = *(toml->get_qualified_as<bool>("Server.Insecure"));
    config.mVoiceID = *(toml->get_qualified_as<std::string>("Server.VoiceID"));
    if (config.mVoiceID.empty())
    {
        throw std::runtime_error("missing Luna voice ID in config file");
    }

    // Get the player values
    std::string playApp, playArgs;
    playApp = *(toml->get_qualified_as<std::string>("Playback.Application"));
    playArgs = *(toml->get_qualified_as<std::string>("Playback.Args"));
    if (playApp.empty())
    {
        throw std::runtime_error("missing playback application in config file");
    }

    config.mPlaybackCmd = playApp + " " + playArgs;

    return config;
}

LunaCLIConfig::LunaCLIConfig() {}
LunaCLIConfig::~LunaCLIConfig() {}

bool LunaCLIConfig::streaming() const { return mStreaming; }

const std::string &LunaCLIConfig::lunaServerAddress() const
{
    return mLunaAddress;
}

bool LunaCLIConfig::lunaServerInsecure() const { return mLunaInsecure; }

const std::string &LunaCLIConfig::voiceID() const { return mVoiceID; }

const std::string &LunaCLIConfig::playbackCmd() const { return mPlaybackCmd; }
