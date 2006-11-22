from skim_util import *
import common

class Submitter:
    def __init__(self, cfg_params):
        self.cfg_params = cfg_params

        return
    
    def run(self):
        """
        The main method of the class.
        """

        # read in parameters
        file = open(common.work_space.shareDir()+'/saved_parameters')
        line = file.readline()
        paras = line.split(',')
        self.parameter = {}
        for element in paras :
            temp = element.split('::')
            self.parameter[temp[0]] = temp[1]
        
        print ''
        print 'Creation summary from '+common.work_space.shareDir()+'/saved_parameters'
        print ''
        print self.parameter
        print ''


        return
