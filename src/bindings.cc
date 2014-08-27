#include <node.h>
#include <v8.h>

#include <wiringPi.h>
#include <drcSerial.h>
#include <max5322.h>
#include <max31855.h>
#include <mcp23s08.h>
#include <mcp23s17.h>
#include <mcp3002.h>
#include <mcp3004.h>
#include <mcp3422.h>
#include <mcp4802.h>
#include <mcp23008.h>
#include <mcp23016.h>
#include <mcp23017.h>
#include <pcf8574.h>
#include <pcf8591.h>
#include <sn3218.h>
#include <pca9685.h>
#include <softPwm.h>
#include <softServo.h>
#include <softTone.h>
#include <sr595.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <wiringShift.h>

using namespace v8;

#define DECLARE(name) \
  static Handle<Value> name(const Arguments& args)
#define IMPLEMENT(name) \
  Handle<Value> wpi::name(const Arguments& args)
#define EXPORT(name)  \
  target->Set(String::NewSymbol(#name), \
  FunctionTemplate::New(wpi::name)->GetFunction())

namespace wpi {
  // Setup
  DECLARE(wiringPiSetup);
  DECLARE(wiringPiSetupGpio);
  DECLARE(wiringPiSetupSys);
  DECLARE(wiringPiSetupPhys);
  
  // Core wiringPi functions
  DECLARE(pinModeAlt);
  DECLARE(pinMode);
  DECLARE(pullUpDnControl);
  DECLARE(digitalRead);
  DECLARE(digitalWrite);
  DECLARE(pwmWrite);
  DECLARE(analogRead);
  DECLARE(analogWrite);
  DECLARE(pulseIn);
  
  DECLARE(delay);
  DECLARE(delayMicroseconds);
  DECLARE(millis);
  DECLARE(micros);
  
  // PiFace specifics (Deprecated)
  //DECLARE(wiringPiSetupPiFace);
  //DECLARE(wiringPiSetupPiFaceForGpioProg); // Don't use this - for gpio program only
  
  // On-Board Rasberry Pi hardware specific stuff
  DECLARE(piBoardRev);
  DECLARE(piBoardId);
  DECLARE(wpiPinToGpio);
  DECLARE(physPinToGpio);
  DECLARE(setPadDrive);
  DECLARE(getAlt);
  DECLARE(digitalWriteByte);
  DECLARE(pwmSetMode);
  DECLARE(pwmSetRange);
  DECLARE(pwmSetClock);
  DECLARE(gpioClockSet);
  
  // Extensions
  DECLARE(drcSetupSerial);
  DECLARE(max5322Setup);
  DECLARE(max31855Setup);
  DECLARE(mcp23s08Setup);
  DECLARE(mcp23s17Setup);
  DECLARE(mcp3002Setup);
  DECLARE(mcp3004Setup);
  DECLARE(mcp3422Setup);
  DECLARE(mcp4802Setup);
  DECLARE(mcp23008Setup);
  DECLARE(mcp23016Setup);
  DECLARE(mcp23017Setup);
  DECLARE(pcf8574Setup);
  DECLARE(pcf8591Setup);
  DECLARE(sn3218Setup);
  DECLARE(sr595Setup);
  DECLARE(pca9685Setup);
  
  // Soft PWM
  DECLARE(softPwmCreate);
  DECLARE(softPwmWrite);
  DECLARE(softPwmStop);
  
  // Soft Servo
  DECLARE(softServoWrite);
  DECLARE(softServoSetup);
  
  // Soft Tone
  DECLARE(softToneCreate);
  DECLARE(softToneWrite);
  DECLARE(softToneStop);
  
  // WiringPI I2C
  DECLARE(wiringPiI2CRead);
  DECLARE(wiringPiI2CReadReg8);
  DECLARE(wiringPiI2CReadReg16);
  DECLARE(wiringPiI2CWrite);
  DECLARE(wiringPiI2CWriteReg8);
  DECLARE(wiringPiI2CWriteReg16);
  DECLARE(wiringPiI2CSetupInterface);
  DECLARE(wiringPiI2CSetup);
  
  // WiringPI SPI
  DECLARE(wiringPiSPIGetFd);
  DECLARE(wiringPiSPIDataRW);
  DECLARE(wiringPiSPISetup);
  
  // WiringPi Serial
  DECLARE(serialOpen);
  DECLARE(serialClose);
  DECLARE(serialFlush);
  DECLARE(serialPutchar);
  DECLARE(serialPuts);
  DECLARE(serialPrintf);
  DECLARE(serialDataAvail);
  DECLARE(serialGetchar);
  
  // WiringPi Shift
  DECLARE(shiftIn);
  DECLARE(shiftOut);
}

// === Setup ===

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
  HandleScope scope;
  int res;

  // CHECK: Number of argument
  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::wiringPiSetup();

  return scope.Close(Int32::New(res));
}

// Func : int wiringPiSetupGpio(void)
// Returns : error code if v1 mode otherwise always returns 0
// Description : This is indential to above, however it allows the calling programs to use
// the Broadcom GPIO pin numbers directly with no re-mapping.
// As above, this function needs to be called with root privileges, and note that some pins
// are different from revision 1 to revision 2 boards.

IMPLEMENT(wiringPiSetupGpio) {
  HandleScope scope;
  int res;

  // CHECK: Number of argument
  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::wiringPiSetupGpio();

  return scope.Close(Int32::New(res));
}

