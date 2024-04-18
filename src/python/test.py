import ctypes

comms_lib = ctypes.CDLL('../../bin/test.so',  ctypes.RTLD_GLOBAL)

test_c = comms_lib.test
test_c.restype = ctypes.c_int
result = test_c()
print("result: ", result)