var c = require('../build/Release/wiringPi');

function noop() {}

function extend(source, destination) {
  var property;
  for (property in source) {
    destination[property] = source[property];
  }
  return destination;
}

exports = {
  VERSION: require('../package').version,
  
  // Handy defines
  NUM_PINS: 17, //Deprecated
  WPI_MODE_PINS: 0,
  WPI_MODE_GPIO: 1,
  WPI_MODE_GPIO_SYS: 2,
  WPI_MODE_PHYS: 3,
  WPI_MODE_PIFACE: 4,
  WPI_MODE_UNINITIALISED: -1,
  
  // Pin modes
  INPUT: 0,
  OUTPUT: 1,
  PWM_OUTPUT: 2,
  GPIO_CLOCK: 3,
  LOW: 0,
  HIGH: 1,
  
  // Pull up/down/none
  PUD_OFF: 0,
  PUD_DOWN: 1,
  PUD_UP: 2,
  
  // PWM
  PWM_MODE_MS: 0,
  PWM_MODE_BAL: 1,
  
  // Interrupt levels
  INT_EDGE_SETUP: 0,
  INT_EDGE_FALLING: 1,
  INT_EDGE_RISING: 2,
  INT_EDGE_BOTH: 3,
  
  // Backward compatibility
  modes: {
    INPUT: 0,
    OUTPUT: 1,
    PWM_OUTPUT: 2
  },
  pull: {
    OFF: 0,
    DOWN: 1,
    UP: 2
  },
  
  // Extensions
  
  // WiringPiShift
  LSBFIRST: 0,
  MSBFIRST: 1,
  
  //MCP3422
  MCP3422_SR_3_75: 0,
  MCP3422_SR_15: 1,
  MCP3422_SR_60: 2,
  MCP3422_SR_240: 3,

  MCP3422_GAIN_1: 3,
  MCP3422_GAIN_2: 2,
  MCP3422_GAIN_4: 1,
  MCP3422_GAIN_8: 0,
};

// Extends exports with properties of 'c'
extend(c, exports);

// Handy function to setup wiringPi
exports.setup = function setup(mode) {

  if (mode === undefined)
    mode = 'wpi';

  if (typeof mode == 'string')
    mode = mode.toLowerCase();

  var setup;
  if (mode === 'wpi')
    setup = 'wiringPiSetup';
  else if (mode === 'gpio')
    setup = 'wiringPiSetupGpio';
  else if (mode === 'sys')
    setup = 'wiringPiSetupSys';
  else if (mode === 'phys')
    setup = 'wiringPiSetupPhys';
  else throw new Error('Invalid argument');

  // libWiringPi v2 setup functions always returns 0, so this check is kind of useless, unless v1 behaviour is restored
  // NOTE: If you want to restore the v1 behaviour, then you need to set the 
  // environment variable: WIRINGPI_CODES (to any value, it just needs to exist)
  if (c[setup]() == -1) {
    throw new Error('wiringPiSetup failed');
  }

  exports.setup = noop;
};

module.exports = exports;