// Func : int wiringPiSetupPhys(void)
// Returns : error code if v1 mode otherwise always returns 0
// Description : Identical to above, however it allows the calling programs to use 
// the physical pin numbers on the P1 connector only.
// As above, this function needs to be called with root priviliges.

IMPLEMENT(wiringPiSetupPhys) {
  HandleScope scope;
  int res;
  
  // CHECK: Number of argument
  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  res = ::wiringPiSetupPhys();
  
  return scope.Close(Int32::New(res));
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
  HandleScope scope;
  int res;

  // CHECK: Number of argument
  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::wiringPiSetupSys();

  return scope.Close(Int32::New(res));
}

// === Core functions ===

// Func : void pinModeAlt(int pin, int mode)
// Description : This is an un-documented special to let you set any pin to any mode.
// Modes are WPI_MODE_PINS, WPI_MODE_PHYS, WPI_MODE_GPIO.

IMPLEMENT(pinModeAlt) {
  HandleScope scope;
  int pin;
  int mode;
  
  // CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  // CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  mode = args[1]->NumberValue();
  
  // CHECK: Allowed values
  if (mode != WPI_MODE_PINS && mode != WPI_MODE_PHYS && mode != WPI_MODE_GPIO) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect mode value. WPI_MODE_PINS, WPI_MODE_PHYS or WPI_MODE_GPIO expected.")));
    return scope.Close(Undefined());
  }
  
  ::pinModeAlt(pin, mode);
  
  return scope.Close(Undefined());
}

// Func : void pinMode(int pin, int mode)
// Description : This sets the mode of a pin to either INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK.
// Note that only wiringPi pin 1 (BCM_GPIO 18) supports PWM output and only wiringPi pin 7 (BCM_GPIO 4)
// supports CLOCK output modes.
// This function has no effect when in Sys mode. If you need to change the pin mode, the you can
// do it with the gpio program in a script before you start your program.

IMPLEMENT(pinMode) {
  HandleScope scope;
  int mode;
  int pin;

  // CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  // CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  mode = args[1]->NumberValue();
  
  // CHECK: Allowed values
  if (mode != INPUT && mode != OUTPUT && mode != PWM_OUTPUT && mode != GPIO_CLOCK && mode != SOFT_PWM_OUTPUT && mode != SOFT_TONE_OUTPUT) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect mode value. INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK expected.")));
    return scope.Close(Undefined());
  }

  ::pinMode(pin, mode);

  return scope.Close(Undefined());
}

// Func : void pullUpDnControl(int pin, int pud)
// Description : This sets the pull-up or pull-down resistor mode on the given pin, which should be set
// as an input. Unlike Arduino, the BCM2835 has both pull-up and down internal resistors.
// The parameter pud should be; PUD_OFF (no pull up/down), PUD_DOWN (pull to ground) or PUD_UP (pull to 3.3v).
// The internal pull up/down resistors have a value of approximately 50KΩ on the Raspberry Pi.

IMPLEMENT(pullUpDnControl) {
  HandleScope scope;
  int pin;
  int pud;
  
  // CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  // CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  pud = args[1]->NumberValue();
  
  // CHECK: Allowed values
  if (pud != PUD_OFF && pud != PUD_DOWN && pud != PUD_UP) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect mode value. PUD_OFF, PUD_DOWN or PUD_UP expected.")));
    return scope.Close(Undefined());
  }
  
  ::pullUpDnControl(pin, pud);

  return scope.Close(Undefined());
}

// Func : int digitalRead(int pin)
// Description : This function returns the value read at the given pin. It will be HIGH or LOW (1 or 0)
// depending on the logic level at the pin.

IMPLEMENT(digitalRead) {
  HandleScope scope;
  int pin;
  int res;

  // CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  // CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }

  pin = args[0]->NumberValue();
  res = ::digitalRead(pin);
  
  // Make sure the function returns strictly 1 or 0
  // §4.7/4 from the C++ Standard says (Integral Conversion)
  // If the source type is bool, the value false is converted to zero and the value true is converted to one.
  res = (res != 0);

  return scope.Close(Int32::New(res));
}

// Func : void digitalWrite(int pin, int value)
// Description : Write the value HIGH or LOW (1 or 0) to the given pin which must have been
// previously set as an output.
// WiringPi treats any non-zero number as HIGH, however 0 is the only representation of LOW.

IMPLEMENT(digitalWrite) {
  HandleScope scope;
  int pin;
  int value;

  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  value = args[1]->NumberValue();
  
  // Make sure value is strictly 1 or 0
  // §4.7/4 from the C++ Standard says (Integral Conversion)
  // If the source type is bool, the value false is converted to zero and the value true is converted to one.
  value = (value != 0);

  ::digitalWrite(pin, value);

  return scope.Close(Undefined());
}

// Func : void pwmWrite(int pin, int value)
// Description : Writes the value to the PWM register for the given pin. The Raspberry Pi has
// one on-board PWM pin, pin 1 (BCM_GPIO 18, Phys 12) and the range is 0-1024. Other PWM
// devices may have other PWM ranges.
// This function is not able to control the Pi's on-board PWM when in Sys mode.

IMPLEMENT(pwmWrite) {
  HandleScope scope;
  int pin;
  int value;

  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  value = args[1]->NumberValue();

  ::pwmWrite(pin, value);

  return scope.Close(Undefined());
}

