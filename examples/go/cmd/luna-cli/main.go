// Copyright (2019) Cobalt Speech and Language Inc.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package main

import (
	"context"
	"flag"
	"fmt"
	"io"
	"os"
	"strings"
	"time"

	"github.com/c-bata/go-prompt"
	"github.com/cobaltspeech/sdk-luna/grpc/go-luna"
	"github.com/cobaltspeech/sdk-luna/grpc/go-luna/lunapb"
)

func main() {
	// Print the banner
	fmt.Printf("Luna CLI - Copyright (2019) Cobalt Speech and Language, Inc.\n\n")
	fmt.Printf("Luna is Cobalt's text-to-speech (TTS) engine.\n\n")

	// Read the config file
	configFile := flag.String("config", "config.toml", "Path to the config file")
	flag.Parse()
	config, err := ReadConfigFile(*configFile)
	if err != nil {
		fmt.Printf("error reading config file: %v\n", err)
		os.Exit(1)
	}

	// Create the Luna client for TTS.
	var lunaClient *luna.Client
	if config.Server.Insecure {
		// Create an insecure connection (not recommended for production)
		lunaClient, err = luna.NewClientWithInsecure(config.Server.Address)
	} else {
		lunaClient, err = luna.NewClient(config.Server.Address)
	}

	if err != nil {
		fmt.Printf("error initializing Luna client: %v\n", err)
		os.Exit(1)
	}
	defer lunaClient.Close()

	// Get the Luna version
	lunaVersionResp, err := lunaClient.Version(context.Background())
	if err != nil {
		fmt.Printf("error getting Luna version: %v\n", err)
		return
	}
	fmt.Printf("%s\n", lunaVersionResp.Version)
	fmt.Printf("Connected to %s\n", config.Server.Address)

	// Get the list of available models
	listResponse, err := lunaClient.ListVoices(context.Background())
	if err != nil {
		fmt.Printf("error getting voice list: %v\n", err)
	}

	fmt.Printf("Available voices:\n")
	for _, voice := range listResponse.Voices {
		fmt.Printf("  Name: %s  ID: %s\n", voice.Name, voice.Id)
	}
	fmt.Printf("\n")

	// Setup the synthesis config with the desired voice and encoding
	synthConfig := &lunapb.SynthesizerConfig{
		VoiceId:  config.Server.VoiceID, // As set in the luna_server config file.
		Encoding: lunapb.SynthesizerConfig_RAW_LINEAR16,
	}

	// Setup the playback application
	player := NewPlayer(config)

	// Setup the CLI prompt
	fmt.Printf("Enter text to synthesize at the prompt. To exit, use Ctrl+D.\n\n")

	promptHandler := func(userInput string) {
		trimmed := strings.TrimSpace(userInput)
		if trimmed == "" {
			// Ignore empty strings
			return
		}

		var synthErr error
		if config.Streaming {
			synthErr = streamSynthesis(userInput, lunaClient, synthConfig, &player)
		} else {
			synthErr = batchSynthesis(userInput, lunaClient, synthConfig, &player)
		}

		if synthErr != nil {
			fmt.Printf("synthesis error: %v\n", synthErr)
		}
	}

	p := prompt.New(
		promptHandler,
		func(prompt.Document) []prompt.Suggest { return nil },
		prompt.OptionPrefix("Luna> "),
	)

	p.Run()
	fmt.Printf("Exiting...\n")
}

func batchSynthesis(text string, client *luna.Client,
	synthConfig *lunapb.SynthesizerConfig, player *Player) error {

	// Call the batch synthesis method
	start := time.Now()
	response, err := client.Synthesize(context.Background(),
		&lunapb.SynthesizeRequest{
			Config: synthConfig,
			Text:   text,
		})

	if err != nil {
		return err
	}

	synthDuration := time.Since(start)
	fmt.Printf("batch synthesis took %v\n", synthDuration)

	// Run playback
	start = time.Now()
	if err := player.BatchPlay(response.Audio); err != nil {
		return err
	}

	// Print statistics
	playDuration := time.Since(start)
	fmt.Printf("playback took %v\n", playDuration)

	rtf := float32(playDuration) / float32(synthDuration)
	fmt.Printf("real time factor: %v\n\n", rtf)

	return nil
}

func streamSynthesis(text string, client *luna.Client,
	synthConfig *lunapb.SynthesizerConfig, player *Player) error {

	// Display the total time of this method
	start := time.Now()
	defer func() {
		duration := time.Since(start)
		fmt.Printf("streaming synthesis took %v\n\n", duration)
	}()

	// Start the player application
	if err := player.Start(); err != nil {
		return err
	}

	defer player.Stop()

	// Setup the synthesis stream
	stream, err := client.SynthesizeStream(context.Background(),
		&lunapb.SynthesizeRequest{
			Config: synthConfig,
			Text:   text,
		})

	if err != nil {
		return err
	}

	// Write the binary data as it comes from the stream to the player.
	firstResponse := true
	for {
		// Get the next samples
		response, err := stream.Recv()

		// This indicates the stream has finished.
		if err == io.EOF {
			break
		}
		if err != nil {
			return err
		}

		// Check if this is the first time we've received a response
		if firstResponse {
			firstResponse = false

			// Display some statistics for the first response
			timeToFirst := time.Since(start)
			fmt.Printf("time to first samples: %v\n", timeToFirst)
		}

		// Write the samples to the player.
		if err := player.PushAudio(response.Audio); err != nil {
			return err
		}
	}

	return nil
}
