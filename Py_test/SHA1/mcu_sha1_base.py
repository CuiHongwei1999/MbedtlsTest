#!/usr/bin/python

import serial
import serial.tools.list_ports
import struct
import time

# find the attached serial ports
port_list = list(serial.tools.list_ports.comports())

# remove devices that are not descripted with key words CH340
dev_list_len = len(port_list)
ind = 0;
while(ind < dev_list_len):
    # print(port_list[i].device, port_list[i].name, port_list[i].description)
    if "CH340" in port_list[ind].description:
        ind += 1;
    else:
        del port_list[ind]
        dev_list_len -= 1


# check the number of attached USB serial devices
if len(port_list) == 0:
    raise Exception("No connected USB serial device!")
else:
    print(str(len(port_list)) + " USB serial device(s) found:")
    for i in range(len(port_list)):
        print(port_list[i])

if len(port_list) > 1:
    raise Warning("More than 1 CH340 serial devices connected to the USB port")

print("====================================================")

ser = serial.Serial(port_list[0].device, 115200, timeout = 1.5)
ser_sta = ser.isOpen()
if ser_sta:
    print (port_list[0].device + " opened successfully!")
else:
    print (port_list[0].device + " open failed!")
    raise Exception("Serial port open failed!")

print("====================================================")

key = 'abcDfff'
msg = '1234'

key_len = len(key)
msg_len = len(msg)

headL = 'L'.encode()
headD = 'D'.encode()

len_pkg = headL + key_len.to_bytes(4, 'big') + msg_len.to_bytes(4, 'big')
ser.write(len_pkg)

data_read = ser.read(1)

data_pkg = headD + key.encode() + msg.encode()
ser.write(data_pkg)

data_read = ser.read(20)
print(data_read)

ser.close()
            
