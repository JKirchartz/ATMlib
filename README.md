### FILE/ARRAY FORMAT DESCRIPTION

| **Section**              | **Field**             | **Type**        | **Description** |
| ---                      | ---                   | ---             | ---             |
| **Track table**          |                       |                 | **Number of tracks and their addresses** |
|                          | Track count           | UBYTE (8-bits)  | Number of tracks in the file/array |
|                          | Address track 1       | UWORD (16-bits) | Location in the file/array for track 1 |
|                          | …                     | …               | … |
|                          | Address track *__N__* | UWORD (16-bits) | Location in the file/array for track *__N__ (0 … 255)* |
|   |
| **Channel entry tracks** |                       |                 | **For each channel, track to start with** |
|                          | Channel 1 track       | UBYTE (8-bits)  | Starting track index for channel 1 |
|                          | …                     | …               | … |
|                          | Channel 4 track       | UBYTE (8-bits)  | Starting track index for channel 4 |
|   |
| **Track 0**              |                       |                 | **Commands and parameters for track 0** |
|                          | Command 0             | UBYTE (8-bits)  | See command list |
|                          | *and its* Parameters  | none/variable   | *See __parameter list__ for each command* |
|                          | …                     | …               | … |
|                          | Command N             | UBYTE (8-bits)  | |
|                          | *and its* Parameters  | none/variable   | |
| **…**                    | **…**                 | **…**           | **…** |
| **Track _N_**            |                       |                 | **Commands and parameters for track _N_** *(0-255)* |


### COMMAND LIST

| **Command (_X_)** | **Parameter**        | **Type**           | **Description** |
| ---               | ---                  | ---                | --- |
|           0 …  62 |                      |                    | Start playing note *[__X__]* where 0 is a C1 |
|   |
|                63 |                      |                    | Stop playing |
|   |
|          64 … 159 |                      |                    | Configure effects (fx) |
|                   | *See __fx list__*    | none/variable      | Effect is *[__X__ - 64]* |
|   |
|         160 … 223 |                      |                    | Delay for *[__X__ - 159]* ticks |
|   |
|               224 |                      |                    | Long delay |
|                   | Ticks (*__Y__*)      | VLE (8/16/24-bits) | Delay for *[__Y__ + 64]* ticks |
|   |
|     ~~225 … 251~~ |                      |                    | ~~RESERVED~~ |
|   |
|               252 |                      |                    | Call/run/goto specified track |
|                   | Track                | UBYTE (8-bits)     | Track index |
|   |
|               253 |                      |                    | Repeated call/run/goto specified track |
|                   | Loop count (*__Y__*) | UBYTE (8-bits)     | Repeat *[__Y__ + 2]* times (total) |
|                   | Track                | UBYTE (8-bits)     | Track index |
|   |
|               254 |                      |                    | Return/end of track marker |
|   |
|               255 |                      |                    | Binary data |
|                   | Length               | VLE (8/16/24-bits) | Length in bytes of data to follow |
|                   | Data                 | variable           | Binary data chunk (notify host application) |


### FX LIST

| **Effect** | **Parameter**    | **Type**      | **Description** |
| ---        | ---              | ---           | ---             |
|          0 | Volume (*__Y__*) | UBYTE (8-bit) | Set volume to *[__Y__]*. <br /> **_Note:_** If the combined volume of all channels exceed 255 there may be rollover distortion. This should not be disallowed, as it may be usesful as an effects hack for the musician. There should however be a non-interfering warning when a musician enters a value above 63 for ch 1-3 or 32 for ch 4 (noise). ch 4 the volume is counted double, so 32 is actually 64 |
|          1 | slide Volume ON (*__Y__*) | UBYTE (8-bit) | Slide the volume with an amount of *[__Y__]* for every tick. <br />The amount for slideing can be positive or nagative for a fade in, fade out effect. The fysical limit of the volume is 127, but should not exceed 0 or 63 for ch 1-3 or 32 for ch 4.|
|          2 | slide Volume ON advanced (__X__*) (*__Y__*) | UBYTE (8-bit), UBYTE (8-bit) |  a value above 64 for ch 1-3 or 63 for ch 4 (noise). |
|          3 | slide Volume ON(*__Y__*) | UBYTE (8-bit) |  a value above 64 for ch 1-3 or 63 for ch 4 (noise). |
| ~~TBD~~    | ~~TBD~~          | ~~TBD~~       | ~~TBD~~         |

#### Thoughts on effects:

**Note:** These are the primitives to be implemented in the playroutine effects processor. Most will have several effect command numbers associated with them for various aspects of the same primitive. Effects can be combined but not stacked, but some combinations may have undesired/interesting interference.

* Volume slide: a gradual increasing or decreasing of the volume.
* Frequency slide: a gradual increasing or decreasing of the [frequency](https://en.wikipedia.org/wiki/Frequency "frequency wikipedia").
* Portamento: a gradual slide from one [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia") to another [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia").
* Arpeggio: a group of [notes](https://en.wikipedia.org/wiki/Musical_note "note wikipedia") which are rapidly and automatically played one after the other.
* Retriggering (on [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia") or by automation): oscillators are restarted either automatically or at the start of each new note.
* Envelopes (instruments): the [attack, sustain, and decay](https://en.wikipedia.org/wiki/Synthesizer#Attack_Decay_Sustain_Release_.28ADSR.29_envelope "envelope wikipedia") of a sound.
* Vibrato: a slight, rapid and regular fluctuation in the [pitch](https://en.wikipedia.org/wiki/Pitch_(music) "pitch wikipedia") of a [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia").
* Tremolo: a slight, rapid and regular fluctuation in the amplitude/volume of a [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia").
* Glissando: controls if and how a gradual frequency slide "snaps" to adjacent notes.
* Transpose (also for microtonals): play [notes](https://en.wikipedia.org/wiki/Musical_note "note wikipedia") in a different key, or fine tune notes to provide microtonals; frequencies that are in between notes.
* Note cut (with delay and automation): provides a method to stutter and adjust note timing.
