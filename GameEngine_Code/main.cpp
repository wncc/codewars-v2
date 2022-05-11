#include <bits/stdc++.h>

#define PI 3.141592654

using namespace std;

int numSecurties = 20;
int randseed = 1568;
vector<double> alpha;
vector<double> beta;
double growthAlpha = 0.0007;
double interest = 0.06;
double corrAlpha = 0.1;
double corrAlphaSig = 0.05;
double corrAlpha2 = 0.000225554;
double corrAlphaSig2 = 0.0004;
vector<vector<double> > corr;
double changePeriodS = 0.0333;
double changePeriodL = 0.02;
int interestPeriod = 300;
double initPricMean = 75;
double initPricSig = 20;

string finalHashCatch;
int shortPeriod = 20;
double shortAmount = 0.0005;


ofstream resU;

map<int,int> traded;

uniform_real_distribution<double> unif(0.0, 1.0);
normal_distribution<double> distribution (0.0,1.0);
default_random_engine *re;
double getNetworth();

void ResetCorr();
void ResetFeatures();
void IncrementPrice();
int numIters = 4800;
double bet = 0.8;

vector<double> prices;
vector<pair<double,int> > prior;
vector<double> incr;
vector<double> vals;

vector<double> sigs;

map<int,int> portfolio;

int arrseed[12] = {0,1,2,3,4,17,30,25,20,60,10,5};

vector<int> res;
vector<vector<double> > features;
vector<vector<double> > basis;
struct classcomp {
    bool operator() (const pair<int, double> & lhs, const pair<int,double> & rhs) const
        {return lhs.second>rhs.second;}
};
vector< set< pair<int,double>, classcomp > > corrsec;
vector<string> parse;
vector<double> meanPrice;
vector<double> meanPrice2;
bool gametype;
long long counterstart;
int tem;
string password;
double initbal = 100000;
double balance = initbal;
double bal = initbal;
// password of 5 characters
string initialHash(int teamcode, string p){
    string res0 = to_string(teamcode);
    tem = teamcode;
    password = p;
    for(; res0.length() < 3;){
        res0 = "0" + res0;
    }
    res0+=p;
    counterstart = time(NULL);
    string res1 = to_string(counterstart);
    for(;res1.length()<13;){
        res1 = "0"+res1;
    }
    return res0+res1;
}



string GameEndHash(){
    string res0 = to_string(tem);
    for(;res0.length()<3;){
        res0 = "0" + res0;
    }
    res0 += password;
    string res1 = to_string(counterstart);
    for(;res1.length()<13;){
        res1 = "0"+res1;
    }
    res0 += res1;
    res1 = to_string(time(NULL));
    for(;res1.length()<13;){
        res1 = "0"+res1;
    }
    res0 += res1;
    res0 += finalHashCatch;
    res0 += to_string((getNetworth()-initbal)/(bal -initbal));
    
    return res0;
}

void SetEnvironment(int nums, int rs, double ga, double i, double cA, double cAs, double cA2, double cAs2, double cps, double cpl, int ip, double ips){
    numSecurties = nums;
    randseed = rs;
    growthAlpha = ga;
    interest = i;
    corrAlpha = cA;
    corrAlphaSig = cAs;
    corrAlpha2 = cA2;
    corrAlphaSig2 = cAs2;
    changePeriodS = cps;
    changePeriodL = cpl;
    interestPeriod = ip;
    initPricSig = ips;
    finalHashCatch = to_string(nums + rs + ((int)(ga*1000)) + ((int)(i*1000)) + ((int)(cA*1000))+ ((int)(cAs*100000))  + ((int)(cA2*1000))+ ((int)(cAs2*100000)) + ((int)cps*100) + ((int)cpl*100) + ip + ((int)ips*1000) % 10000000);
    // 7 digits
    for(;finalHashCatch.size()<7;){
        finalHashCatch = "0" + finalHashCatch;
    }

 }

