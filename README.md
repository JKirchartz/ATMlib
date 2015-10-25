COMMAND list
    0       = stop playing note<br>
    1 … 127 = start playing note, NOTE = note_tab[COMMAND - 1];
  128 … 159 = effect
    [params] UNK    = effect specific parameters, format is effect specific
  160 … 223 = delay, TICKS = COMMAND - 159; // see below
  224       = delay
    [ticks] VLE     = number of ticks to delay until next event (add 64)
  225 … 251 = RESERVED
  252       = call
    [track index]
  253       = loop call
    [loop count]
    [track index]
  254       = ret
  255       = data chunk
    [length] VLE    = length in bytes of data to follow
    [data] UNK      = binary data chunk
