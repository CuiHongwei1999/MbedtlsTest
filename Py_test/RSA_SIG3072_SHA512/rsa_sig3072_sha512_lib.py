#!/usr/bin/python

import json
import binascii
from Crypto.Hash import SHA512
from Crypto.Signature import PKCS1_v1_5
from Crypto.PublicKey import RSA



with open('../testvectors/rsa_signature_3072_sha512_test.json', 'r') as f:
    data = json.load(f)

test_groups = data['testGroups']

for i in range(len(test_groups)):
    group = test_groups[i]
    
    print("====================================================")
    print("keySize: " + str(group['keysize']))
    print("SHA: " + str(group['sha']))
    print("type: " + group['type'])
    print("public exponent: " + group['e'])
    print("====================================================")

    tests = group['tests']
    public_key = group['keyPem']
    
    rsa_key = RSA.importKey(public_key)
    sign = PKCS1_v1_5.new(rsa_key)
    
    for j in range(len(tests)):
        test_samp = tests[j]
        tcId = test_samp['tcId']
        msg = binascii.unhexlify(test_samp['msg'])
        sig = binascii.unhexlify(test_samp['sig'])
        exp_res = test_samp['result']

        sha_msg = SHA512.new(msg)
        comp = sign.verify(sha_msg, sig)

        if (comp):
            if exp_res == 'valid' or exp_res == 'acceptable':
                print("-- ID: " + str(tcId) + ", PASS-" + exp_res)
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", ERROR" + exp_res)
                print(msg)
                print(sig)
                print("............................")
        else:
            if exp_res == 'invalid':
                print("-- ID: " + str(tcId) + ", PASS-" + exp_res)
            else:
                print("............................")
                print("-- ID: " + str(tcId) + ", ERROR" + exp_res)
                print(msg)
                print(sig)
                print("............................")
print("###### test finished! ######")
            
