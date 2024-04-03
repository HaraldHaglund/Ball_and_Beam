import ctypes

comms_lib = ctypes.CDLL('../../bin/receiver.so')


def setInnerParameters(K, Ti, Tr, beta, H, integratorOn):
    """
    Sets parameters for inner controller.

    Args:
        K (double): K
        Ti (double): Ti
        Tr (double): Tr
        beta (double): beta
        H (double): H
        integratorOn (int): 1 if integrator is on or 0 if not (needs to be int for c reasons)

        Returns:
        int: 0 if sucessfull, non-0 of not 
    """
    setInnerParameters_c = comms_lib.setInnerParameters
    setInnerParameters_c.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int]

    setInnerParameters_c.restype = ctypes.c_int
    return setInnerParameters_c(K, Ti, Tr, beta, H, integratorOn)

def getInnerParameters():
    """
    Gets parameters for inner controller.

    Args:

    Returns:
        double: K
        double: Ti
        double: Tr
        double: beta
        double: H
        int: integratorOn. 1 if integrator is on or 0 if not (needs to be int for c reasons)
    """
    getInnerParameters_c = comms_lib.getInnerParameters
    getInnerParameters_c.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int)]
    getInnerParameters_c.restype = ctypes.c_int

    K = ctypes.c_double()
    Ti = ctypes.c_double()
    Tr = ctypes.c_double()
    beta = ctypes.c_double()
    H = ctypes.c_double()
    integratorOn = ctypes.c_int()

    getInnerParameters_c(ctypes.byref(K), ctypes.byref(Ti), ctypes.byref(Tr), ctypes.byref(beta), ctypes.byref(H), ctypes.byref(integratorOn))
    return K.value, Ti.value, Tr.value, beta.value, H.value, integratorOn.value

def setOuterParameters(K, Ti, Td, Tr, N, beta, H, integratorOn):
    """
    Sets parameters for outer controller.

    Args:
        K (double): K
        Ti (double): Ti
        Td (double): Td
        Tr (double): Tr
        N (double): N
        beta (double): beta
        H (double): H
        integratorOn (int): 1 if integrator is on or 0 if not (needs to be int for c reasons)

        Returns:
        int: 0 if sucessfull, non-0 of not 
    """
    setOuterParameters_c = comms_lib.setOuterParameters
    setOuterParameters_c.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int]

    setOuterParameters_c.restype = ctypes.c_int
    return setOuterParameters_c(K, Ti, Td, Tr, N, beta, H, integratorOn)

def getOuterParameters():
    """
    Gets parameters for outer controller.

    Args:

    Returns:
        double: K
        double: Ti
        double: Td
        double: Tr
        double: N
        double: beta
        double: H
        int: integratorOn. 1 if integrator is on or 0 if not (needs to be int for c reasons)
    """

    getOuterParameters_c = comms_lib.getOuterParameters
    getOuterParameters_c.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int)]
    getOuterParameters_c.restype = ctypes.c_int

    K = ctypes.c_double()
    Ti = ctypes.c_double()
    Td = ctypes.c_double()
    Tr = ctypes.c_double()
    N = ctypes.c_double()
    beta = ctypes.c_double()
    H = ctypes.c_double()
    integratorOn = ctypes.c_int()

    getOuterParameters_c(ctypes.byref(K), ctypes.byref(Ti), ctypes.byref(Td), ctypes.byref(Tr), ctypes.byref(N), ctypes.byref(beta), ctypes.byref(H), ctypes.byref(integratorOn))
    return K.value, Ti.value, Td.value, Tr.value, N.value, beta.value, H.value, integratorOn.value

def setMode(mode):
    """
    Sets Mode for regulation. OFF = 0, BEAM = 1, BALL = 2

    Args:
        mode (int): mode for regulation. OFF = 0, BEAM = 1, BALL = 2

    Returns:
        int: 0 if sucessfull, non-0 of not 
    """
    setMode_c = comms_lib.setModeFromPy
    setMode_c.argtypes = [ctypes.c_int]

    setMode_c.restype = ctypes.c_int
    return setMode_c(mode)

def setRef(newRef):
    """
    Sets reference value for regulation

    Args:
        newRef (double): New reference value

    Returns:
        int: 0 if sucessfull, non-0 of not 
    """
    setRef_c = comms_lib.setRefFromPy
    setRef_c.argtypes = [ctypes.double]

    setRef_c.restype = ctypes.c_int
    return setRef_c(newRef)

def shutDown():
    """
    Does nothing yet

    Args:

    Returns:
        int: 0 if sucessfull, non-0 of not 
    """

    shutDown_c = comms_lib.shutDownFromPy
    shutDown_c.argtypes = []

    shutDown_c.restype = ctypes.c_int
    return shutDown_c()

def getControlData():
    """
    Gets control data.

    Args:

    Returns:
        double: x value
        double: u value
    """
    getControlData_c = comms_lib.getControlData
    getControlData_c.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
    getControlData_c.restype = ctypes.c_int

    x = ctypes.c_double()
    u = ctypes.c_double()

    getControlData_c(ctypes.byref(x), ctypes.byref(u))
    return x.value, u.value

def getMeasurementData():
    """
    Gets measurement data.

    Args:

    Returns:
        double: x value
        double: yRef value
        double: y value
    """
    getMeasurementData_c = comms_lib.getMeasurementData
    getMeasurementData_c.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
    getMeasurementData_c.restype = ctypes.c_int

    x = ctypes.c_double()
    yRef = ctypes.c_double()
    y = ctypes.c_double()

    getMeasurementData_c(ctypes.byref(x), ctypes.byref(yRef), ctypes.byref(y))
    return x.value, yRef.value, y.value