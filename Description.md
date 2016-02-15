### FILE/ARRAY FORMAT DESCRIPTION

| **Section** | **Field** | **Type** | **Description** |
| --- | --- | --- | --- |
| **Track table** |  |  |  | **Number of tracks and the address of each track** |
|  | count | UBYTE (8-bits) | Number of tracks in the array |
|  | Address track 1 | UWORD (16-bits) | Location in the array for track 1 |
|  | Address track 2 | UWORD (16-bits) | Location in the array for track 2 |
|  | ... | ... | ... | ... | ... | ... |
|  | Address track N | UWORD (16-bits) | Location in the array for track N (0-255) |
| **Channel entry tracks** |  |  | **For each channel, track to start the song with** |
|  | Channel 0 track | UBYTE (8-bits) | Starting track index for channel 0 |
|  | ... | ... | ... |
|  | Channel 3 track | UBYTE (8-bits) | Starting track index for channel 3 |
| **Track 0** | | | **Commands and parameters for track 0** |
|  | Command 0 | UBYTE (8-bits) | See command list |
|  | + parameters | variable | *See __parameter list__ for each command* |
|  | ... | ... | ... | ... |
|  | Command N | UBYTE (8-bits) | |
|  | + parameters | variable | |
| ... | | | | ... |
| **Track N** | | | **Commands and parameters for track N (0-255)** |


### COMMAND LIST

| **Command (_Z_)** | **Parameter**      | **Type**           | **Description** |
| ---                     | ---                | ---                | --- |
|                 0 |                    |                    | Stop playing |
|           1 … 127 |                    |                    | Start playing note *[Z - 1] where 0 is a C1* |
|         128 … 159 |                    |                    | Apply an effect |
|                   | *See __fx list__*  | -                  | - |
|         160 … 223 |                    |                    | Delay for *[Z - 159] ticks* |
|               224 |                    |                    | Long delay |
|                   | Ticks (*A*)        | VLE (8/16/24-bits) | Delay for *[A + 64] ticks* |
|         225 … 251 |                    |                    | RESERVED |
|               252 |                    |                    | Call/run/goto specified track |
|                   | Track              | UBYTE (8-bits)     | Track index |
|               252 |                    |                    | Repeated call/run/goto specified track |
|                   | Loop count (*A*)   | UBYTE (8-bits)     | Repeat *[A + 2] times* |
|                   | Track              | UBYTE (8-bits)     | Track index |
|               254 |                    |                    | Return/end of track marker |
|               255 |                    |                    | Binary data |
|                   | Length             |                    | Length in bytes of data to follow |
|                   | Data               | variable           | Binary data chunk (notify host application) |
