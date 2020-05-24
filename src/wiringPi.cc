#include "wiringPi.h"
#include <wiringPi.h>
#include <strings.h>

// Setup
DECLARE(setup);
DECLARE(wiringPiSetup);
DECLARE(wiringPiSetupGpio);
DECLARE(wiringPiSetupSys);
DECLARE(wiringPiSetupPhys);

// Core functions
DECLARE(pinModeAlt);
DECLARE(pinMode);
DECLARE(pullUpDnControl);
DECLARE(digitalRead);
DECLARE(digitalWrite);
DECLARE(pwmWrite);
DECLARE(analogRead);
DECLARE(analogWrite);
//DECLARE(pulseIn);

DECLARE(delay);
DECLARE(delayMicroseconds);
DECLARE(millis);
DECLARE(micros);

// On-Board Rasberry Pi hardware specific stuff
DECLARE(piBoardRev);
DECLARE(piBoardId);
DECLARE(wpiPinToGpio);
DECLARE(physPinToGpio);
DECLARE(setPadDrive);
DECLARE(getAlt);
DECLARE(pwmToneWrite);
DECLARE(digitalWriteByte);
DECLARE(pwmSetMode);
DECLARE(pwmSetRange);
DECLARE(pwmSetClock);
DECLARE(gpioClockSet);

IMPLEMENT(setup) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, mode);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_STRING(0);

  #if NODE_VERSION_AT_LEAST(0, 11, 0)
    String::Utf8Value mode(GET_ARGUMENT_AS_STRING(0));
  #else
    String::AsciiValue mode(GET_ARGUMENT_AS_STRING(0));
  #endif

  CHECK_ARGUMENT_IN_STRINGS(0, mode, ("wpi", "gpio", "sys", "phys"));

  int res = 0;
  if (!strcasecmp(*mode, "wpi")) {
    res = ::wiringPiSetup();
  }
  else if (!strcasecmp(*mode, "gpio")) {
    res = ::wiringPiSetupGpio();
  }
  else if (!strcasecmp(*mode, "sys")) {
    res = ::wiringPiSetupSys();
  }
  else if (!strcasecmp(*mode, "phys")) {
    res = ::wiringPiSetupPhys();
  }

  // libWiringPi v2 setup functions always returns 0, so this check is kind of useless, unless v1 behaviour is restored
  // NOTE: If you want to restore the v1 behaviour, then you need to set the
  // environment variable: WIRINGPI_CODES (to any value, it just needs to exist)
  SCOPE_CLOSE(INT32(res));
}

// Func : int wiringPiSetup(void)
// Returns : error code if v1 mode otherwise always returns 0
// Description : Initialises wiringPi and assumes that the calling program is going
// to be using the wiringPi pin numbering scheme.
// This is a simplified numbering scheme which provides a mapping from virtual
// pin numbers 0 through 16 to the real underlying Broadcom GPIO pin numbers.
// see the pins page (https://projects.drogon.net/raspberry-pi/wiringpi/pins/) for a table
// which maps the wiringPi pin number to the Broadcom GPIO pin number to the physical location
// on the edge connector.
// This function needs to be called with root privileges.

IMPLEMENT(wiringPiSetup) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  int res = ::wiringPiSetup();

  SCOPE_CLOSE(INT32(res));
}

// Func : int wiringPiSetupGpio(void)
// Returns : error code if v1 mode otherwise always returns 0
// Description : This is indential to above, however it allows the calling programs to use
// the Broadcom GPIO pin numbers directly with no re-mapping.
// As above, this function needs to be called with root privileges, and note that some pins
// are different from revision 1 to revision 2 boards.

IMPLEMENT(wiringPiSetupGpio) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  int res = ::wiringPiSetupGpio();

  SCOPE_CLOSE(INT32(res));
}

// Func : int wiringPiSetupSys(void)
// Returns : error code if v1 mode otherwise always returns 0
// Description : This initialises wiringPi but uses the /sys/class/gpio interface rather than
// accessing the hardware directly. This can be called as a non-root user provided the GPIO pins
// have been exported before-hand using gpio program. Pin numbering in this mode is the native
// Broadcom GPIO numbers - the same as wiringPiSetGpio above, so be aware of the differences
// between Rev 1 and Rev 2 boards.
// Note: In this mode you can only use the pins which have been exported via the
// /sys/class/gpio interface before you run your program. You can do this in a seperate
// shell script, or by using the system() function from inside your program to call the gpio program.
// Also note that some functions have no effect when using this mode as they're not currently
// possible to action unless called with root privileges. (although you can use system() to call
// gpio to set/change modes if needed).

