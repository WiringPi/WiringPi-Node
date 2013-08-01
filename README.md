Node.js bindings to [wiringPi](https://projects.drogon.net/raspberry-pi/wiringpi/)

## Install

    > npm install wiring-pi

## Use

```javascript

var wpi = require('wiring-pi');

```

### `wpi.setup([mode])`

```javascript
wpi.setup();
```

```javascript
wpi.setup('gpio');
```

Valid Modes:

- `wpi`: sets up pin numbering with `wiringPiSetup`
- `gpio`: sets up pin numbers with `wiringPiSetupGpio`
- `sys`: sets up pin numbers with `wiringPiSetupSys`

See [wiringPi Pins](http://wiringpi.com/pins/) for the differences in Pin numbering;

### `wpi.pinMode(pin, mode)`

```javascript
wpi.pinMode(0, wpi.modes.OUTPUT);
```

- `pin`: pin number
- `mode`: `wpi.modes.INPUT`, `wpi.modes.OUTPUT`, or `wpi.modes.PWM_OUTPUT`

### `wpi.digitalWrite(pin, value)`

```javascript
wpi.digitalWrite(0, wpi.HIGH);
```

```javascript
wpi.digitalWrite(0, wpi.LOW);
```

- `pin`: pin number
- `value`: 0 (`wpi.LOW`) or 1 (`wpi.HIGH`)

### `wpi.digitalRead(pin)`

```javascript
wpi.digitalRead(0);
//=> 1
```

- `pin`: pin number

### Wrapped WiringPI calls:

These have the same arguments as their WiringPi counterparts

- `piBoardRev`
- `pwmSetRange`
- `pwmSetClock`
- `pwmWrite`
