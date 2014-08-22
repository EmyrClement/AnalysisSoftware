set -e
export ORIGDIR=`pwd`
echo "Original directory"
pwd

cd ../
pwd

. $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc472
cmsrel CMSSW_6_2_12
cd CMSSW_6_2_12/src/

cmsenv

echo "CMSSW dir"
ls

git clone https://github.com/BristolTopGroup/AnalysisSoftware.git BristolAnalysis/Tools

rm -r BristolAnalysis/Tools/test

cp $ORIGDIR/jobConfig.py .

scram b -j 4

echo "After build"
ls
echo "Run test job"
python jobConfig.py -j $1

echo "After running"
ls -l

cat script.sh

tar -zcvf $ORIGDIR/output.tar.gz *.root

ls
ls $ORIGDIR