// Func : int analogRead(int pin)
// Description : This returns the value read on the supplied analog input pin. You will need to
// register additional analog modules to enable this function for device such as the Gertboard,
// quick2Wire analog board, etc.

IMPLEMENT(analogRead) {
  HandleScope scope;
  int pin;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  res = ::analogRead(pin);
  
  return scope.Close(Int32::New(res));
}

// Func : void analogWrite(int pin, int value)
// Description : This writes the given value to the supplied analog pin. You will need to register
// additional analog modules to enable this function for devices such as the Gertboard.

IMPLEMENT(analogWrite) {
  HandleScope scope;
  int pin;
  int value;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  value = args[1]->NumberValue();
  
  ::analogWrite(pin, value);
  
  return scope.Close(Undefined());
}

IMPLEMENT(pulseIn) {
    HandleScope scope;
    int pin;
    int state;
    int microseconds;
    
    //CHECK: Number of argument
    if (args.Length() != 2) {
      ThrowException(Exception::TypeError(
        String::New("Wrong number of arguments.")));
      return scope.Close(Undefined());
    }
    
    //CHECK: Argument types
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
      ThrowException(Exception::TypeError(
        String::New("Incorrect argument type. Number expected.")));
      return scope.Close(Undefined());
    }
    
    pin = args[0]->NumberValue();
    state = args[1]->NumberValue();
    
    microseconds = ::pulseIn(pin, state);
    
    return scope.Close(Int32::New(microseconds));
}

IMPLEMENT(delay) {
    HandleScope scope;
    unsigned int howLong;
    
    if (args.Length() != 1) {
        ThrowException(Exception::TypeError(
          String::New("Wrong number of arguments.")));
        return scope.Close(Undefined());
    }
    
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(
          String::New("Incorrect argument type. Number expected.")));
        return scope.Close(Undefined());
    }
    
    howLong = args[0]->Uint32Value();
    
    ::delay(howLong);
    
    return scope.Close(Undefined());
}

IMPLEMENT(delayMicroseconds) {
    HandleScope scope;
    unsigned int howLong;
    
    if (args.Length() != 1) {
        ThrowException(Exception::TypeError(
          String::New("Wrong number of arguments.")));
        return scope.Close(Undefined());
    }
    
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(
          String::New("Incorrect argument type. Number expected.")));
        return scope.Close(Undefined());
    }
    
    howLong = args[0]->Uint32Value();
    
    ::delayMicroseconds(howLong);
    
    return scope.Close(Undefined());
}

IMPLEMENT(millis) {
    HandleScope scope;
    
    unsigned int ms = ::millis();
    
    return scope.Close(Uint32::New(ms));
}

IMPLEMENT(micros) {
    HandleScope scope;
    
    unsigned int us = ::micros();
    
    return scope.Close(Uint32::New(us));
}

// === Raspberry Pi specific ===

// Func : int piBoardRev(void)
// Description : This returns the board revision of the Raspberry Pi. It will be either 1 or 2.
// Some of the BCM_GPIO pins changed number and function when moving from board revision 1 to 2,
// so if you are using BCM_GPIO pin numbers, then you need to be aware of the differences.

IMPLEMENT(piBoardRev) {
  HandleScope scope;
  int res;

  //CHECK: Number of argument
  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::piBoardRev();

  return scope.Close(Int32::New(res));
}

IMPLEMENT(piBoardId) {
    HandleScope scope;
    int model;
    int rev;
    int mem;
    char* marker;
    
    //CHECK: Number of argument
    if (args.Length() != 0) {
      ThrowException(Exception::TypeError(
        String::New("Wrong number of arguments.")));
      return scope.Close(Undefined());
    }
    
    ::piBoardId(&model, &rev, &mem, &marker);
    
    Local<Object> obj = Object::New();
    obj->Set(String::NewSymbol("model"), Int32::New(model));
    obj->Set(String::NewSymbol("rev"), Int32::New(rev));
    obj->Set(String::NewSymbol("mem"), Int32::New(mem));
    obj->Set(String::NewSymbol("marker"), String::New(marker));
    
    return scope.Close(obj);
}

// Func : int wpiPinToGpio(int wpiPin)
// Description : This returns the BCM_GPIO pin number of the supplied wiringPi pin.
// It takes the board revision into account.

IMPLEMENT(wpiPinToGpio) {
  HandleScope scope;
  int wpiPin;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  wpiPin = args[0]->NumberValue();
  res = ::wpiPinToGpio(wpiPin);
  
  return scope.Close(Int32::New(res));
}

// Func : int physPinToGpio (int physPin)
// Description : This returns the BCM_GPIO pin number of the suppled physical pin on the P1 connector.

IMPLEMENT(physPinToGpio) {
  HandleScope scope;
  int physPin;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  physPin = args[0]->NumberValue();
  res = ::physPinToGpio(physPin);
  
  return scope.Close(Int32::New(res));
}

// Func : void setPadDrive(int group, int value)
// Description : This sets the "strength" of the pad drivers for a particular group of pins.
// There are 3 groups of pins and the drive strength is from 0 to 7. Do not use the unless you
// know what you are doing.

IMPLEMENT(setPadDrive) {
  HandleScope scope;
  int group;
  int value;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  group = args[0]->NumberValue();
  value = args[1]->NumberValue();
  ::setPadDrive(group, value);
  
  return scope.Close(Undefined());
}

// Func : int getAlt(int pin)
// Description : Returns the ALT bits for a given port. Only really of-use
// for the gpio readall command (I think).

