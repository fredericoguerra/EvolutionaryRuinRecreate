class Instance:
	def __init__(self, name, value, isOptimal):
		self.name = name
		self.value = value
		self.isOptimal = isOptimal

def read_ref():

        values = {}
        f = open("opt-values-CG-SMINVCMAX.txt", "r")

        content = f.read().splitlines()

        for i in range(5, len(content)):
            splitted = content[i].split()
            #values[splitted[0]] = splitted[1]
            if(len(splitted) > 3):
                values[splitted[0]] = Instance(splitted[0], int(splitted[1]), False)
            else:
                values[splitted[0]] = Instance(splitted[0], int(splitted[1]), True)


        return values


def read_results(file):
    bestValues = {}
    avgTimes = {}
    avgValues = {}
    f = open(file, "r")

    content = f.read().splitlines()

    for i in range(len(content)):
        if(content[i][0:9] == "instances"):
            instance = content[i].split(',')[0].split('/')[1]
            bestCost = content[i].split(',')[1]
            avgCost = content[i].split(',')[2]
            avgTime = content[i].split(',')[3]

            bestValues[instance] = int(bestCost)
            avgValues[instance] = float(avgCost)
            avgTimes[instance] = float(avgTime)
    
    return bestValues, avgValues, avgTimes



ref = read_ref()



[bests, avgs, times]  = read_results("HGS_results-5_10_5_3_1000.txt")

wins = 0
notOPTinst = 0
notOPTwins = 0
total = 0
avgGap = 0
avgTime = 0
bestGap = 0

instBest = {};

for key in bests:
    total+=1
    if(ref[key].value >= bests[key]):
        wins+=1
        instBest[key] = bests[key]
    else:
        instBest[key] = ref[key].value
    #    print(key, ref[key].value - bests[key])

    bestGap += bests[key] - ref[key].value
    avgGap += avgs[key] - ref[key].value
    avgTime += times[key]
    
    if(ref[key].isOptimal == False):
        notOPTinst+=1
        if(ref[key].value >= bests[key]):
            notOPTwins+=1
    
    if(ref[key].isOptimal == True and bests[key] < ref[key].value):
        print("lascou", key)
        #data40_68.txt 3487 3481
        #data60_13.txt 3491 3483
        #3487 #3491
        #3481 #3483

avgGap = avgGap/total
bestGap = bestGap/total

print("Best gap: ", bestGap)
print("Average gap: ", avgGap)
print("Average Time: ", avgTime/total)
print("hgs ganhou em ", wins, " de ", total)
print("hgs ganhou em ", notOPTwins, " de ", notOPTinst, "instancias em que o opt não foi encontrado")


#generating file with bests
'''
g = open("bests-SMINVCMAX.csv", "w")

import pandas as pd

(pd.DataFrame.from_dict(data=instBest, orient='index')
   .to_csv('bests-SMINVCMAX.csv', header=False))'''
