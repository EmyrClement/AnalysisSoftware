#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/Pileup_2011_to_173692_LPLumiScale_68mb.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False

#input files for analysis
inputFiles = ['/scratch/TopQuarkGroup/data/ElectronHad/nTuple_v4a_Run2011A-05Aug2011-v1_GoldenJSON_electronSkim/919e07a072adba25d339f168e288c1b2/ElectronHad_nTuple_42x_data_1_1_8Iz.root']

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 3934.085
