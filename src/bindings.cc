#include <node.h>
#include <v8.h>

#include <wiringPi.h>

using namespace v8;

#define DECLARE(name) \
  static Handle<Value> name(const Arguments& args)
#define IMPLEMENT(name) \
  Handle<Value> wpi::name(const Arguments& args)
#define EXPORT(name)  \
  target->Set(String::NewSymbol(#name), \
  FunctionTemplate::New(wpi::name)->GetFunction())

namespace wpi {
  DECLARE(wiringPiSetup);
  DECLARE(wiringPiSetupGpio);
  DECLARE(wiringPiSetupSys);
  DECLARE(piBoardRev);
  DECLARE(pinMode);
  DECLARE(digitalWrite);
  DECLARE(digitalRead);
  DECLARE(pwmSetRange);
  DECLARE(pwmSetClock);
  DECLARE(pwmWrite);
  DECLARE(pullUpDnControl);
}

IMPLEMENT(pullUpDnControl) {
  HandleScope scope;
  
  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }
    
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }
  
  ::pullUpDnControl(args[0]->NumberValue(), args[1]->NumberValue());

  return scope.Close(Undefined());
}

IMPLEMENT(wiringPiSetup) {
  HandleScope scope;
  int res;

  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::wiringPiSetup();

  return scope.Close(Int32::New(res));
}

IMPLEMENT(wiringPiSetupGpio) {
  HandleScope scope;
  int res;

  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::wiringPiSetupGpio();

  return scope.Close(Int32::New(res));
}

IMPLEMENT(wiringPiSetupSys) {
  HandleScope scope;
  int res;

  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::wiringPiSetupSys();

  return scope.Close(Int32::New(res));
}

IMPLEMENT(piBoardRev) {
  HandleScope scope;
  int res;

  if (args.Length() != 0) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  res = ::piBoardRev();

  return scope.Close(Int32::New(res));
}

IMPLEMENT(pinMode) {
  HandleScope scope;

  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }

  ::pinMode(args[0]->NumberValue(), args[1]->NumberValue());

  return scope.Close(Undefined());
}

IMPLEMENT(digitalWrite) {
  HandleScope scope;

  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }

  ::digitalWrite(args[0]->NumberValue(), args[1]->NumberValue());

  return scope.Close(Undefined());
}

IMPLEMENT(digitalRead) {
  HandleScope scope;
  int res;

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

  res = ::digitalRead(args[0]->NumberValue());

  return scope.Close(Int32::New(res));
}

IMPLEMENT(pwmSetRange) {
  HandleScope scope;

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

  ::pwmSetRange(args[0]->Uint32Value());

  return scope.Close(Undefined());
}

IMPLEMENT(pwmSetClock) {
  HandleScope scope;

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

  ::pwmSetClock(args[0]->NumberValue());

  return scope.Close(Undefined());
}

IMPLEMENT(pwmWrite) {
  HandleScope scope;

  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(
      String::New("Wrong number of arguments.")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(
      String::New("Incorrect argument types. Numbers expected.")));
    return scope.Close(Undefined());
  }

  ::pwmWrite(args[0]->NumberValue(), args[1]->NumberValue());

  return scope.Close(Undefined());
}

void init(Handle<Object> target) {
  EXPORT(wiringPiSetup);
  EXPORT(wiringPiSetupGpio);
  EXPORT(wiringPiSetupSys);
  EXPORT(piBoardRev);
  EXPORT(pinMode);
  EXPORT(digitalWrite);
  EXPORT(digitalRead);
  EXPORT(pwmSetRange);
  EXPORT(pwmSetClock);
  EXPORT(pwmWrite);
  EXPORT(pullUpDnControl);
}
NODE_MODULE(wiringPi, init)
