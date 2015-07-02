#!/home/hiro/local/Python-2.7.9/bin/python
from irdb import irdb
from irdev import irdev
import config as cfg

import sys

def main():
    argv = sys.argv
    if len(argv) != 3 :
        print "# Argument error #"
        print "# Usage : %s Command Argment" % argv[0]
        return 1

    db = irdb()
    dev = irdev(cfg.serial_port, cfg.serial_baud)

    cmd = argv[1]
    if cmd == 'read':
        key = argv[2]
        dat = dev.read_code()
        db.store(key, dat)
        print "#---------IR CODE ------------#"
        print dat
        print "#------- END OF IR CODE ------#"

    if cmd == 'transmit':
        key = argv[2]
        dat = db.get(key)
        dev.transmit(dat)
        print "Transmitted : %s" % key

if __name__ == "__main__":
    ret = main()
    sys.exit(ret)

