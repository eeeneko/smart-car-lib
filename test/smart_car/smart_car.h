
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
        this->_auto = false;

        this->read_sensor_values();                      //读取初值 
        this->calc_pid();                                //计算pid 
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

    void adjust_cmd(){
        if(this->_motor_left > 1) this->_motor_left = 1;
        if(this->_motor_left < -1) this->_motor_left = -1;
        if(this->_motor_right > 1) this->_motor_right = 1;
        if(this->_motor_right < -1) this->_motor_right = -1;
        
    }

    void core(){
        f_motor_left = _motor_left;
        f_motor_right = _motor_right;
        reload_shift_reg(); //刷新传感器数据
        this->_serial_com();
        if(this->_auto)this->_auto_run();
        adjust_cmd();
        motor_set_PWM((int)((-this->_motor_left) * (float)_speed), (int)((-this->_motor_right) * (float)_speed));
    }

private:
    Adafruit_NeoPixel *_led;
    float _motor_left;
    float _motor_right;
    float f_motor_left;
    float f_motor_right;
    const char *_serialCmd;
    unsigned int _speed;
    String t_res;
    unsigned long t;
    bool _auto;

    float Kp = 10, Ki = 0.5, Kd = 0;                    //pid弯道参数参数 
    float error = 0, P = 0, I = 0, D = 0, PID_value = 0;//pid直道参数 
    float decide = 0;                                   //元素判断
    float previous_error = 0, previous_I = 0;           //误差值 
    int _sensor[5] = {0, 0, 0, 0, 0};    
    int initial_motor_speed = 60;


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
            t_to += PID_value;
            Serial.println(t_to);

            if(t_res == "Disconnnected!") return;
            this->_serialCmd = t_res.c_str();
            this->_adjust_left_motor(this->_serialCmd[0]);
            this->_adjust_right_motor(this->_serialCmd[1]);
            this->_adjust_speed(this->_serialCmd[2]);
            this->_change_mode(this->_serialCmd[3]);
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

    void _change_mode(char msg){
        if(msg == 'A') this->_auto = true;
        if(msg == 'B') this->_auto = false;
    }
    void _auto_run(){
        read_sensor_values();
        calc_pid();
        //this->_motor_left = (float)(initial_motor_speed + PID_value)/255;
        //this->_motor_right = (float)(initial_motor_speed - PID_value)/255;

    }


    void read_sensor_values(){
      static unsigned int p = 0;
      static int e = 0;

      if(sensor.ir_left_3){
          e = -3;
          p = 0;
          this->_motor_left = 0;
          this->_motor_right = .8;
      }else if(sensor.ir_right_3){
          e = 3;
          p = 0;
          this->_motor_left = .8;
          this->_motor_right = 0;
      }else if(sensor.ir_mid){
          e = 0;
          p = 0;
          this->_motor_left = 1;
          this->_motor_right = 1;
      }else if(sensor.ir_left_1){
          e = -1;
          p = 0;
          this->_motor_left = .7;
          this->_motor_right = 1;
      }else if(sensor.ir_right_1){
          e = 1;
          p = 0;
          this->_motor_left = 1;
          this->_motor_right = .7;
      }else if(sensor.ir_left_2){
          e = -2;
          p = 0;
          this->_motor_left = .6;
          this->_motor_right = 1;
      }else if(sensor.ir_right_2){
          e = 2;
          p = 0;
          this->_motor_left = .6;
          this->_motor_right = 1;
      }

      
      /*
      //0001000
      if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = 1;
          this->_motor_right = 1;
      }
      //0010000
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = -1;
          this->_motor_left = .4;
          this->_motor_right = .7;
      }
      //0000100
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 1;
          this->_motor_left = .7;
          this->_motor_right = .4;
      }
      //0100000
      else if(!sensor.ir_left_3 && sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = -2;
          this->_motor_left = .1;
          this->_motor_right = .3;
      }
      //0000010
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && sensor.ir_right_2 && !sensor.ir_right_3){
          error = 2;
          this->_motor_left = .3;
          this->_motor_right = .1;
      }
      //1000000
      else if(sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = -3;
          this->_motor_left = 0;
          this->_motor_right = .5;
      }
      //0000001
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && sensor.ir_right_3){
          error = 3;
          this->_motor_left = .5;
          this->_motor_right = 0;
      }
   /*   //0011000
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && sensor.ir_left_1 && sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = .6;
          this->_motor_right = .8;
      }
      //0001100
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && sensor.ir_mid && sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = .8;
          this->_motor_right = .6;
      }
      //0111000
      else if(!sensor.ir_left_3 && sensor.ir_left_2 && sensor.ir_left_1 && sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = 0;
          this->_motor_right = .5;
      }
      //0001110
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && sensor.ir_mid && sensor.ir_right_1 && sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = .5;
          this->_motor_right = 0;
      }

      else if(!sensor.ir_left_3 && sensor.ir_left_2 && sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = .3;
          this->_motor_right = .6;
      }

      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && sensor.ir_right_1 && sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = .6;
          this->_motor_right = .3;
      }

      else if(sensor.ir_left_3 && sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
          this->_motor_left = 0;
          this->_motor_right = .4;
      }

      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && sensor.ir_right_2 && sensor.ir_right_3){
          error = 0;
          this->_motor_left = .4;
          this->_motor_right = 0;
      }
*/
/*
      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
      }

      else if(!sensor.ir_left_3 && !sensor.ir_left_2 && !sensor.ir_left_1 && !sensor.ir_mid && !sensor.ir_right_1 && !sensor.ir_right_2 && !sensor.ir_right_3){
          error = 0;
      }
*/

      else{
          this->_motor_left = this->f_motor_left;
          this->_motor_right = this->f_motor_right;
          p++;

          if(p > 600){
              this->_motor_left = -0.5;
               this->_motor_right = -0.5;
          }
      }
      error = e;
    }

      void calc_pid()
      {
        P = error;
        I = I + error;
        D = error - previous_error;
       
        PID_value = (Kp * P) + (Ki * I) + (Kd * D);
       
        previous_error = error;

        error = 0;
      }


};





#endif
