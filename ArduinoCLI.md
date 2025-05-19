# RAW arduino-cli commands

Attach platform to project
```
sudo arduino-cli board attach -p /dev/ttyUSB0 -b esp32:esp32:esp32 -P esptool
```

Compile & Upload (-u)
```
sudo arduino-cli compile Blink/Blink.ino -u
```

Compile & Upload with full platform spec
```
sudo arduino-cli compile Blink/Blink.ino -b esp32:esp32:esp32 -P esptool -p /dev/ttyUSB0 -l serial -u
```