IMPLEMENT(wiringPiSetupSys) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  int res = ::wiringPiSetupSys();

  SCOPE_CLOSE(INT32(res));
}

// Func : int wiringPiSetupPhys(void)
// Returns : error code if v1 mode otherwise always returns 0
// Description : Identical to above, however it allows the calling programs to use
// the physical pin numbers on the P1 connector only.
// As above, this function needs to be called with root priviliges.

IMPLEMENT(wiringPiSetupPhys) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  int res = ::wiringPiSetupPhys();

  SCOPE_CLOSE(INT32(res));
}

// Func : void pinModeAlt(int pin, int mode)
// Description : This is an un-documented special to let you set any pin to any mode.
// Modes are FSEL_INPT, FSEL_OUTP, FSEL_ALT0, FSEL_ALT1, FSEL_ALT2, FSEL_ALT3, FSEL_ALT4, FSEL_ALT5.

IMPLEMENT(pinModeAlt) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, mode);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int mode = GET_ARGUMENT_AS_INT32(1);

  CHECK_ARGUMENT_IN_INTS(1, mode, (FSEL_INPT, FSEL_OUTP, FSEL_ALT0, FSEL_ALT1, FSEL_ALT2, FSEL_ALT3, FSEL_ALT4, FSEL_ALT5));

  ::pinModeAlt(pin, mode);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void pinMode(int pin, int mode)
// Description : This sets the mode of a pin to either INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK.
// Note that only wiringPi pin 1 (BCM_GPIO 18) supports PWM output and only wiringPi pin 7 (BCM_GPIO 4)
// supports CLOCK output modes.
// This function has no effect when in Sys mode. If you need to change the pin mode, the you can
// do it with the gpio program in a script before you start your program.

IMPLEMENT(pinMode) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, mode);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int mode = GET_ARGUMENT_AS_INT32(1);

  CHECK_ARGUMENT_IN_INTS(1, mode, (INPUT, OUTPUT, PWM_OUTPUT, GPIO_CLOCK, SOFT_PWM_OUTPUT, SOFT_TONE_OUTPUT));

  ::pinMode(pin, mode);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void pullUpDnControl(int pin, int pud)
// Description : This sets the pull-up or pull-down resistor mode on the given pin, which should be set
// as an input. Unlike Arduino, the BCM2835 has both pull-up and down internal resistors.
// The parameter pud should be; PUD_OFF (no pull up/down), PUD_DOWN (pull to ground) or PUD_UP (pull to 3.3v).
// The internal pull up/down resistors have a value of approximately 50KΩ on the Raspberry Pi.

IMPLEMENT(pullUpDnControl) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, pud);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int pud = GET_ARGUMENT_AS_INT32(1);

  CHECK_ARGUMENT_IN_INTS(1, pud, (PUD_OFF, PUD_DOWN, PUD_UP));

  ::pullUpDnControl(pin, pud);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : int digitalRead(int pin)
// Description : This function returns the value read at the given pin. It will be HIGH or LOW (1 or 0)
// depending on the logic level at the pin.

IMPLEMENT(digitalRead) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int res = ::digitalRead(pin);

  // Make sure the function returns strictly 1 or 0
  // §4.7/4 from the C++ Standard says (Integral Conversion)
  // If the source type is bool, the value false is converted to zero and the value true is converted to one.
  res = (res != 0);

  SCOPE_CLOSE(INT32(res));
}

// Func : void digitalWrite(int pin, int value)
// Description : Write the value HIGH or LOW (1 or 0) to the given pin which must have been
// previously set as an output.
// WiringPi treats any non-zero number as HIGH, however 0 is the only representation of LOW.

IMPLEMENT(digitalWrite) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, state);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int state = GET_ARGUMENT_AS_INT32(1);

  CHECK_ARGUMENT_IN_INTS(1, state, (HIGH, LOW));

  ::digitalWrite(pin, state);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void pwmWrite(int pin, int value)
// Description : Writes the value to the PWM register for the given pin. The Raspberry Pi has
// one on-board PWM pin, pin 1 (BCM_GPIO 18, Phys 12) and the range is 0-1024. Other PWM
// devices may have other PWM ranges.
// This function is not able to control the Pi's on-board PWM when in Sys mode.

IMPLEMENT(pwmWrite) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, value);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int value = GET_ARGUMENT_AS_INT32(1);

  ::pwmWrite(pin, value);

  SCOPE_CLOSE(UNDEFINED());
}

IMPLEMENT(analogRead) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int res = ::analogRead(pin);

  SCOPE_CLOSE(INT32(res));
}

