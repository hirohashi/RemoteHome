import serial
import time

class irdev:
    def __init__(self, fname_dev="/dev/ttyACM0", baudrate=115200):
        self.fname_dev = fname_dev
        self.baudrate = baudrate
        self.ser = serial.Serial(self.fname_dev, self.baudrate, timeout=0)
        time.sleep(2)

    def read_code(self, timeout=30):
        self.txcmd('rx:enable')
        s_time = time.time()
        while True :
            time.sleep(0.1)
            if time.time() - s_time >= timeout : break
            self.ser.flushInput()
            self.txcmd('rx:status')
            if not 'Yes' in self.ser.readline() : continue
            
            self.txcmd('buf:dump')
            buf = ""
            while True :
                time.sleep(0.1)
                line = self.ser.readline()
                if line == '\n' : break
                buf += line
            return buf
        return None
        
    def transmit(self, ir_code):
        print "Transmit"
        self.txcmd( "buf:reg_start" )
        self.txdat( ir_code )
        self.txcmd( "buf:reg_stop" )
        self.txcmd( "tx:enable" )

    def txcmd(self, cmd, sleeptime=0.01):
        print (cmd)
        cmd += "\n"
        self.ser.write(cmd)
        time.sleep(sleeptime)

    def txdat(self, dat):
        div = 50
        dat = dat.replace('\n', '')
        dat = dat.replace('\n', '')
        L = len(dat)
        s = 0
        while s + div < L :
            cmd = "dat:%s" % dat[s:s+div]
            self.txcmd(cmd)
            s += div
        if s < L :
            cmd = "dat:%s" % dat[s:]
            self.txcmd(cmd)
    
    def __del__(self):
        self.ser.close()

def main():
    db = irdb()

    dev = irdev("/dev/ttyACM0", 115200)
    print dev.recv_code()
    # Power Off
    '''
    dev.txcmd( "buf:reg_start" )
    dev.txdat( db.get("aircon_poweroff") )
    dev.txcmd( "buf:reg_stop" )
    dev.txcmd( "tx:enable" )
    '''

if __name__ == "__main__":
    main()