IMPLEMENT(getAlt) {
  HandleScope scope;
  int pin;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  res = ::getAlt(pin);
  
  return scope.Close(Int32::New(res));
}

// Func : void digitalWriteByte(int value)
// Description : This writes the 8-bit byte supplied to the first 8 GPIO pins. 
// It’s the fastest way to set all 8 bits at once to a particular value, although it still takes 
// two write operations to the Pi’s GPIO hardware.

IMPLEMENT(digitalWriteByte) {
  HandleScope scope;
  int value;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  value = args[0]->NumberValue();
  ::digitalWriteByte(value);
  
  return scope.Close(Undefined());
}

// Func : void pwmSetMode(int mode)
// Description : The PWM generator can run in 2 modes – “balanced” and “mark:space”. 
// The mark:space mode is traditional, however the default mode in the Pi is “balanced”. 
// You can switch modes by supplying the parameter: PWM_MODE_BAL or PWM_MODE_MS.

IMPLEMENT(pwmSetMode) {
  HandleScope scope;
  int mode;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  mode = args[0]->NumberValue();
  
  //CHECK: Allowed values
  if (mode != PWM_MODE_BAL && mode != PWM_MODE_MS) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect mode value. PWM_MODE_BAL or PWM_MODE_MS expected.")));
    return scope.Close(Undefined());
  }
  
  ::pwmSetMode(mode);
  
  return scope.Close(Undefined());
}

// Func : void pwmSetRange(unsigned int range)
// Description : This sets the range register in the PWM generator. The default is 1024.
// Note: The PWM control functions can not be used when in Sys mode. To understand more about 
// the PWM system, you’ll need to read the Broadcom ARM peripherals manual.

IMPLEMENT(pwmSetRange) {
  HandleScope scope;
  unsigned int range;

  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  range = args[0]->Uint32Value();
  ::pwmSetRange(range);

  return scope.Close(Undefined());
}

// Func : void pwmSetClock(int divisor)
// Description : This sets the divisor for the PWM clock.
// Note: The PWM control functions can not be used when in Sys mode. To understand more about 
// the PWM system, you’ll need to read the Broadcom ARM peripherals manual.

IMPLEMENT(pwmSetClock) {
  HandleScope scope;
  int divisor;

  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }

  divisor = args[0]->NumberValue();
  ::pwmSetClock(divisor);

  return scope.Close(Undefined());
}

// Func : void gpioClockSet(int pin, int freq)
// Description : Set the frequency on a GPIO clock pin

IMPLEMENT(gpioClockSet) {
  HandleScope scope;
  int pin;
  int freq;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->NumberValue();
  freq = args[1]->NumberValue();
  ::gpioClockSet(pin, freq);
  
  return scope.Close(Undefined());
}

// === Extensions ===

// Func : int drcSetupSerial(const int pinBase, const int numPins, const char* device, const int baud)
// Description : https://projects.drogon.net/drogon-remote-control/drc-protocol-arduino/

IMPLEMENT(drcSetupSerial) {
  HandleScope scope;
  int pinBase;
  int numPins;
  const char* device;
  int baud;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 4) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsString() || !args[3]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  numPins = args[1]->Int32Value();
  v8::String::AsciiValue deviceString(args[2]->ToString());
  device = *deviceString;
  baud = args[3]->Int32Value();
  
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::drcSetupSerial(pinBase, numPins, device, baud);
  
  return scope.Close(Int32::New(res));
}

// Func : int max5233Setup(int pinBase, int spiChannel)

