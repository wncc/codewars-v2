#include<bits/stdc++.h>
#include <economy.h>
#include<Eigen/Dense>
#define threshold 0.5


using namespace Eigen;
using namespace std;

class Government{

    // You may use this instead of the other classes, Feel free to define your variables, as and when you require them, right here!
    int numSecurities;
    int numberReturn;
    int numberSwap;
    int period;
    int userperiod;
    vector<User*> users;
    double InterestLambda;
    default_random_engine gen;
    normal_distribution<double> dist;
    double Highvalues[4] = {0.4,0.5,0.3,0.7};
    double Lowvalues[4] = {0.1,0.09,0.03,0.05};
    struct classcomp {
        bool operator() (const pair<double,int> & lhs, const pair<double,int> & rhs) const
        {return lhs.first>rhs.first;}
    };
    VectorXd numbers;
    list<VectorXd*> prices;
    list<VectorXd*> misbalances;
    list<VectorXd*> volumes;
    list<VectorXd*> quantities;
    poisson_distribution<int> *poiss;
    vector<security> securities;
    map<string, int> secname;
    VectorXd gm;
    VectorXd g2m;
    double beta1;
    double beta2;
    double epsilon = 0.001;
    MatrixXd correlations;
    VectorXd *prevPrice;
    VectorXd Price;
    double interestRate;
    int np;
    int iter;
    int iterCal;
    public:
        Government(int numUser, vector<SecurityRec> secu, double initialInterest, double InterestLambda,double bet1,double bet2, int numg, int swapg, int numprev, int per, int userper);
        void InitializeGraph();
        void MarketAction();
        void ResetCorr();
        void UpdateGraph();
        void RunMarket();
        int AddUser(double cas);
};