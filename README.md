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
|                          | Channel 0 track       | UBYTE (8-bits)  | Starting track index for channel 0 |
|                          | …                     | …               | … |
|                          | Channel 3 track       | UBYTE (8-bits)  | Starting track index for channel 3 |
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
|                 0 |                      |                    | Stop playing |
|   |
|           1 …  63 |                      |                    | Start playing note *[__X__ - 1]* where 0 is a C1 |
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
|               252 |                      |                    | Repeated call/run/goto specified track |
|                   | Loop count (*__Y__*) | UBYTE (8-bits)     | Repeat *[__Y__ + 2]* times (total) |
|                   | Track                | UBYTE (8-bits)     | Track index |
|   |
|               254 |                      |                    | Return/end of track marker |
|   |
|               255 |                      |                    | Binary data |
|                   | Length               |                    | Length in bytes of data to follow |
|                   | Data                 | variable           | Binary data chunk (notify host application) |


### FX LIST

| **Effect** | **Parameter**    | **Type**      | **Description** |
| ---        | ---              | ---           | ---             |
|          0 | Volume (*__Y__*) | UBYTE (8-bit) | Set volume to *[__Y__]*. <br /> **_Note:_** If the combined volume of all channels exceed 255 there may be rollover distortion. This should not be disallowed, as it may be usesful as an effects hack for the musician. There should however be a non-interfering warning when a musician enters a value above 63. |
| ~~TBD~~    | ~~TBD~~          | ~~TBD~~       | ~~TBD~~         |

#### Thoughts on effects:

**Note:** These are the primitives to be implemented in the playroutine effects processor. Most will have several effect command numbers associated with them for various aspects of the same primitive. Effects can be combined but not stacked, but some combinations may have undesired/interesting interference.

* Volume slide: a gradual increasing or decreasing of the volume 
* Frequency slide: a gradual increasing or decreasing of the frequency 
* Portamento: a gradual slide from one note to another note
* Arpeggio: a group of notes which are played one after the other, added either going up or going down
* Retriggering (on note or by automation): a sound is replayed a set number of times within a certain timeframe
* Envelopes (instruments): the attack, sustain, and decay of a sound
* Vibrato: a slight, rapid, and regular fluctuation in the pitch of a note
* Tremolo: rapid repetition of a single tone or by rapid alternation of two tones
* Glissando: a glide from one pitch to another
* Transpose (also for microtonals): play notes in a different key from that originally intended
* Note cut (with delay and automation)

_Pitch is a perceptual property of sounds that allows their ordering on a frequency-related scale,[1] or more commonly, pitch is the quality that makes it possible to judge sounds as "higher" and "lower" in the sense associated with musical melodies.[2] Pitch can only be determined in sounds that have a frequency that is clear and stable enough to distinguish from noise.[3] Pitch is a major auditory attribute of musical tones, along with duration, loudness, and timbre.[4]_

_Pitch may be quantified as a frequency, but pitch is not a purely objective physical property; it is a subjective psychoacoustical attribute of sound. Historically, the study of pitch and pitch perception has been a central problem in psychoacoustics, and has been instrumental in forming and testing theories of sound representation, processing, and perception in the auditory system._
