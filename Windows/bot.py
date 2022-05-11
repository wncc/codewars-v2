import numpy as np
class Bot:


    def __init__(self, game, nu):
        self.price = 10
        self.Game = game
        self.numSecurities = nu
        self.Portfolio = {}
        self.prices = np.zeros((20,100))      
        self.iter = 0  

    def GetBalance(self):
        return self.Game.GetBalance()
    
    def GetPrice(self, security):
        return self.Game.GetPrice(security)

    def BuySecurity(self,security,quantity,price):
        return self.Game.BuySecurity(security,quantity,price)

    def SellSecurity(self,security,quantity,price):
        return self.Game.SellSecurity(security,quantity,price)
    
    def GetInterest(self):
        return self.Game.GetInterest()
    
    def GetBeta(self,security):
        return self.Game.GetBeta(security)
    
    def GetShareBalance(self,security):
        return self.Game.GetShareBalance(security)
    
    def GetFeatures(self, security):
        return self.Game.GetFeatures(security)

    def GetMostCorrelated(self, security):
        return self.Game.GetMostCorrelated(security)
    
    def GetLeastCorrelated(self, security):
        return self.Game.GetLeastCorrelated(security)

    def Run(self):
        if self.iter > 10 and self.iter < 100:
            self.prices[:,self.iter] = np.array([self.Game.GetPrice(i) for i in range(0,self.numSecurities)])
            X = self.prices[:,0]
            for i in range(0,self.iter-1):
                X = 0.8*X + 0.2*self.prices[:,i]
            w = X / self.prices[:,self.iter]
            for i in range(0,self.numSecurities):
                if self.Game.GetBalance() - (self.prices[i,self.iter]+1)*900*w[i] > 0 and w[i] > 1:
                    self.Game.BuySecurity(i,900*w[i],self.prices[i,self.iter]+1)
                elif w[i] < 0.5:
                    pass
        elif self.iter > 10:
            self.prices[:,:-1] = self.prices[:,1:]
            self.prices[:,-1] = np.array([self.Game.GetPrice(i) for i in range(0,self.numSecurities)])
            X = self.prices[:,0]
            for i in range(0,99):
                X = 0.8*X + 0.2*self.prices[:,i]
            w = X / self.prices[:,-1]
            for i in range(0,self.numSecurities):
                if self.Game.GetBalance()- (self.prices[i,-1]+1)*900*w[i]  > 0 and w[i] > 1:
                    self.Game.BuySecurity(i,900*w[i],self.prices[i,-1]+1)
        else:
            self.prices[:,self.iter] = np.array([self.Game.GetPrice(i) for i in range(0,self.numSecurities)])
            for i in range(0,self.numSecurities):
                if self.Game.GetBalance() - (self.prices[i,self.iter]+1)*900 > 0:
                    self.Game.BuySecurity(i,900,self.prices[i,self.iter]+1)

        self.iter += 1