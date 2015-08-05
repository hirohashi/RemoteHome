#!/home/hiro/local/Python-2.7.9/bin/python
import socket, time
from irdev import irdev
from irdb import irdb
from config import *

db = irdb()

def handle_cmd(dev, client, cmd):
    cmds = cmd.split(" ")
    if len(cmds) == 1 and cmds[0] == "keys":
        s = ""
        keys = db.keys()
        keys.sort()
        for key in keys:
            s += "%s " % key
        s += "\r\n"
        client.send(s)
        return True
    
    if len(cmds) != 2 : return False

    if cmds[0] == 'transmit' :
        key = cmds[1]
        if not key in db.keys() : return False
        dat = db.get(cmds[1])
        dev.transmit(dat)
        return True

    if cmds[0] == 'store' :
        key = cmds[1]
        if key in db.keys() : return False
        dat = dev.read_code()
        db.store(key, dat)
        return True
    
    return False

def service(dev, client):
    buf = ""
    while True:
        buf += client.recv(1024)
        idx = buf.find("\n")
        if idx == -1 : continue
        cmd = buf[:idx].strip()
        buf = buf[idx+1:]
        if cmd == "" : break
        ret = handle_cmd(dev, client, cmd)
        if ret : client.send("OK\n")
        else : client.send("NG\n")
    client.close()

def main():
    dev = irdev(DEVICE_PATH, DEVICE_BAUD)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((SERVICE_HOST, SERVICE_PORT))
    s.listen(1)

    while True :
        client, addr = s.accept()
        service(dev, client)

if __name__ == "__main__":
    main()