IMPLEMENT(max5322Setup) {
  HandleScope scope;
  int pinBase;
  int spiChannel;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  spiChannel = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (spiChannel != 0 && spiChannel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect spiChannel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::max5322Setup(pinBase, spiChannel);
  
  return scope.Close(Int32::New(res));
}

// Func : int max31855Setup(int pinBase, int spiChannel)

IMPLEMENT(max31855Setup) {
  HandleScope scope;
  int pinBase;
  int spiChannel;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  spiChannel = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (spiChannel != 0 && spiChannel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect spiChannel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::max31855Setup(pinBase, spiChannel);
  
  return scope.Close(Int32::New(res));
}

// Func int mcp23s08Setup(const int pinBase, const int spiPort, const int devId)

IMPLEMENT(mcp23s08Setup) {
  HandleScope scope;
  int pinBase;
  int spiPort;
  int devId;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 3) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->NumberValue();
  spiPort = args[1]->NumberValue();
  devId = args[2]->NumberValue();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (spiPort != 0 && spiPort != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect spiPort value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  //MCP23S08 3bits addressing
  if (devId < 0 || devId > 7) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect devId value. Value from 0 to 7 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp23s08Setup(pinBase, spiPort, devId);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp23s17Setup(int pinBase, int spiPort, int devId)
// Description : Initialise libWiringPi to be used with MCP23S17
// pinBase is any number above 64 that doesn’t clash with any other wiringPi expansion module, 
// spiPort is 0 or 1 for one of the two SPI ports on the Pi and devId is the ID of that MCP23s17 on the SPI port.

IMPLEMENT(mcp23s17Setup) {
  HandleScope scope;
  int pinBase;
  int spiPort;
  int devId;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 3) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->NumberValue();
  spiPort = args[1]->NumberValue();
  devId = args[2]->NumberValue();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (spiPort != 0 && spiPort != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect spiPort value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  //MCP23S17 3bits addressing
  if (devId < 0 || devId > 7) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect devId value. Value from 0 to 7 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp23s17Setup(pinBase, spiPort, devId);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp3002Setup(int pinBase, int spiChannel)

IMPLEMENT(mcp3002Setup) {
  HandleScope scope;
  int pinBase;
  int spiChannel;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  spiChannel = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (spiChannel != 0 && spiChannel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect spiChannel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp3002Setup(pinBase, spiChannel);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp3004Setup(int pinBase, int spiChannel)

IMPLEMENT(mcp3004Setup) {
  HandleScope scope;
  int pinBase;
  int spiChannel;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  spiChannel = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (spiChannel != 0 && spiChannel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect spiChannel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp3004Setup(pinBase, spiChannel);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp3422Setup(int pinBase, int i2cAddress, int sampleRate, int gain)

IMPLEMENT(mcp3422Setup) {
  HandleScope scope;
  int pinBase;
  int i2cAddress;
  int sampleRate;
  int gain;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 4) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  i2cAddress = args[1]->Int32Value();
  sampleRate = args[2]->Int32Value();
  gain = args[3]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (sampleRate < 0 || sampleRate > 3) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect sampleRate value. MCP3422_SR_3_75, MCP3422_SR_15, MCP3422_SR_60 or MCP3422_SR_240 expected.")));
    return scope.Close(Undefined());
  }
  
  if (gain < 0 || gain > 3) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect gain value. MCP3422_GAIN_1, MCP3422_GAIN_2, MCP3422_GAIN_3 or MCP3422_GAIN_4 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp3422Setup(pinBase, i2cAddress, sampleRate, gain);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp4802Setup(int pinBase, int spiChannel)

IMPLEMENT(mcp4802Setup) {
  HandleScope scope;
  int pinBase;
  int spiChannel;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  spiChannel = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  if (spiChannel != 0 && spiChannel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect spiChannel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp4802Setup(pinBase, spiChannel);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp23008Setup(int pinBase, int i2cAddress)

IMPLEMENT(mcp23008Setup) {
  HandleScope scope;
  int pinBase;
  int i2cAddress;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  i2cAddress = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp23008Setup(pinBase, i2cAddress);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp23016Setup(const int pinBase, const int i2cAddress)

IMPLEMENT(mcp23016Setup) {
  HandleScope scope;
  int pinBase;
  int i2cAddress;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  i2cAddress = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp23016Setup(pinBase, i2cAddress);
  
  return scope.Close(Int32::New(res));
}

// Func : int mcp23017Setup(const int pinBase, const int i2cAddress)

IMPLEMENT(mcp23017Setup) {
  HandleScope scope;
  int pinBase;
  int i2cAddress;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  i2cAddress = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::mcp23017Setup(pinBase, i2cAddress);
  
  return scope.Close(Int32::New(res));
}

// Func : int pcf8574Setup(const int pinBase, const int i2cAddress)

IMPLEMENT(pcf8574Setup) {
  HandleScope scope;
  int pinBase;
  int i2cAddress;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  i2cAddress = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::pcf8574Setup(pinBase, i2cAddress);
  
  return scope.Close(Int32::New(res));
}

// Func : int pcf8591Setup(const int pinBase, const int i2cAddress)

IMPLEMENT(pcf8591Setup) {
  HandleScope scope;
  int pinBase;
  int i2cAddress;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  i2cAddress = args[1]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::pcf8591Setup(pinBase, i2cAddress);
  
  return scope.Close(Int32::New(res));
}

// Func : int sn3128Setup(int pinBase)

IMPLEMENT(sn3218Setup) {
  HandleScope scope;
  int pinBase;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::sn3218Setup(pinBase);
  
  return scope.Close(Int32::New(res));
}

// Func : int sr595Setup(const int pinBase, const int numPins, const int dataPin, const int clockPin, const int latchPin)

IMPLEMENT(sr595Setup) {
  HandleScope scope;
  int pinBase;
  int numPins;
  int dataPin;
  int clockPin;
  int latchPin;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 5) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber() || !args[4]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pinBase = args[0]->Int32Value();
  numPins = args[1]->Int32Value();
  dataPin = args[2]->Int32Value();
  clockPin = args[3]->Int32Value();
  latchPin = args[4]->Int32Value();
  
  //CHECK: Allowed values
  if (pinBase <= 64) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect pinBase value. >64 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::sr595Setup(pinBase, numPins, dataPin, clockPin, latchPin);
  
  return scope.Close(Int32::New(res));
}

IMPLEMENT(pca9685Setup) {
    HandleScope scope;
    int pinBase;
    int i2cAddress;
    int freq;
    int res;
    
    if (args.Length() != 3) {
        ThrowException(Exception::TypeError(
            String::New("Wrong number of arguments.")));
        return scope.Close(Undefined());
    }
    
    if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
        ThrowException(Exception::TypeError(
            String::New("Incorrect arguments type. Number expected.")));
        return scope.Close(Undefined());
    }
    
    pinBase = args[0]->Int32Value();
    i2cAddress = args[1]->Int32Value();
    freq = args[2]->Int32Value();
    
    if (pinBase <= 64) {
        ThrowException(Exception::TypeError(
            String::New("Incorrect pinBase value. >64 expected")));
        return scope.Close(Undefined());
    }
    
    res = ::pca9685Setup(pinBase, i2cAddress, freq);
    
    return scope.Close(Int32::New(res));
}

// === Soft PWM ===

// Func : int softPwmCreate(int pin, int value, int range)
// Description : This creates a software controlled PWM pin. 
// You can use any GPIO pin and the pin numbering will be that of the wiringPiSetup() function you used. 
// Use 100 for the pwmRange, then the value can be anything from 0 (off) to 100 (fully on) for the given pin.
// The return value is 0 for success. Anything else and you should check the global errno variable to see what went wrong.
// NOTE : You must initialise wiringPi with one of wiringPiSetup(),  wiringPiSetupGpio() or wiringPiSetupPhys() functions. 
// wiringPiSetupSys() is not fast enough, so you must run your programs with sudo.
// NOTE2 : Each “cycle” of PWM output takes 10mS with the default range value of 100, 
// so trying to change the PWM value more than 100 times a second will be futile.
// NOTE3 : Each pin activated in softPWM mode uses approximately 0.5% of the CPU.
// NOTE4 : There is currently no way to disable softPWM on a pin while the program in running.
// NOTE5 : You need to keep your program running to maintain the PWM output!

IMPLEMENT(softPwmCreate) {
  HandleScope scope;
  int pin;
  int value;
  int range;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 3) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->Int32Value();
  value = args[1]->Int32Value();
  range = args[2]->Int32Value();
  
  res = ::softPwmCreate(pin, value, range);
  
  return scope.Close(Int32::New(res));
}

// Func void softPwmWrite(int pin, int value)
// Description : This updates the PWM value on the given pin. 
// The value is checked to be in-range and pins that haven’t previously been initialised via softPwmCreate will be silently ignored.

IMPLEMENT(softPwmWrite) {
  HandleScope scope;
  int pin;
  int value;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->Int32Value();
  value = args[1]->Int32Value();
  
  ::softPwmWrite(pin, value);
  
  return scope.Close(Undefined());
}

IMPLEMENT(softPwmStop) {
    HandleScope scope;
    int pin;
    
    if (args.Length() != 1) {
      ThrowException(Exception::TypeError(
        String::New("Wrong number of arguments.")));
      return scope.Close(Undefined());
    }
    
    //CHECK: Argument types
    if (!args[0]->IsNumber()) {
      ThrowException(Exception::TypeError(
        String::New("Incorrect argument type. Number expected.")));
      return scope.Close(Undefined());
    }
    
    pin = args[0]->Int32Value();
    
    ::softPwmStop(pin);
    
    return scope.Close(Undefined());
}

// === Soft Servo ===

// Func : void softServoWrite(int pin, int value)
// Description : Write a Servo value to the given pin

IMPLEMENT(softServoWrite) {
  HandleScope scope;
  int pin;
  int value;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->Int32Value();
  value = args[1]->Int32Value();
  
  ::softServoWrite(pin, value);
  
  return scope.Close(Undefined());
}

// Func : int softServoSetup(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7)
// Description : Setup the software servo system

IMPLEMENT(softServoSetup) {
  HandleScope scope;
  int p0;
  int p1;
  int p2;
  int p3;
  int p4;
  int p5;
  int p6;
  int p7;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 8) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber()
      || !args[4]->IsNumber() || !args[5]->IsNumber() || !args[6]->IsNumber() || !args[7]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  p0 = args[0]->Int32Value();
  p1 = args[1]->Int32Value();
  p2 = args[2]->Int32Value();
  p3 = args[3]->Int32Value();
  p4 = args[4]->Int32Value();
  p5 = args[5]->Int32Value();
  p6 = args[6]->Int32Value();
  p7 = args[7]->Int32Value();
  
  res = ::softServoSetup(p0, p1, p2, p3, p4, p5, p6, p7);
  
  return scope.Close(Int32::New(res));
}

// === Soft Tone ===

// Func : int softToneCreate(int pin);
// Description : This creates a software controlled tone pin. 
// You can use any GPIO pin and the pin numbering will be that of the wiringPiSetup() function you used.
// The return value is 0 for success. 
// Anything else and you should check the global errno variable to see what went wrong.
// NOTE : You must initialise wiringPi with one of wiringPiSetup(),  wiringPiSetupGpio() or wiringPiSetupPhys() functions. 
// wiringPiSetupSys() is not fast enough, so you must run your programs with sudo.
// NOTE2 : Each pin activated in softTone mode uses approximately 0.5% of the CPU.
// NOTE3 : You need to keep your program running to maintain the sound output!

IMPLEMENT(softToneCreate) {
  HandleScope scope;
  int pin;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->Int32Value();
  
  res = ::softToneCreate(pin);
  
  return scope.Close(Int32::New(res));
}

// Func : void softToneWrite(int pin, int freq);
// Description : This updates the tone frequency value on the given pin. The tone will be played until you set the frequency to 0.

IMPLEMENT(softToneWrite) {
  HandleScope scope;
  int pin;
  int freq;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  pin = args[0]->Int32Value();
  freq = args[1]->Int32Value();
  
  ::softToneWrite(pin, freq);
  
  return scope.Close(Undefined());
}

IMPLEMENT(softToneStop) {
    HandleScope scope;
    int pin;
    
    if (args.Length() != 1) {
      ThrowException(Exception::TypeError(
        String::New("Wrong number of arguments.")));
      return scope.Close(Undefined());
    }
    
    //CHECK: Argument types
    if (!args[0]->IsNumber()) {
      ThrowException(Exception::TypeError(
        String::New("Incorrect argument type. Number expected.")));
      return scope.Close(Undefined());
    }
    
    pin = args[0]->Int32Value();
    
    ::softToneStop(pin);
    
    return scope.Close(Undefined());
}

// === WiringPI I2C ===

IMPLEMENT(wiringPiI2CRead) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiI2CReadReg8) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiI2CReadReg16) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiI2CWrite) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiI2CWriteReg8) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiI2CWriteReg16) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiI2CSetupInterface) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiI2CSetup) {
  HandleScope scope;
  ThrowException(Exception::TypeError(
    String::New("Not implemented")));
  return scope.Close(Undefined());
}

