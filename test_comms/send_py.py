import subprocess

proc = subprocess.Popen(["./recieve_c.out"], stdin=subprocess.PIPE)

data = "from python\n"

proc.stdin.write(data.encode())
proc.stdin.close()

proc.wait()
proc.terminate()
