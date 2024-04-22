import random


def setInnerParameters(K, Ti, Tr, beta, H, integratorOn):
    # print('Setting inner parameters')

    print('Set the inner parameters to ', K, Ti, Tr, beta, H, integratorOn)
    return 0


def getInnerParameters():
    # print('Returning inner parameters')

    K = 1
    Ti = 2
    Tr = 3
    beta = 4
    H = 5
    integratorOn = True

    print('Retrieved the inner parameters ', K, Ti, Tr, beta, H, integratorOn)
    return K, Ti, Tr, beta, H, integratorOn


def setOuterParameters(K, Ti, Td, Tr, N, beta, H, integratorOn):
    # print('Setting outer parameters')
    print('Set the outer parameters to ', K, Ti, Td, Tr, N, beta, H, integratorOn)
    return 0


def getOuterParameters():
    # print('Returning outer parameters')

    K = 1
    Ti = 2
    Td = 3
    Tr = 3
    N = 3
    beta = 4
    H = 5
    integratorOn = True

    print('Retrieved the outer parameters ', K, Ti, Td, Tr, N, beta, H, integratorOn)
    return K, Ti, Td, Tr, N, beta, H, integratorOn


def setMode(mode):
    print('Mode set to ', mode)
    return 0


def setRef(newRef):
    # print('Reference set to ', newRef)
    return 0


def shutDown():
    print('Shutting down')
    return 0


def getControlData():
    # print('Returning control data')

    x = 1
    u = random.uniform(-10, 10)
    return x, u


def getMeasurementData():
    # print('Returning measurement data')

    x = 1
    yRef = 2
    y = 3

    return x, yRef, y
