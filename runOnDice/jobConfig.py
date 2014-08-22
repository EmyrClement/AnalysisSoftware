from optparse import OptionParser
import os
from time import time

class Timer():

    def __init__(self):
        self.start_time =  time()
    
    def elapsed_time(self):
        return time() - self.start_time
    
    def restart(self):
        self.start_time =  time()
        pass
    pass
        
def writeRunningScript(job):
    runningScript = open('script.sh','w')
    
    runningScript.write('#!/bin/bash\n')
    runningScript.write('machine="soolin"\n')
    runningScript.write('exe=BAT\n')
    runningScript.write('toolsFolder=BristolAnalysis/Tools/\n')
    runningScript.write('export toolsFolder=${toolsFolder}\n')
    runningScript.write('export exe=${exe}\n')
    runningScript.write('export TQAFPath=${TQAFPath}\n')
    runningScript.write(job+'\n')
    runningScript.close()
    pass

parser = OptionParser()
parser.add_option('-j','--job_number',type='int',dest='jobNumber',default=0)
(options, _) = parser.parse_args()

jobs=[]

print 'Running job',jobs[options.jobNumber-1]

writeRunningScript(jobs[options.jobNumber-1])

# Run script
start = time()

os.system('chmod a+rwx script.sh')
os.system('./script.sh')
# os.system('source script.sh')

print 'Elapsed time :',time()-start



