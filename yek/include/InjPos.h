#ifndef INJ_POS
#define INJ_POS

class InjPos{
    public:
    InjPos(int id, int cap, double lo, double la);
    double longitude;
    double latitude;
    int InjID;
    int capacity;
    void updateCap(int newCap);
};

#endif
