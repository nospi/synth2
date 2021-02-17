# synth2

A basic additive synthesiser in C++ with accompanying DSP library. 

Using [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine), 
olcPGEX_Sound (modified), [Dear ImGui](https://github.com/ocornut/imgui) and 
[olcPGE Dear ImGui backend implementation](https://github.com/dandistine/olcPGEDearImGui).

MIDI support now included with [RtMidi](https://github.com/thestk/rtmidi).

## Compiling
All dependencies are included directly or as git submodules. To clone all required files 
simply use the `--recurse-submodules` flag when cloning this repository:

`git clone --recurse-submodules https://github.com/nospi/synth2`

The project is built with Visual Studio 2019 - no alternative build scripts are attached 
but I'd be open to pull requests for additional environments.

## Key Map
![keymap](https://user-images.githubusercontent.com/19985451/108187493-a2620a80-7162-11eb-96bb-2c6687261bbe.png)

## Current Features
- 3 oscillators
- 4 wave functions (Sine, Square, Sawtooth, Triangle)
- 2 resonant filters
- ADSR envelope (applies to all 3 oscillators)
- Compressor
- Limiter
- 4 unit FX rack (currently running as inserts with a dry/wet mix)
- Realtime waveform and FFT visualizers
- Custom presets
- QWERTY *or* MIDI keyboard input
- Configurable UI

### FX Units
- Mono Delay

## Upcoming Plans
#### Proposed Features
- Parameter Smoothing
- Concurrent multi-channel support
- Efficient wave function approximations
- LFO
- Wave functions:
    - Noise
    - PWM

#### Proposed FX Units
- Algorithmic Reverb(s)
- Overdrive
- Fuzz
- Flanger
- Phaser
- Chorus
- Ping Pong Delay

## Dependencies
For convenience, all dependencies are included as submodules under `$(SolutionDir)/libs`. To include these use the `--recurse-submodules` flag when cloning this repository.

- [olcPixelGameEngine.h](https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/olcPixelGameEngine.h)
- [olcPGEX_Sound.h](https://github.com/nospi/synth2/blob/main/libs/olcPGEX_Sound.h) **MODIFIED:** *modified file included in project*
- [Dear ImGui](https://github.com/ocornut/imgui/tree/docking) *docking branch*
- [imgui_impl_pge.h](https://github.com/dandistine/olcPGEDearImGui)
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog/tree/Lib_Only)
- [json.hpp](https://github.com/nlohmann/json/releases/tag/v3.9.1)
- [dirent.h](https://github.com/tronkko/dirent)
- [rtmidi](https://github.com/thestk/rtmidi)

## Signal Path

#### Current
```
OSC 1 --+
        |
OSC 2 --+-- ADSR ENV -- FILTER 1 -- FILTER 2 -- COMPRESSOR -- FX 1 -- FX 2 -- FX 3 -- FX 4 -- LIMITER -- MASTER FADER
        |
OSC 3 --+
```

#### Proposed
Modify FX from insert routing to send routing.
```
                                                             +- FX 1 -+
OSC 1 --+                                                    |        |
        |                                                    +- FX 2 -+
OSC 2 --+-- ADSR ENV -- FILTER 1 -- FILTER 2 -- COMPRESSOR --+        +-- LIMITER -- MASTER FADER
        |                                                    +- FX 3 -+
OSC 3 --+                                                    |        |
                                                             +- FX 4 -+
```
Ideally the fx unit will be configurable between insert and send.

#### Pre/Post Settings
- Filters should be configurable as pre/post dynamics; and pre/post FX
- FX units should be configurable as pre/post dynamics

#### Generalised Stages
```
OSCS -- ENV -- PRE-DYN -- (DYN) -- POST-DYN -- (FX) -- POST-FX -- LIMITER -- MASTER FADER
```

## [License (OLC-3)](https://github.com/nospi/synth2/blob/main/LICENSE.md)
Copyright 2018, 2019, 2020 OneLoneCoder.com

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1) Redistributions or derivations of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2) Redistributions or derivative works in binary form must reproduce the above copyright notice. This list of conditions and the following disclaimer must be reproduced in the documentation and/or other materials provided with the distribution.

3) Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.