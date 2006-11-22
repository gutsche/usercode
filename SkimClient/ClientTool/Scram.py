import common
import os, os.path
from skim_exceptions import *
from skim_util import *

class Scram:
    def __init__(self, cfg_params):
        self.build = 'tgz'
        self.tgz_name = 'default.tgz'
        self.tgzNameWithPath = None

        self.scramVersion = ''
        scramArea = ''

        if os.environ.has_key("SCRAMRT_LOCALRT"):
            # try scram v1
            self.scramArea = os.environ["SCRAMRT_LOCALRT"]
            self.scramVersion = 1
        elif os.environ.has_key("LOCALRT"):
            # try scram v0
            self.scramArea = os.environ["LOCALRT"]
            reVer=re.compile( r'V(\d*)_' )
            if (os.path.exists(self.scramArea+'/config/scram_version')):
                verFile=open(self.scramArea+'/config/scram_version','r')
                lines = verFile.readlines()
                for line in lines:
                    if reVer.search(line):
                        self.scramVersion=int(reVer.search(line).groups()[0])
                        break
                    pass
                verFile.close()
            pass
        elif os.environ.has_key("BASE_PATH"):
            # try scram v0
            self.scramArea = os.environ["BASE_PATH"]
            reVer=re.compile( r'V(\d*)_' )
            if (os.path.exists(self.scramArea+'/config/scram_version')):
                verFile=open(self.scramArea+'/config/scram_version','r')
                lines = verFile.readlines()
                for line in lines:
                    if reVer.search(line):
                        self.scramVersion=int(reVer.search(line).groups()[0])
                        break
                    pass
                verFile.close()
            pass
        elif os.environ.has_key("ORCA_DATA_PATH"): 
            ## Nasty hack to work with ORCA 8.13.3
            tmp = string.split(os.environ["ORCA_DATA_PATH"],":")[1]
            self.scramArea = string.join(string.split(tmp,"/")[:-2],"/")
            reVer=re.compile( r'V(\d*)_' )
            if (os.path.exists(self.scramArea+'/config/scram_version')):
                verFile=open(self.scramArea+'/config/scram_version','r')
                lines = verFile.readlines()
                for line in lines:
                    if reVer.search(line):
                        self.scramVersion=int(reVer.search(line).groups()[0])
                        break
                    pass
                verFile.close()
            else:
                self.scramVersion=1
            pass
        else:
            msg = 'Did you do eval `scram(v1) runtime ...` from your working area ?\n'
            raise SkimException(msg)
        pass

    def commandName(self):
        """ return scram or scramv1 """
        if self.scramVersion == 1: return 'scramv1'
        else: return 'scram'

    def getSWArea_(self):
        """
        Get from SCRAM the local working area location
        """
        return string.strip(self.scramArea)

    def getSWVersion(self):
        """
        Get the version of the sw
        """

        ver = ''
        ##SL take  sw version from scramArea/.SCRAM/Environment
        envFileName=self.scramArea+"/.SCRAM/Environment"
        if os.path.exists(envFileName):
            reVer=re.compile( r'SCRAM_PROJECTVERSION=(\S*)' )
            envFile = open(envFileName,'r')
            lines = envFile.readlines()
            for line in lines:
                if reVer.search(line):
                    ver=reVer.search(line).groups()[0]
                    break
                pass
            envFile.close()
        else:
            msg = 'Cannot find sw version:\n'
            raise SkimException(msg)
        return string.strip(ver)
        
    def getReleaseTop_(self):
       """ get release top """

       result = ''
       envFileName = self.scramArea+"/.SCRAM/Environment"
       try:
           envFile = open(envFileName, 'r')
           for line in envFile:
               line = string.strip(line)
               (k, v) = string.split(line, '=')
               if k == 'RELEASETOP':
                   result=v
                   break
               pass
           pass
       except IOError:
           msg = 'Cannot open scram environment file '+envFileName
           raise SkimException(msg)
           pass
       pass
       return string.strip(result)

    def findFile_(self, filename):
        """
        Find the file in $PATH
        """
        search_path=os.environ["PATH"]
        file_found = 0
        paths = string.split(search_path, os.pathsep)
        for path in paths:
            if os.path.exists(os.path.join(path, filename)):
                file_found = 1
                break
        if file_found:
            return os.path.abspath(os.path.join(path, filename))
        else:
            return None

