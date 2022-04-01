#include <bits/stdc++.h>

// TRadebook needs to be processed
// Iter needs to be updated
// Misbalance has to be recorded and re-initialised
// The LO list needs re-organisation


#define ll long long

struct trade{
        int buyerID;
        int sellerID;
        int amount;
        double deductBuy;
        double deductSell;
        trade(int bid, int sid, int am, double db, double ds);
    };

class LimitOrderBook{
    
    class Unit{
        bool ty;    // 0 for SELL, 1 for BUY
        bool ifused;    // 0 for not used, 1 for used!
        double volume;
        int userID; // -1 for Govt!
        public : 
            int number;  
            double price;
            Unit(bool typ, bool ifu ,int am, double pr, int user);
            int getID();
            void Reset();
            void reassign(bool ty, bool ifu,int am, double pr, int user);
    };

    

    struct classcomp {
        bool operator() (const Unit *& lhs, const Unit *& rhs) const
        {return lhs->price<rhs->price;}
    };
    map<int,Unit*> BO;  // This denotes the Buying order corresponding to the ith guy
    map<int,Unit*> SO;  // This denotes the sell order corresponding to the ith guy
    map<int, Unit*> mapping;
    double misbalance;
    
    double PriceStart, PriceEnd;
    double currentPrice;
    double tolerance = 10;
    double TickSize = 0.1;
    double totalNum = 5000;
    list<Unit*> LO; 
    
    set<double> B;
    set<double, greater <double> > S;


    public:
        double tradedVolume;
        int iter;
        double tradedNumber;
        list<trade*> tradebook;
        string PlaceOrder(bool ty, int user, double price, int amount);      // 0 for sell, 1 for buy
        LimitOrderBook(double currectPrice, double initUnits, double tol, double spread);
        void manipulatePrice(double pr);
        double getPrice();
        double getAndResetMisbalance();
};