// Func : void analogWrite(int pin, int value)
// Description : This writes the given value to the supplied analog pin. You will need to register
// additional analog modules to enable this function for devices such as the Gertboard.

IMPLEMENT(analogWrite) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, value);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int value = GET_ARGUMENT_AS_INT32(1);

  ::analogWrite(pin, value);

  SCOPE_CLOSE(UNDEFINED());
}

//IMPLEMENT(pulseIn) {
//  SCOPE_OPEN();

//  SET_ARGUMENT_NAME(0, pin);
//  SET_ARGUMENT_NAME(1, state);

//  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

//  CHECK_ARGUMENT_TYPE_INT32(0);
//  CHECK_ARGUMENT_TYPE_INT32(1);

//  int pin = GET_ARGUMENT_AS_INT32(0);
//  int state = GET_ARGUMENT_AS_INT32(1);

//  CHECK_ARGUMENT_IN_INTS(1, state, (HIGH, LOW));

//  int us = ::pulseIn(pin, state);

//  SCOPE_CLOSE(INT32(us));
// }

IMPLEMENT(delay) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, ms);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int ms = GET_ARGUMENT_AS_INT32(0);

  ::delay(ms);

  SCOPE_CLOSE(UNDEFINED());
}

IMPLEMENT(delayMicroseconds) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, us);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int us = GET_ARGUMENT_AS_INT32(0);

  ::delayMicroseconds(us);

  SCOPE_CLOSE(UNDEFINED());
}

IMPLEMENT(millis) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  unsigned int ms = ::millis();

  SCOPE_CLOSE(UINT32(ms));
}

IMPLEMENT(micros) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  unsigned int us = ::micros();

  SCOPE_CLOSE(UINT32(us));
}

// === Raspberry Pi specific ===

// Func : int piBoardRev(void)
// Description : This returns the board revision of the Raspberry Pi. It will be either 1 or 2.
// Some of the BCM_GPIO pins changed number and function when moving from board revision 1 to 2,
// so if you are using BCM_GPIO pin numbers, then you need to be aware of the differences.

IMPLEMENT(piBoardRev) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  int res = ::piBoardRev();

  SCOPE_CLOSE(INT32(res));
}

IMPLEMENT(piBoardId) {
  SCOPE_OPEN();

  CHECK_ARGUMENTS_LENGTH_EQUAL(0);

  // libWiringPi 2.20 changes:
  // maker is now a int indexing makerNames string tables
  // a fifth arguments was added named overvolted
  int model, rev, mem, marker, overvolted;

  ::piBoardId(&model, &rev, &mem, &marker, &overvolted);

  #if NODE_VERSION_AT_LEAST(0, 11, 0)
    Local<Object> obj = Object::New(isolate);
    obj->Set(String::NewFromUtf8(isolate, "model", v8::String::kInternalizedString), INT32(model));
    obj->Set(String::NewFromUtf8(isolate, "rev", v8::String::kInternalizedString), INT32(rev));
    obj->Set(String::NewFromUtf8(isolate, "mem", v8::String::kInternalizedString), INT32(mem));
    obj->Set(String::NewFromUtf8(isolate, "marker", v8::String::kInternalizedString), INT32(marker));
    obj->Set(String::NewFromUtf8(isolate, "overvolted", v8::String::kInternalizedString), INT32(overvolted));
  #else
    Local<Object> obj = Object::New();
    obj->Set(String::NewSymbol("model"), INT32(model));
    obj->Set(String::NewSymbol("rev"), INT32(rev));
    obj->Set(String::NewSymbol("mem"), INT32(mem));
    obj->Set(String::NewSymbol("marker"), INT32(marker));
    obj->Set(String::NewSymbol("overvolted"), INT32(overvolted));
  #endif

  SCOPE_CLOSE(obj);
}

// Func : int wpiPinToGpio(int wpiPin)
// Description : This returns the BCM_GPIO pin number of the supplied wiringPi pin.
// It takes the board revision into account.

IMPLEMENT(wpiPinToGpio) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int res = ::wpiPinToGpio(pin);

  SCOPE_CLOSE(INT32(res));
}

// Func : int physPinToGpio (int physPin)
// Description : This returns the BCM_GPIO pin number of the suppled physical pin on the P1 connector.

IMPLEMENT(physPinToGpio) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int res = ::physPinToGpio(pin);

  SCOPE_CLOSE(INT32(res));
}

// Func : void setPadDrive(int group, int value)
// Description : This sets the "strength" of the pad drivers for a particular group of pins.
// There are 3 groups of pins and the drive strength is from 0 to 7. Do not use the unless you
// know what you are doing.

