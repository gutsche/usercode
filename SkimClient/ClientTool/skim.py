#!/usr/bin/env python
from skim_help import *
from skim_util import *
from skim_exceptions import *
from WorkSpace import WorkSpace
from Creator import Creator
from Submitter import Submitter
import common

import sys, os, time, string

###########################################################################
class Skim:
    def __init__(self, opts):
        # The order of main_actions is important !
        self.main_actions = [ '-create', '-submit' ] 
        self.aux_actions = []

        # Dictionary of actions, e.g. '-create' -> object of class Creator
        self.actions = {}
      
        # Configuration file
        self.cfg_fname = None
        # Dictionary with configuration parameters
        self.cfg_params = {}

        # Current working directory
        self.cwd = os.getcwd()+'/'
        # Current time in format 'yymmdd_hhmmss'
        self.current_time = time.strftime('%y%m%d_%H%M%S',
                                          time.localtime(time.time()))

        # Session name (?) Do we need this ?
        self.name = '0'

        # Continuation flag
        self.flag_continue = 0

        # quiet mode, i.e. no output on screen
        self.flag_quiet = 0
        # produce more output
        self.debug_level = 0

        # printout
        print ''
        print common.prog_name,common.prog_version_str
        print ''

        self.initialize_(opts)

        return

    def version():
        return common.prog_version_str

    version = staticmethod(version)

    def initialize_(self, opts):

        # Process the '-continue' option first because
        # in the case of continuation the SKIM configuration
        # parameters are loaded from already existing Working Space.
        self.processContinueOption_(opts)

        # Process ini-file first, then command line options
        # because they override ini-file settings.

        self.processIniFile_(opts)

        if self.flag_continue: opts = self.loadConfiguration_(opts)
        
        self.processOptions_(opts)

        if not self.flag_continue:
            self.createWorkingSpace_()
            optsToBeSaved={}
            for it in opts.keys():
                if (it in self.main_actions) or (it in self.aux_actions) or (it == '-debug'):
                    pass
                else:
                    optsToBeSaved[it]=opts[it]
            common.work_space.saveConfiguration(optsToBeSaved, self.cfg_fname)
            pass

        # At this point all configuration options have been read.
        
        args = string.join(sys.argv,' ')

        self.updateHistory_(args)

        self.initializeActions_(opts)
        return

    def processContinueOption_(self,opts):

        continue_dir = None

        # Look for the '-continue' option.

        for opt in opts.keys():
            if ( opt in ('-continue','-c') ):
                self.flag_continue = 1
                val = opts[opt]
                if val:
                    if val[0] == '/': continue_dir = val     # abs path
                    else: continue_dir = self.cwd + val      # rel path
                    pass
                break
            pass

        # Look for actions which has sense only with '-continue'

        if not self.flag_continue:
            for opt in opts.keys():
                if ( opt in (self.aux_actions) ):
                    self.flag_continue = 1
                    break
                pass
            pass
            submit_flag=0
            create_flag=0
            for opt in opts.keys():
                if opt == "-submit": submit_flag=1
                if opt == "-create": create_flag=1
                pass
            if (submit_flag and not create_flag):
                msg = "'-submit' must be used with either '-create' or '-continue'."
                raise SkimException(msg)
            pass

        if not self.flag_continue: return

        if not continue_dir:
            prefix = common.prog_name + '_' + self.name + '_'
            continue_dir = findLastWorkDir(prefix)
            pass

        if not continue_dir:
            raise SkimException('Cannot find last working directory.')

        if not os.path.exists(continue_dir):
            msg = 'Cannot continue because the working directory <'
            msg += continue_dir
            msg += '> does not exist.'
            raise SkimException(msg)

        # Instantiate WorkSpace
        common.work_space = WorkSpace(continue_dir, self.cfg_params)

        return

    def processIniFile_(self, opts):
        """
        Processes a configuration INI-file.
        """

        # Extract cfg-file name from the cmd-line options.
    
        for opt in opts.keys():
            if ( opt == '-cfg' ):
                if self.flag_continue:
                    raise SkimException('-continue and -cfg cannot coexist.')
                if opts[opt] : self.cfg_fname = opts[opt]
                else : usage()
                pass

            elif ( opt == '-name' ):
                self.name = opts[opt]
                pass

            pass

        # Set default cfg-fname
        
        if self.cfg_fname == None:
            if self.flag_continue:
                self.cfg_fname = common.work_space.cfgFileName()
            else:
                self.cfg_fname = common.prog_name+'.cfg'
                pass
            pass

        # Load cfg-file
        
        if string.lower(self.cfg_fname) != 'none':
            if os.path.exists(self.cfg_fname):
                self.cfg_params = loadConfig(self.cfg_fname)
                self.cfg_params['user'] = os.environ['USER']
                pass
            else:
                msg = 'cfg-file '+self.cfg_fname+' not found.'
                raise SkimException(msg)
                pass
            pass

        return

    def processOptions_(self, opts):
        """
        Processes the command-line options.
        """

        for opt in opts.keys():
            val = opts[opt]

            # Skip actions, they are processed later in initializeActions_()
            if opt in self.main_actions:
                self.cfg_params['SKIM.'+opt[1:]] = val
                continue
            if opt in self.aux_actions:
                self.cfg_params['SKIM.'+opt[1:]] = val
                continue
            

            elif ( opt == '-cfg' ):
                pass

            elif ( opt in ('-continue', '-c') ):
                # Already processed in processContinueOption_()
                pass

            elif ( opt == '-Q' ):
                self.flag_quiet = 1
                pass

            elif ( opt == '-debug' ):
                if val: self.debug_level = int(val)
                else: self.debug_level = 1
                pass

            elif string.find(opt,'.') == -1:
                print common.prog_name+'. Unrecognized option '+opt
                usage()
                pass

            # Override config parameters from INI-file with cmd-line params
            if string.find(opt,'.') == -1 :
                self.cfg_params['SKIM.'+opt[1:]] = val
                pass
            else:
                # Command line parameters in the form -SECTION.ENTRY=VALUE
                self.cfg_params[opt[1:]] = val
                pass
            pass
        return

    def parseRange_(self, aRange):
        """
        Takes as the input a string with a range defined in any of the following
        way, including combination, and return a tuple with the ints defined
        according to following table. A consistency check is done.
        NB: the first job is "1", not "0".
        'all'       -> [1,2,..., NJobs]
        ''          -> [1,2,..., NJobs]
        'n1'        -> [n1]
        'n1-n2'     -> [n1, n1+1, n1+2, ..., n2-1, n2]
        'n1,n2'     -> [n1, n2]
        'n1,n2-n3'  -> [n1, n2, n2+1, n2+2, ..., n3-1, n3]
        """
        result = []
 
        common.logger.debug(5,"parseRange_ "+str(aRange))
        if aRange=='all' or aRange==None or aRange=='':
            result=range(1,common.jobDB.nJobs()+1)
            return result
        elif aRange=='0':
            return result

        subRanges = string.split(aRange, ',')
        for subRange in subRanges:
            result = result+self.parseSimpleRange_(subRange)

        if self.checkUniqueness_(result):
            return result
        else:
            common.logger.message("Error "+result)
            return []

    def checkUniqueness_(self, list):
        """
        check if a list contains only unique elements
        """

        uniqueList = []
        # use a list comprehension statement (takes a while to understand) 

        [uniqueList.append(it) for it in list if not uniqueList.count(it)]

        return (len(list)==len(uniqueList))

    def parseSimpleRange_(self, aRange):
        """
        Takes as the input a string with two integers separated by
        the minus sign and returns the tuple with these numbers:
        'n1-n2' -> [n1, n1+1, n1+2, ..., n2-1, n2]
        'n1'    -> [n1]
        """
        (start, end) = (None, None)
        
        result = []
        minus = string.find(aRange, '-')
        if ( minus < 0 ):
            if isInt(aRange) and int(aRange)>0:
                # FEDE
                #result.append(int(aRange)-1)
                ###
                result.append(int(aRange))
            else:
                common.logger.message("parseSimpleRange_  ERROR "+aRange)
                usage()
                pass
  
            pass
        else:
            (start, end) = string.split(aRange, '-')
            if isInt(start) and isInt(end) and int(start)>0 and int(start)<int(end):
                #result=range(int(start)-1, int(end))
                result=range(int(start), int(end)+1) #Daniele  
            else:
                common.logger.message("parseSimpleRange_ ERROR "+start+end)

        return result

    def initializeActions_(self, opts):
        """
        For each user action instantiate a corresponding
        object and put it in the action dictionary.
        """

        for opt in opts.keys():
          
            val = opts[opt]
                                                                                                               
 
            if (  opt == '-create' ):
                ncjobs = 0
                if val:
                    if ( isInt(val) ):
                        ncjobs = int(val)
                    elif ( val == 'all'):
                        ncjobs = val
                    else:
                        msg = 'Bad creation bunch size <'+str(val)+'>\n'
                        msg += '      Must be an integer or "all"'
                        msg += '      Generic range is not allowed"'
                        raise SkimException(msg)
                    pass
                else: ncjobs = 'all'

                if ncjobs != 0:
                    # Instantiate Creator object
                    self.creator = Creator(self.cfg_params,
                                           ncjobs)
                    self.actions[opt] = self.creator
                    pass
                pass

            elif ( opt == '-submit' ):

                self.actions[opt] = Submitter(self.cfg_params)

        return

    def createWorkingSpace_(self):
        new_dir = ''

        try:
            new_dir = self.cfg_params['USER.ui_working_dir']
            self.cfg_params['taskId'] = self.cfg_params['user'] + '_' + string.split(new_dir, '/')[len(string.split(new_dir, '/')) - 1] + '_' + self.current_time
            if os.path.exists(new_dir):
                if os.listdir(new_dir):
                    msg = new_dir + ' already exists and is not empty. Please remove it before create new task'
                    raise SkimException(msg)
        except KeyError:
            new_dir = common.prog_name + '_' + self.name + '_' + self.current_time
            self.cfg_params['taskId'] = self.cfg_params['user'] + '_' + new_dir 
            new_dir = self.cwd + new_dir
            pass
        common.work_space = WorkSpace(new_dir, self.cfg_params)
        common.work_space.create()
        return

    def loadConfiguration_(self, opts):

        save_opts = common.work_space.loadSavedOptions()

        # Override saved options with new command-line options

        for k in opts.keys():
            save_opts[k] = opts[k]
            pass

        # Return updated options
        return save_opts

    def updateHistory_(self, args):
        history_fname = common.prog_name+'.history'
        history_file = open(history_fname, 'a')
        history_file.write(self.current_time+': '+args+'\n')
        history_file.close()
        return

    def headerString_(self):
        """
        Creates a string describing program options either given in
        the command line or their default values.
        """
        header = common.prog_name + ' (version ' + common.prog_version_str + \
             ') running on ' + \
             time.ctime(time.time())+'\n\n' + \
             common.prog_name+'. Working options:\n'
        #print self.job_type_name 
        header = header +\
                 '  scheduler           ' + self.scheduler_name + '\n'+\
                 '  job type            ' + self.job_type_name + '\n'+\
                 '  working directory   ' + common.work_space.topDir()\
                 + '\n'
        return header
    
    def createScheduler_(self):
        """
        Creates a scheduler object instantiated by its name.
        """
        klass_name = 'Scheduler' + string.capitalize(self.scheduler_name)
        file_name = klass_name
        try:
            klass = importName(file_name, klass_name)
        except KeyError:
            msg = 'No `class '+klass_name+'` found in file `'+file_name+'.py`'
            raise SkimException(msg)
        except ImportError, e:
            msg = 'Cannot create scheduler '+self.scheduler_name
            msg += ' (file: '+file_name+', class '+klass_name+'):\n'
            msg += str(e)
            raise SkimException(msg)

        common.scheduler = klass()
        common.scheduler.configure(self.cfg_params)
        return

