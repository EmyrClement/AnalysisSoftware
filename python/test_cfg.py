#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/PileUp2011_v2.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = "BristolAnalysis/Tools/bJetReso.root"
lightJetResoFile = "BristolAnalysis/Tools/lightJetReso.root"
#number of events to be processed
maxEvents = 5000
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False
inputFiles = [
              '/storage/TopQuarkGroup/data/ElectronHad/nTuple_v6_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets/*.root',
#              '/storage/TopQuarkGroup/data/MuHad/nTuple_v6_Run2011B-19Nov2011-v1_GoldenJSON_LeptonPlus3Jets/*.root',
#              '/storage/TopQuarkGroup/mc/TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v6a_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets/*.root'
              ]

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 16000#pb-1
