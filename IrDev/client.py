#!/home/hiro/local/Python-2.7.9/bin/python
import config as cfg
import socket, sys

def main():
    argv = sys.argv
    argc = len(argv)

    service_prop = (cfg.SERVICE_HOST, cfg.SERVICE_PORT)
    try :
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(service_prop)
    except :
        print "Failed to connect the server : (%s:%s)" % service_prop
        return 1

    ret = 1
    if argc == 2 and argv[1] == 'keys' :
        s.send("keys\n")
        buf = s.recv(128)
        print buf
        s.send("\n")
        ret = 0
    
    if argc == 3 :
        cmd, key = argv[1:]
        if cmd == "transmit" or cmd == "store" : 
            s.send(cmd + " " + key + "\n")
            # print s.recv(128)
            ret = 0

    s.close()
    return ret

if __name__ == "__main__":
    ret = main()
    sys.exit(ret)

