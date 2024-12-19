#include <stdio.h>
#include "pico/stdlib.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define SHIFT_REGISTER_PINS 24

static inline void s29_wait(){
    asm(
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"
        "NOP \n\t"

        
    );
    
   //sleep_us(1);
}

void s29_set(uint gpio, bool state) {

    gpio_put(gpio,state);
    s29_wait();

}

const int d[16] = {

    0,
    1,

    2,
    3,
    4,
    5,

    6,
    7,
    8,
    9,

    10,
    11,
    12,
    13,

    14,
    15

};

const int SER    = 16;
const int OE     = 17;
const int RCLK   = 18;
const int SRCLK  = 19;
//const int SRCLR  = 20;

const int S29_CE = 20;
const int S29_OE = 21;
const int A_1    = 22;


void init_data_pins() {

    int len = sizeof(d)/sizeof(d[0]);

    for(int i = 0; i < len; ++i) {

        gpio_init(d[i]);
        gpio_set_dir(d[i],GPIO_IN);
        //gpio_pull_down(d[i]);

    }

    gpio_init(S29_CE);
    gpio_init(S29_OE);

    gpio_set_dir(S29_CE,GPIO_OUT); 
    gpio_set_dir(S29_OE,GPIO_OUT); 

    gpio_put(S29_CE,1);
    s29_set(S29_OE,1);
}


void sr_write_bit(u8 bit) {

    s29_set(SRCLK,0);
    s29_set(SER,bit);
    s29_set(SRCLK,1);

}

void sr_addr_send() {

    s29_set(OE,1);
    s29_set(RCLK,1);
    s29_set(OE,0);
    s29_set(RCLK,0);

    gpio_put(S29_CE,0);
    s29_set(S29_OE,0);

}


void sr_end_addr_send() {
    
    s29_set(S29_OE,1);
    s29_set(S29_CE,1);

    s29_set(OE,0);
    s29_set(RCLK,0);

}


void sr_write_bit_and_set(u8 bit) {

    s29_set(OE,1);
    s29_set(RCLK,0);
    sr_write_bit(bit);
    s29_set(RCLK,1);
    s29_set(OE,0);
    s29_set(RCLK,0);

}



void init_sr_pins() {

        gpio_init(SER);
        gpio_set_dir(SER,GPIO_OUT);

        gpio_init(OE);
        gpio_set_dir(OE,GPIO_OUT);

        gpio_init(RCLK);
        gpio_set_dir(RCLK,GPIO_OUT);

        gpio_init(SRCLK);
        gpio_set_dir(SRCLK,GPIO_OUT);

        gpio_init(A_1);
        gpio_set_dir(A_1,GPIO_OUT);

        gpio_put(OE,1);
        gpio_put(RCLK,0);
    
}

u16 sr_read_data(int pin_mode) {
    //s29_wait();
    u16 ret = 0;

    for(int i = 0; i < pin_mode; ++i)
        ret |= gpio_get(d[i])<<i;

    return ret;

}

void sr_clear() {
    s29_set(OE,1);

    for(int i = 0; i < SHIFT_REGISTER_PINS; ++i)
        sr_write_bit(0);

    s29_set(RCLK,1);
    s29_set(RCLK,0);


}






