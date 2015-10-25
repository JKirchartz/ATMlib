**COMMAND list**<br>
    >0       = stop playing note<br>
    >1 … 127 = start playing note, NOTE = note_tab[COMMAND - 1];<br>
  >128 … 159 = effect<br>
*[params]* UNK    = effect specific parameters, format is effect specific<br>
  >160 … 223 = delay, TICKS = COMMAND - 159; // see below<br>
  >224       = delay<br>
*[ticks]* VLE     = number of ticks to delay until next event (add 64)<br>
  >225 … 251 = RESERVED<br>
  >252       = call<br>
*[track index]*<br>
  >253       = loop call<br>
*[loop count]*<br>
*[track index]*<br>
  >254       = ret<br>
  >255       = data chunk<br>
*[length]* VLE    = length in bytes of data to follow<br>
*[data]* UNK      = binary data chunk<br>
