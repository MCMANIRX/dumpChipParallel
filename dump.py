import serial
import time

addr = 0
sector = 0
start_time = time.time()

ser = serial.Serial ("COM9", 115200,timeout=3)

f = open('flash.bin', 'wb')

clear = "clr\r\n"
ser.write(clear.encode())
time.sleep(0.5)
addr = 0x0 #0x68000 + 0x5c0
for i in range(0,64 *1024 *128):
    


    if(addr%(1024*64)==0):

        sector+=1
        print("reading sector ",sector,"(",addr,"bytes )...")

    payload = '{:X}'.format(addr)
    payload+='h' 
    payload+='\r\n'   
    ser.write(payload.encode())
    buffer = ser.readline()[:-2].decode('utf-8')
    if not buffer:
        print("ERROR!")
        break
    
    #print(buffer)
    if 'ERR' in buffer:
        print("ERROR!",addr)
        print(buffer)
        #print("Error at ",hex(payload),"! maybe I should start day trading...")
        break  
    #print(hex(int(buffer,16)),bin(addr),hex(addr),addr)
    f.write(int(buffer,16).to_bytes(1,"big"))
        
    addr+=1
    
    
print("done")
#ser.write(clear.encode())
#ser.close()

end_time = time.time()

elapsed_time = end_time - start_time

print(f"Start Time: {start_time}")
print(f"End Time: {end_time}")
print(f"Elapsed Time: {elapsed_time:.2f} seconds")