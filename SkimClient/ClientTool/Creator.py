from WorkSpace import WorkSpace
from skim_exceptions import *
from skim_util import *
import Scram
import PsetManipulator
import common

import os, string, math
class Creator:
    def __init__(self, cfg_params, ncjobs):

        # parameter
        self.cfg_params = cfg_params
        self.ncjobs = ncjobs

        # dataset
        try: 
            self.Dataset = cfg_params['SKIM.dataset']
        except:
            msg = 'Please specify a dataset'
            raise SkimException(msg)

        # pset
        try:
            self.pset = cfg_params['SKIM.pset']
            if (not os.path.exists(self.pset)):
                raise SkimException("User defined PSet file "+self.pset+" does not exist")
        except KeyError:
            raise SkimException("PSet file missing. Cannot run cmsRun ")

        # scram
        self.scram = Scram.Scram(cfg_params)
        scramArea = ''
        self.additional_input_files = []
        self.version = self.scram.getSWVersion()
        
        # name of tarball
        self.tgz_name = 'default.tgz'

        # additional input files
        try:
            tmpAddFiles = string.split(cfg_params['SKIM.additional_input_files'],',')
            for tmp in tmpAddFiles:
                if not os.path.exists(tmp):
                    raise SkimException("Additional input file not found: "+tmp)
                self.additional_input_files.append(string.strip(tmp))
                pass
            pass
        except KeyError:
            pass

        # files per job
        try:
            if (cfg_params['SKIM.files_per_jobs']):
                self.filesPerJob =int( cfg_params['SKIM.files_per_job'])
                self.selectFilesPerJob = 1
        except KeyError:
            self.filesPerJob = -1
            self.selectFilesPerJob = 0

        # Events per job
        try:
            self.eventsPerJob =int( cfg_params['SKIM.events_per_job'])
            self.selectEventsPerJob = 1
        except KeyError:
            self.eventsPerJob = -1
            self.selectEventsPerJob = 0

        # number of jobs
        try:
            self.theNumberOfJobs =int( cfg_params['SKIM.number_of_jobs'])
            self.selectNumberOfJobs = 1
        except KeyError:
            self.theNumberOfJobs = 0
            self.selectNumberOfJobs = 0

        # total number of events   
        try:
            self.total_number_of_events = int(cfg_params['SKIM.total_number_of_events'])
            self.selectTotalNumberEvents = 1
        except KeyError:
            self.total_number_of_events = 0
            self.selectTotalNumberEvents = 0

        # total number of files
        try:
            self.total_number_of_files = int(cfg_params['SKIM.total_number_of_files'])
            self.selectTotalNumberFiles = 1
        except KeyError:
            self.total_number_of_files = 0
            self.selectTotalNumberFiles = 0


        if ( self.selectFilesPerJob + self.selectEventsPerJob == 2) :
            msg = 'Must define either files per job or events per job.'
            raise SkimException(msg)
            
        if ( self.selectEventsPerJob ) :
            if ( (self.selectTotalNumberEvents + self.selectEventsPerJob + self.selectNumberOfJobs) != 2 ):
                msg = 'Must define exactly two of total_number_of_events, events_per_job, or number_of_jobs.'
                raise SkimException(msg)

        if ( self.selectFilesPerJob ) :
            if ( (self.selectTotalNumberFiles + self.selectNumberOfJobs + self.selectFilesPerJob) != 2 ):
                msg = 'Must define exactly two of total_number_of_events, events_per_job, or number_of_jobs.'
                raise SkimException(msg)
        return

    def run(self):
        """
        The main method of the class.
        """

        # modify Pset
        try:
            self.PsetEdit = PsetManipulator.PsetManipulator(self.pset)
            self.PsetEdit.inputModule("INPUT")
            self.PsetEdit.maxEvent("INPUTMAXEVENTS")
            self.PsetEdit.skipEvent("INPUTSKIPEVENTS")
            self.PsetEdit.psetWriter(common.prog_name+'.cfg')
        except:
            msg='Error while manipuliating ParameterSet: exiting...'
            raise SkimException(msg)

        # prepare tarball
        self.tgzNameWithPath = self.getTarBall()

        # write parameter in txt file in skim-dir/share
        file1 = open(common.work_space.shareDir()+'/saved_parameters',"w")
        output  = 'parameter_set::'+common.work_space.jobDir()+common.prog_name+'.cfg,'
        output += 'tarball::'+self.tgzNameWithPath+','
        output += 'total_number_of_events::'+str(self.total_number_of_events)+','
        output += 'events_per_job::'+str(self.eventsPerJob)+','
        output += 'total_number_of_files::'+str(self.total_number_of_files)+','
        output += 'files_per_job::'+str(self.filesPerJob)+','
        output += 'number_of_jobs::'+str(self.theNumberOfJobs)+','
        output += 'create::'+str(self.ncjobs)+','
        output += 'dataset::'+self.Dataset+','
        output += 'version::'+self.version
        file1.write(output)
        file1.close()

        print ''
        print 'Creation summary in '+common.work_space.shareDir()+'/saved_parameters'
        print ''
        print '%22s: %s' % ('parameter_set',common.work_space.jobDir()+common.prog_name+'.cfg')
        print '%22s: %s' % ('tarball',self.tgzNameWithPath)
        print '%22s: %s' % ('dataset',self.Dataset)
        print '%22s: %s' % ('version',self.version)
        print ''
        print '%22s: %s' % ('total_number_of_events',self.total_number_of_events)
        print '%22s: %s' % ('events_per_job',self.eventsPerJob)
        print '%22s: %s' % ('total_number_of_files',self.total_number_of_files)
        print '%22s: %s' % ('files_per_job',self.filesPerJob)
        print '%22s: %s' % ('number_of_jobs',self.theNumberOfJobs)
        print ''
        print '%22s: %s' % ('create jobs:',self.ncjobs)
        

        return

    def getTarBall(self):
        """
        Return the TarBall with libs
        """

        # if it exist, just return it
        self.tgzNameWithPath = common.work_space.shareDir()+self.tgz_name
        if os.path.exists(self.tgzNameWithPath):
            return self.tgzNameWithPath

        # Prepare a tar gzipped file with user binaries.
        self.buildTar_()

        return string.strip(self.tgzNameWithPath)

    def buildTar_(self):

        # First of all declare the user Scram area
        swArea = self.scram.getSWArea_()
        #print "swArea = ", swArea
        swVersion = self.scram.getSWVersion()
        #print "swVersion = ", swVersion
        swReleaseTop = self.scram.getReleaseTop_()
        #print "swReleaseTop = ", swReleaseTop

        ## check if working area is release top
        if swReleaseTop == '' or swArea == swReleaseTop:
            return

        filesToBeTarred = []

        ## Now get the libraries: only those in local working area
        libDir = 'lib'
        lib = swArea+'/' +libDir
        if os.path.exists(lib):
            filesToBeTarred.append(libDir)

        ## Now check if module dir is present
        moduleDir = 'module'
        if os.path.isdir(swArea+'/'+moduleDir):
            filesToBeTarred.append(moduleDir)

        ## Now check if the Data dir is present
        dataDir = 'src/Data/'
        if os.path.isdir(swArea+'/'+dataDir):
            filesToBeTarred.append(dataDir)

        ## additional input files
        # check total size of additional input files
        size = 0
        for file in self.additional_input_files :
            size += os.path.getsize(file)
        # if greater than 10 MB, skip)
        copied_additional_input_files = 0
        if ( size > 10485760 ) :
            print 'WARNING: size input files larger than 10 MB, skipped'
        else :
            copied_additional_input_files = 1
            for file in self.additional_input_files :
                cmd = 'cp '+file+' '+swArea
                cmd_out = runCommand(cmd)
                if cmd_out != '' :
                    msg = 'Could not copy file '+file+' to '+swArea+'. Skipping it.'
                    raise SkimException(msg)
                else :
                    filesToBeTarred.append(os.path.basename(file))
        
        ## Create the tar-ball
        if len(filesToBeTarred)>0:
            cwd = os.getcwd()
            os.chdir(swArea)
            tarcmd = 'tar zcvf ' + self.tgzNameWithPath + ' '
            for line in filesToBeTarred:
                tarcmd = tarcmd + line + ' '
            cout = runCommand(tarcmd)
            if not cout:
                raise SkimException('Could not create tar-ball')
            os.chdir(cwd)

        # delete additonal input files from swArea
        if copied_additional_input_files == 1 :
            for file in self.additional_input_files :
                cmd = 'rm -f '+swArea+'/'+os.path.basename(file)
                cmd_out = runCommand(cmd)
                if cmd_out != '' :
                    msg = 'Could not rm file '+swArea+'/'+os.path.basename(file)
                    raise SkimException(msg)


        return
