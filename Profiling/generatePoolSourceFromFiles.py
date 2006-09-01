#!/usr/bin/python

import sys, os, getopt, glob

def main(argv) :
    """
    
    generatePoolSourceFromFiles
    
    take a file or a wildcard expression in \" and generat PoolSource

    required parameters
    --file <names>                         :       file names (single file or wildcard expression in \"

    optional parameters                    :
    --help (-h)                            :       help
    --debug (-d)                           :       debug statements
    
    """

    # default
    filename = ''
    debug = 0

    try:
        opts, args = getopt.getopt(argv, "", ["help", "debug", "file="])
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
        elif opt == "--file" :
            logfilename = arg

    if logfilename == '' :
        print main.__doc__
        sys.exit(2)

    output = ''
        
    output += '  source = PoolSource\n'
    output += '  {\n'
    output += '    untracked vstring fileNames = {\n'
    for filename in glob.glob(logfilename) :
        output += '      "file:'+filename+'",\n'
    output = output[:-2]+'\n'
    output += '    }\n'
    output += '    untracked int32 maxEvents = -1\n'
    output += '  }\n'

    print output




        

if __name__ == '__main__' :
    main(sys.argv[1:])
