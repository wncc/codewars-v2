import subprocess
import bot
import requests
import time
num2Name = {
    0: "RELIANCE INDUSTRIES",
    1: "TATA MOTORS PVT. LTD.",
    2: "FEDERAL BANK",
    3: "INFOSYS",
    4: "TATA CONSULTANCY SERVICES",
    5: "NAZARA TECHNOLOGIES",
    6: "IRCTC",
    7: "SAIL",
    8: "BHEL",
    9: "CANARA BANK",
    10: "HDFC BANK",
    11: "ADANI WILMAR",
    12: "ADANI PORTS",
    13: "ASIAN PAINTS",
    14: "BHARATI AIRTEL",
    15: "TITAN",
    16: "SUNPHARMA",
    17: "WIPRO",
    18: "ITC",
    19: "BAJAJ FINANCE",
    20: "INDUSLND BANK",
    21: "KOTAK MAHINDRA BANK",
    22: "TECH MAHINDRA",
    23: "L & T",
    24: "STATE BANK OF INDIA",
    25: "MARUTI SUZUKI",
    26: "NESTLE INDIA",
    27: "ULTRATECH CEMENT",
    28: "PAYTM",
    29: "ZOMATO",
    30: "BYJUS"}

url_num = int(time.time())%4
print(url_num)
if url_num==0:
    url = 'https://codewars-server.herokuapp.com/'
elif url_num==1:
    url = 'https://codewars-server2.herokuapp.com/'
elif url_num==2:
    url = 'https://codewars-server3.herokuapp.com/'
else:
    url = 'https://codewars-server4.herokuapp.com/'

