import numpy as np
class Bot:


    def _init_(self, game, nu):
        self.Game = game
        self.numSecurities = nu     
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
    
    def GetBeta(self):
        return self.Game.GetBeta()
    
    def GetShareBalance(self,security):
        return self.Game.GetShareBalance(security)
    
    def GetFeatures(self, security):
        return self.Game.GetFeatures(security)

    def GetMostCorrelated(self, security):
        return self.Game.GetMostCorrelated(security)
    
    def GetLeastCorrelated(self, security):
        return self.Game.GetLeastCorrelated(security)

    try:
        def Run(self):
            for i in range(self.numSecurities):
                p=self.GetPrice(i)
                if self.iter%2==0:
                    self.BuySecurity(i,100,p+10)
                else:
                    self.SellSecurity(i,100,p-10)
            self.iter+=1

    except:
        print("exception")
