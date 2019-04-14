
/**
 * smart-car-lib
 *
 * @category smart-car-lib
 * @package smart-car-lib
 * @copyright EEENeko (https://github.com/eeeneko)
 * @license GNU General Public License 3.0
 * @version 0.0.1
 */


#ifndef _SMART_CAR_H_
#define _SMART_CAR_H_


#include "Adafruit_NeoPixel.h"  //彩色灯珠驱动
#include "comm.h"               //传感器数据读取
#include "motor.h"              //电机控制


#define PIN            4
#define NUMPIXELS      2

//灯控制实例化
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void car_ini(){

    //串口通信 初始化
    Serial.begin(115200);

    shift_reg_init();   //传感器初始化
    motor_init();       //电机初始化
    pixels.begin();     //彩色灯珠初始化
    //默认led不亮
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
}


class car{

public:
    car(){

        this->_led = &pixels;
        _motor_left = 0;
        _motor_right = 0;
    };

    void led_left(unsigned int val){
        
    };
    void led_left(unsigned int r, unsigned int g, unsigned int b){
        _led->setPixelColor(0, _led->Color(r % 256, g % 256, b % 256));
        _led->show();
    };
    void led(unsigned int val){

    };


    void move(int speed = 0){
        this->_motor_left = speed % 256;
        this-> _motor_right = speed % 256;
    }

    void turn_left(int speed){
        this->_motor_left = 0;
        this->_motor_right = speed;
    }

    void turn_right(int speed){
        this->_motor_left = speed;
        this->_motor_right = 0;
    }

    void stop(){
        this->_motor_right = 0;
        this->_motor_left = 0;
    }


    void core(){

        this->_serial_com();
        motor_set_PWM(this->_motor_left, this->_motor_right);
    }

private:
    Adafruit_NeoPixel *_led;
    int _motor_left;
    int _motor_right;
    unsigned int _serialCmd;

    void _serial_com(){

        if (Serial.available() > 0){
            _serialCmd = Serial.read();
            if(_serialCmd == '0') this->stop();
            if(_serialCmd == '1') this->move(-100);
            if(_serialCmd == '2') this->move(100);
            if(_serialCmd == '3') this->turn_left(100);
            if(_serialCmd == '4') this->turn_right(100);
            if(_serialCmd == '5') this->turn_left(-100);
            if(_serialCmd == '6') this->turn_right(-100);
        }
    };


};




#endif