// === WiringPI SPI ===

// Func : int wiringPiSPIGetFd(int channel)

IMPLEMENT(wiringPiSPIGetFd) {
  HandleScope scope;
  int channel;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  channel = args[0]->Int32Value();
  
  // CHECK: Allowed values
  if (channel != 0 && channel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect channel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::wiringPiSPIGetFd(channel);
  
  return scope.Close(Int32::New(res));
}

// Func : wiringPiSPIDataRW(int channel, unsigned char* data, int len)

IMPLEMENT(wiringPiSPIDataRW) {
  HandleScope scope;
  int channel;
  unsigned char* data;
  int len;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 3) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsString() || !args[2]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type.")));
    return scope.Close(Undefined());
  }
  
  channel = args[0]->Int32Value();
  v8::String::AsciiValue dataString(args[1]->ToString());
  data = (unsigned char*)*dataString;
  len = args[2]->Int32Value();
  
  // CHECK: Allowed values
  if (channel != 0 && channel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect channel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::wiringPiSPIDataRW(channel, data, len);
  
  return scope.Close(Int32::New(res));
}

// Func : int wiringPiSPISetup(int channel, int speed)

IMPLEMENT(wiringPiSPISetup) {
  HandleScope scope;
  int channel;
  int speed;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  channel = args[0]->Int32Value();
  speed = args[1]->Int32Value();
  
  // CHECK: Allowed values
  if (channel != 0 && channel != 1) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect channel value. 0 or 1 expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::wiringPiSPISetup(channel, speed);
  
  return scope.Close(Int32::New(res));
}

