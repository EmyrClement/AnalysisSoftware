import os, datetime

baseDir='/hdfs/tmp/TQG/'
targetBaseDir = '/hdfs/TopQuarkGroup/results/histogramfiles/AN-14-071_4th_draft/'

def getSampleSuffix(sample):
    sampleType = sample.split('_')[-1].split('.')[0]
    sampleSuffix = ''

    if sampleType.find('plus')>=0 or sampleType.find('minus')>=0 or sampleType.find('mb')>=0:
        sampleSuffix = samples[sampleType]
    else:
        sampleSuffix='central'
    
    if sampleType.find('Bjet')>=0:
        sampleSuffix=sampleSuffix.replace('Bjet','BJet')

    return sampleSuffix

samples={
'minusBJet':'BJet_down',
'plusBJet':'Bjet_up',
'minusElectron':'Electron_down',
'plusElectron':'Electron_up',
'minusJER':'JER_down',
'plusJER':'JER_up',
'plusJES':'JES_up',
'minusJES':'JES_down',
'plusLightJet':'LightJet_down',
'minusLightJet':'LightJet_up',
'plusMuon':'Muon_down',
'minusMuon':'Muon_up',
'64600mb':'PU_down',
'71400mb':'PU_up',
'65835mb':'PU_down',
'72765mb':'PU_up',
}

lumi = {
    '19584' : '8',
    '5050' : '7'
}

listOfFiles = os.listdir(baseDir)

# Make new dir
today = datetime.date.today()
todaystr = today.isoformat()
if os.path.exists(targetBaseDir+todaystr):
    print 'Target direcotry of the day alreayd exists'
    print 'Delete it first'
    os.sys.exit()
os.mkdir(targetBaseDir+todaystr)
targetBaseDir += todaystr+'/'

for file in listOfFiles:
    # Work out lumi to work out whether it's 7 or 8 TeV
    lumiString = lumi[file.split('_')[1][:-2]]

    targetdir = targetBaseDir+lumiString+'TeV/'

    if not os.path.exists(targetdir):
        os.mkdir(targetdir)

    sampleSuffix = getSampleSuffix(file)
    targetdir += sampleSuffix

    if not os.path.exists(targetdir):
        os.mkdir(targetdir)

    command = 'cp '+baseDir+file+' '+targetdir+'/'
    os.system(command)
    




# for sample in samples:
    
#     sampleSuffix = ''
#     if sample.find('central')<0:
#         if sample.find('PU')>=0:
#             if sample.find('down')>=0:
#                 sampleSuffix = '_PU_65835mb'
#             elif sample.find('up')>=0:
#                 sampleSuffix = '_PU_72765mb'
#                 pass
#             pass
#         else:
#             if sample.find('down')>=0:
#                 sampleSuffix = '_minus'+sample.split('_')[0]
#             elif sample.find('up')>=0:
#                 sampleSuffix = '_plus'+sample.split('_')[0]
#                 pass
        
#         if sample.find('Bjet')>=0:
#             sample=sample.replace('Bjet','BJet')
#             pass
#         pass
            
#     sampleName = sampleBase+sampleSuffix+'.root'
#     command = 'cp '+sampleName+' '+targetBaseDir+'/'+sample
#     print command
#     os.system(command)