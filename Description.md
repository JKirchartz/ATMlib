| Part of | FieldName | Type | Length | Description |
| --- | --- | --- | --- | --- |
| Track table |  |  |  | amount of tracks and the address of each track  |
|  | count | UBYTE | 8-bits | tells the playroutine how many tracks there are in the array |
|  | Address track 1 | UWORD | 16-bits | tells the playroutine the place in the array where track 1 is |
|  | Address track 2 | UWORD | 16-bits | tells the playroutine the place in the array where track 2 is |
|  | ... | ... | ... | ... | ... | ... |
|  | Address track N | UWORD | 16-bits | tells the playroutine the place in the array where track N (0-255) is |
| Channel entry tracks |  |  |  | track indexes for what track to start the song with for each channel |
|  | Channel 0 track | UBYTE | 8-bits | tells the playroutine which track channel 0 starts with |
|  | Channel 1 track | UBYTE | 8-bits | tells the playroutine which track channel 1 starts with |
|  | Channel 2 track | UBYTE | 8-bits | tells the playroutine which track channel 2 starts with |
|  | Channel 3 track | UBYTE | 8-bits | tells the playroutine which track channel 3 starts with |
| Track 0 |  |  | | the commands and parameters for track 0 |
|  | command 0 | UBYTE | 8-bits | see command list |
|  | parameters 0 | - | variable | see parameter list for each command |
|  | ... | ... | ... | ... | ... |
|  | command N | UBYTE | 8-bits | |
|  | parameters N | - | variable | |
| ... | | | | | |
| Track N |  |  | | the commands and parameters for track N (0-255) |
