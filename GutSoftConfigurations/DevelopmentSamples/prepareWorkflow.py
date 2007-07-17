#!/usr/bin/env python

import sys, os

def pickleParameterSet(parameter_set,version):
    """

    pickle parameter set

    """

    import FWCore.ParameterSet.Config as cms
    import pickle

    # load parameter-set
    try:
        process = cms.include(parameter_set)
    except:
        print ''
        print 'ParameterSet: ',parameter_set,'could not be converted to python dictionary'
        print ''
        sys.exit(1)

    # pickle parameter-set by replacing cfg extension with pkl extension
    file = open(addCMSSWVersionToName(parameter_set,version)+".pkl",'w')
    pickle.dump(process,file)

def prepareWorkflow(parameter_set,events,speed_category,version) :
    """

    prepareWorkflow for parameter-set

    """

    from xml.dom.minidom import Document, Element, Text
    
    topNode = Element("RelValSpec")

    element = Element("RelValTest")
    element.setAttribute("Name", parameter_set.replace(".cfg",""))

    SpeedCategoryElement = Element("SpeedCategory")
    SpeedCategoryElement.setAttribute("Value", speed_category)
    element.appendChild(SpeedCategoryElement)

    TotalEventsElement = Element("TotalEvents")
    TotalEventsElement.setAttribute("Value", str(events))
    element.appendChild(TotalEventsElement)

    pickle_file = os.path.join(os.getcwd(), addCMSSWVersionToName(parameter_set,version)+".pkl")

    PickleFileElement = Element("PickleFile")
    PickleFileElement.setAttribute("Value", pickle_file)
    element.appendChild(PickleFileElement)

    topNode.appendChild(element)
    
    doc = Document()
    doc.appendChild(topNode)
    handle = open(addCMSSWVersionToName(parameter_set,version)+".xml", "w")
    handle.write(doc.toprettyxml())
    handle.close()

def addCMSSWVersionToName(name,version) :
    """

    add CMSSW version tag to name starting with DevelopmentSample

    """

    array = name.split(".")[0].split("Sample")

    new_name = array[0] + "Sample" + str(version) + array[-1]

    return new_name

def main(argv) :
    """
    
    prepareWorkflow
    
    prepare PA workflow for cmssw parameter-set including parameter-set pickling
    requires setup'ed CMSSW user project area

    required parameters
    --parameter-set  <name>     : name and path of input parameter-set
    --events         <number>   : number of events to generate
    --cmssw-version  <version>  : CMSSW version string (format: CMSSW_X_Y_Z)

    optional parameters         :
    --speed-category <category> : speed category: Fast, Medium, Slow (default: Fast)
    --help (-h)                 : help
    --debug (-d)                : debug statements
    
    
    """

    # default
    cmssw_version  = ''
    parameter_set  = ''
    events         = 0
    speed_category = "Fast"
    debug          = 0

    import getopt

    try:
        opts, args = getopt.getopt(argv, "", ["help", "debug", "parameter-set=", "events=", "speed-category=","cmssw-version="])
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
        elif opt == "--parameter-set" :
            parameter_set = arg
        elif opt == "--speed-category" :
            speed_category = arg
        elif opt == "--cmssw-version" :
            cmssw_version = arg
        elif opt == "--events" :
            try:
                events = int(arg)
            except:
                print ''
                print 'Events have to be given as a integer'
                print ''
                print main.__doc__
                sys.exit(2)

    if cmssw_version == "" :
        print main.__doc__
        sys.exit(2)

    if parameter_set == '' :
        print main.__doc__
        sys.exit(2)

    if events == 0 :
        print main.__doc__
        sys.exit(2)

    if speed_category != "Fast" and speed_category != "Medium" and speed_category != "Slow" :
        print ''
        print 'Unsupported speed category!'
        print ''
        print main.__doc__
        sys.exit(2)

    if os.path.exists(parameter_set) == False :
        print ''
        print 'Parameter-Set:',parameter_set,'cannot be found!'
        print ''
        print main.__doc__
        sys.exit(2)

    # prepare version tag
    cmssw_version = cmssw_version.replace("CMSSW","").replace("_","")


    # pickle parameter_set
    try:
        pickleParameterSet(parameter_set,cmssw_version)
    except:
        print ''
        print 'Parameter-Set:',parameter_set,'could not be converted into a python dictionary and pickled!'
        print ''
        print main.__doc__
        sys.exit(1)
        
    # prepare workflow
    prepareWorkflow(parameter_set,events,speed_category,cmssw_version)
    try:
        prepareWorkflow(parameter_set,events,speed_category,cmssw_version)
    except:
        print ''
        print 'Workflow for parameter-set:',parameter_set,'could not be created!'
        print ''
        print main.__doc__
        sys.exit(1)

    print 'Workflow:',addCMSSWVersionToName(parameter_set,cmssw_version)+".xml",'created!'
    
if __name__ == '__main__' :
    main(sys.argv[1:])
