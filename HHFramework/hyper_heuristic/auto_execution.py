import os 
import re

for a in range(1,2):
    for j in range(0,6):

        print a,
        print ' ',
        print j

        out_file = open('nsga_config.h', 'w')

        array = []
        with open('nsga_config.h.bak', 'r') as lines:
            for line in lines:
                temp = line.split(' ')
                if len(temp) < 2: 
                    pass
                else:
                    if temp[1] == 'ALG':
                        temp[2] = str(a)
                    if temp[1] == 'TYPE':
                        temp[2] = str(j)
                for i in temp:
                    out_file.write(i + ' ')

        out_file.close()

        os.system('make clean && make')
        os.system('./main_lang_spea.out')

'''
num = [-1, -2, 3, 4]
for a in num:
    for j in range(-1,0):

        print a,
        print ' ',
        print j

        out_file = open('nsga_config.h', 'w')

        array = []
        with open('nsga_config.h.bak', 'r') as lines:
            for line in lines:
                temp = line.split(' ')
                if len(temp) < 2: 
                    pass
                else:
                    if temp[1] == 'ALG':
                        temp[2] = str(a)
                    if temp[1] == 'TYPE':
                        temp[2] = str(j)
                for i in temp:
                    out_file.write(i + ' ')

        out_file.close()

        os.system('make clean && make')
        os.system('./main_joda_learn.out')
'''


