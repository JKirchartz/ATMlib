| Part of | FieldName | Type | Length | Required | Description |
| --- | --- | --- | --- | --- | --- |
| Track table |  |  |  |  | amount of tracks and the address of each track  |
|  | count | UBYTE | 8-bits | yes | tells the playroutine how many tracks there are in the array |
|  | Address track # | UWORD | 16-bits | yes | tells the playroutine the place in the array where track # is |
| Channel entry tracks |  |  |  |  | track indexes for what track to start the song with for each channel |
|  | Channel 0 track | UBYTE | 8-bits | yes | tells the playroutine with which track this channel starts |
|  | Channel 1 track | UBYTE | 8-bits | yes | tells the playroutine with which track this channel starts |
|  | Channel 2 track | UBYTE | 8-bits | yes | tells the playroutine with which track this channel starts |
|  | Channel 3 track | UBYTE | 8-bits | yes | tells the playroutine with which track this channel starts |
| Track # |  |  |  |  | the commands and parameters used for this track |
|  | command | UBYTE | 8-bits | no | see command list |
|  | parameter | UBYTE | 8-bits | no | see parameter list |
