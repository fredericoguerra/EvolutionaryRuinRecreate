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


def read_results():
    values = {}
    f = open("HGS_RuinRecreate_results.txt", "r")

    content = f.read().splitlines()

    for i in range(len(content)):
        if(content[i][0:9] == "instances"):
            instance = content[i].split(',')[0].split('/')[1]
            value = content[i].split(',')[1]
            values[instance] = int(value)
    
    return values



ref = read_ref()
res = read_results()

wins = 0
notOPTinst = 0
notOPTwins = 0
total = 0

for key in res:
    total+=1
    if(ref[key].value >= res[key]):
        wins+=1
        print('win')
    else:
        print(key, ref[key].value - res[key])
    
    if(ref[key].isOptimal == False):
        notOPTinst+=1
        if(ref[key].value >= res[key]):
            notOPTwins+=1
    
    if(ref[key].isOptimal == True and res[key] < ref[key].value):
        print("lascou", key)
        #data40_68.txt 3487 3481
        #data60_13.txt 3491 3483
        #3487 #3491
        #3481 #3483


print("HGS ganhou em ", wins, " de ", total)
print("HGS ganhou em ", notOPTwins, " de ", notOPTinst, "instancias em que o opt não foi encontrado")

'''
data_SMInvCMax/data40_68.txt,3481,3481,8.83678
seed: 1639638328
best_sol: 24 4 13 39 28 3 14 19 21 34 37 40 29 23 27 1 5 35 9 10 20 36 26 11 2 12 17 32 25 6 16 38 22 8 15 18 33 31 30 7'''