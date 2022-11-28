import binascii
import hmac
import hashlib
key = binascii.unhexlify("06c0dcdc16ff81dce92807fa2c82b44d28ac178a")
msg = binascii.unhexlify("")
tag = binascii.unhexlify("7d91d1b4748077b28911b4509762b6df24365810")

print(hmac.compare_digest(tag, hmac.HMAC(key, msg, hashlib.sha1).digest()))
