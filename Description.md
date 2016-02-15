| **Section** | **Field** | **Type** | **Length** | **Description** |
| --- | --- | --- | --- | --- |
| **Track table** |  |  |  | **Number of tracks and the address of each track** |
|  | count | UBYTE | 8-bits | Number of tracks in the array |
|  | Address track 1 | UWORD | 16-bits | Location in the array for track 1 |
|  | Address track 2 | UWORD | 16-bits | Location in the array for track 2 |
|  | ... | ... | ... | ... | ... | ... |
|  | Address track N | UWORD | 16-bits | Location in the array for track N (0-255) |
| **Channel entry tracks** |  |  |  | **For each channel, track to start the song with** |
|  | Channel 0 track | UBYTE | 8-bits | Starting track index for channel 0 |
|  | ... | ... | ... | ... |
|  | Channel 3 track | UBYTE | 8-bits | Starting track index for channel 3 |
| **Track 0** |  |  | | **Commands and parameters for track 0** |
|  | Command 0 | UBYTE | 8-bits | See command list |
|  | +Parameters | - | variable | See parameter list for each command |
|  | ... | ... | ... | ... | ... |
|  | Command N | UBYTE | 8-bits | |
|  | +parameters | - | variable | |
| ... | | | | | ... |
| **Track N** |  |  | | **Commands and parameters for track N (0-255)** |
