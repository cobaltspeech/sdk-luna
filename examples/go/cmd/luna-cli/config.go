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
	"fmt"
	"os"
	"os/exec"
	"strings"

	"github.com/BurntSushi/toml"
)

// ServerConfig contains the Diatheke server settings
type ServerConfig struct {
	Address  string
	Insecure bool
	VoiceID  string
}

// AudioConfig contains the settings for audio I/O
type AudioConfig struct {
	Application string
	Args        string
}

// ArgList returns the arguments as a list of strings.
func (cfg AudioConfig) ArgList() []string {
	return strings.Fields(cfg.Args)
}

// LunaCLIConfig contains the application configuration
type LunaCLIConfig struct {
	Streaming bool
	Server    ServerConfig
	Playback  AudioConfig
}

// ReadConfigFile attempts to load the given config file
func ReadConfigFile(filename string) (LunaCLIConfig, error) {
	var config LunaCLIConfig

	_, err := toml.DecodeFile(filename, &config)
	if err != nil {
		return config, err
	}

	if config.Server.Address == "" {
		return config, fmt.Errorf("missing server address")
	}

	// Check that the playback app is valid
	if err := checkAudioConfig(config.Playback); err != nil {
		return config, fmt.Errorf("playback config error - %v", err)
	}

	return config, nil
}

func checkAudioConfig(config AudioConfig) error {
	// Make sure the application was specified
	if config.Application == "" {
		return fmt.Errorf("application not specified")
	}

	// Verify that the file (executable) exists
	info, err := os.Stat(config.Application)
	if err != nil {
		// This is a path error, which means we couldn't find the file.
		// Check the system path to see if we can find it there.
		_, err = exec.LookPath(config.Application)
		if err != nil {
			return fmt.Errorf("could not find application %s", config.Application)
		}
	} else if info.IsDir() {
		return fmt.Errorf("application is a directory, not an executable")
	}

	return nil
}
