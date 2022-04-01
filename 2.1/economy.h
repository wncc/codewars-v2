#include <bits/stdc++.h>
#include "LOB.h"

using namespace std; // Can be removed as and when deemed necessary!

class User
{
    int id;
    map<int, int> portfolio;
    double networth;
    double cash;
    double interest;
    int iter;
    int period;

public:
    User(int _id, double initialCash, int p);
    void ApplyInterest(double rate);
    void addInterest();
    double getBalance();
    void changeBalance(double addi);
    int getShareBalance(int share);
    void updateShareBalance(int share, int val);
    void incrementIter();
};

struct SecurityRec
{
    string name;
    vector<int> cor;
    vector<int> anticor;
    double beta;
    double alphaGov;
    SecurityRec(string nam, vector<int> corr, vector<int> anticorr, double bet, double alpha);
};

class security
{
    double price;
    string name;
    int id;
    int iter;

    vector<int> corel; // Correlation of the security with other securities
    vector<int> anticorel;

    double tolerance;

    int number; // Quantity that remains in the market at present point of time

    double market_cap; // The Market Cap, We may make use of complex methods to compute it, as done by many firms, or it may as well be Price * number
    double beta;
    double alphaGov;
    vector<double> volumes; // The Volume of trade of said security over the past various time-frames.

public:
    LimitOrderBook *lb; // You may choose to maintain it here or somewhere else, inside the Broker Class Perhaps!

    security(double iniPrice, int initu, string name, int _id,  vector<int> cor, vector<int> anticor, double toler, double spr, double bet, double alphaGo);

    void manipulatePrice(double price);
    void increaseQuant(int num);
    int GovtStepIn(double sprice, double eprice, int num);
    double getPrice();
    double getAlpha();
    double getBeta();
    double getQuant();
    void incrementIter();
};
