import subprocess

def setInnerParameters(beta, H, integratorOn, K, Ti, Tr):
    path = "./comms/setInnerParameters"
    data = [beta, H, integratorOn, K, Ti, Tr]
    write(path, data)

def setOuterParameters(beta, H, integratorOn, K, N, Td, Ti, Tr):
    path = "./comms/setOuterParameters"
    data = [beta, H, integratorOn, K, N, Td, Ti, Tr]
    write(path, data)

#values are OFF = 0, BEAM = 1, BALL = 2
def setMode(mode):
    path = "./comms/setMode"
    data = [mode]
    write(path, data)

def setRef(newRef):
    path = "./comms/setRef"
    data = [str(newRef)]
    write(path, data)

def shutDown():
    path = "./comms/shutDown"
    data = []
    write(path, data)

def write(path, data):
    proc = subprocess.Popen([path], stdin=subprocess.PIPE)
    for send in data:
        proc.stdin.write(send.encode())
    
    proc.stdin.close()
    proc.wait()
    proc.terminate()