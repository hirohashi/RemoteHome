import pickle

class irdb:
    def __init__(self):
        self.fname_db = "irdb.pkl"
        try :
            fp = open(self.fname_db, "r")
            self.dat = pickle.load(fp)
            fp.close()
        except :
            self.dat = dict()
    
    def get_dat(self):
        print self.dat

    def keys(self):
        return self.dat.keys()
        
    def update(self):
        try :
            fp = open(self.fname_db, "w")
            pickle.dump(self.dat, fp)
            fp.close()
        except :
            return False
        return True
        
    def store(self, key, value):
        self.dat[key] = value
        return self.update()
    
    def get(self, key):
        return self.dat[key]
    
    def remove(self, key):
        self.dat[key].remove()
        return self.update()
    
if __name__ == "__main__":
    db = irdb()
    print db.keys()