// === WiringPi Serial ===

// Func : int serialOpen(const char* device, const int baud)

IMPLEMENT(serialOpen) {
  HandleScope scope;
  const char* device;
  int baud;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type.")));
    return scope.Close(Undefined());
  }
  
  v8::String::AsciiValue deviceString(args[0]->ToString());
  device = *deviceString;
  baud = args[1]->Int32Value();
  
  res = ::serialOpen(device, baud);
  
  return scope.Close(Int32::New(res));
}

// Func : void serialClose(const int fd)

IMPLEMENT(serialClose) {
  HandleScope scope;
  int fd;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  fd = args[0]->Int32Value();
  
  ::serialClose(fd);
  
  return scope.Close(Undefined());
}

// Func : void serialFlush(const int fd);

IMPLEMENT(serialFlush) {
  HandleScope scope;
  int fd;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  fd = args[0]->Int32Value();
  
  ::serialFlush(fd);
  
  return scope.Close(Undefined());
}

// Func : void serialPutchar(const int fd, const unsigned char c)

IMPLEMENT(serialPutchar) {
  HandleScope scope;
  int fd;
  unsigned char c;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  fd = args[0]->Int32Value();
  c = (args[0]->Uint32Value() & 0xFF);
  
  ::serialPutchar(fd, c);
  
  return scope.Close(Undefined());
}

// Func : void serialPuts(const int fd, const char* s)

IMPLEMENT(serialPuts) {
  HandleScope scope;
  int fd;
  const char* s;
  
  //CHECK: Number of argument
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type.")));
    return scope.Close(Undefined());
  }
  
  fd = args[0]->Int32Value();
  v8::String::AsciiValue sString(args[0]->ToString());
  s = *sString;
  
  ::serialPuts(fd, s);
  
  return scope.Close(Undefined());
}

// Func : void serialPrintf(const int fd, const char* message, ...)

IMPLEMENT(serialPrintf) {
  return serialPuts(args);
}

// Func : int serialDataAvail(const int fd)

IMPLEMENT(serialDataAvail) {
  HandleScope scope;
  int fd;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  fd = args[0]->Int32Value();
  
  res = ::serialDataAvail(fd);
  
  return scope.Close(Int32::New(res));
}

// Func : int serialGetchar(const int fd)
// NOTE TO MYSELF : I don't understand why serialPutchar takes a unsigned char and on the other side
// serialGetchar returns a int ... serialGetchar should returns a unsigned char too.

IMPLEMENT(serialGetchar) {
  HandleScope scope;
  int fd;
  int res;
  
  //CHECK: Number of argument
  if (args.Length() != 1) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  //CHECK: Argument types
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  fd = args[0]->Int32Value();
  
  res = ::serialGetchar(fd);
  
  return scope.Close(Int32::New(res));
}

// === WiringPi Shift ===

// Func : uint8_t shiftIn(uint8_t dPin, uint8_t cPin, uint8_t order)
// Description : This shifts an 8-bit data value in with the data appearing on the dPin and the clock being sent out on the cPin.
// Order is either LSBFIRST or MSBFIRST.
// The data is sampled after the cPin goes high.
// (So cPin high, sample data, cPin low, repeat for 8 bits) The 8-bit value is returned by the function.

