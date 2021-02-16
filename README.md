# synth2

A basic additive synthesiser in C++ with accompanying DSP library. 

This begain as an extension of [olcSynthVisualizer](https://github.com/nospi/olcsynthvisualizer) 
but quickly outgrew the confines, and exemplifies what I hope is a more structured project.

Despite this being a relatively small application; I've attempted to implement a scalable structure 
that allows this DSP library and synth app to be extended upon as necessary.

Using [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine), 
olcPGEX_Sound (modified), [Dear ImGui](https://github.com/ocornut/imgui) and
[dandistine](https://github.com/dandistine)'s [olcPGE Dear ImGui backend implementation](https://github.com/dandistine/olcPGEDearImGui).

## Install
All dependencies are included directly or as submodules. To clone all required files simply use the `--recurse-submodules` flag when cloning this repository:
`git clone --recurse-submodules https://github.com/nospi/synth2`

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
- QWERTY keyboard input (needs to be offset)
- Configurable UI (so configurable that it comes unconfigured!)

## Key Map
```
(QWERTY)
 S    F  G  H    K  L
Z  X C  V  B  N M  ,  .
=======================
(NOTE)
 D#   F# G# A#   C# D#
D  E F  G  A  B C  D  E
```

## FX Units
- Mono Delay

## Upcoming Plans
### Features
- Pre-configured and semi-locked UI
- Concurrent multi-channel support
- MIDI Input
- Efficient wave function approximations
- LFO
- Wave functions:
    - Noise
    - PWM

### Proposed FX Units
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

# [License](https://github.com/nospi/synth2/LICENSE.md)


# Signal Path
### Current
```
OSC 1 --+
        |
OSC 2 -0+-- ADSR ENV -- FILTER 1 -- FILTER 2 -- COMPRESSOR -- FX 1 -- FX 2 -- FX 3 -- FX 4 -- LIMITER -- MASTER FADER
        |
OSC 3 --+
```

### Proposed
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

### Pre/Post Settings
- Filters should be configurable as pre/post dynamics; and pre/post FX
- FX units should be configurable as pre/post dynamics

### Generalised Stages
```
OSCS -- ENV -- PRE-DYN -- (DYN) -- POST-DYN -- (FX) -- POST-FX -- LIMITER -- MASTER FADER
```