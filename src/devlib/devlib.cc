#include "devlib.h"

#include "ds1302.h"
#include "gertboard.h"
#include "lcd.h"
#include "lcd128x64.h"
#include "maxdetect.h"
#include "piFace.h"
#include "piGlow.h"
#include "piNes.h"
//#include "tcs34725.h"

IMPLEMENT_EXPORT_INIT(devlib) {
  INIT(ds1302);
  INIT(gertboard);
  INIT(lcd);
  INIT(lcd128x64);
  INIT(maxdetect);
  INIT(piFace);
  INIT(piGlow);
  INIT(piNes);
  //INIT(tcs34725);
}
