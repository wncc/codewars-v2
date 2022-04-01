#include <bits/stdc++.h>

using namespace std;

struct classcomp {
    bool operator() (const pair<int, double> & lhs, const pair<int,double> & rhs) const
        {return lhs.second>rhs.second;}
};

int main(){
	double x = -856.656;
	string s = to_string(x);
	cout<<x<<endl;
}