string Encrypt(string s){
    int M = s.length();
    vector<int> st;
    string result;
    for(int i = 2;i<M;i++){
        if(__gcd(i,M)==1){
            st.push_back(i);
        }
    }
    int gok = time(NULL) % 255;
    int mok = st[time(NULL) % st.size()];
    result = to_string(gok);
    result += "b";
    result += to_string(mok);
    result += "b";
    result += to_string(M);
    result += "b";
    vector<int> arr;
    for(int i = 0; i <s.size();i++){
        arr.push_back(0);
    }
    for(int i = 0;i<s.size();i++){
        arr[(i*mok)%M] = ((((int) s[i]) + gok) % 255);
    }
    for(int i = 0;i < arr.size();i++){
        int q = arr[i]/16;
        int r = arr[i]%16;
        result += 'A' + q;
        result += 'A'+ r;
    }
    return result;
}

void Initialize(){
    re  = new default_random_engine(randseed);
    prior.assign(numSecurties,make_pair(0,0));
    for(int i = 0;i<numSecurties;i++){
        vector<double> nums(numSecurties,0.0);
        set< pair<int,double>, classcomp > sd;
        corr.push_back(nums);
        corrsec.push_back(sd);
        basis.push_back(nums);
        vals.push_back(0);
        sigs.push_back(0.1);
        double alph;
        alph = distribution(*re)*0.5 + 0.75;
        if(alph < 0) alph = 0.3;
        if(alph > 1) alph = 1;
        alpha.push_back(alph);
        beta.push_back(1-alpha[alpha.size()-1]);
        features.push_back(nums);
        corr[i][i] = 1.0;
        prior[i].second = i;
        prior[i].first = 1.0;
    }
    prices.assign(numSecurties,initPricMean);
    for(int i = 0;i< numSecurties;i++){
        prices[i] += distribution(*re)*initPricSig;
        meanPrice.push_back(prices[i]);
        meanPrice2.push_back(prices[i]*prices[i]);

    }
    vector<int> indices;
    for(int i = 0;i<numSecurties;i++)indices.push_back(i);
    srand(randseed);
    random_shuffle(indices.begin(),indices.end());

    for(int i = 0;i < numSecurties;i++){
        double val = 0;
        for(int j = 0; j < numSecurties; j++){
            if (j==0){
                basis[indices[i]][j] = sqrt(1.00/((double)numSecurties));
            }else{
                basis[indices[i]][j] = sqrt(2.00/((double)numSecurties))*abs(cos((PI*((2*((double)j))+1)*((double)i))/(2*((double)numSecurties))));
            }
            val += basis[i][j];
        }
        for(int j = 0; j < numSecurties; j++){
            basis[i][j] /= val;
        }
    }
    ResetCorr();
    ResetFeatures();
    IncrementPrice();
}

bool ifExec(bool ty, double pricem, double pricesig, double price){
    if(ty && price <= pricem + pricesig) return (2*(abs(price-pricem)/pricesig))*1000 < unif(*re);
    if(!ty && price >= pricem - pricesig) return (2*(abs(price-pricem)/pricesig))*1000 < unif(*re);
    return true;
    
}

bool comp(pair<double,int> l, pair<double,int> r){
   return l.first < r.first;
}

void ResetCorr(){
    for(int i = 0;i<numSecurties;i++)corrsec[i].clear();
    for(int i = 0;i<numSecurties;i++){
        for(int j = i+1;j<numSecurties;j++){
            corr[i][j] = distribution(*re)*corrAlphaSig  + corrAlpha;
            if(corr[i][j] > 1){
                corr[i][j] = 1;  
            }else if(corr[i][j]<-1){
                corr[i][j] = -1;
            }
            corr[j][i] = corr[i][j];
            corrsec[i].insert(make_pair(j,corr[i][j]));
            corrsec[j].insert(make_pair(i,corr[i][j]));
            prior[i].first += abs(corr[i][j]);
            prior[j].first += abs(corr[i][j]); 
        }
    }
    sort(prior.begin(),prior.end(),comp);
}