IMPLEMENT(setPadDrive) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, group);
  SET_ARGUMENT_NAME(1, value);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int group = GET_ARGUMENT_AS_INT32(0);
  int value = GET_ARGUMENT_AS_INT32(1);

  ::setPadDrive(group, value);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : int getAlt(int pin)
// Description : Returns the ALT bits for a given port. Only really of-use
// for the gpio readall command (I think).

IMPLEMENT(getAlt) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int res = ::getAlt(pin);

  SCOPE_CLOSE(INT32(res));
}

IMPLEMENT(pwmToneWrite) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, frequency);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int frequency = GET_ARGUMENT_AS_INT32(1);

  ::pwmToneWrite(pin, frequency);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void digitalWriteByte(int value)
// Description : This writes the 8-bit byte supplied to the first 8 GPIO pins.
// It’s the fastest way to set all 8 bits at once to a particular value, although it still takes
// two write operations to the Pi’s GPIO hardware.

IMPLEMENT(digitalWriteByte) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, byte);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int byte = GET_ARGUMENT_AS_INT32(0);
  ::digitalWriteByte(byte);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void pwmSetMode(int mode)
// Description : The PWM generator can run in 2 modes – “balanced” and “mark:space”.
// The mark:space mode is traditional, however the default mode in the Pi is “balanced”.
// You can switch modes by supplying the parameter: PWM_MODE_BAL or PWM_MODE_MS.

IMPLEMENT(pwmSetMode) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, mode);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int mode = GET_ARGUMENT_AS_INT32(0);

  CHECK_ARGUMENT_IN_INTS(0, mode, (PWM_MODE_BAL, PWM_MODE_MS));

  ::pwmSetMode(mode);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void pwmSetRange(unsigned int range)
// Description : This sets the range register in the PWM generator. The default is 1024.
// Note: The PWM control functions can not be used when in Sys mode. To understand more about
// the PWM system, you’ll need to read the Broadcom ARM peripherals manual.

IMPLEMENT(pwmSetRange) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, range);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_UINT32(0);

  unsigned int range = GET_ARGUMENT_AS_UINT32(0);
  ::pwmSetRange(range);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void pwmSetClock(int divisor)
// Description : This sets the divisor for the PWM clock.
// Note: The PWM control functions can not be used when in Sys mode. To understand more about
// the PWM system, you’ll need to read the Broadcom ARM peripherals manual.

IMPLEMENT(pwmSetClock) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, divisor);

  CHECK_ARGUMENTS_LENGTH_EQUAL(1);

  CHECK_ARGUMENT_TYPE_INT32(0);

  int divisor = GET_ARGUMENT_AS_INT32(0);
  ::pwmSetClock(divisor);

  SCOPE_CLOSE(UNDEFINED());
}

// Func : void gpioClockSet(int pin, int freq)
// Description : Set the frequency on a GPIO clock pin

IMPLEMENT(gpioClockSet) {
  SCOPE_OPEN();

  SET_ARGUMENT_NAME(0, pin);
  SET_ARGUMENT_NAME(1, frequency);

  CHECK_ARGUMENTS_LENGTH_EQUAL(2);

  CHECK_ARGUMENT_TYPE_INT32(0);
  CHECK_ARGUMENT_TYPE_INT32(1);

  int pin = GET_ARGUMENT_AS_INT32(0);
  int frequency = GET_ARGUMENT_AS_INT32(1);

  ::gpioClockSet(pin, frequency);

  SCOPE_CLOSE(UNDEFINED());
}

