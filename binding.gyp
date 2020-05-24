{
  'targets': [
    {
      'target_name': 'wiringPi',
      'sources': [
        'src/addon.cc',
        
        'src/wiringPi.cc',
        'src/softPwm.cc',
        'src/softServo.cc',
        'src/softTone.cc',
        'src/wiringPiI2C.cc',
        'src/wiringPiSPI.cc',
        'src/wiringSerial.cc',
        'src/wiringShift.cc',
        'src/wiringPiISR.cc',
        'src/wpi.cc',
        
        'src/extensions/extensions.cc',
        'src/extensions/drcSerial.cc',
        'src/extensions/max5322.cc',
        'src/extensions/max31855.cc',
        'src/extensions/mcp23s08.cc',
        'src/extensions/mcp23s17.cc',
        'src/extensions/mcp3002.cc',
        'src/extensions/mcp3004.cc',
        'src/extensions/mcp3422.cc',
        'src/extensions/mcp4802.cc',
        'src/extensions/mcp23008.cc',
        'src/extensions/mcp23016.cc',
        'src/extensions/mcp23017.cc',
        'src/extensions/pcf8574.cc',
        'src/extensions/pcf8591.cc',
        'src/extensions/sn3218.cc',
        'src/extensions/sr595.cc',
        'src/extensions/pca9685.cc',
        'src/extensions/dac7678.cc',
        
        'src/devlib/devlib.cc',
        'src/devlib/ds1302.cc',
        'src/devlib/gertboard.cc',
        'src/devlib/lcd.cc',
        'src/devlib/lcd128x64.cc',
        'src/devlib/maxdetect.cc',
        'src/devlib/piFace.cc',
        'src/devlib/piGlow.cc',
        'src/devlib/piNes.cc'
      ],
      'include_dirs': [
        'WiringPi/wiringPi',
        'WiringPi/devLib'
      ],
      'libraries': [
        '<!(pwd)/WiringPi/wiringPi/libwiringPi.a',
        '<!(pwd)/WiringPi/devLib/libwiringPiDev.a'
      ],
      'cflags': [
        '-Wall'
      ]
    }
  ]
}