IMPLEMENT(shiftIn) {
  HandleScope scope;
  uint8_t dPin;
  uint8_t cPin;
  uint8_t order;
  uint8_t res;
  
  // CHECK: Number of argument
  if (args.Length() != 3) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  // CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  dPin = args[0]->Uint32Value();
  cPin = args[1]->Uint32Value();
  order = args[2]->Uint32Value();
  
  // CHECK : Allowed values
  if (order != LSBFIRST || order != MSBFIRST) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect order value. LSBFIRT or MSBFIRST expected.")));
    return scope.Close(Undefined());
  }
  
  res = ::shiftIn(dPin, cPin, order);
  
  return scope.Close(Uint32::New(res));
}

// Func : void shiftOut(uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val) ;
// Description : The shifts an 8-bit data value val out with the data being sent out on dPin and the clock being sent out on the cPin.
// order is as above. 
// Data is clocked out on the rising or falling edge – ie. dPin is set, then cPin is taken high then low – repeated for the 8 bits.

IMPLEMENT(shiftOut) {
  HandleScope scope;
  uint8_t dPin;
  uint8_t cPin;
  uint8_t order;
  uint8_t val;
  
  // CHECK: Number of argument
  if (args.Length() != 4) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
  
  // CHECK: Argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument type. Number expected.")));
    return scope.Close(Undefined());
  }
  
  dPin = args[0]->Uint32Value();
  cPin = args[1]->Uint32Value();
  order = args[2]->Uint32Value();
  val = args[3]->Uint32Value();
  
  // CHECK : Allowed values
  if (order != LSBFIRST || order != MSBFIRST) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect order value. LSBFIRT or MSBFIRST expected.")));
    return scope.Close(Undefined());
  }
  
  ::shiftOut(dPin, cPin, order, val);
  
  return scope.Close(Undefined());
}

void init(Handle<Object> target) {
  // Setup
  EXPORT(wiringPiSetup);
  EXPORT(wiringPiSetupGpio);
  EXPORT(wiringPiSetupSys);
  EXPORT(wiringPiSetupPhys);
  
  // Core wiringPi functions
  EXPORT(pinModeAlt);
  EXPORT(pinMode);
  EXPORT(pullUpDnControl);
  EXPORT(digitalRead);
  EXPORT(digitalWrite);
  EXPORT(pwmWrite);
  EXPORT(analogRead);
  EXPORT(analogWrite);
  EXPORT(pulseIn);
  
  EXPORT(delay);
  EXPORT(delayMicroseconds);
  EXPORT(millis);
  EXPORT(micros);
  
  // PiFace specifics (Deprecated)
  //EXPORT(wiringPiSetupPiFace);
  //EXPORT(wiringPiSetupPiFaceForGpioProg); // Don't use this - for gpio program only
  
  // On-Board Rasberry Pi hardware specific stuff
  EXPORT(piBoardRev);
  EXPORT(piBoardId);
  EXPORT(wpiPinToGpio);
  EXPORT(physPinToGpio);
  EXPORT(setPadDrive);
  EXPORT(getAlt);
  EXPORT(digitalWriteByte);
  EXPORT(pwmSetMode);
  EXPORT(pwmSetRange);
  EXPORT(pwmSetClock);
  EXPORT(gpioClockSet);
  
  // Extensions
  EXPORT(drcSetupSerial);
  EXPORT(max5322Setup);
  EXPORT(max31855Setup);
  EXPORT(mcp23s08Setup);
  EXPORT(mcp23s17Setup);
  EXPORT(mcp3002Setup);
  EXPORT(mcp3004Setup);
  EXPORT(mcp3422Setup);
  EXPORT(mcp4802Setup);
  EXPORT(mcp23008Setup);
  EXPORT(mcp23016Setup);
  EXPORT(mcp23017Setup);
  EXPORT(pcf8574Setup);
  EXPORT(pcf8591Setup);
  EXPORT(sn3218Setup);
  EXPORT(sr595Setup);
  EXPORT(pca9685Setup);
  
  // Soft PWM
  EXPORT(softPwmCreate);
  EXPORT(softPwmWrite);
  EXPORT(softPwmStop);
  
  // Soft Servo
  EXPORT(softServoWrite);
  EXPORT(softServoSetup);
  
  // Soft Tone
  EXPORT(softToneCreate);
  EXPORT(softToneWrite);
  EXPORT(softToneStop);
  
  // WiringPI I2C
  EXPORT(wiringPiI2CRead);
  EXPORT(wiringPiI2CReadReg8);
  EXPORT(wiringPiI2CReadReg16);
  EXPORT(wiringPiI2CWrite);
  EXPORT(wiringPiI2CWriteReg8);
  EXPORT(wiringPiI2CWriteReg16);
  EXPORT(wiringPiI2CSetupInterface);
  EXPORT(wiringPiI2CSetup);
  
  // WiringPI SPI
  EXPORT(wiringPiSPIGetFd);
  EXPORT(wiringPiSPIDataRW);
  EXPORT(wiringPiSPISetup);
  
  // WiringPi Serial
  EXPORT(serialOpen);
  EXPORT(serialClose);
  EXPORT(serialFlush);
  EXPORT(serialPutchar);
  EXPORT(serialPuts);
  EXPORT(serialPrintf);
  EXPORT(serialDataAvail);
  EXPORT(serialGetchar);
  
  // WiringPi Shift
  EXPORT(shiftIn);
  EXPORT(shiftOut);
}

NODE_MODULE(wiringPi, init)
