#include <bits/stdc++.h>

using namespace std;


uniform_real_distribution<double> unif(0.0, 1.0);
normal_distribution<double> norma(0,1);
default_random_engine re(5832);


int numSecurties = 20;
int randseed = 1568;
double growthAlpha = 0.0075;
double interest = 0.05;
double corrAlpha = 0.1;
double corrAlphaSig = 0.05;
double corrAlpha2 = 0.0001;
double corrAlphaSig2 = 0.0002;
vector<vector<double> > corr;
double changePeriodS = 0.02;
double changePeriodL = 0.25;
int interestPeriod = 300;
double initPricMean = 75;
double initPricSig = 20;


int main(){
    ofstream myf;
    myf.open("test.json");
    int N = 120;
    myf << '['<<endl;
    for(int i = 0;i < N;i++){
        myf << '\t'<< '{'<<endl;
        
        int nums = ((double)numSecurties) +norma(re)*4;
        if(nums > 29){
            nums = 25;
        }
        if(nums < 15){
            nums = 15;
        }

        int rdsd = unif(re)*100000000;
        double ga = growthAlpha + norma(re)*0.003;
        double inte = interest + norma(re)*0.03;
        if(inte < 0) inte = 0.01;
        double cA = corrAlpha + norma(re)*0.05;
        double cAs = corrAlphaSig + norma(re)*0.005;
        double cA2 = corrAlpha2 + norma(re)*0.0002;
        double cAs2 = corrAlphaSig2 + norma(re)*0.00005;
        double cps = changePeriodS + norma(re)*0.005;
        double cpl = changePeriodL + norma(re)*0.0005;
        int ip = ((double)interestPeriod) + norma(re)*50;
        double ips = initPricSig + norma(re)*10;
        myf << '\t' << '\t' << '"';
        myf<<"numSecurities"<<'"'<<':' << nums << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"randseed"<<'"'<<':' << rdsd << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"growthAlpha"<<'"'<<':' << ga << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"interestRate"<<'"'<<':' << inte << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"corrAlpha"<<'"'<<':' << cA << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"corrAlphaSig"<<'"'<<':' << cAs << ',' <<endl;
         myf << '\t' << '\t' << '"';
        myf<<"corrAlpha2"<<'"'<<':' << cA2 << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"corrAlphaSig2"<<'"'<<':' << cAs2 << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"changePeriodS"<<'"'<<':' << cps << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"changePeriodL"<<'"'<<':' << cpl << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"InterestPeriod"<<'"'<<':' << ip << ',' <<endl;
        myf << '\t' << '\t' << '"';
        myf<<"InitPricSig"<<'"'<<':' << ips << ',' <<endl;
        string finalHashCatch = to_string(nums + rdsd + ((int)(ga*1000)) + ((int)(inte*1000)) + ((int)(cA*1000))+ ((int)(cAs*100000))  + ((int)(cA2*1000))+ ((int)(cAs2*100000)) + ((int)cps*100) + ((int)cpl*100) + ip + ((int)ips*1000) % 10000000);
        myf << '\t' << '\t' << '"';
        myf<<"FinalHash"<<'"'<<':' << " \" "<<finalHashCatch << " \" " <<endl;
        if(i!= (N-1)){
            myf << '\t'<< "},"<<endl;
        }else{
            myf << '\t'<< '}'<<endl;
        }   
    }
    myf << ']';
    myf.close();
}