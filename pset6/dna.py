# -*- coding: utf-8 -*-
"""
Created on Wed Jul 14 16:50:35 2021

@author: nehneh
"""

import csv
import sys


def main():

    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py STR-csv CSV-txt")
        
    db = {}
    filename = sys.argv[2]
    with open(filename) as file:
        reader = csv.reader(file)
        headers = next(reader, None)
        for header in headers:
            db[header] = []
        for row in reader:
            for h, v in zip(headers, row):
                db[h].append(v)
    txt_file = sys.argv[1]
    f = open(txt_file, "r")
    dna = f.read()
    headers.remove('name') 
    count_dic = {}
    count =0
    for header in headers:
        count_dic[header] = 0
        gene = header
        while gene in dna:
            count+=1
            gene+=header
        count_dic[header] = count
        count = 0
    match_count = {}
    count = 0
    for names in db['name']:
        match_count[names] = 0
        index_name = db['name'].index(names)
        for header in headers:
            if int(db[header][index_name]) == int(count_dic[header]):
                match_count[names] += 1
    for names in db['name']:
        if match_count[names] == len(headers):
            print(names)
            return
        
    print('No match')
            
        
  
main()