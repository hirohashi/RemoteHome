import pickle

class irdb:
    def __init__(self):
        self.fname_db = "irdb.pkl"
        fp = open(self.fname_db)
        self.dat = pickle.load(fp)
        fp.close()

    def get_dat(self):
        print self.dat

    def keys(self):
        return self.dat.keys()

    def store(self, key, value):
        self.dat[key] = value
        fp = open(self.fname_db, "w")
        pickle.dump(self.dat, fp)
        fp.close()

    def get(self, key):
        return self.dat[key]

if __name__ == "__main__":
    db = irdb()
    print db.keys()

