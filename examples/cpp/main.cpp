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
#include "player.h"
#include "timer.h"

#include <exception>
#include <iostream>
#include <luna_client.h>
#include <string>

// Prints the help message
void printHelp(const char *appName)
{
    std::cout << "USAGE: " << appName << " [-config <path>][--help]\n\n";

    std::cout << "  -config <path> Path to the configuration file.\n"
                 "                 (Default=\"config.toml\")\n"
                 "  --help         show this help message\n\n"
              << std::endl;
}

/*
 * Parse the command line arguments to find the config file name. Also
 * checks if any other args were present (like --help). Returns true if
 * parsing was successful and false if the application should exit.
 */
bool parseCLIArgs(int argc, char *argv[], std::string &configFile)
{
    if (argc == 1)
    {
        // No arguments were specified. Return a default config filename.
        configFile = "config.toml";
        return true;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            printHelp(argv[0]);
            return false;
        }
        else if (strcmp(argv[i], "-config") == 0)
        {
            // TODO: Better arg parsing that will also check that the file
            // has been specified
            // Get the config file path.
            i++;
            configFile.assign(argv[i]);
        }
    }

    return true;
}

void synthesizeBatch(const std::string &userInput, LunaClient &client,
                     const cobaltspeech::luna::SynthesizerConfig &synthConfig,
                     const std::string &playbackCmd)
{
    Player player(playbackCmd);
    Timer timer;

    // Call the batch synthesis method
    ByteVector audio = client.synthesize(synthConfig, userInput);

    double synthDuration = timer.elapsed();
    std::cout << "batch synthesis took " << synthDuration << " seconds."
              << std::endl;

    // Play the result
    timer.restart();
    player.batchPlay(audio);

    // Print how long this method took
    double playDuration = timer.elapsed();
    std::cout << "playback took " << playDuration << " seconds.\n";

    double rtf = playDuration / synthDuration;
    std::cout << "real time factor: " << rtf << "\n" << std::endl;
}

void synthesizeStream(const std::string &userInput, LunaClient &client,
                      const cobaltspeech::luna::SynthesizerConfig &synthConfig,
                      const std::string &playbackCmd)
{
    Player player(playbackCmd);
    Timer timer;

    // Start the player application
    player.start();

    // Setup the synthesis stream
    LunaSynthesizerStream stream =
        client.synthesizeStream(synthConfig, userInput);

    // Write binary data as it comes from the stream to the player
    bool firstResponse = true;
    ByteVector audio;
    while (stream.receiveAudio(audio))
    {
        if (firstResponse)
        {
            firstResponse = false;

            // Display some statistics for the first response
            double timeToFirst = timer.elapsed();
            std::cout << "time to first samples: " << timeToFirst << " seconds"
                      << std::endl;
        }

        // Play the current samples
        player.pushAudio(audio);
    }
    stream.close();

    // Stop the player application
    player.stop();

    // Print how long this method took
    std::cout << "streaming synthesis took " << timer.elapsed() << " seconds.\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    // Print the banner
    std::cout << "Luna CLI - "
              << "Copyright (2019) Cobalt Speech and Language, Inc.\n\n";
    std::cout << "Luna is Cobalt's text-to-speech (TTS) engine.\n\n";
    std::cout << std::flush;

    // Parse the config file
    std::string configFile;
    if (!parseCLIArgs(argc, argv, configFile))
    {
        return 0;
    }

    LunaCLIConfig config;
    try
    {
        config = LunaCLIConfig::parseFile(configFile);
    }
    catch (const std::exception &err)
    {
        std::cerr << "error parsing config file: " << err.what() << std::endl;
        return 1;
    }

    // Setup the Luna client
    LunaClient client(config.lunaServerAddress(), !config.lunaServerInsecure());

    // Get the Luna server version
    std::cout << client.lunaVersion() << std::endl;
    std::cout << "Connected to " << config.lunaServerAddress() << std::endl;

    // Get the list of available voice models
    std::cout << "Available voices:\n";
    std::vector<LunaVoice> voices = client.listVoices();
    for (const LunaVoice &v : voices)
    {
        std::cout << "  ID: " << v.id() << "  Name: " << v.name()
                  << "  Sample Rate(Hz): " << v.sampleRate()
                  << "  Language: " << v.language() << "\n";
    }
    std::cout << std::endl;

    // Setup the synthesis config with the desired voice and encoding
    cobaltspeech::luna::SynthesizerConfig synthConfig;
    synthConfig.set_voice_id(config.voiceID());
    synthConfig.set_encoding(
        cobaltspeech::luna::SynthesizerConfig::RAW_LINEAR16);

    // Start the main loop
    std::cout << "Enter text to synthesize at the prompt. ";
    std::cout << "To exit, use Ctrl+D.\n" << std::endl;
    while (true)
    {
        // Display the prompt (with color using ANSI escape codes)
        std::cout << "\x1B[94m"
                  << "Luna> "
                  << "\x1B[0m" << std::flush;

        // Get the text to synthesize from the user
        std::string userInput;
        std::getline(std::cin, userInput);
        if (std::cin.eof())
        {
            std::cout << "\nExiting..." << std::endl;
            break;
        }

        // Synthesize the text
        if (config.streaming())
        {
            synthesizeStream(userInput, client, synthConfig,
                             config.playbackCmd());
        }
        else
        {
            synthesizeBatch(userInput, client, synthConfig,
                            config.playbackCmd());
        }
    }

    return 0;
}
