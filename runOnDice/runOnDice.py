from optparse import OptionParser
import os
from BristolAnalysis.Tools.datasets2012_cfg import datasets as datasets2012
from BristolAnalysis.Tools.datasets2011_cfg import datasets as datasets2011

datasets2012 = datasets2012.keys()
datasets2011 = datasets2011.keys()

def makeCrabConfig(nJobs):
    oldCrabConfig = open('BristolAnalysis/Tools/runOnDice/crab.cfg','r')
    newCrabConfig = open('crabConfig.cfg','w')
    
    for line in oldCrabConfig.readlines():
        if line.find('number_of_jobs')>=0:
            newCrabConfig.write('number_of_jobs = %i\n' % nJobs)
        else:
            newCrabConfig.write(line)
        pass
    oldCrabConfig.close()
    newCrabConfig.close()
    pass

def writeJobConfig(jobs):
    oldConfig = open('BristolAnalysis/Tools/runOnDice/jobConfig.py','r')
    newConfig = open('jobConfig.py','w')
    
    for line in oldConfig.readlines():
        if line.find('jobs=[]')>=0:
            newConfig.write('jobs = [\n')
            for job in jobs:
                newConfig.write('\''+job+'\',\n')
                pass
            newConfig.write(']\n')
        else:
            newConfig.write(line)
        pass
    oldConfig.close()
    newConfig.close()
    pass

def submitCrabJob():
    os.system('crab -create -cfg crabConfig.cfg')
    os.system('crab -submit -c dummy')
    pass

def removeDataset(dataset, sampleString):
    index = 0
    while index<len(dataset):
        sample = dataset[index]
        if sample.find(sampleString) >= 0:
            dataset.remove(sample)
            pass
        else:
            index += 1
        pass
    return dataset

def pickDatasets( datasets2011, datasets2012 ):
    if not options.runTTJets:
        datasets2012 = removeDataset(datasets2012, 'TT')
        datasets2011 = removeDataset(datasets2011, 'TT')
        pass

    if not options.runSingleTop:
        datasets2012 = removeDataset(datasets2012, 'T_s')
        datasets2012 = removeDataset(datasets2012, 'T_t')
        datasets2012 = removeDataset(datasets2012, 'Tbar_s')
        datasets2012 = removeDataset(datasets2012, 'Tbar_t')
        datasets2011 = removeDataset(datasets2011, 'T_s')
        datasets2011 = removeDataset(datasets2011, 'T_t')
        datasets2011 = removeDataset(datasets2011, 'Tbar_s')
        datasets2011 = removeDataset(datasets2011, 'Tbar_t')
        pass
    
    if not options.runVJets:
        datasets2012 = removeDataset(datasets2012, 'Z')
        datasets2012 = removeDataset(datasets2012, 'W')
        datasets2012 = removeDataset(datasets2012, 'DY')
        datasets2011 = removeDataset(datasets2011, 'Z')
        datasets2011 = removeDataset(datasets2011, 'W')
        datasets2011 = removeDataset(datasets2011, 'DY')
        pass
    
    if not options.runQCD:
        datasets2012 = removeDataset(datasets2012, 'QCD')
        datasets2012 = removeDataset(datasets2012, 'GJets')
        datasets2011 = removeDataset(datasets2011, 'QCD')
        datasets2011 = removeDataset(datasets2011, 'GJets')
        pass
    
    if not options.runData:
        datasets2012 = removeDataset(datasets2012, 'SingleMu')
        datasets2012 = removeDataset(datasets2012, 'SingleElectron')
        datasets2011 = removeDataset(datasets2011, 'SingleMu')
        datasets2011 = removeDataset(datasets2011, 'ElectronHad')
        pass
    
    return datasets2011, datasets2012

def checkOptions():
    # A few checks
    if not ( options.run7TeV or options.run8TeV ):
        print 'You need to run on at least 7 or 8 TeV'
        os.sys.exit()
        pass
    
    if not ( options.runQCD or options.runVJets or options.runSingleTop or options.runTTJets or options.runData ):
        print 'You need to run on at least one type of sample'
        os.sys.exit()
        pass
    
    if not ( options.runCentral or options.runSystematics ):
        print 'You need to run on at least one type of analysis (central or systematics)'
        os.sys.exit()
        pass
    pass

centralCommand7TeV = 'sample="TTJet" analysisMode="central" ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath}'
centralCommand8TeV = 'sample="TTJet" analysisMode="central" ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath}'
systematics = [
               'Electron_up',
               'Electron_down',
               'Muon_up',
               'Muon_down',
               'JES_up',
               'JES_down',
               'JetSmearing_up',
               'JetSmearing_down',
               'PU_up',
               'PU_down',
               'BJet_up',
               'BJet_down',
               'LightJet_up',
               'LightJet_down',
                              ]

parser = OptionParser()
parser.add_option('-c',action='store_true',dest='runCentral')
parser.add_option('--sys',action='store_true',dest='runSystematics')

parser.add_option('-q',action='store_true',dest='runQCD')
parser.add_option('-v',action='store_true',dest='runVJets')
parser.add_option('-s',action='store_true',dest='runSingleTop')
parser.add_option('-t',action='store_true',dest='runTTJets')
parser.add_option('-d',action='store_true',dest='runData')

parser.add_option('--run7',action='store_true',dest='run7TeV')
parser.add_option('--run8',action='store_true',dest='run8TeV')
(options, _) = parser.parse_args()

checkOptions()

# Work out which samples to consider
# Remove them from datasets

datasets2011, datasets2012 = pickDatasets(datasets2011,datasets2012)


# Work out how many jobs there are, and the command to run each one
jobs = [
        ]

if options.run7TeV:
    if options.runCentral:
        jobs.append(centralCommand7TeV)
#         for sample in datasets2011:
#             command = centralCommand7TeV.replace('TTJet',sample)
#             jobs.append(command)
#             if options.runSystematics:
#                 # Don't run systematics for scaleup/down etc. samples
#                 if sample.find('scaleup')>=0 or sample.find('scaledown')>=0 or sample.find('matchingup')>=0 or sample.find('matchingdown')>=0: continue
#                 for systematic in systematics:
#                     command = centralCommand7TeV.replace('central',systematic).replace('TTJet',sample)
#                     jobs.append(command)
#                     pass
#                 pass
#             pass
        pass
    pass

if options.run8TeV:
    if options.runCentral:
        jobs.append(centralCommand8TeV)
#         for sample in datasets2012:
#             command = centralCommand8TeV.replace('TTJet',sample)
#             jobs.append(command)
#             if options.runSystematics:
#                 if sample.find('scaleup')>=0 or sample.find('scaledown')>=0 or sample.find('matchingup')>=0 or sample.find('matchingdown')>=0: continue
#                 for systematic in systematics:
#                     command = centralCommand8TeV.replace('central',systematic).replace('TTJet',sample)
#                     jobs.append(command)
#                     pass
#                 pass
#             pass
        pass
    pass

if len(jobs) == 0:
    print "No jobs to run over.  Provide some options!"
    os.sys.exit()
    pass

nJobs = len(jobs)
print 'Running',nJobs,'jobs'

# Modify crab configuration
makeCrabConfig(nJobs)
     
# Wirte out the config file
writeJobConfig(jobs)
     
submitCrabJob()


