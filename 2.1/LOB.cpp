#include "LOB.h"
#include "economy.h"

using namespace std;

int LimitOrderBook::Unit::getID(){
    return userID;
}

void LimitOrderBook::Unit::reassign(bool typ, bool ifu,int am, double pr, int user){
    ty = typ;
    ifused = ifu;
    number = am;
    price = pr;
    userID = user;
    volume = pr*((double)number);
}

void LimitOrderBook::Unit::Reset(){
    ty = 0;
    ifused = 0;
    volume = 0;
    userID = 0;
    number = 0;
    price = 0;
}

LimitOrderBook::Unit::Unit(bool typ, bool ifu ,int am, double pr, int user){
    ty = typ;
    ifused = ifu;
    number = am;
    price = pr;
    volume = pr*((double)number);
    userID = user;
}

trade::trade(int bid, int sid, int am, double db, double ds){
    buyerID = bid;
    sellerID = sid;
    amount = am;
    deductBuy = db;
    deductSell = ds;
};

double LimitOrderBook::getAndResetMisbalance(){
    double res = misbalance;
    misbalance = 0;
    return res;
}

LimitOrderBook::LimitOrderBook(double currectPri, double initUnits, double tol, double spread){
    currentPrice = currectPri;
    misbalance = 0;
    iter = 0;
    tolerance = tol;
    tradedNumber = 0;
    tradedVolume = 0;
    double priceStart =((double)((int)(currentPrice*(spread-1))))/10.0;
    double priceEnd =((double)((int)(currentPrice*(spread+1))))/10.0;
    int num =((int)(currentPrice*(spread+1))) - ((int)(currentPrice*(spread-1)));
    int iniu = initUnits/num;
    PriceStart = priceStart - (((totalNum-num)/2)*TickSize);
    double pri = PriceStart;
    for(int i = 0; i < totalNum; i++, pri += TickSize){
        if(pri < priceStart || pri > priceEnd){
            LO.push_back(new Unit(0,0,0,pri,-1));
        }else{
            LO.push_back(new Unit(0,1,iniu,pri,-1));
            S.insert(pri);
            mapping[((int)pri*10)] = *LO.end();
        }
    } 
    PriceEnd = pri;
}

// Failure Codes : 
// 01 : Not Within Spread
// 02 : Insufficient Security Balance (This isn't given from here but done by the government itself!)
// 03 : 
//
//
//Success Codes :
// 01 : Order Executed!
// 02 : Order put in LOB, no guarantee of execution yet, MIGHT BE PARTIALLY EXECUTED IN TURNS!
//
//
string LimitOrderBook::PlaceOrder(bool ty, int user, double price, int amount){
    string res;
    if (!ty){
        if(price < PriceStart || price > PriceEnd){
            return "FAILED_01";
        } 
        int am = amount;
        vector<double> toDelete;
        for(set<double>::iterator it = B.lower_bound(price);it!= B.end();it++){
            Unit* manip = mapping[((int)(10*(*it)))];
            if(am > manip->number){
                am -= manip -> number;
                toDelete.push_back(manip->number);
                tradedVolume += (manip->number)*price;
                tradedNumber += manip -> number;
                misbalance += (manip->number)*((*it)-price);
                tradebook.push_back(new trade(manip->getID(),user,manip->number,(manip->number)*(manip->price),(manip->number)*price));
                BO.erase(BO.find(manip->getID()));
                manip->Reset();
            }else{
                tradedVolume += am*price;
                tradedNumber += am;
                misbalance += am*((*it)-price);
                tradebook.push_back(new trade(manip->getID(),user,am,am*(manip->price),am*price));
                am = 0;
            }
            if(am <= 0) break;
        }
        if(am > 0){
            Unit* manip = mapping[((int)(price*10))];
            manip->reassign(0,1,am,price,user);
            S.insert(price);
            SO[user] = manip;
            res =  "SUCCESS_02";
        }else{
            res = "SUCCESS_01";
        }
        for(int i = 0;i<toDelete.size();i++){
            B.erase(toDelete[i]);
        }
    }else{
        if(price < PriceStart || price > PriceEnd){
            return "FAILED_01";
        } 
        int am = amount;
        vector<double> toDelete;
        for(set<double>::iterator it = S.lower_bound(price);it!= S.end();it++){
            Unit* manip = mapping[((int)(10*(*it)))];
            if(am > manip->number){
                am -= manip -> number;
                toDelete.push_back(manip->number);
                tradedVolume += (manip->number)*(*it);
                tradedNumber += manip -> number;
                misbalance += (manip->number)*(price-(*it));
                tradebook.push_back(new trade(user,manip->getID(),manip->number,(manip->number)*price,(manip->number)*(manip->price)));
                SO.erase(BO.find(manip->getID()));
                manip->Reset();
            }else{
                tradedVolume += am*(*it);
                tradedNumber += am;
                misbalance += am*(price-(*it));
                tradebook.push_back(new trade(user,manip->getID(),am,am*price,am*(manip->price)));
                am = 0;
            }
            if(am <= 0) break;
        }
        if(am > 0){
            Unit* manip = mapping[((int)(price*10))];
            manip->reassign(1,1,am,price,user);
            B.insert(price);
            BO[user] = manip;
            res =  "SUCCESS_02";
        }else{
            res = "SUCCESS_01";
        }
        for(int i = 0;i<toDelete.size();i++){
            B.erase(toDelete[i]);
        }
    }
    return res;
}


void LimitOrderBook::manipulatePrice(double pr){
    currentPrice = pr;
    
    // Todo, retesting the limits of the Linked List
    
}

double LimitOrderBook::getPrice(){
    return currentPrice;
}



