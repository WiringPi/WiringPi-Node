
var NUM_PINS = 17;

var INPUT = 0;
var OUTPUT = 1;
var PWM_OUTPUT = 2;

var c = require('../build/Release/wiringPi');

function noop() {}

function validateMode(mode) {
  if ( mode != INPUT
    && mode != OUTPUT
    && mode != PWM_OUTPUT)
    throw new Error('Invalid mode');
}

function validatePin(pin) {
  if (pin < 0 || pin >= NUM_PINS)
    throw new Error('Invalid pin number');
}

exports = {
  VERSION: require('../package').version,
  modes: {
    INPUT: INPUT,
    OUTPUT: OUTPUT,
    PWM_OUTPUT: PWM_OUTPUT
  },
  HIGH: 1,
  LOW: 0,
  piBoardRev: c.piBoardRev,
  pwmSetRange: c.pwmSetRange,
  pwmSetClock: c.pwmSetClock,
  pwmWrite: c.pwmWrite
};

exports.setup = function setup(mode) {

  if (mode === undefined)
    mode = 'wpi';

  if (typeof mode == 'string')
    mode = mode.toLowerCase();

  var setup;
  /**/ if (mode === 'wpi')
    setup = 'wiringPiSetup';
  else if (mode === 'gpio')
    setup = 'wiringPiSetupGpio';
  else if (mode === 'sys')
    setup = 'wiringPiSetupSys';
  else throw new Error('Invalid argument');

  if (c[setup]() == -1) {
    throw new Error('wiringPiSetup failed');
  }

  exports.setup = noop;
};

exports.pinMode = function pinMode(pin, mode) {
  validatePin(pin);
  validateMode(mode);
  c.pinMode(pin, mode);
};

exports.digitalWrite = function digitalWrite(pin, value) {
  validatePin(pin);
  value = +(value != 0);
  c.digitalWrite(pin, value);
};

exports.digitalRead = function digitalRead(pin) {
  validatePin(pin);
  return +(c.digitalRead(pin) != 0);
};

module.exports = exports;