#    def createJobtype_(self):
#        """
#        Create the jobtype specified in the skim.cfg file
#        """
#        file_name = 'cms_'+ string.lower(self.job_type_name)
#        klass_name = string.capitalize(self.job_type_name)
#
#        try:
#            klass = importName(file_name, klass_name)
#        except KeyError:
#            msg = 'No `class '+klass_name+'` found in file `'+file_name+'.py`'
#            raise SkimException(msg)
#        except ImportError, e:
#            msg = 'Cannot create job type '+self.job_type_name
#            msg += ' (file: '+file_name+', class '+klass_name+'):\n'
#            msg += str(e)
#            raise SkimException(msg)
#        job_type = klass(self.cfg_params)
#        return job_type

    def run(self):
        """
        For each 
        """

        for act in self.main_actions:
            if act in self.actions.keys(): self.actions[act].run()
            pass

        for act in self.aux_actions:
            if act in self.actions.keys(): self.actions[act].run()
            pass
        return

###########################################################################
def processHelpOptions(opts):

    if len(opts):
        for opt in opts.keys():
            if opt in ('-v', '-version', '--version') :
                print Skim.version()
                return 1
            if opt in ('-h','-help','--help') :
                if opts[opt] : help(opts[opt])
                else:          help()
                return 1
    else:
        usage()

    return 0

###########################################################################
if __name__ == '__main__':

    # Parse command-line options and create a dictionary with
    # key-value pairs.

    options = parseOptions(sys.argv[1:])

    # Process "help" options, such as '-help', '-version'

    if processHelpOptions(options) : sys.exit(0)

    # Create, initialize, and run a Skim object

    try:
        skim = Skim(options)
        skim.run()
    except SkimException, e:
        print '\n' + common.prog_name + ': ' + str(e) + '\n'
        pass

    pass
