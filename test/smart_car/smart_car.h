
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
    Serial.setTimeout(12); 

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
        this->_motor_left = 0;
        this->_motor_right = 0;
        this->_speed = 200;
        this->_serialCmd = new char[8];
        t = millis();
    };

    ~car(){
        delete this->_serialCmd;
    }
    void led_left(unsigned int val){
        
    };
    void led_left(unsigned int r, unsigned int g, unsigned int b){
        _led->setPixelColor(0, _led->Color(r % 256, g % 256, b % 256));
        _led->show();
    };
    void led(unsigned int val){

    };


    void core(){
        reload_shift_reg(); //刷新传感器数据
        this->_serial_com();
        motor_set_PWM((int)((-this->_motor_left) * (float)_speed), (int)((-this->_motor_right) * (float)_speed));
    }

private:
    Adafruit_NeoPixel *_led;
    float _motor_left;
    float _motor_right;
    const char *_serialCmd;
    unsigned int _speed;
    String t_res;
    unsigned long t;

    void _serial_com(){

        if(Serial.available() > 0){
            t_res = Serial.readStringUntil('\n');
            String t_to = (sensor.ir_left_3)?"1":"0";
            t_to += (sensor.ir_left_2)?"1":"0";
            t_to += (sensor.ir_left_1)?"1":"0";
            t_to += (sensor.ir_mid)?"1":"0";
            t_to += (sensor.ir_right_1)?"1":"0";
            t_to += (sensor.ir_right_2)?"1":"0";
            t_to += (sensor.ir_right_3)?"1":"0";
            Serial.println(t_to);

            if(t_res == "Disconnnected!") return;
            this->_serialCmd = t_res.c_str();
            this->_adjust_left_motor(this->_serialCmd[0]);
            this->_adjust_right_motor(this->_serialCmd[1]);
            this->_adjust_speed(this->_serialCmd[2]);
        }

    };

    void _adjust_left_motor(char msg){
        if(msg == '0') this->_motor_left = 0;
        if(msg == '1') this->_motor_left = .2;
        if(msg == '2') this->_motor_left = .4;
        if(msg == '3') this->_motor_left = .6;
        if(msg == '4') this->_motor_left = .8;
        if(msg == '5') this->_motor_left = 1;
        if(msg == 'a') this->_motor_left = -.2;
        if(msg == 'b') this->_motor_left = -.4;
        if(msg == 'c') this->_motor_left = -.6;
        if(msg == 'd') this->_motor_left = -.8;
        if(msg == 'e') this->_motor_left = -1;
    }

    void _adjust_right_motor(char msg){
        if(msg == '0') this->_motor_right = 0;
        if(msg == '1') this->_motor_right = .2;
        if(msg == '2') this->_motor_right = .4;
        if(msg == '3') this->_motor_right = .6;
        if(msg == '4') this->_motor_right = .8;
        if(msg == '5') this->_motor_right = 1;
        if(msg == 'a') this->_motor_right = -.2;
        if(msg == 'b') this->_motor_right = -.4;
        if(msg == 'c') this->_motor_right = -.6;
        if(msg == 'd') this->_motor_right = -.8;
        if(msg == 'e') this->_motor_right = -1;
    }

    void _adjust_speed(char msg){
        if(msg == 'A') this->_speed = 0;
        if(msg == 'B') this->_speed = 100;
        if(msg == 'C') this->_speed = 200;
        if(msg == 'D') this->_speed = 255;
    }
    static void _send(){

    }

};





#endif
