#!/usr/bin/env python

import sys,os,urllib,json,getopt

run = None
url = ''

try:
    opts, args = getopt.getopt(sys.argv[1:], "", ["run="])
except getopt.GetoptError:
    print 'Please specify run: --run=132440'
    sys.exit(2)

# check command line parameter
for opt, arg in opts :
    if opt == "--run" :
        run = arg

if run == None:
    print 'Current configuration!'
    print ''

print ''
print 'EXPRESS'
print ''

if run == None:
    url = 'http://vocms52.cern.ch:8889/tier0/express_config?run=&stream=Express&return_type=application/json'
else :
    url = 'http://vocms52.cern.ch:8889/tier0/express_config?run='+run+'&stream=Express&return_type=application/json'

result = json.load(urllib.urlopen(url))

for entry in result:
    print 'run:',entry['run']
    print 'stream:',entry['stream']
    print 'proc_version:',entry['proc_version']
    print 'global_tag:',entry['global_tag']
    print 'processing_config_url:',entry['processing_config_url']
    print ''

print 'Prompt'
print ''

if run == None:
    url = 'http://vocms52.cern.ch:8889/tier0/reco_config?run=&dataset=&return_type=application/json'
else :
    url = 'http://vocms52.cern.ch:8889/tier0/reco_config?run='+run+'&dataset=&return_type=application/json'

result = json.load(urllib.urlopen(url))

for entry in result:
    if entry['cmssw_version'] != 'Undefined':
        print 'run:',entry['run']
        print 'primary_dataset:',entry['primary_dataset']
        print 'proc_version:',entry['proc_version']
        print 'cmssw_version:',entry['cmssw_version']
        print 'global_tag:',entry['global_tag']
        print ''
