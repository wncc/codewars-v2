#include "economy.h"
#include "LOB.cpp"

User::User(int _id, double initialCash, int p){
    cash = initialCash;
    id = _id;
    networth = cash;
    interest = 0;
    iter = 0;
    period = p;
}

void User::changeBalance(double addi){
    cash += addi;
}
void User::incrementIter(){
    iter++;
}

void security::incrementIter(){
    iter++;
    lb->iter++;
}

void User::ApplyInterest(double rate){
    interest += cash*pow((1+rate),iter/period);
}

void User::addInterest(){
    cash += interest;
    interest = 0;
    iter = 0;
}

double User::getBalance(){
    return cash;
}

int User::getShareBalance(int share){
    return portfolio[share];
}

void User::updateShareBalance(int share, int val){
    portfolio[share] += val;
}


SecurityRec::SecurityRec(string nam, vector<int> corr, vector<int> anticorr, double bet, double alpha){
    name = nam;
    cor = corr;
    anticor = anticorr;
    beta = bet;
    alphaGov = alpha;
}

security::security(double iniPrice, int initu, string nam, int _id, vector<int> cor, vector<int> anticor, double toler, double spr, double bet, double alphaGo){
    price = iniPrice;
    name = nam;
    id = _id;
    corel = cor;
    iter = 0;
    anticorel = anticor;
    number = initu;
    market_cap = initu*price;
    tolerance = toler;
    beta = bet;
    alphaGov = alphaGo;
    lb = new LimitOrderBook(price, initu, toler, spr);
}

void security::manipulatePrice(double price){
    this->price = price;
    lb->manipulatePrice(price);
}



double security::getPrice(){
    return price;
}


double security::getAlpha(){
    return alphaGov;
}

double security::getBeta(){
    return beta;
}

double security::getQuant(){
    return number;
}

int security::GovtStepIn(double sprice, double eprice, int num){
    int t = 0;
    for(double i = sprice; i<=eprice;i+=0.1){
        lb->PlaceOrder(0,-1,i,num);
        t+= num;
    }
    return t;
}

void security::increaseQuant(int num){
    number += num;
}