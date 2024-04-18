import ctypes

comms_lib = ctypes.CDLL('./test.so',  ctypes.RTLD_GLOBAL)
print("called python program")
test_c = comms_lib.test
test_c.restype = ctypes.c_int
result = test_c()
print("result: ", result)