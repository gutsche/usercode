#!/usr/bin/env python

import sys,getopt
import urllib
from xml.dom import minidom

datasetpath = None
allSites = 0
try:
    opts, args = getopt.getopt(sys.argv[1:], "", ["dataset=","allSites"])
except getopt.GetoptError:
    print 'Please specify dataset with --dataset'
    print 'Specify --allSites to show all site location, otherwise show only T1 sites'
    sys.exit(2)

# check command line parameter
for opt, arg in opts :
    if opt == "--dataset" :
        datasetpath = arg
    if opt == "--allSites" :
        allSites = 1
        
if datasetpath == None:
    print 'Please specify dataset with --dataset'
    sys.exit(2)

url='http://cmsweb.cern.ch/phedex/datasvc/xml/prod/blockreplicas?block=' + datasetpath + '*'
input = urllib.urlopen(url)

xmldoc = minidom.parse(input)

custodial = {}
non_custodial = {}

for phedex in xmldoc.childNodes :
    for block in phedex.childNodes:
        name = block.attributes['name'].value
        for replica in block.childNodes:
            site = replica.attributes['node'].value
            files = replica.attributes['files'].value
##             if site[-4:] == '_MSS':
##                 site = site[:-4]
##             if site[-7:] == '_Buffer':
##                 site = site[:-7]
##             if site[-7:] == '_Export':
##                 site = site[:-7]
            if allSites == 1 or ( site[0:2] == 'T1' or site[0:2] == 'T0') :
                if replica.attributes['custodial'].value == 'y':
                    if site not in custodial.keys() :
                        custodial[site] = int(files)
                    else :
                        custodial[site] += int(files)
                else :
                    if site not in non_custodial.keys() :
                        non_custodial[site] = int(files)
                    else :
                        non_custodial[site] += int(files)


custodial_sites = custodial.keys()
non_custodial_sites = non_custodial.keys()

custodial_sites.sort()
non_custodial_sites.sort()


if len(custodial_sites) == 0 :
    custodial['NONE'] = 0
if len(non_custodial_sites) == 0 :
    non_custodial['NONE'] = 0
if len(custodial_sites) == 1 and custodial_sites[0].count('T0') > 0 :
    custodial['NONE'] = 0
if len(non_custodial_sites) == 1 and non_custodial_sites[0].count('T0') > 0 :
    non_custodial['NONE'] = 0

sites = ''
custsites = ''
for site in non_custodial_sites :
    if site not in custodial_sites :
        sites = sites + site + '(' + str(non_custodial[site]) + '),'
if sites[-1:] == ',' :
    sites = sites[:-1]
for custsite in custodial_sites :
    custsites = custsites + custsite + '(' + str(custodial[custsite]) + '),'
if custsites[-1:] == ',' :
    custsites = custsites[:-1]

print 'dataset:',datasetpath,'custodial:',custsites,'non-custodial:',sites