void ResetFeatures(){
    for(int i = 0; i <numSecurties;i++){
        vals[i] = 0.0;
        for(int j = 0;j <numSecurties;j++){
            features[i][j] = distribution(*re)*corrAlphaSig2 + corrAlpha2;
            vals[i] += features[i][j]*basis[i][j];
        }
    }
}

void IncrementPrice(){
    incr.assign(numSecurties,0);
    for(int i = 0; i < numSecurties;i++){
        double valu = 0;
        for(int j = 0; j < numSecurties;j++){
            incr[prior[i].second] += incr[j] * corr[i][j];
            if(incr[j] > 1e-5 || incr[j] < -1e-5){
                valu += abs(corr[i][j]);
            }
        }
        if(valu < 1e-5){
            incr[prior[i].second] = (distribution(*re)*growthAlpha + vals[i]);
        }else{
            incr[prior[i].second] = ((incr[prior[i].second]*beta[prior[i].second])/valu) + alpha[prior[i].second]*(distribution(*re)*growthAlpha + vals[i]);
        }
            
    }
}

void getMostCorr(int sec){
    res.clear();
    auto v = corrsec[sec].begin();
    if(v->second > 0){
        res.push_back(v->first);
    }
    v++;
    if(v->second > 0){
        res.push_back(v->first);
    }
    v++;
    if(v->second > 0){
        res.push_back(v->first);
    }
    v++;
    if(v->second > 0){
        res.push_back(v->first);
    }
    v++;
    if(v->second > 0){
        res.push_back(v->first);
    }
}

void getLeastCorr(int sec){
    res.clear();
    auto v = corrsec[sec].end();
    v--;
    if(v->second < 0){
        res.push_back(v->first);
    }
    v--;
    if(v->second < 0){
        res.push_back(v->first);
    }
    v--;
    if(v->second < 0){
        res.push_back(v->first);
    }
    v--;
    if(v->second < 0){
        res.push_back(v->first);
    }
    v--;
    if(v->second < 0){
        res.push_back(v->first);
    }
}

//0 for sell and 1 for buy
string PlaceOrder(int sec, bool ty, double price, int num){
    if(sec < 0 || sec >= numSecurties || num < 0){
        return "You are trying to trade in non-existent security!";
    }
    if(ifExec(ty,prices[sec],0,price)){
        if(ty){
            if(portfolio[sec] + num > 10000 || traded[sec] +num > 1000){
                return "You are trying to buy a lot!";
            }
            portfolio[sec] += num;
            balance -= price*((double)num);
            traded[sec] += num;
            return "Your order for buying " + to_string(sec) + "th security has been placed!";
        }else{
            if(portfolio[sec] - num < -10000 || traded[sec] +num > 1000){
                return "You are trying to short a lot!";
            }
            portfolio[sec] -= num;
            traded[sec] += num;
            balance += price*((double)num);
            return "Your order for selling " + to_string(sec) + "th security has been placed!";
        }
            
    }else{
        return "Sorry, Your order couldn't be placed, due to inappropriate pricing!";
    }
}


void split (string str, char seperator)  
{  
    parse.clear();
    int currIndex = 0, i = 0;  
    int startIndex = 0, endIndex = 0;  
    while (i <= str.length())  
    {  
        if (str[i] == seperator || i == str.length())  
        {  
            endIndex = i;
            parse.push_back(str.substr(startIndex,endIndex-startIndex));
            startIndex = endIndex + 1;  
        }  
        i++;  
        }     
}  


double getNetworth(){
    double value = 0.00;
    for(int i = 0;i<numSecurties;i++){
        value += prices[i]*((double)portfolio[i]);
    }
    return balance + value;
}

