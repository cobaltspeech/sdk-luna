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
	"encoding/binary"
	"fmt"
	"io"
	"os/exec"
)

// Player represents the external playback executable
// with args.
type Player struct {
	app   string
	args  []string
	cmd   *exec.Cmd
	stdin io.WriteCloser
}

// NewPlayer creates a new player object based on the
// given config.
func NewPlayer(config LunaCLIConfig) Player {
	return Player{
		app:  config.Playback.Application,
		args: config.Playback.ArgList(),
	}
}

// BatchPlay starts the playback app, plays all the given
// audio, then closes the playback app.
func (p *Player) BatchPlay(audio []byte) error {
	if err := p.Start(); err != nil {
		return err
	}
	defer p.Stop()

	p.PushAudio(audio)

	return nil
}

// Start the player app.
func (p *Player) Start() error {
	// Ignore if it is already running
	if p.cmd != nil {
		return nil
	}

	// Setup the command
	cmd := exec.Command(p.app, p.args...)
	stdin, err := cmd.StdinPipe()
	if err != nil {
		return err
	}

	if err := cmd.Start(); err != nil {
		return err
	}

	p.cmd = cmd
	p.stdin = stdin

	return nil
}

// Stop the player app.
func (p *Player) Stop() error {
	// Ignore if it is not running
	if p.cmd == nil {
		return nil
	}

	defer func() {
		p.cmd = nil
		p.stdin = nil
	}()

	// Close the stdin pipe (which should also close the application)
	// and wait for the app to complete
	p.stdin.Close()
	if err := p.cmd.Wait(); err != nil {
		return err
	}

	return nil
}

// PushAudio data to the player app. Start() should
// be called prior to using this function.
func (p *Player) PushAudio(audio []byte) error {
	if p.stdin == nil {
		return fmt.Errorf("player application is not running")
	}

	// Write the audio data to stdin
	return binary.Write(p.stdin, binary.LittleEndian, audio)
}
