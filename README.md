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

| **Command (_Z_)** | **Parameter**        | **Type**           | **Description** |
| ---               | ---                  | ---                | --- |
|                 0 |                      |                    | Stop playing |
|   |
|           1 …  63 |                      |                    | Start playing note *[__Z__ - 1]* where 0 is a C1 |
|   |
|          64 … 159 |                      |                    | Configure effects (fx) |
|                   | *See __fx list__*    | none/variable      | Effect is *[__Z__ - 64]* |
|   |
|         160 … 223 |                      |                    | Delay for *[__Z__ - 159]* ticks |
|   |
|               224 |                      |                    | Long delay |
|                   | Ticks (*__A__*)      | VLE (8/16/24-bits) | Delay for *[__A__ + 64]* ticks |
|   |
|     ~~225 … 251~~ |                      |                    | ~~RESERVED~~ |
|   |
|               252 |                      |                    | Call/run/goto specified track |
|                   | Track                | UBYTE (8-bits)     | Track index |
|   |
|               252 |                      |                    | Repeated call/run/goto specified track |
|                   | Loop count (*__A__*) | UBYTE (8-bits)     | Repeat *[__A__ + 2]* times (total) |
|                   | Track                | UBYTE (8-bits)     | Track index |
|   |
|               254 |                      |                    | Return/end of track marker |
|   |
|               255 |                      |                    | Binary data |
|                   | Length               |                    | Length in bytes of data to follow |
|                   | Data                 | variable           | Binary data chunk (notify host application) |


### FX LIST

| **Effect** | **Parameter** | **Type** | **Description** |
| ---        | ---           | ---      | ---             |
| ~~TBD~~    | ~~TBD~~       | ~~TBD~~  | ~~TBD~~         |


