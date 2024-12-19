#include "hardware/irq.h"
#include "pico/bootrom.h"

#include "pin_io.h"



#define BYTE_MODE 8







u8 str[100] = {0};
u8 str_idx = 0;


bool marker_found(char *marker) {
    int pos = 0;  // correct characters received so far
    for(int i = 0 ; i < str_idx; ++i){
    char c= str[i];

    if (c == -1) return false;  // shortcut for a common case
    if (c == marker[pos]) {
        pos++;
       // putchar(pos+'0');                      // count a correct character
        if (marker[pos] == '\0') {  // we got the complete string
            pos = 0;                // reset for next time
            return true;            // report success

        }
    } else if (c == marker[0]) {
        pos = 1;        // we have the initial character again
    } else {
        pos = 0;        // we received a wrong character
    }
    }
    return false;
}



u16 s29_read(u32 addr) {

    sr_clear();

    int bit = addr&1;

    if(BYTE_MODE==8){
        addr = addr>>1;
        gpio_put(A_1,bit);
    }
    


    for(int i = 0; i < 32; ++i)
        sr_write_bit((addr>>(31-i))&1);

    u16 ret = 0;
    sr_addr_send();
    ret = sr_read_data(BYTE_MODE);
    sr_end_addr_send();

    if(BYTE_MODE==8)    
        gpio_put(A_1,0);


    return ret;

}




void read_from_address_and_write() {


    int idx = 0;
    char c = '\0';

    u32 addr = 0;
    str_idx-=2;

    for(int i = 0; i < str_idx; ++i){
        c = str[i];
        addr+=  (c - (c > 0x39 ? 0x37 : 0x30))* pow(16,str_idx-(i+1));
    }
    addr&=0xffffff;
    //printf("addr: %x\n");
    u16 ret_0 = 0;
    u16 ret_1 = 0;


int checks = 0;


while(checks<8) {
    ret_0 = s29_read(addr);
    ret_1 = s29_read(addr);
    checks++;
    if(ret_0 != ret_1){
        checks = 0;
        s29_wait();
    }
            //goto try_again;
            //printf("ERR %02x %02x\n",ret_0,ret_1);
            //exit(0);
        }
    u16 ret = ret_1;

    if(BYTE_MODE==16)
         ret = ((ret_1 << 8)&0xff00) | ((ret_1 >> 8)&0xff);
  // putchar((ret_1>>8)&0xff);  
  // putchar(ret_1&0xff);
   printf("%02x\n",ret);
 //   putchar('\n');
    
}

void console() {
    str_idx =0;

    while(1){

        u8 in = getchar();

        if(in){
            //putchar(in);
            str[str_idx++]  = in;

            if(in == 0xd){

                //putchar('\n');
                if(marker_found("bootrom"))
                    reset_usb_boot(0,0);
                
                else if(marker_found("h"))
                    read_from_address_and_write();
                
                else if(marker_found("wb"))
                    sr_write_bit_and_set(str[str_idx-2]-'0');

                else if(marker_found("clr"))
                    sr_clear();
                
                str_idx = 0;


                }

            }
            
            else if(in == 0x8 && str_idx>0)
                str_idx -=1;

        }

    }



int main() {

    stdio_init_all();

    init_sr_pins();
    sr_clear();
    
    console();



    return 0 ;

}




