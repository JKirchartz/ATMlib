#ATMlib

ATMlib stands for **Arduboy Tracker Music** and is based on [_**Squawk**_](https://github.com/stg/Squawk "Squawk Github Page") a minimalistic 8-bit software synthesizer & playroutine library for Arduino, created by Davey Taylor aka STG.

While _Squawk_ provides a very nice synth, it wasn't optimized for a small footprint. Songs are not very efficient in size, so Joeri Gantois aka JO3RI asked Davey to help him work on a new songformat and so ATMlib was born.

Contributers:

* Davey Taylor - ATMsynth - Effects
* Joeri Gantois - Effects

### FILE/ARRAY FORMAT DESCRIPTION

**Section**					| **Field**					| **Type**			| **Description**
---							| ---						| ----------------	| ---
**Track table**				|							|					| **Number of tracks and their addresses**
							| Track count				| UBYTE (8-bits)	| Number of tracks in the file/array
							| Address track 1			| UWORD (16-bits)	| Location in the file/array for track 1
							| …							| …					| …
							| Address track *__N__*		| UWORD (16-bits)	| Location in the file/array for track *__N__ (0 … 255)*
							|							|					|
**Channel entry tracks**	|							|					| **For each channel, track to start with**
							| Channel 0 track			| UBYTE (8-bits)	| Starting track index for channel 0
							| … 						| …					| …
							| Channel 3 track			| UBYTE (8-bits)	| Starting track index for channel 3
							|							|					|
**Track 0**					|							|					| **Commands and parameters for track 0**
							| Command 0					| UBYTE (8-bits)	| See command list
							| *and its* Parameters		| none/variable		| *See __parameter list__ for each command*
							| …							| …					| …
							| Command N					| UBYTE (8-bits)	|
							| *and its* Parameters		| none/variable		|
**…**						| **…**						| **…**				| **…**
**Track _N_**				|							|					| **Commands and parameters for track _N_** *(0-255)*


### COMMAND LIST

**Command (_X_)**			| **Parameter**			| **Type**				| **Description**
---               	   		| ---					| ------------------	| ---
**0<br/>0x00**				|						|						| Stop playing
							|						|						|
**1…63<br/>0x00+__X__**		| note *(__X__)*		|						| Start playing note *[__X__]* where 1 is a C1. See [Frequency to Tone](./extras/frequencyToTone.md "Frequency to Tone table")
							|						|						|
**64…159<br/>0x40…0x9F**	|						|						| Configure effects (fx)
							| *See __fx list__*		| none/variable			| Effect is *[__X__ - 64]*
							|						|						|
**160…223<br/>0x9F+__t__**	| Ticks (*__t__*)		|						| Delay for *[__X__ - 159]* or *[__t__]* ticks<br/> **_Note:_** delay of 0 does not exist
							|						|						|
**224<br/>0xE0**			|						|						| Long delay
                  	   		| Ticks (*__Y__*)		| VLE (8/16/24-bits)	| Delay for *[__Y__ + 64]* ticks
  					   		|						|						|
**~~225…251~~**		  	   	|						|						| ~~RESERVED~~
  					   		|						|						|
**252<br/>0xFC**			|						|						| Call/run/goto specified track
                  	   		| Track *__N__*			| UBYTE (8-bits)		| Track index where *__N__* is the number of the track to go to
  					   		|						|						|
**253<br/>0xFD**			|						|						| Repeated call/run/goto specified track
                  	   		| Loop count (*__Y__*)	| UBYTE (8-bits)		| Repeat *[__Y__ + 1]* times (total)
                  	   		| Track  *__N__*		| UBYTE (8-bits)		| Track index where *__N__* is the number of the track to go to
  					   		|						|						|
**254<br/>0xFE**			|						|						| Return/end of track marker
							|						|						|
**255<br/>0xFF**			|						|						| Binary data
							| Length				| VLE (8/16/24-bits)	| Length in bytes of data to follow
							| data					| variable				| Binary data chunk (notify host application)


### FX LIST

**Effect**					| **Parameter**										| **Type**						| **Description**
---							| ------------------------							| ---------------------------	| ---
**64+0<br/>64<br/>0x40**	| set Volume (*__X__*)								| UBYTE (8-bit) 				| Set volume to *[__X__]*. <br/> **_Note:_** If the combined volume of all channels exceed 255 there may be rollover distortion. This should not be disallowed, as it may be usesful as an effects hack for the musician. There should however be a non-interfering warning when a musician enters a value above 63 for ch 1-3 or 32 for ch 4 (noise). ch 4 the volume is counted double, so 32 is actually 64
**64+1<br/>65<br/>0x41**	| slide Volume ON (*__X__*)							| UBYTE (8-bit) 				| Slide the volume with an amount (positive or negative) of *[__X__]* for every tick. <br/> **_Note:_** This results in a fade-in or fade-out effect. There should be a non-interfering warning when sliding would result in exceeding 63 for ch 1-3 and 32 for ch 4.
**64+2<br/>66<br/>0x42**	| slide Volume ON advanced (*__X__*) (*__Y__*)		| UBYTE (8-bit) UBYTE (8-bit)	| Slide the volume with an amount (positive or negative) of *[__X__]* for every [*__Y__*] ticks. <br/> *[__Y__]* includes 2 parameters: RRtttttt R = reserved and t = ticks.
**64+3<br/>67<br/>0x43**	| slide Volume OFF									|  								| Stops the volume slide
**64+4<br/>68<br/>0x44**	| slide Frequency ON (*__X__*)						| UBYTE (8-bit)					| Slide the frequency with an amount (positive or negative) of *[__X__]* for every tick. <br/> **_Note:_** The amount of slide is limited between -127 to 127
**64+5<br/>69<br/>0x45**	| slide Frequency ON advanced (*__X__*) (*__Y__*)	| UBYTE (8-bit) UBYTE (8-bit)	| Slide the frequency with an amount (positive or negative) of *[__X__]* for every [*__Y__*] ticks. <br/> **_Note:_** The amount of slide and ticks devider is limited between -127 to 127
**64+6<br/>70<br/>0x46**	| slide Frequency OFF								|  								| Stops the frequency slide
**64+7<br/>71<br/>0x47**	| set Arpeggio (*__X__*)(*__Y__*)					| UBYTE (8-bit) UBYTE (8-bit)	| Next to the current playing note, play a second and third note *[__X__]* for every *[__Y__]* ticks. *[__X__]* includes 2 parameters: AAAABBBB, where AAAA = base + amount to second note and BBBB = second note + amount to third note.<br/> *[__Y__]* includes 4 parameters: FEDttttt, where F = reserved, E = toggle no third note, D = toggle retrigger, ttttt = tick amount. <br/> **_Note:_** Arpeggio is used for playing 3 notes out of a chord indivually
**64+8<br/>72<br/>0x48**	| Arpeggio OFF										|								| Stops the arpeggio
**64+9<br/>73<br/>0x49**	| set Retriggering (*__X__*)						| UBYTE (8-bit)					| Noise channel consists of white noise. By setting retriggering *[__X__]* it swithes the entrypoint at a given speed. *[__X__]*  includes 2 parameters: AAAAAABB , where AAAAAA = entry point and BB = speed (0 = fastest, 1 = faster , 2 = fast)
**64+10<br/>74<br/>0x4A**	| Retriggering OFF									|								| Stops the retriggering for the noise on channel 3
**64+11<br/>75<br/>0x4B**	| add Transposition (*__X__*)						| UBYTE (8-bit)					| Shifts the played notes by adding *[__X__]* to the existing transposition for all playing notes.<br/>**_Note:_** The amount of shift is limited between -127 to 127. However there should be a non-interfering warning when transposing would result in exceeding 63 or get lower than 0
**64+12<br/>76<br/>0x4C**	| set Transposition (*__X__*)						| UBYTE (8-bit)					| Shifts the played notes by setting the transposition to [__X__]* for all playing notes.<br/>**_Note:_** The amount of shift is limited between -127 to 127. However there should be a non-interfering warning when transposing would result in exceeding 63 or get lower than 0
**64+13<br/>77<br/>0x4D**	| Transposition OFF									|								| Stops the transposition
**64+14<br/>78<br/>0x4E**	| set Tremolo (*__X__*)(*__Y__*)					| UBYTE (8-bit) UBYTE (8-bit)	| *[__X__]* sets Depth.<br/> *[__Y__]* includes 4 parameters: RxxBBBBB R = Retrig, x = reserved , B = rate<br/>**_Note:_** Tremolo and Vibrato can **NOT** be combined in the same stack
**64+15<br/>79<br/>0x4F**	| Tremolo OFF										|								| Stops the tremolo or vibrato |
**64+16<br/>80<br/>0x50**	| set Vibrato (*__X__*)(*__Y__*)					| UBYTE (8-bit) UBYTE (8-bit)	| *[__X__]* sets Depth.<br/> *[__Y__]* includes 4 parameters: RxxBBBBB R = Retrig, x = reserved , B = rate<br/>**_Note:_** Tremolo and Vibrato can **NOT** be combined in the same stack
**64+17<br/>81<br/>0x51**	| Vibrato OFF										|								| Stops the vibrato
**64+18<br/>82<br/>0x52**	| SET Glissando (*__X__*)							| UBYTE (8-bit)					| *[__X__]* includes 2 parameters: Vttttttt  V = value ( 0 = go 1 note up, 1 = go 1 note down) and t = amount of ticks, between each step
**64+19<br/>83<br/>0x53**	| Glissando OFF										|								| Stops the Glissando
**64+20<br/>84<br/>0x54**	| SET Note Cut (*__X__*)							| UBYTE (8-bit)					| *[__X__]* sets the equal amount of ticks between note ON and OFF
**64+21<br/>85<br/>0x55**	| Note Cut OFF										|								| Stops the Note Cut
**…**						| **…**												| **…**							| **…**
**64+93<br/>157<br/>0x9D**	| SET song tempo (*__X__*)							| UBYTE (8-bit)					| *[__X__]* (re-)sets the tempo of the song. Standard is 25. Value should be between 0 - 127<br/>**_Note:_** the higher the tempo to more CPU it takes.
							|													|								|
**64+94<br/>158<br/>0x9E**	| GOTO advanced										| 								| **_Note:_** handy command for having an intro and a repeating song part
							| (*__W__*)											| UBYTE (8-bit)					| For channel __0__ go to track __W__
							| (*__X__*)											| UBYTE (8-bit)					| For channel __1__ go to track __X__
							| (*__Y__*)											| UBYTE (8-bit)					| For channel __2__ go to track __Y__
							| (*__Z__*)											| UBYTE (8-bit)					| For channel __3__ go to track __Z__
							|													|								|
**64+95<br/>159<br/>0x9F**	| Stop current channel								|								| channel is no longer being processed<br/>**_Note:_** if all channels have reached STOP, the song ends
~~TBD~~						| ~~TBD~~											| ~~TBD~~						| ~~TBD~~



#### Thoughts on effects:

**Note:** These are the primitives to be implemented in the playroutine effects processor. Most will have several effect command numbers associated with them for various aspects of the same primitive. Effects can be combined but not stacked, but some combinations may have undesired/interesting interference.

* Volume slide: a gradual increasing or decreasing of the volume.
* Frequency slide: a gradual increasing or decreasing of the [frequency](https://en.wikipedia.org/wiki/Frequency "frequency wikipedia").
* Arpeggio: a group of [notes](https://en.wikipedia.org/wiki/Musical_note "note wikipedia") which are rapidly and automatically played one after the other.
* Retriggering (on [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia") or by automation): oscillators are restarted either automatically or at the start of each new note.
* Transposition (also for microtonals): play [notes](https://en.wikipedia.org/wiki/Musical_note "note wikipedia") in a different key, or fine tune notes to provide microtonals; frequencies that are in between notes.
* Tremolo: a slight, rapid and regular fluctuation in the amplitude/volume of a [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia").
* Vibrato: a slight, rapid and regular fluctuation in the [pitch](https://en.wikipedia.org/wiki/Pitch_(music) "pitch wikipedia") of a [note](https://en.wikipedia.org/wiki/Musical_note "note wikipedia").
* Glissando: controls if and how a gradual frequency slide "snaps" to adjacent notes.
* Note cut (with delay and automation): provides a method to stutter and adjust note timing.

