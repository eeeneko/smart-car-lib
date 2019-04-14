#include "smart_car.h"

void setup()
{
    car_ini();
}

car car;

void loop()
{
    //car.led(millis(),millis(),millis());

    car.core();
  
}