int getPortfolioBalance(int sec){
    return portfolio[sec];
}

double getPrice(int sec){
    return prices[sec];
}

// BUY "SECURITY NUMBER" "QUANTITY" "PRICE"
// SELL "SECURITY NUMBER" "QUANTITY" "PRICE"
// GETPRICE "SECURITY NUMBER"
// GETBALANCE
// GETINTEREST
// GETSHAREBALANCE "SECURITY NUMBER"
// GETBETA "SECURITY NUMBER"
// GETFEATURES "SECURITY NUMBER"
// GETMOST "SECURITY NUMBER"  -CORRELATED 5 STOCKS
// GETLEAST "SECURITY NUMBER" CO-RELATED 5 STOCKS
// SHORT "SECURITY NUMBER" "QUANTITY"
// NEXT

void GameSwitch(int randseed){

    if(randseed % 3==0){
        corrAlpha2 = 0;
        corrAlphaSig2 = 0.0001;
    }else if(randseed%3==1){
        corrAlpha2 = 0.00022;
        corrAlphaSig2 = 0.0004;
    }else{
        corrAlpha2 = -0.00022;
        corrAlphaSig2 = 0.0004;
    }
    // Even means higher overall volatility
    if(randseed%2==0){
        growthAlpha = 0.001;
    }else{
        growthAlpha = 0.0001;
    }
    // Divisible by 5 means a larger interest rate
    if(randseed%5==0){
        interest = 0.1;
    }else{
        interest = 0.04;
    }
}

void StartGame(){
    string s;
    getline(cin,s);
    split(s,' ');
    if(parse[0]=="TRADE"){
        gametype = 1;
        cout<<Encrypt(initialHash(stoi(parse[1]),parse[2]))<<endl;
        getline(cin,s);
        split(s,' ');
        SetEnvironment(stoi(parse[0]),stoi(parse[1]),stod(parse[2]),stod(parse[3]),stod(parse[4]),stod(parse[5]),stod(parse[6]),stod(parse[7]),stod(parse[8]),stod(parse[9]),stoi(parse[10]),stod(parse[11]));
    }else{
        gametype = 0;
        randseed = stoi(parse[1]);
        resU.open(parse[2]+".result");
        // Divisible by 3 means fail
        // Giving a modulus of 1 when failing means bullish
        // Giving a modulus of 2 when failing means bearish
        // double growthAlpha = 0.0075;
        // double interest = 0.07;
        // double corrAlpha = 0.1;
        // double corrAlphaSig = 0.05;
        // double corrAlpha2 = 0.0000;
        // double corrAlphaSig2 = 0.001;
        // vector<vector<double> > corr;
        // double changePeriodS = 0.2;
        // double changePeriodL = 0.25;
        // int interestPeriod = 300;
        // double initPricMean = 75;
        // double initPricSig = 20;
        if(randseed % 3==0){
            corrAlpha2 = 0;
            corrAlphaSig2 = 0.0001;
        }else if(randseed%3==1){
            corrAlpha2 = 0.00022;
            corrAlphaSig2 = 0.0004;
        }else{
            corrAlpha2 = -0.00022;
            corrAlphaSig2 = 0.0004;
        }
        // Even means higher overall volatility
        if(randseed%2==0){
            growthAlpha = 0.001;
        }else{
            growthAlpha = 0.0001;
        }
        // Divisible by 5 means a larger interest rate
        if(randseed%5==0){
            interest = 0.1;
        }else{
            interest = 0.04;
        }
    }
}

void EndGame(){
    cout<<Encrypt(GameEndHash())<<endl;
}