class Game:

    def __init__(self,game,type):
        self.Game = subprocess.Popen(['./' + game],bufsize=1, universal_newlines=True, stdin=subprocess.PIPE, stdout = subprocess.PIPE)
        self.bot = None
        self.numSecurities = None
        self.TOP = ''
        self.TYPE = type
        self.NUMR = {}
        self.RESPONSE = {}

    def Reset(self):
        for i in range(0, self.numSecurities):
            self.NUMR[i] = 0

    def SetEnv(self, encry):
        while True:
             try:
                 response = requests.get(url=url+'start', json={'msg': encry})
                 break
             except requests.Timeout:
                 continue
        try:
            self.RESPONSE = response.json()
           
            print(self.RESPONSE)
            res = ''
            for i in self.RESPONSE.keys():
                print('we were here')
                res += str(self.RESPONSE[i]) + ' '

            res = res[:-1]
            res += '\n'
           
            self.Game.stdin.write(res)
            
            
            
        except:
            print(response.text)
            


    def PassEnv(self, st):
         while True:
             try:
                 response = requests.get(url=url+'end', json={'msg': st})
                 break
             except requests.Timeout:
                 continue
         print(response.text)

    def RunGame(self):
        try:
            self.bot.Run()
        except:
            print("Something's wrong with the submission")
    def NextIter(self):
        self.Game.stdin.write('NEXT\n')

    def BuySecurity(self, security, quantity, price):
        if security < 0 or security >= self.numSecurities:
            print('TRYING TO TRADE IN INVALID SECURITY!')
            return False
        if self.NUMR[security] + abs(quantity) > 1000:
            print('CROSSED ORDER LIMIT FOR THIS SECURITY!')
            return False
        if quantity < 0:
            print(
                'YOU CANNOT BUY A NEGATIVE QUANTITY OF A SECURITY! USE SellSecurity() TO SHORT THE MARKET!')
            return False
        order = 'BUY ' + str(security) + ' ' + \
            str(quantity) + ' ' + str(price)+'\n'
        self.Game.stdin.write(order)
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        print(line)
        if line[0:4] == 'You ' or line[0:5] == 'Sorry':
            print("FAILURE : "+'BUY ' + str(security)+':' +
                  num2Name[security] + ' ' + str(quantity) + ' ' + str(price))
            return False
        else:
            print("SUCCESS : "+'BUY ' + str(security)+':' +
                  num2Name[security] + ' ' + str(quantity) + ' ' + str(price))
            self.NUMR[security] += abs(quantity)
            return True

    def SellSecurity(self, security, quantity, price):
        if security < 0 or security >= self.numSecurities:
            print('TRYING TO TRADE IN INVALID SECURITY!')
            return False
        if self.NUMR[security] + abs(quantity) > 1000:
            print('CROSSED ORDER LIMIT FOR THIS SECURITY!')
            return False
        order = 'SELL ' + str(security) + ' ' + \
            str(quantity) + ' ' + str(price)+'\n'
        self.Game.stdin.write(order)
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        print(line)
        if line[0:4] == 'You ' or line[0:5] == 'Sorry':
            print("FAILURE : "+'SELL ' + str(security)+':' +
                  num2Name[security] + ' ' + str(quantity) + ' ' + str(price))
            return False
        else:
            print("SUCCESS : "+'SELL ' + str(security)+':' +
                  num2Name[security] + ' ' + str(quantity) + ' ' + str(price))
            self.NUMR[security] += abs(quantity)
            return True

    def GetPrice(self, sec):
        if sec < 0 or sec >= self.numSecurities:
            print('TRYING TO TRADE IN INVALID SECURITY!')
            return 0
        self.Game.stdin.write('GETPRICE ' + str(sec)+'\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        print("CURRENT PRICE OF "+str(sec) + ": " +
              num2Name[sec] + ":" + str(float(line)))
        return float(line)

    def GetBalance(self):
        self.Game.stdin.write('GETBALANCE\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        print("YOUR CURRENT BALANCE : ", float(line))
        return float(line)

    def GetInterest(self):
        self.Game.stdin.write('GETINTEREST\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        print('THE CURRENT RATE OF INTEREST : ', float(line))
        return float(line)

    def GetShareBalance(self, sec):
        if sec < 0 or sec >= self.numSecurities:
            print('TRYING TO REFER TO INVALID SECURITY!')
            return 0
        self.Game.stdin.write('GETSHAREBALANCE '+str(sec)+'\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        print("YOU CURRENTLY HAVE "+str(int(line)) +
              " shares of "+str(sec)+":"+num2Name[sec])
        return int(line)

    def GetBeta(self, sec):
        if sec < 0 or sec >= self.numSecurities:
            print('TRYING TO REFER TO INVALID SECURITY!')
            return 0
        self.Game.stdin.write('GETBETA ' + str(sec) + '\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        print("BETA FOR "+str(sec)+":"+num2Name[sec]+' is '+float(line))
        return float(line)

    def GetFeatures(self, sec):
        if sec < 0 or sec >= self.numSecurities:
            print('TRYING TO REFER TO INVALID SECURITY!')
            return 0
        self.Game.stdin.write('GETFEATURES ' + str(sec) + '\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        L = []
        for l in line.split(','):
            L.append(float(l))
        return L

    def GetMostCorrelated(self, sec):
        if sec < 0 or sec >= self.numSecurities:
            print('TRYING TO REFER TO INVALID SECURITY!')
            return 0
        self.Game.stdin.write('GETMOST ' + str(sec) + '\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        L = []
        for l in line.split(','):
            L.append(int(l))
        return L

    def Read(self):
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        self.TOP = line

    def Run(self):
        
        if self.TYPE:
            teamcode = int(input('TEAM CODE : '))
            password = input('PASSWORD :')
            self.Game.stdin.write(
                'TRADE ' + str(teamcode) + ' ' + password + '\n')
            self.Read()
            encry = self.TOP.strip()
            self.SetEnv(encry)

        else:
            randcode = int(input('Give me a random seed?'))
            self.Game.stdin.write('TEST ' + str(randcode)+'\n')

        self.Read()
        self.numSecurities = int(self.TOP)
        self.Reset()
        self.bot = bot.Bot(self,self.numSecurities)
        while True:
            if self.TOP.strip() == "ENDGAME":
                self.Read()
                break
            self.RunGame()
            self.Game.stdin.write("NEXT\n")
            self.Read()
            print(self.TOP.strip())
            self.Read()
            if self.TOP.strip() == "ENDGAME":
                break
            else:
                print(self.TOP.strip())
            self.Reset()
        self.Read()
        print(self.TOP.strip())
        if self.TYPE:
            self.PassEnv(self.TOP.strip())

    def GetLeastCorrelated(self, sec):
        if sec < 0 or sec >= self.numSecurities:
            print('TRYING TO REFER TO INVALID SECURITY!')
            return 0
        self.Game.stdin.write('GETLEAST ' + str(sec) + '\n')
        line = ''
        while True:
            line = self.Game.stdout.readline()
            if line:
                break
        L = []
        for l in line.split(','):
            L.append(int(l))
        return L


x = input("Do you want to test Locally? [Y/N]")
g = None
if x == 'Y':
    g = Game('wnccATfifteen.exe', False)
else:
    g = Game('wnccATfifteen.exe',True)

g.Run()
