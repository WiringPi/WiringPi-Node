# Table of Contents

* [Install](#install)
* [Usage](#usage)
* [APIs](#apis)
  * [Setup](#setup)
  * [Core functions](#core)
  * [Interrupts](#interrupts)
  * [Raspberry Pi specific](#raspberry-pi-hardware-specific-functions)
  * [I2C](#i2c)
  * [SPI](#spi)
  * [Serial](#serial)
  * [Shift](#shift)
  * [Soft PWM](#soft-pwm)
  * [Soft Servo](#soft-servo)
  * [Soft Tone](#soft-tone)
  * [Extensions](#extensions)
    * [dac7678](#dac7678)
    * [drcSerial](#drcserial)
    * [max31855](#max31855)
    * [max5322](#max5322)
    * [mcp23008](#mcp23008)
    * [mcp23016](#mcp23016)
    * [mcp23017](#mcp23017)
    * [mcp23s08](#mcp23s08)
    * [mcp23s17](#mcp23s17)
    * [mcp3002](#mcp3002)
    * [mcp3004/8](#mcp30048)
    * [mcp3422/3/4](#mcp342234)
    * [mcp4802/12/22](#mcp48021222)
    * [pca9685](#pca9685)
    * [pcf8574](#pcf8574)
    * [pcf8591](#pcf8591)
    * [sn3218](#sn3218)
    * [sr595](#sr595)
  * [DevLib](#devlib)
    * [ds1302](#ds1302)
    * [GertBoard](#gertboard)
    * [LCD](#lcd)
    * [LCD 128x64](#lcd-128x64)
    * [piFace](#piface)
    * [piGlow](#piglow)
    * [piNew](#pinew)
    * [tcs34725](#tcs34725)

---

# Install

```
npm install wiring-pi
```

---

# Usage

```javascript
var wpi = require('wiring-pi');
```

---

# APIs

## Setup

### wiringPiSetup()
<span class="api-info"><code> >= 0.1.0 </code></span>

Initialises wiringPi and assumes that the calling program is going to be using the wiringPi pin numbering scheme.

This is a simplified numbering scheme which provides a mapping from virtual pin numbers 0 through 16 to the real underlying Broadcom GPIO pin numbers.

see the [pins](http://wiringpi.com/pins/) page for a table which maps the wiringPi pin number to the Broadcom GPIO pin number to the physical location on the edge connector.

This function needs to be called with root privileges.

### wiringPiSetupGpio()
<span class="api-info"><code> >= 0.1.1 </code></span>

This is indential to above, however it allows the calling programs to use the Broadcom GPIO pin numbers directly with no re-mapping.

As above, this function needs to be called with root privileges, and note that some pins are different from revision 1 to revision 2 boards.

### wiringPiSetupPhys()
<span class="api-info"><code> >= 1.0.0 </code></span>

Identical to above, however it allows the calling programs to use the physical pin numbers on the P1 connector only.

As above, this function needs to be called with root priviliges.

### wiringPiSetupSys()
<span class="api-info"><code> >= 0.1.1 </code></span>

This initialises wiringPi but uses the /sys/class/gpio interface rather than accessing the hardware directly.

This can be called as a non-root user provided the GPIO pins have been exported before-hand using gpio program.

Pin numbering in this mode is the native Broadcom GPIO numbers - the same as wiringPiSetGpio above, so be aware of the differences between Rev 1 and Rev 2 boards.

**NOTE: In this mode you can only use the pins which have been exported via the /sys/class/gpio interface before you run your program.**

**You can do this in a seperate shell script, or by using the system() function from inside your program to call the gpio program.**

**Also note that some functions have no effect when using this mode as they're not currently possible to action unless called with root privileges. (although you can use system() to call gpio to set/change modes if needed).**

### setup(mode)
<span class="api-info"><code> >= 0.1.1 </code></span>

An handy function to setup wiringPi

`mode` can be one of the following values:

* `wpi`: sets up pin numbering with wiringPiSetup
  <span class="api-info-list"><code> >= 0.1.1 </code></span>
* `gpio`: sets up pin numbering with wiringPiSetupGpio
  <span class="api-info-list"><code> >= 0.1.1 </code></span>
* `sys`: sets up pin numbering with wiringPiSetupSys
  <span class="api-info-list"><code> >= 0.1.1 </code></span>
* `phys`: sets up pin numbering with wiringPiSetupPhys
  <span class="api-info-list"><code> >= 1.0.0 </code></span>

More info about pin numbering systems at [wiringpi.com/pins/](http://wiringpi.com/pins/)

**NOTE: `>= 2.0.0` no longer accept calling setup without mode specified. (defaulting to `wpi` in `< 2.0.0`)**

---

## Core functions

### pinModeAlt(pin, mode)
<span class="api-info"><code> >= 1.0.0 </code></span>

This is an un-documented special to let you set any pin to any mode.

`mode` can be one of the following values:

* `FSEL_INPT`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
* `FSEL_OUTP`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
* `FSEL_ALT0`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
* `FSEL_ALT1`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
* `FSEL_ALT2`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
* `FSEL_ALT3`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
* `FSEL_ALT4`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
* `FSEL_ALT5`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>

### pinMode(pin, mode)
<span class="api-info"><code> >= 0.1.0 </code></span>

This sets the mode of a pin.

Note that only wiringPi pin 1 (BCM_GPIO 18) supports PWM output and only wiringPi pin 7 (BCM_GPIO 4) supports CLOCK output modes.

This function has no effect when in Sys mode.

If you need to change the pin mode, the you can do it with the gpio program in a script before you start your program.

`mode` can be one of the following values:

* `modes.INPUT`
  <span class="api-info-list"><code> >= 0.1.1 < 2.0.0 </code><code class="api-info-removed">removed</code></span>
* `modes.OUTPUT`
  <span class="api-info-list"><code> >= 0.1.1 < 2.0.0 </code><code class="api-info-removed">removed</code></span>
* `modes.PWM_OUTPUT`
  <span class="api-info-list"><code> >= 0.1.1 < 2.0.0 </code><code class="api-info-removed">removed</code></span>
* `modes.GPIO_CLOCK`
  <span class="api-info-list"><code> >= 0.1.1 < 2.0.0 </code><code class="api-info-removed">removed</code></span>
* `INPUT`
  <span class="api-info-list"><code> >= 1.0.0 </code></span>
* `OUTPUT`
  <span class="api-info-list"><code> >= 1.0.0 </code></span>
* `PWM_OUTPUT`
  <span class="api-info-list"><code> >= 1.0.0 </code></span>
* `GPIO_CLOCK`
  <span class="api-info-list"><code> >= 1.0.0 </code></span>
* `SOFT_PWM_OUTPUT`
  <span class="api-info-list"><code> >= 1.1.0 </code></span>
* `SOFT_TONE_OUTPUT`
  <span class="api-info-list"><code> >= 1.1.0 </code></span>

### pullUpDnControl(pin, pud)
<span class="api-info"><code> >= 0.2.0 </code></span>

This sets the pull-up or pull-down resistor mode on the given pin, which should be set as an input.

Unlike Arduino, the BCM2835 has both pull-up and down internal resistors.

The internal pull up/down resistors have a value of approximately 50KΩ on the Raspberry Pi.

`pud` can be one of the following values:

* `PUD_OFF` *no pull up/down*
<span class="api-info-list"><code> >= 0.2.0 </code></span>
* `PUD_DOWN` *pull to ground*
<span class="api-info-list"><code> >= 0.2.0 </code></span>
* `PUD_UP` *pull to 3.3v*
<span class="api-info-list"><code> >= 0.2.0 </code></span>

### digitalRead(pin)
<span class="api-info"><code> >= 0.1.1 </code></span>

This function returns the value read at the given pin.

It will be `HIGH` (1) or `LOW` (0) depending on the logic level at the pin.

### digitalWrite(pin, state)
<span class="api-info"><code> >= 0.1.0 </code></span>

Write the value `HIGH` (1) or `LOW` (0) to the given pin which must have been previously set as an output.

WiringPi treats any non-zero number as `HIGH`, however 0 is the only representation of `LOW`.

`state` can be one of the following value:

* `HIGH`
  <span class="api-info-list"><code> >= 0.1.2 </code></span>
* `LOW`
  <span class="api-info-list"><code> >= 0.1.2 </code></span>

### pwmWrite(pin, value)
<span class="api-info"><code> >= 0.1.1 </code></span>

Writes the value to the PWM register for the given pin.

The Raspberry Pi has one on-board PWM pin, pin 1 (BCM_GPIO 18, Phys 12) and the range is [0, 1024].

Other PWM devices may have other PWM ranges.

This function is not able to control the Pi's on-board PWM when in Sys mode.

### analogRead(pin)
<span class="api-info"><code> >= 1.0.0 </code></span>

This returns the value read on the supplied analog input pin.

You will need to register additional analog modules to enable this function for device such as the Gertboard, quick2Wire analog board, etc.

### analogWrite(pin, value)
<span class="api-info"><code> >= 1.0.0 </code></span>

This writes the given value to the supplied analog pin.

You will need to register additional analog modules to enable this function for devices such as the Gertboard.

### pulseIn(pin, state)
<span class="api-info"><code> >= 1.1.0 </code></span>

Reads a pulse (either HIGH or LOW) on a pin.

For example, if `state` is HIGH, pulseIn waits for the pin to go HIGH, starts timing, then waits for the pin to go LOW and stops timing.

Returns the length of the pulse in microseconds.

Gives up and returns 0 if no pulse starts within a specified time out.

`state` can be one of the following values:

* `HIGH`
  <span class="api-info-list"><code> >= 0.1.2 </code></span>
* `LOW`
  <span class="api-info-list"><code> >= 0.1.2 </code></span>

### delay(milliseconds)
<span class="api-info"><code> >= 1.1.0 </code></span>

Pauses the program for the amount of time (in miliseconds) specified as parameter.

There are 1000 milliseconds in a second.

### delayMicroseconds(microseconds)
<span class="api-info"><code> >= 1.1.0 </code></span>

Pauses the program for the amount of time (in microseconds) specified as parameter.

There are a thousand microseconds in a millisecond, and a million microseconds in a second.

For delays longer than a few thousand microseconds, you should use delay() instead.

### millis()
<span class="api-info"><code> >= 1.1.0 </code></span>

Returns the number of milliseconds since the beginning running of the current program

### micros()
<span class="api-info"><code> >= 1.1.0 </code></span>

Returns the number of microseconds since the beginning running of the current program

---

## Interrupts

### wiringPiISR(pin, edgeType, callback)
<span class="api-info"><code> >= 2.0.0 </code></span>

This function registers a function to received interrupts on the specified pin.

The edgeType parameter is either `INT_EDGE_FALLING`, `INT_EDGE_RISING`, `INT_EDGE_BOTH` or `INT_EDGE_SETUP`.

If it is `INT_EDGE_SETUP` then no initialisation of the pin will happen – it’s assumed that you have already setup the pin elsewhere (e.g. with the gpio program), but if you specify one of the other types, then the pin will be exported and initialised as specified.

This is accomplished via a suitable call to the gpio utility program, so it need to be available.

The pin number is supplied in the current mode – native wiringPi, BCM_GPIO, physical or Sys modes.

This function will work in any mode, and does not need root privileges to work.

The callback will be called when the interrupt triggers.

When it is triggered, it’s cleared in the dispatcher before calling your function, so if a subsequent interrupt fires before you finish your handler, then it won’t be missed.

However it can only track one more interrupt, if more than one interrupt fires while one is being handled then they will be ignored.

```javascript
wpi.setup('wpi');
wpi.pinMode(7, wpi.INPUT);
wpi.pullUpDnControl(7, wpi.PUD_UP);
wpi.wiringPiISR(7, wpi.INT_EDGE_FALLING, function(delta) {
  console.log('Pin 7 changed to LOW (', delta, ')');
});
```
### wiringPiISRCancel(pin)
<span class="api-info"><code> >= 2.1.0 </code></span>

---

## Raspberry Pi hardware specific functions

### piBoardRev()
<span class="api-info"><code> >= 0.1.1 </code></span>

This returns the board revision of the Raspberry Pi.

It will be either 1 or 2.

Some of the BCM_GPIO pins changed number and function when moving from board revision 1 to 2, so if you are using BCM_GPIO pin numbers, then you need to be aware of the differences.

### piBoardId()
<span class="api-info"><code> >= 1.1.0 </code></span>

Do more digging into the board revision string as above, but return as much details as we can.

Returns an object with the following keys:

* `model`: indexes to `PI_MODEL_NAMES` string table
* `rev`: indexes to `PI_REVISION_NAMES` string table
* `mem`: 256 or 512
* `maker`: indexes to `PI_MAKER_NAMES` string table
* `overvolted`: 0 or 1
  <span class="api-info-list"><code> >= 2.0.0 </code></span>

**NOTE: `maker` was a string in versions >= 1.1.0 and < 2.0.0**

Indexes of each string table have corresponding constants

* `PI_MODEL_NAME`
  * `PI_MODEL_UNKNOWN`
  <span class="api-info-list"><code> < 3.0.0 </code></span>
  * `PI_MODEL_A`
  <span class="api-info-list"><code> >= 1.1.0 </code></span>
  * `PI_MODEL_B`
  <span class="api-info-list"><code> >= 1.1.0 </code></span>
  * `PI_MODEL_AP`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
  * `PI_MODEL_BP`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>
  * `PI_MODEL_2`
  <span class="api-info-list"><code> >= 2.1.0 </code></span>
  * `PI_ALPHA`
  <span class="api-info-list"><code> >= 3.0.0 </code></span>
  * `PI_MODEL_CM`
  <span class="api-info-list"><code> >= 1.1.1 </code></span>
  * `PI_MODEL_07`
  <span class="api-info-list"><code> >= 3.0.0 </code></span>
  * `PI_MODEL_3`
  <span class="api-info-list"><code> >= 3.0.0 </code></span>
  * `PI_MODEL_ZERO`
  <span class="api-info-list"><code> >= 3.0.0 </code></span>
  * `PI_MODEL_CM3`
  <span class="api-info-list"><code> >= 3.0.0 </code></span>
  * `PI_MODEL_ZERO_W`
  <span class="api-info-list"><code> >= 3.0.0 </code></span>


* `PI_REVISION_NAMES`
  * `PI_VERSION_UNKNOWN`
  <span class="api-info-list"><code> < 3.0.0 </code></span>
  * `PI_VERSION_1`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>
  * `PI_VERSION_1_1`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>
  * `PI_VERSION_1_2`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>
  * `PI_VERSION_2`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>

* `PI_MAKER_NAMES`
  * `PI_MAKER_SONY`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>
  * `PI_MAKER_EGOMAN`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>
  * `PI_MAKER_EMBEST`
  <span class="api-info-list"><code> >= 3.0.0 </code></span>
  * `PI_MAKER_UNKNOWN`
  <span class="api-info-list"><code> >= 2.0.0 </code></span>
  * `PI_MAKER_MBEST`
  <span class="api-info-list"><code> < 3.0.0 </code></span>
  * `PI_MAKER_QISDA`
  <span class="api-info-list"><code> < 3.0.0 </code></span>


### wpiPinToGpio(pin)
<span class="api-info"><code> >= 1.0.0 </code></span>

This returns the BCM_GPIO pin number of the supplied wiringPi pin.

It takes the board revision into account.

### physPinToGpio(pin)
<span class="api-info"><code> >= 1.0.0 </code></span>

This returns the BCM_GPIO pin number of the suppled physical pin on the P1 connector.

### setPadDrive(group, value)
<span class="api-info"><code> >= 1.0.0 </code></span>

This sets the "strength" of the pad drivers for a particular group of pins.

There are 3 groups of pins and the drive strength is from 0 to 7.

**NOTE: Do not use the unless you know what you are doing.**

### getAlt(pin)
<span class="api-info"><code> >= 1.0.0 </code></span>

Returns the ALT bits for a given port.

### digitalWriteByte(byte)
<span class="api-info"><code> >= 1.0.0 </code></span>

This writes the 8-bit byte supplied to the first 8 GPIO pins.

It’s the fastest way to set all 8 bits at once to a particular value, although it still takes two write operations to the Pi’s GPIO hardware.

### pwmSetMode(mode)
<span class="api-info"><code> >= 1.0.0 </code></span>

The PWM generator can run in 2 modes – “balanced” and “mark:space”.

The mark:space mode is traditional, however the default mode in the Pi is “balanced”.

`mode` can be one of the following values:

* `PWM_MODE_BAL` *balanced*
* `PWM_MODE_MS` *mark:space*

### pwmSetRange(range)
<span class="api-info"><code> >= 0.1.1 </code></span>

This sets the range register in the PWM generator.

The default is 1024.

**NOTE: The PWM control functions can not be used when in Sys mode. To understand more about the PWM system, you’ll need to read the Broadcom ARM peripherals manual.**

### pwmSetClock(divisor)
<span class="api-info"><code> >= 0.1.1 </code></span>

This sets the divisor for the PWM clock.

**NOTE: The PWM control functions can not be used when in Sys mode. To understand more about the PWM system, you’ll need to read the Broadcom ARM peripherals manual.**

### pwmToneWrite(pin, frequency)
<span class="api-info"><code> >= 2.0.0 </code></span>

Output the given frequency on the Pi's PWM pin

### gpioClockSet(pin, frequency)
<span class="api-info"><code> >= 1.0.0 </code></span>

Set the frequency on a GPIO clock pin

---

## I2C

### wiringPiI2CSetup(devId)
<span class="api-info"><code> >= 2.0.0 </code></span>

This initialises the I2C system with your given device identifier.

The ID is the I2C number of the device and you can use the i2cdetect program to find this out.

wiringPiI2CSetup() will work out which revision Raspberry Pi you have and open the appropriate device in /dev.

The return value is the Linux file-descriptor for the device, or -1 if any error – in which case, you can consult errno as usual.

### wiringPiI2CSetupInterface(device, devId)
<span class="api-info"><code> >= 2.0.0 </code></span>

Undocumented access to set the interface explicitly - might be used for the Pi's 2nd I2C interface...

### wiringPiI2CClose(fd)
<span class="apt-info"><code> >= 2.2.0 </code></span>

Closes the device identified by the file descriptor given.

### wiringPiI2CRead(fd)
<span class="api-info"><code> >= 2.0.0 </code></span>

Simple device read.

Some devices present data when you read them without having to do any register transactions.

### wiringPiI2CReadReg8(fd, reg)
<span class="api-info"><code> >= 2.0.0 </code></span>

This read an 8-bit value from the device register indicated.

### wiringPiI2CReadReg16(fd, reg)
<span class="api-info"><code> >= 2.0.0 </code></span>

This read an 16-bit value from the device register indicated.

### wiringPiI2CWrite(fd, data)
<span class="api-info"><code> >= 2.0.0 </code></span>

Simple device write.

Some devices accept data this way without needing to access any internal registers.

### wiringPiI2CWriteReg8(fd, reg, data)
<span class="api-info"><code> >= 2.0.0 </code></span>

This write an 8-bit data value into the device register indicated.

### wiringPiI2CWriteReg16(fd, reg, data)
<span class="api-info"><code> >= 2.0.0 </code></span>

This write an 16-bit data value into the device register indicated.

---

## SPI

### wiringPiSPIGetFd(channel)
<span class="api-info"><code> >= 1.0.0 </code></span>

Returns the file-descriptor for the given channel

### wiringPiSPIDataRW(channel, data)
<span class="api-info"><code> >= 1.0.0 </code></span>

This performs a simultaneous write/read transaction over the selected SPI bus.

Data that was in your buffer is overwritten by data returned from the SPI bus.

It is possible to do simple read and writes over the SPI bus using the standard read() and write() system calls though – write() may be better to use for sending data to chains of shift registers, or those LED strings where you send RGB triplets of data.

Devices such as A/D and D/A converters usually need to perform a concurrent write/read transaction to work.

### wiringPiSPISetup(channel, speed)
<span class="api-info"><code> >= 1.0.0 </code></span>

This is the way to initialise a channel (The Pi has 2 channels; 0 and 1).

The speed parameter is an integer in the range 500,000 through 32,000,000 and represents the SPI clock speed in Hz.

The returned value is the Linux file-descriptor for the device, or -1 on error.

If an error has happened, you may use the standard errno global variable to see why.

### wiringPiSPISetupMode(channel, speed, mode)
<span class="apt-info"><code> >= 2.1.0 </code></span>

### wiringPiSPIClose(fd)
<span class="apt-info"><code> >= 2.2.0 </code></span>

Closes the device identified by the file descriptor given.

---

## Serial

### serialOpen(device, baudrate)
<span class="api-info"><code> >= 1.0.0 </code></span>

This opens and initialises the serial device and sets the baud rate.

It sets the port into “raw” mode (character at a time and no translations), and sets the read timeout to 10 seconds.

The return value is the file descriptor or -1 for any error, in which case errno will be set as appropriate.

**NOTE: The file descriptor (fd) returned is a standard Linux file descriptor.**

**You can use the standard read(), write(), etc. system calls on this file descriptor as required.**

**E.g. you may wish to write a larger block of binary data where the serialPutchar() or serialPuts() function may not be the most appropriate function to use, in which case, you can use write() to send the data.**

### serialClose(fd)
<span class="api-info"><code> >= 1.0.0 </code></span>

Closes the device identified by the file descriptor given.

### serialFlush(fd)
<span class="api-info"><code> >= 1.0.0 </code></span>

This discards all data received, or waiting to be send down the given device.

### serialPutchar(fd, character)
<span class="api-info"><code> >= 1.0.0 </code></span>

Sends the single byte to the serial device identified by the given file descriptor.

### serialPuts(fd, string)
<span class="api-info"><code> >= 1.0.0 </code></span>

Sends the nul-terminated string to the serial device identified by the given file descriptor.

### serialPrintf(fd, string)
<span class="api-info">
  <code>Alias: serialPuts</code>
  <code> >= 2.0.0 </code>
</span>

### serialDataAvail(fd)
<span class="api-info"><code> >= 1.0.0 </code></span>

Returns the number of characters available for reading, or -1 for any error condition, in which case errno will be set appropriately.

### serialGetchar(fd)
<span class="api-info"><code> >= 1.0.0 </code></span>

Returns the next character available on the serial device.

This call will block for up to 10 seconds if no data is available (when it will return -1)

---

## Shift

### shiftIn(dPin, cPin, order)
<span class="api-info"><code> >= 1.0.0 </code></span>

This shifts an 8-bit data value in with the data appearing on the dPin and the clock being sent out on the cPin.

Order is either `LSBFIRST` or `MSBFIRST`.

The data is sampled after the cPin goes high. (So cPin high, sample data, cPin low, repeat for 8 bits) The 8-bit value is returned by the function.

### shiftOut(dPin, cPin, order, value)
<span class="api-info"><code> >= 1.0.0 </code></span>

The shifts an 8-bit data value val out with the data being sent out on dPin and the clock being sent out on the cPin.

Order is as above.

Data is clocked out on the rising or falling edge – ie. dPin is set, then cPin is taken high then low – repeated for the 8 bits.

---

## Soft PWM

### softPwmCreate(pin, value, range)
<span class="api-info"><code> >= 1.0.0 </code></span>

This creates a software controlled PWM pin.

You can use any GPIO pin and the pin numbering will be that of the wiringPiSetup() function you used.

Use 100 for the pwmRange, then the value can be anything from 0 (off) to 100 (fully on) for the given pin.

The return value is 0 for success.

Anything else and you should check the global errno variable to see what went wrong.

**NOTE: Each “cycle” of PWM output takes 10mS with the default range value of 100, so trying to change the PWM value more than 100 times a second will be futile.**

**NOTE: Each pin activated in softPWM mode uses approximately 0.5% of the CPU.**

**NOTE: You need to keep your program running to maintain the PWM output!**

### softPwmWrite(pin, value)
<span class="api-info"><code> >= 1.0.0 </code></span>

This updates the PWM value on the given pin.

The value is checked to be in-range and pins that haven’t previously been initialised via softPwmCreate will be silently ignored.

### softPwmStop(pin)
<span class="api-info"><code> >= 1.1.0 </code></span>

---

## Soft Servo

### softServoWrite(pin, value)
<span class="api-info"><code> >= 1.0.0 </code></span>

### softServoSetup(p0, p1, p2, p3, p4, p5, p6, p7)
<span class="api-info"><code> >= 1.0.0 </code></span>

---

## Soft Tone

### softToneCreate(pin);
<span class="api-info"><code> >= 1.0.0 </code></span>

This creates a software controlled tone pin.

You can use any GPIO pin and the pin numbering will be that of the wiringPiSetup() function you used.

The return value is 0 for success.

Anything else and you should check the global errno variable to see what went wrong.

**NOTE: Each pin activated in softTone mode uses approximately 0.5% of the CPU.**

**NOTE: You need to keep your program running to maintain the sound output!**

### softToneWrite(pin, frequency);
<span class="api-info"><code> >= 1.0.0 </code></span>

This updates the tone frequency value on the given pin.

The tone will be played until you set the frequency to 0.

### softToneStop(pin);
<span class="api-info"><code> >= 1.1.0 </code></span>

---

## Extensions

### dac7678

12-Bit octal-channel DAC with 2.5V internal reference (I2C)

*Datasheet*: http://www.ti.com/lit/ds/sbas493c/sbas493c.pdf

#### dac7678Setup(pinBase, i2cAddress, vrefMode)
<span class="api-info"><code> >= 2.0.0 </code></span>

`state` can be one of the following value:

* `DAC7678_VREF_MODE_STATIC_ON`
<span class="api-info-list"><code> >= 2.0.0 </code></span>
* `DAC7678_VREF_MODE_STATIC_OFF`
<span class="api-info-list"><code> >= 2.0.0 </code></span>
* `DAC7678_VREF_MODE_FLEXIBLE_ON`
<span class="api-info-list"><code> >= 2.0.0 </code></span>
* `DAC7678_VREF_MODE_FLEXIBLE_ALWAYS_ON`
<span class="api-info-list"><code> >= 2.0.0 </code></span>
* `DAC7678_VREF_MODE_FLEXIBLE_ALWAYS_OFF`
<span class="api-info-list"><code> >= 2.0.0 </code></span>

### drcSerial

#### drcSetupSerial(pinBase, numPins, device, baudrate)
<span class="api-info"><code> >= 1.0.0 </code></span>

### max31855

Cold-junction compensated thermocouple-to-digital converter (SPI)

*Datasheet*: http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf

#### max31855Setup(pinBase, spiChannel)
<span class="api-info"><code> >= 1.0.0 </code></span>

### max5322

12-Bit DAC (SPI)

*Datasheet*: http://datasheets.maximintegrated.com/en/ds/MAX5322.pdf

#### max5322Setup(pinBase, spiChannel)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp23008

8-Bit I/O expander (I2C)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/21919e.pdf

#### mcp23008Setup(pinBase, i2cAddress)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp23016

16-Bit I/O expander (I2C)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/20090C.pdf

#### mcp23016Setup(pinBase, i2cAddress)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mpc23017

16-Bit I/O expander (I2C)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/21952b.pdf

#### mcp23017Setup(pinBase, i2cAddress)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp23s08

8-Bit I/O expander (SPI)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/21919e.pdf

#### mcp23s08Setup(pinBase, spiChannel, devId)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp23s17

16-Bit I/O expander (SPI)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/21952b.pdf

#### mcp23s17Setup(pinBase, spiChannel, devId)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp3002

2-Channel 10-Bit ADC (SPI)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/21294E.pdf

#### mcp3002Setup(pinBase, spiChannel)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp3004/8

4/8-Channel 10-Bit ADC (SPI)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/21295C.pdf

#### mcp3004Setup(pinBase, spiChannel)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp3422/3/4

2/4-Channel 18-Bit ADC (I2C)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/22088b.pdf

#### mcp3422Setup(pinBase, i2cAddress, sampleRate, gain)
<span class="api-info"><code> >= 1.0.0 </code></span>

### mcp4802/12/22

2-Channel 8/10/12-Bit DAC (SPI)

*Datasheet*: http://ww1.microchip.com/downloads/en/DeviceDoc/22249A.pdf

#### mcp4802Setup(pinBase, spiChannel)
<span class="api-info"><code> >= 1.0.0 </code></span>

### pca9685

16-Channel 12-Bit PWM led/servo driver (I2C)

*Datasheet*: http://www.adafruit.com/datasheets/PCA9685.pdf

#### pca9685Setup(pinBase, i2cAddress, frequency)
<span class="api-info"><code> >= 1.1.0 </code></span>

### pcf8574

8-Bit I/O expander (I2C)

*Datasheet*: http://www.nxp.com/documents/data_sheet/PCF8574.pdf

#### pcf8574Setup(pinBase, i2cAddress)
<span class="api-info"><code> >= 1.0.0 </code></span>

### pcf8591

8-Bit ADC and DAC (I2C)

*Datasheet*: http://www.nxp.com/documents/data_sheet/PCF8591.pdf

#### pcf8591Setup(pinBase, i2cAddress)
<span class="api-info"><code> >= 1.0.0 </code></span>

### sn3218

18-Channel PWM led driver (I2C)

*Datasheet*: http://www.si-en.com/uploadpdf/s2011517171720.pdf

#### sn3218Setup(pinBase)
<span class="api-info"><code> >= 1.0.0 </code></span>

### sr595

74x595 shift register

*Datasheet*: http://www.nxp.com/documents/data_sheet/74HC_HCT595.pdf

#### sr595Setup(pinBase, numPins, dataPin, clockPin, latchPin)
<span class="api-info"><code> >= 1.0.0 </code></span>

---

## DevLib

### ds1302

Trickle-charge timekeeping chip

*Datasheet*: http://datasheets.maximintegrated.com/en/ds/DS1302.pdf

#### ds1302setup(clockPin, dataPin, csPin)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### ds1302rtcRead(reg)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### ds1302rtcWrite(reg, data)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### ds1302ramRead(address)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### ds1302ramWrite(address, data)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### ds1302clockRead()
<span class="api-info"><code> >= 2.0.0 </code></span>

#### ds1302clockWrite(clockData[8])
<span class="api-info"><code> >= 2.0.0 </code></span>

#### ds1302trickleCharge(diodes, resistors)
<span class="api-info"><code> >= 2.0.0 </code></span>

### GertBoard

#### getboardAnalogSetup(pinBase)
<span class="api-info"><code> >= 2.0.0 </code></span>

pinBase is the base pin that you want the analog ports to appear as.

The setup routine allocates 2 pins and overlays the analog to digital input pins with the digital to analog output pins.

So reading channel pinBase + 0 reads the first analog input channel (pin DA0 on the Gertboard), and writing pinBase + 0 outputs to the first analog output channel. (Pin AD0).

**NOTE: The analog interface chips on the Gertboard are connected to the SPI bus. You need to load the SPI kernel modules with gpio load spi and make sure that the 5 SPI jumpers are in-place on the Gertboard – pins SCLK, MOSI, MISO, CSnA and CSnB.**

**NOTE: The analog to digital convertor is a 10-bit device. Values returned will be in the range 0-1023 representing an input voltage of 0 to 3.3 volts.**

**NOTE: Input Voltage calculation: volts = analogIn * 3.3 / 1023**

**NOTE: The digital to analog converter is an 8-bit device. Values sent to it should be in the range 0-255 representing an output voltage on 0 to 2.047 volts.**

**NOTE: Output voltage calculation: volts = analogOut / 255 * 2.047**

**NOTE: The gpio program has commands to directly read and write the analog ports on the Gertboard: gpio gbr 0 will read input channel 0, gpio gbw 1 55 will write the value 55 output channel 1.**

### LCD

#### lcdInit(rows, cols, bits, rs, strb, d0, d1, d2, d3, d4, d5, d6, d7)
<span class="api-info"><code> >= 2.0.0 </code></span>

This is the main initialisation function and must be called before you use any other LCD functions.

Rows and cols are the rows and columns on the display (e.g. 2, 16 or 4,20).

Bits is the number of bits wide on the interface (4 or 8).

The rs and strb represent the pin numbers of the displays RS pin and Strobe (E) pin.

The parameters d0 through d7 are the pin numbers of the 8 data pins connected from the Pi to the display.

Only the first 4 are used if you are running the display in 4-bit mode.

The return value is the ‘handle’ to be used for all subsequent calls to the lcd library when dealing with that LCD, or -1 to indicate a fault. (Usually incorrect parameters)

#### lcdHome(fd)
<span class="api-info"><code> >= 2.0.0 </code></span>

This home the cursor

#### lcdClear(fd)
<span class="api-info"><code> >= 2.0.0 </code></span>

This clear the screen

#### lcdDisplay(fd, state)
<span class="api-info"><code> >= 2.0.0 </code></span>

This turns the display on or off

#### lcdCursor(fd, state)
<span class="api-info"><code> >= 2.0.0 </code></span>

This turns the cursor on or off

#### lcdCursorBlink(fd, state)
<span class="api-info"><code> >= 2.0.0 </code></span>

This turns blinking cursor on or off

#### lcdSendCommand(fd, command)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### lcdPosition(fd, x, y)
<span class="api-info"><code> >= 2.0.0 </code></span>

Set the position of the cursor for subsequent text entry.

x is the column and 0 is the left-most edge.

y is the line and 0 is the top line.

#### lcdCharDef(fd, index, data[8])
<span class="api-info"><code> >= 2.0.0 </code></span>

This allows you to re-define one of the 8 user-definable chanracters in the display.

The data array is 8 bytes which represent the character from the top-line to the bottom line.

Note that the characters are actually 5×8, so only the lower 5 bits are used.

The index is from 0 to 7 and you can subsequently print the character defined using the lcdPutchar() call.

#### lcdPutchar(fd, character)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### lcdPuts(fd, string)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### lcdPrintf(fd, string)
<span class="api-info"><code> >= 2.0.0 </code></span>

### LCD 128x64

### MaxDetect

#### maxDetectRead(pin)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### readRHT03(pin)
<span class="api-info"><code> >= 2.0.0 </code></span>

### piFace

#### piFaceSetup(pinBase)
<span class="api-info"><code> >= 2.0.0 </code></span>

pinBase is the base pin that you want your PiFace to appear as – the examples provided use 200 as the base pin.

The setup assigns 32 pins, although the way it works is to overlay the 8 input pins with the 8 output pins.

So you read from pin base + 0 to read the first input pin, and write to base + 0 to write to the first output pin.

To read the state of the output latch, read from pin + 8 + outputPin.

The remaining 16 pins are used by the underlying MCP23S17 driver and should not be used directly.

**NOTE: The PiFace uses the MCP23S17 SPI GPIO expansion chip so you need to load the SPI kernel driver first. Use the gpio command: gpio load spi**

**NOTE: If you want to use the input pins as outputs, then you need to use the mcp23s17 expansion module for wiringPI instead of the piFace expansion. Then setting up is relatively straighforward – pins 0 through 7 are the output pins, and 8 through 15 are the normal input pins which you can use pinMode() on to change to outputs, if required.**

**NOTE: The gpio command supports the PiFace board directly using the -p flag, but assumes the pin-base is 200. So gpio -p write 200 1 will set the first output pin high – that’s the first relay.**

### piGlow

#### piGlowSetup(clear)
<span class="api-info"><code> >= 2.0.0 </code></span>

This initialises the PiGlow devLib software.

You need to make sure the I2C kernel module is pre-loaded and if you have not used the gpio program to load it, then you may have to run your program as root (ie. with sudo)

The clear parameter is TRUE or FALSE.

If TRUE, then all the LEDs will be turned off to start with.

**NOTE: You need to load the I2C kernel modules before you can use I2C devices. Use the gpio command: gpio load i2c**

**NOTE: If this is the only I2C device on your Pi (and it almost certianly will be unless you’re using some sort of break-out connector!), then it will run at 400KHz, so try: gpio load i2c 400**

**NOTE: Use the i2cdetect program to scan your I2C bus to make sure the Pi can see the SN3218 which will show up as 0×54.**

**NOTE: If you have a Rev 1 Pi, then the i2cdetect command is: i2cdetect -q -y 0 if you have a Rev. 2 Pi, then use i2cdetect -q -y 1**

**NOTE: The gpio command supports the i2cdetect command and automatically caters for board revision. Simply type: gpio i2cd**

**NOTE: The wiringPi SN3218 driver knows which revision Pi you have, so you know need to take any special precautions – your code will work on either a Revision 1 or 2 Pi.**

**NOTE: Internally the PiGlow devLib extension adds 18 more pins to wiringPi’s pin map. These pin are normally at location 577. This should not be an issue as the PiGlow is designed to be the only peripheral on the Pi, but if you have used a breakout board to add other devices to it, then you should pick a pinBase that’s outside the range 577 through 595.**

#### piGlow1(leg, ring, intensity)
<span class="api-info"><code> >= 2.0.0 </code></span>

This lights up an individual LED to the intensity given.

The leg and ring parameters specify the LED to set.

#### piGlowLeg(leg, intensity)
<span class="api-info"><code> >= 2.0.0 </code></span>

This will light up all 6 LEDs on the given led (0, 1 or 2) to the supplied intensity.

The leg number will depend on which way up you have the Pi, but leg 0 is normally the one that points to the same edge the composite video connector in on, 1 is to the right (clockwise) and 2 is to the left (anticlockwise)

#### piGlowRing(ring, intensity)
<span class="api-info"><code> >= 2.0.0 </code></span>

This will light up all 3 LEDs on the given ring at the given intensity – 0 (off) to 255 (really bright!) The ring number is 0 from the outside to 5 for the inside.

You can use the constants:

* `PIGLOW_RED`
* `PIGLOW_YELLOW`
* `PIGLOW_ORANGE`
* `PIGLOW_GREEN`
* `PIGLOW_BLUE`
* `PIGLOW_WHITE`

### piNes

#### setupNesJoystick(dPin, cPin, lPin)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### readNesJoystick(joystick)
<span class="api-info"><code> >= 2.0.0 </code></span>

### tcs34725

Color light-to-digital converter with IR filter

*Datasheet*: http://www.adafruit.com/datasheets/TCS34725.pdf

#### tcs34725Setup(i2cAddress, integrationTime, gain)
<span class="api-info"><code> >= 2.0.0 </code></span>

Initialize the device and returns the assigned id.
Don't forget to call `tcs34725Enable`, the device is in power sleep mode after initialization.

#### tcs34725ReadRGBC(id)
<span class="api-info"><code> >= 2.0.0 </code></span>

Read the raw red, green, blue and clear channel values (0 - 255)

#### tcs34725ReadHSV(id)
<span class="api-info"><code> >= 2.0.0 </code></span>

Returns the cylindrical-coordinate representation of the sensor red, green and blue channels.

#### tcs34725GetCorrelatedColorTemperature(r, g, b)
<span class="api-info"><code> >= 2.0.0 </code></span>

Convert the raw R/G/B values to color temperature in degrees Kelvin

#### tcs34725GetIlluminance(r, g, b)
<span class="api-info"><code> >= 2.0.0 </code></span>

Convert the raw R/G/B values to illuminance in Lux

#### tcs34725SetInterrupt(id, aien)
<span class="api-info"><code> >= 2.0.0 </code></span>

Enable/Disable interrupt

#### tcs34725ClearInterrupt(id)
<span class="api-info"><code> >= 2.0.0 </code></span>

Clear interrupt

#### tcs34725SetInterruptLimits(id, low, high)
<span class="api-info"><code> >= 2.0.0 </code></span>

#### tcs34725Enable(id)
<span class="api-info"><code> >= 2.0.0 </code></span>

Enable the device

#### tcs34725Disable(id)
<span class="api-info"><code> >= 2.0.0 </code></span>

Disable the device (putting it in lower power sleep mode)
