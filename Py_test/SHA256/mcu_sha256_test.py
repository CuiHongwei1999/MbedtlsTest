#!/usr/bin/python

import serial
import serial.tools.list_ports
import struct
import time
import json
import binascii
import hmac
import hashlib

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

with open('../testvectors/hmac_sha256_test.json', 'r') as f:
    data = json.load(f)

test_groups = data['testGroups']

headL = 'L'.encode()
headD = 'D'.encode()

time_start = time.time()

for i in range(len(test_groups)):
    group = test_groups[i]
    
    # print("====================================================")
    # print("keySize: " + str(group['keySize']))
    # print("tagSize: " + str(group['tagSize']))
    # print("type: " + group['type'])
    # print("====================================================")

    tests = group['tests']
    tag_size = group['tagSize']

    for j in range(len(tests)):
        test_samp = tests[j]
        key = binascii.unhexlify(test_samp['key'])
        msg = binascii.unhexlify(test_samp['msg'])
        tag = binascii.unhexlify(test_samp['tag'])
        exp_res = test_samp['result']
        tcId = test_samp['tcId']
        comment = test_samp['comment']

        key_len = len(key)
        msg_len = len(msg)
        len_pkg = headL + key_len.to_bytes(4, 'big') + msg_len.to_bytes(4, 'big')

        ser.write(len_pkg)
        data_read = ser.read(1)

        data_pkg = headD + key + msg

        ser.write(data_pkg)
        
        ver = ser.read(32)
        comp = hmac.compare_digest(tag, ver[0: int(tag_size / 8)])
        
        if (comp):
            if exp_res == 'valid':
                # print("-- ID: " + str(tcId) + ", Valid-PASS")
                continue
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", Invalid-ERROR")
                print(tag)
                print(ver)
                print("............................")
        else:
            if exp_res == 'invalid':
                # print("-- ID: " + str(tcId) + ", Invalid-PASS")
                continue
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", Valid-ERROR")
                print(tag)
                print(ver)
                print("............................")

time_end = time.time()
ser.close()
print("time cost: ", time_end - time_start, 's') 
print("###### test finished! ######")
