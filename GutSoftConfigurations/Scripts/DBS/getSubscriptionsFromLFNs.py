#!/usr/bin/env python

import sys,getopt,urllib2,json,os,datetime,subprocess,shlex

try:
    opts, args = getopt.getopt(sys.argv[1:], "", ["lfns="])
except getopt.GetoptError:
    print 'Please specify txt file with LFNs with --lfns'
    sys.exit(2)

lfn_filename = None
# check command line parameter
for opt, arg in opts :
    if opt == "--lfns" :
        lfn_filename = arg
        
if lfn_filename == None:
    print 'Please specify PhEDEx request ID with --id'
    sys.exit(2)

files = {}
lfn_file = open(lfn_filename)
for lfn in lfn_file.readlines():
    if lfn == '\n': continue
    lfn = lfn.strip()
    if not lfn.startswith('/store/') :
        print 'lfn:',lfn,'does not begin with /store/, skipping'
        continue
    base = '/'.join(lfn.split('/')[:-2])
    if base not in files.keys(): files[base] = []
    files[base].append(lfn)

datasets = {}

for base in files.keys():
    lfn = files[base][0]
    commandline = "dbs search --query=\"find dataset where file = " + str(lfn) + "\" --noheader --production"
    localargs = shlex.split(commandline)
    output = subprocess.Popen(localargs, shell=False, stdout=subprocess.PIPE)

    lines = output.communicate()[0].split('\n')
    if len(lines) > 2 :
        print ''
        print 'LFN:',lfn,'returned weird output when querying for dataset'
        print ''
        print 'Output of dbs command:',commandline
        print ''
        for line in lines:
            print line
        sys.exit(1)
    if len(lines) > 1 :
        dataset = lines[0].strip()
        if dataset not in datasets.keys(): datasets[dataset] = []
        for file in files[base]:
            datasets[dataset].append(file)
    else :
        print ''
        print 'LFN:',lfn,'returned no output when querying for dataset'
        print ''
        print 'Output of dbs command:',commandline
        print ''
        for line in lines:
            print line
        sys.exit(1)
        
subscriptions = {}

for input_dataset in datasets.keys():
    url='https://cmsweb.cern.ch/phedex/datasvc/json/prod/subscriptions?dataset=' + input_dataset + '&node=*_US_FNAL*'
    jstr = urllib2.urlopen(url).read()
    jstr = jstr.replace("\n", " ")
    result = json.loads(jstr)
    try:
        for dataset in result['phedex']['dataset']:
            for subscription in dataset['subscription']:
                if input_dataset not in subscriptions.keys(): subscriptions[input_dataset] = {}
                subscriptions[input_dataset]['https://cmsweb.cern.ch/phedex/prod/Request::View?request=' + str(subscription['request'])] = {'node':subscription['node'],'custodial':subscription['custodial']}
    except:
        print 'subscription query for dataset:',input_dataset,'did not work'
            
            
for dataset in datasets.keys():
    print ""
    print "dataset:",dataset,"with following LFNs:"
    print ""
    for lfn in datasets[dataset]: print '   '+lfn
    print ""
    if dataset in subscriptions.keys():
        for subscription in subscriptions[dataset].keys() :
            print "   subscription:",subscription,"node:",subscriptions[dataset][subscription]['node'],"custodial:",subscriptions[dataset][subscription]['custodial']
    else:
        print "   NO SUBSCRIPTION INFORMATION FOUND"
