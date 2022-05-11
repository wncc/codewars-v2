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
        