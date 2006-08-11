#!/usr/bin/python

import sys, os, getopt

def main(argv) :
    """
    
    extractTimingFromCMSSW
    
    activate timing service in CMSSW and use this python script tp average timing per module

    required parameters
    --logfile <name>                          :       logfile name

    optional parameters                       :
    --help (-h)                               :       help
    --debug (-d)                              :       debug statements
    
    """

    # default
    logfilename = ''
    debug = 0

    try:
        opts, args = getopt.getopt(argv, "", ["help", "debug", "logfile="])
    except getopt.GetoptError:
        print main.__doc__
        sys.exit(2)

    # check command line parameter
    for opt, arg in opts :
        if opt == "--help" :
            print main.__doc__
            sys.exit()
        elif opt == "--debug" :
            debug = 1
        elif opt == "--logfile" :
            logfilename = arg

    if logfilename == '' :
        print main.__doc__
        sys.exit(2)
        
    try:
        logfile = open(logfilename)
    except IOError:
        print ''
        print 'Could not open logfile: ',logfilename
        print main.__doc__
        sys.exit(2)

    # instantiate dictionaries and counters
    timeEvent       = 0
    timeEventCount  = 0
    timeModuleDict  = {}
    timeModuleCount = {}

    line = logfile.readline()
    tempeventtime = 0
    addedmoduletime = 0
    while line :
        line = line.strip()
        if line.count('TimeEvent>') > 0 :
            if debug :
                print 'TimeEvent line:',line
            linearray = line.split()
            try:
                tempeventtime = float(linearray[-1])
                timeEvent      += tempeventtime
                timeEventCount += 1
            except ValueError:
                print 'line:',line,'could not be used to extract time of the event'
        if line.count('TimeModule>') > 0 :
            if debug :
                print 'TimeModule line:',line
            try:
                linearray = line.split()
                if len(linearray) == 6 :
                    temptime = float(linearray[-1])
                    addedmoduletime += temptime
                    tempmodule = linearray[-2]
                    if tempmodule in timeModuleDict.keys() :
                        timeModuleDict[tempmodule] += temptime
                    else :
                        timeModuleDict[tempmodule] = temptime
                    if tempmodule in timeModuleCount.keys() :
                        timeModuleCount[tempmodule] += 1
                    else :
                        timeModuleCount[tempmodule] = 1
            except ValueError:
                print 'line:',line,'could not be used to extract time of a module'
        line = logfile.readline()
        if tempeventtime != 0 :
            # print '%2.5f' % (tempeventtime-addedmoduletime)
            tempeventtime = 0
            addedmoduletime = 0

    for module in timeModuleDict.keys() :
        print 'module: %30s time: %2.5f percentage: %02.1f' % (module, timeModuleDict[module]/timeModuleCount[module], (timeModuleDict[module]/timeModuleCount[module])/(timeEvent/timeEventCount)*100)

    print 'event :  %2.5f' % (timeEvent/timeEventCount)

    if 'PoolOutputModule' in timeModuleDict.keys():
        print ''
        print 'remove PoolOutput Module form statistic'
        print ''
        tempTimeEvent = 0
        for module in timeModuleDict.keys() :
            if module != 'PoolOutputModule' :
                tempTimeEvent += timeModuleDict[module]/timeModuleCount[module]
        for module in timeModuleDict.keys() :
            if module != 'PoolOutputModule' :
                print 'module: %30s time: %2.5f percentage: %02.1f' % (module, timeModuleDict[module]/timeModuleCount[module], (timeModuleDict[module]/timeModuleCount[module])/(tempTimeEvent)*100)

    modules = ['RoadSearchCloudCleaner','TrackProducer','RoadSearchTrackCandidateMaker','RoadSearchSeedFinder','RoadSearchCloudMaker']

    doTracking = 1
    for target in modules:
        if target not in timeModuleDict.keys():
            doTracking = 0

    if doTracking :
        print ''
        print 'Statistics only for modules:'
        print ''
        for module in modules:
            print module
        print ''
        tempTimeEvent = 0
        for module in modules :
            if module != 'PoolOutputModule' :
                tempTimeEvent += timeModuleDict[module]/timeModuleCount[module]
        for module in modules :
            if module != 'PoolOutputModule' :
                print 'module: %30s time: %2.5f percentage: %02.1f' % (module, timeModuleDict[module]/timeModuleCount[module], (timeModuleDict[module]/timeModuleCount[module])/(tempTimeEvent)*100)
        

if __name__ == '__main__' :
    main(sys.argv[1:])
