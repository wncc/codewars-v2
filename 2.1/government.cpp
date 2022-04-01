#include "government.h"
#include <cstdlib>


VectorXd* getApp(list<VectorXd*> *l, int i){
	int j = 0;
	for(auto v : *l){
		if(i==j){
			return v;
		}
		j++;
	}
}

Government::Government(int numUser, vector<SecurityRec> secu, double initialInterest, double InterestLambda,double bet1,double bet2, int numg, int swapg, int numprev, int per, int userper) {
	std::uniform_real_distribution<double> unif(50.0, 150.0);
	std::default_random_engine re;
	double iniPrice = unif(re);
	int initu = 100;
	period = per;
	beta1 = bet1;
	beta2 = bet2;
	interestRate = initialInterest;
	numberReturn = numg;
	numberSwap = swapg;
	np = numprev;
	userperiod = userper;
	this->InterestLambda = InterestLambda;
	int i = 0;
	poiss = new poisson_distribution<int>(InterestLambda);
	numbers.resize(numSecurities);
	for (auto sr : secu) {
		securities.push_back(security(iniPrice, initu, sr.name, i, sr.cor, sr.anticor, 0.01, 0.02,sr.beta,sr.alphaGov));
		numbers(i) = initu;
		i++;
	}
	iterCal = 0;
	for(int i = 0;i<np;i++){
		prices.push_back(new VectorXd(numSecurities));
		misbalances.push_back(new VectorXd(numSecurities));
		volumes.push_back(new VectorXd(numSecurities));
		quantities.push_back(new VectorXd(numSecurities));
	}
	gm.resize(numSecurities);
	g2m.resize(numSecurities);
	InitializeGraph();
}

void Government::InitializeGraph() {

}


void Government::MarketAction(){
	VectorXd weights = gm.cwiseQuotient(g2m.cwiseSqrt()+ VectorXd::Ones(gm.size())*epsilon);
	VectorXd r = (Price - *prices.front()).cwiseQuotient(*prices.front());
	vector <pair<double,int> > we;
	for(int i  = 0;i<weights.size();i++){
		we.push_back(make_pair(weights(i),i));
	}
	sort(we.begin(),we.end(),[](pair<double,int> const & a, pair<double,int> const & b) -> bool
          { return a.first > b.first; });
	for(int i = 0;i < we.size();i++){
		if(we[i].first > threshold){
			double fall1 = r[we[i].second]*we[i].first*securities[we[i].second].getAlpha();
			double fall2 = r[we[i].second] -(securities[we[i].second].getBeta()*(r.cwiseProduct(numbers)).sum());
			double n0 = ((dist(gen)*0.25)+0.75);
			if(n0 > 1){
				n0 = 1;
			}
			if(n0 <0) n0 = 0;
			double valc = fall1*n0 + fall2*(1-n0);
			double n1 = ((dist(gen)*0.25)+0.25);
			if(n1 > 1){
				n1 = 1;
			}
			if(n1 <0) n1 = 0;
			double pric = securities[we[i].second].getPrice()*(1-valc);
			securities[we[i].second].manipulatePrice(pric);
			if(n1 < 0.1){
				int uy = securities[we[i].second].GovtStepIn(pric*0.95,pric*0.105,10);
				numbers(we[i].second)+= uy;
				securities[we[i].second].increaseQuant(uy);
			}
		}else{
			break;
		}
	}

};


void Government::ResetCorr(){

};

void Government::UpdateGraph(){
    VectorXd r= (Price - *prices.front()).cwiseQuotient(*prices.front());
    MatrixXd C = (correlations + correlations.transpose())/2;
	VectorXd g = (C*r).cwiseAbs() * (MatrixXd::Identity(C.rows(),C.cols())+C.transpose());
	gm = beta1*gm + (1-beta1)*g;
	g2m = beta2 * g2m + (1-beta2)*g.cwiseProduct(g);
};

int Government::AddUser(double cas){
	int res = users.size();
	users.push_back(new User(res,cas,userperiod));
	return res;
}

void Government::RunMarket(){
	iter += 1;
	for(auto v : users){
		v->incrementIter();
	}
	for(auto v:securities){
		v.incrementIter();
	}

	//Tradebook processing
		//Update price
		// Update Balances
	VectorXd *vp;
	VectorXd *vb;
	VectorXd *vv;
	VectorXd *vq;
	if(iter < np+1){
		vp = getApp(&prices,iter-1);
		vb = getApp(&misbalances,iter-1);
		vv = getApp(&volumes, iter-1);
		vq = getApp(&quantities,iter-1);	
	}else{
		vp = prices.front();
		prices.pop_front();
		prices.push_back(vp);
		vb = misbalances.front();
		misbalances.pop_front();
		misbalances.push_back(vb);
		vv = volumes.front();
		volumes.pop_front();
		volumes.push_back(vv);
		vq = quantities.front();
		quantities.pop_front();
		quantities.push_back(vq);
	}
	for(int i = 0;i<numSecurities;i++){
		while(!securities[i].lb->tradebook.empty()){
			trade *t = securities[i].lb->tradebook.front();
			securities[i].lb->tradebook.pop_front();
			users[t->buyerID]->updateShareBalance(i,t->amount);
			users[t->sellerID]->updateShareBalance(i,-t->amount);
			users[t->buyerID]->changeBalance(-t->deductBuy);
			users[t->sellerID]->changeBalance(t->deductSell);
		}
		double prie = securities[i].lb->tradedVolume/securities[i].lb->tradedNumber;
		if(prie < 1) prie = securities[i].getPrice();
		else{
			securities[i].manipulatePrice(prie);
		}
		vb->operator()(i) = securities[i].lb->getAndResetMisbalance();
		vv->operator()(i) = securities[i].lb->tradedVolume;
		vq->operator()(i) = securities[i].lb->tradedNumber;
		vp->operator()(i) = prie;
	}

	
	// Co-realations

	if(iter % (5*period)==0){
		ResetCorr();
	}


	//Running Weights Calculation

	UpdateGraph();

	// Take care of Itercal and Market Action of Government
	if(iter+10 % period ==0){
		MarketAction();
	}


	//Change Interests based on given poisson distribution!
	int z = poiss->operator()(gen);
	if(z > InterestLambda){
		for(auto v:users){
			v->ApplyInterest(interestRate);
		}
		interestRate = dist(gen) + interestRate;
	}

	


	// Increment and Decrement Bank Returns

	if(iter%userperiod){
		for(auto v:users){
			v->ApplyInterest(interestRate);
		}
	}


	// Price Incrementing Volatility
	if(iter%period==0){
		int kl = 0;
		for(auto v:securities){
			double prii;
			if(dist(gen) < 0.3){
				prii = v.getPrice()*1.05;
			}else{
				prii = v.getPrice()*0.95;
			}
				
			v.manipulatePrice(prii);
			int uy = v.GovtStepIn(prii*0.98,prii*1.02,10);
			numbers(kl)+= uy;
			v.increaseQuant(uy);
			kl++;
		}
	}






};