IMPLEMENT_EXPORT_INIT(wiringPi) {
  // Setup
  EXPORT_FUNCTION(setup);
  EXPORT_FUNCTION(wiringPiSetup);
  EXPORT_FUNCTION(wiringPiSetupGpio);
  EXPORT_FUNCTION(wiringPiSetupSys);
  EXPORT_FUNCTION(wiringPiSetupPhys);

  // Core functions
  EXPORT_FUNCTION(pinModeAlt);
  EXPORT_FUNCTION(pinMode);
  EXPORT_FUNCTION(pullUpDnControl);
  EXPORT_FUNCTION(digitalRead);
  EXPORT_FUNCTION(digitalWrite);
  EXPORT_FUNCTION(pwmWrite);
  EXPORT_FUNCTION(analogRead);
  EXPORT_FUNCTION(analogWrite);
  //EXPORT_FUNCTION(pulseIn);

  EXPORT_FUNCTION(delay);
  EXPORT_FUNCTION(delayMicroseconds);
  EXPORT_FUNCTION(millis);
  EXPORT_FUNCTION(micros);

  // On-Board Rasberry Pi hardware specific stuff
  EXPORT_FUNCTION(piBoardRev);
  EXPORT_FUNCTION(piBoardId);
  EXPORT_FUNCTION(wpiPinToGpio);
  EXPORT_FUNCTION(physPinToGpio);
  EXPORT_FUNCTION(setPadDrive);
  EXPORT_FUNCTION(getAlt);
  EXPORT_FUNCTION(pwmToneWrite);
  EXPORT_FUNCTION(digitalWriteByte);
  EXPORT_FUNCTION(pwmSetMode);
  EXPORT_FUNCTION(pwmSetRange);
  EXPORT_FUNCTION(pwmSetClock);
  EXPORT_FUNCTION(gpioClockSet);

  // WPI_MODEs
  EXPORT_CONSTANT_INT(WPI_MODE_PINS);
  EXPORT_CONSTANT_INT(WPI_MODE_PHYS);
  EXPORT_CONSTANT_INT(WPI_MODE_GPIO);
  EXPORT_CONSTANT_INT(WPI_MODE_GPIO_SYS);
  EXPORT_CONSTANT_INT(WPI_MODE_PIFACE);
  EXPORT_CONSTANT_INT(WPI_MODE_UNINITIALISED);
  

  // pinMode
  EXPORT_CONSTANT_INT(INPUT);
  EXPORT_CONSTANT_INT(OUTPUT);
  EXPORT_CONSTANT_INT(PWM_OUTPUT);
  EXPORT_CONSTANT_INT(GPIO_CLOCK);
  EXPORT_CONSTANT_INT(SOFT_PWM_OUTPUT);
  EXPORT_CONSTANT_INT(SOFT_TONE_OUTPUT);

  // pullUpDnControl
  EXPORT_CONSTANT_INT(PUD_OFF);
  EXPORT_CONSTANT_INT(PUD_DOWN);
  EXPORT_CONSTANT_INT(PUD_UP);

  // digitalRead/Write
  EXPORT_CONSTANT_INT(HIGH);
  EXPORT_CONSTANT_INT(LOW);

  // pwmSetMode
  EXPORT_CONSTANT_INT(PWM_MODE_BAL);
  EXPORT_CONSTANT_INT(PWM_MODE_MS);

  // piBoardId
  //EXPORT_CONSTANT_INT(PI_MODEL_UNKNOWN);
  EXPORT_CONSTANT_INT(PI_MODEL_A);
  EXPORT_CONSTANT_INT(PI_MODEL_B);
  EXPORT_CONSTANT_INT(PI_MODEL_BP);
  EXPORT_CONSTANT_INT(PI_MODEL_CM);
  EXPORT_CONSTANT_INT(PI_MODEL_AP);
  EXPORT_CONSTANT_INT(PI_MODEL_2);

  //EXPORT_CONSTANT_INT(PI_VERSION_UNKNOWN);
  EXPORT_CONSTANT_INT(PI_VERSION_1);
  EXPORT_CONSTANT_INT(PI_VERSION_1_1);
  EXPORT_CONSTANT_INT(PI_VERSION_1_2);
  EXPORT_CONSTANT_INT(PI_VERSION_2);

  EXPORT_CONSTANT_INT(PI_MAKER_UNKNOWN);
  EXPORT_CONSTANT_INT(PI_MAKER_EGOMAN);
  EXPORT_CONSTANT_INT(PI_MAKER_SONY);
  //EXPORT_CONSTANT_INT(PI_MAKER_QISDA);
  //EXPORT_CONSTANT_INT(PI_MAKER_MBEST);

  EXPORT_CONSTANT_STRING_ARRAY(PI_MODEL_NAMES, piModelNames, 7);
  EXPORT_CONSTANT_STRING_ARRAY(PI_REVISION_NAMES, piRevisionNames, 5);
  EXPORT_CONSTANT_STRING_ARRAY(PI_MAKER_NAMES, piMakerNames, 5);

  // pinModeAlt
  EXPORT_CONSTANT_INT(FSEL_INPT);
  EXPORT_CONSTANT_INT(FSEL_OUTP);
  EXPORT_CONSTANT_INT(FSEL_ALT0);
  EXPORT_CONSTANT_INT(FSEL_ALT1);
  EXPORT_CONSTANT_INT(FSEL_ALT2);
  EXPORT_CONSTANT_INT(FSEL_ALT3);
  EXPORT_CONSTANT_INT(FSEL_ALT4);
  EXPORT_CONSTANT_INT(FSEL_ALT5);
}
