#!/usr/bin/python

import json
import binascii
import hmac
import hashlib


with open('../testvectors/hmac_sha1_test.json', 'r') as f:
    data = json.load(f)

test_groups = data['testGroups']

for i in range(len(test_groups)):
    group = test_groups[i]
    
    print("====================================================")
    print("keySize: " + str(group['keySize']))
    print("tagSize: " + str(group['tagSize']))
    print("type: " + group['type'])
    print("====================================================")

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

        ver = hmac.HMAC(key, msg, hashlib.sha1).digest()
        comp = hmac.compare_digest(tag, ver[0: int(tag_size / 8)])
        
        if (comp):
            if exp_res == 'valid':
                print("-- ID: " + str(tcId) + ", Valid-PASS")
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", Invalid-ERROR")
                print(tag)
                print(ver)
                print("............................")
        else:
            if exp_res == 'invalid':
                print("-- ID: " + str(tcId) + ", Invalid-PASS")
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", Valid-ERROR")
                print(tag)
                print(ver)
                print("............................")

print("###### test finished! ######")
            
