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

with open('../testvectors/rsa_signature_3072_sha256_test.json', 'r') as f:
    data = json.load(f)

test_groups = data['testGroups']

headL = 'L'.encode()
headD = 'D'.encode()

time_start = time.time()

for i in range(len(test_groups)):
    group = test_groups[i]
    
    # print("====================================================")
    # print("keySize: " + str(group['keysize']))
    # print("SHA: " + str(group['sha']))
    # print("type: " + group['type'])
    # print("public exponent: " + group['e'])
    # print("====================================================")

    tests = group['tests']

    for j in range(len(tests)):
        if j == 0:
            public_key = group['keyPem'].encode()
        else:
            public_key = ''.encode()
            
        test_samp = tests[j]
        tcId = test_samp['tcId']
        msg = binascii.unhexlify(test_samp['msg'])
        sig = binascii.unhexlify(test_samp['sig'])
        exp_res = test_samp['result']
        
        pub_key_len = len(public_key)
        sig_len = len(sig)
        msg_len = len(msg)
        len_pkg = headL + pub_key_len.to_bytes(4, 'big') + sig_len.to_bytes(4, 'big') + msg_len.to_bytes(4, 'big')

        ser.write(len_pkg)
        data_read = ser.read(1)

        data_pkg = headD + public_key + sig + msg

        ser.write(data_pkg)
        
        ver = ser.read(5)

        if ver == b'PASS\x00':
            if exp_res == 'valid' or exp_res == 'acceptable':
                # print("-- ID: " + str(tcId) + ", PASS-" + exp_res)
                continue
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", ERROR-" + exp_res)
                print (ver)
                print("............................")
        elif ver == b'ERR\x00\x00':
            if exp_res == 'invalid':
                # print("-- ID: " + str(tcId) + ", PASS-" + exp_res)
                continue
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", ERROR-" + exp_res)
                print (ver)
                print("............................")
        else:
            print (ver)

time_end = time.time()
ser.close()
print("time cost: ", time_end - time_start, 's') 
print("###### test finished! ######")
