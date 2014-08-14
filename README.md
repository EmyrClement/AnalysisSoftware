AnalysisSoftware
================

Analysis software for TTbar+X differential cross section analysis (TOP-12-042)

## General Recipe

To setup the stand-alone version of the code:

```
# get the code from the repository
git clone https://github.com/BristolTopGroup/AnalysisSoftware.git AnalysisSoftware
cd AnalysisSoftware

# on DICE need to source ROOT:
source /software/root/v5.32.00/bin/thisroot.sh

# create the makefile using cmake (install it if you don't have it: https://github.com/Kitware/CMake)
cmake28 CMakeLists.txt

# for older versions of boost, e.g. on DICE, you will need to
#sed -i 's/lib64\/lib64/lib64/g' ./CMakeFiles/AnalysisSoftware.dir/build.make
#sed -i 's/lib64\/lib64/lib64/g' ./CMakeFiles/AnalysisSoftware.dir/link.txt
# as the library path is wrong (contains lib64/lib64)

# compile and build
make -j24

# adjust the toolsFolder environment variable
export toolsFolder=$PWD/

# test run the code:
nohup ./AnalysisSoftware python/test_cfg.py &> test.log &
```


To setup the code using CMSSW:

```
# on soolin (scientific linux 5 machines)
export SCRAM_ARCH=slc5_amd64_gcc462
cmsrel CMSSW_6_0_0
cd CMSSW_6_0_0/src
cmsenv
# OR on DICE (scientific linux 6 machines)
. $VO_CMS_SW_DIR/cmsset_default.sh # echo $SCRAM_ARCH should now return slc6_amd64_gcc472
cmsrel CMSSW_6_2_12
cd CMSSW_6_2_12/src
cmsenv

# get the code from the repository
git clone https://github.com/BristolTopGroup/AnalysisSoftware.git BristolAnalysis/Tools

# compile
scram b -j2

hash -r #or rehash in case that BAT cannot be found

# test run the code:
nohup BAT BristolAnalysis/Tools/python/test_cfg.py &> test.log &
```

## Notes
More information can be found at [Bristol Analysis Tools twiki page](https://twiki.cern.ch/twiki/bin/view/CMS/BristolAnalysisTools)

## Bugs
Please report any problems on our [issues page](https://github.com/BristolTopGroup/AnalysisSoftware/issues).