int main(int argc, char *argv[] ){
    StartGame();
    
    Initialize();
    int i = 0;
    int ij = 0;
    string s;
    cout<<numSecurties<<endl;
    while(i < numIters){
        getline(cin,s);
        if(s=="NEXT"){
            i++;
            traded.clear();
            // if((i-1)%400 == 0){
            //     GameSwitch(arrseed[ij]);
            //     ij = (ij + 1)%12;
            // }
            bool z = (i%shortPeriod)==0;
            for(int iter = 0;iter<numSecurties;iter++){
                prices[iter]*=(1+incr[iter]);
                meanPrice[iter] = bet*meanPrice[iter] + (1-bet)*prices[iter];
                meanPrice2[iter] = bet*meanPrice2[iter] + (1-bet)*prices[iter]*prices[iter];
                sigs[iter] = meanPrice2[iter]-meanPrice[iter]*meanPrice[iter];
                if(sigs[iter] < 1e-5){
                    sigs[iter] = 0.1;
                }
                if(z && portfolio[iter] < 0){
                    balance += portfolio[iter]*prices[iter]*shortAmount;
                }
            }
            IncrementPrice();
            if(i%interestPeriod==0){
                balance *= (1+interest);
                bal *= (1+interest);
            }
            if(unif(*re) < changePeriodS){
                ResetFeatures();
            }
            if(unif(*re) < changePeriodL){
                ResetCorr();
            }
            double zs = getNetworth();
            cout<<"Your Networth is : "<<getNetworth()<<endl;
            if(i==numIters){
                
                cout<<"ENDGAME"<<endl;
            }else{
                cout<<"ROUNDNUM : "<<i<<endl;
            }

            if( i > 10 && (i % 200) ==0){
                resU << zs <<endl;
            }
        }
        if(s.substr(0,8)=="GETPRICE"){
            split(s,' ');
            cout<<prices[stod(parse[1])]<<endl;
        }
        if(s.substr(0,3)=="BUY"){
            split(s,' ');
            int secu = stoi(parse[1]);
            int num = stoi(parse[2]);
            double pr = stod(parse[3]);
            cout<<PlaceOrder(secu,1,pr,num)<<endl;
        }
        if(s.substr(0,4)=="SELL"){
            split(s,' ');
            int secu = stoi(parse[1]);
            int num = stoi(parse[2]);
            double pr = stod(parse[3]);
            cout<<PlaceOrder(secu,0,pr,num)<<endl;
        }
        if(s.substr(0,10)=="GETBALANCE"){
            cout<<balance<<endl;
        }
        if(s.substr(0,11)=="GETINTEREST"){
            cout<<interest<<endl;
        }
        
        if(s.substr(0,15)=="GETSHAREBALANCE"){
            split(s,' ');
            cout<<portfolio[stoi(parse[1])]<<endl;
        }
        if(s.substr(0,7)=="GETBETA"){
            split(s,' ');
            cout<<beta[stoi(parse[1])]<<endl;
        }
        if(s.substr(0,11)=="GETFEATURES"){
            split(s,' ');
            for(int j = 0;j < numSecurties;j++){
                if(j!=(numSecurties-1))cout<<features[stoi(parse[1])][j]<<',';
                else cout<<features[stoi(parse[1])][j]<<endl;
            }
        }
        if(s.substr(0,7)=="GETMOST"){
            split(s,' ');
            getMostCorr(stoi(parse[1]));
            for(int j = 0;j<res.size();j++){
                if(j!=(res.size()-1)) cout<<res[j]<<',';
                else cout<<res[j]<<endl;
            }
            if(res.size()==0) cout<<endl;
        }
        if(s.substr(0,8)=="GETLEAST"){
            split(s,' ');
            getLeastCorr(stoi(parse[1]));
            for(int j = 0;j<res.size();j++){
                if(j!=(res.size()-1)) cout<<res[j]<<',';
                else cout<<res[j]<<endl;
            }
            if(res.size()==0) cout<<endl;
        }
    }
    if(gametype)EndGame();
    resU.close();
    cout<<"Your Networth is : "<<getNetworth()<<", The Initial Balance was : "<<initbal<<endl;
}
