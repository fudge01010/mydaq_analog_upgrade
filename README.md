## ATTiny85 analog-to-servo bridge upgrade ##

This repository contains all the code, documentation and notes for 
upgrading the ATTiny85 chips on the myDAQ daughter board, for 
Swinburne's MAE20002.


#### Command for flashing ####

Using `avrdude` with a generic ebay USB-ASP clone programmer, the command I used to flash was:

```
avrdude -p t85 -c usbasp-clone -U hfuse:w:0xD7:m -U efuse:w:0xFF:m -U lfuse:w:0xE2:m -U flash:w:mydaq_analog_upgrade.ino.hex:i
```

Jack Fuge, June 2018.
