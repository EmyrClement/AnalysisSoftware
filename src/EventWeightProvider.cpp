/*
 * CrossSections.cpp
 *
 *  Created on: 13 Aug 2010
 *      Author: kreczko
 */

#include "../interface/EventWeightProvider.h"
#include "../interface/GlobalVariables.h"

#include "TFile.h"

#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

using namespace std;
namespace BAT {

EventWeightProvider::EventWeightProvider(string datasetInformationFile) :
		datasetInfo_(datasetInformationFile), //
		xsection(datasetInfo_.getCrossSections()), //
		numberOfProcessedEvents(datasetInfo_.getArrayOfProcessedEvents()), //
		estimatedPileUp_Muon(Globals::estimatedPileup_Muon), //
		estimatedPileUp_up_Muon(Globals::estimatedPileup_up_Muon), //
		estimatedPileUp_down_Muon(Globals::estimatedPileup_down_Muon), //
		estimatedPileUp_Electron(Globals::estimatedPileup_Electron), //
		estimatedPileUp_up_Electron(Globals::estimatedPileup_up_Electron), //
		estimatedPileUp_down_Electron(Globals::estimatedPileup_down_Electron), //
		DATAdistribution(), //
		pileUpWeights_Muon(), //
		pileUpWeights_Electron(), //
		numberOfEventsWithTooHighPileUp(0) {

	generate_weights();
}

EventWeightProvider::~EventWeightProvider() {

}

double EventWeightProvider::getWeight(DataType::value type) {
	if (type == DataType::SingleElectron_Run2016B
	    || type == DataType::SingleElectron_Run2016C
	    || type == DataType::SingleElectron_Run2016D
	    || type == DataType::SingleElectron_Run2016E
	    || type == DataType::SingleMuon_Run2016B
	    || type == DataType::SingleMuon_Run2016C
	    || type == DataType::SingleMuon_Run2016D
	    || type == DataType::SingleMuon_Run2016E
	)
		return 1.;
	else
		return xsection[type] * Globals::luminosity / numberOfProcessedEvents[type];
}

double EventWeightProvider::reweightPileUp(unsigned int numberOfVertices, bool isMuonChannel, int systematic ) {

	boost::array<double, NWEIGHTSSIZE>* weights = &pileUpWeights_Muon;

	if ( isMuonChannel ) {
		if ( systematic == -1 ) {
			weights = &pileUpWeights_down_Muon;
		}
		else if ( systematic == 1 ) {
			weights = &pileUpWeights_up_Muon;
		}		
	}
	else {
		weights = &pileUpWeights_Electron;
		if ( systematic == -1 ) {
			weights = &pileUpWeights_down_Electron;
		}
		else if ( systematic == 1 ) {
			weights = &pileUpWeights_up_Electron;
		}	
	}


	if (numberOfVertices >= weights->size()) {
		++numberOfEventsWithTooHighPileUp;
		return 1.;
	}

	return weights->at(numberOfVertices);
}

double EventWeightProvider::reweightTopPt(const EventPtr event) {
	const MCParticleCollection genPart(event->GenParticles());

	double topPt = 0.;
	double tbarPt = 0.;
	for (unsigned int i = 0; i < genPart.size(); i++) {

		if (genPart.at(i)->pdgId() == (6))
		  	  topPt = genPart.at(i)->pt();

		if (genPart.at(i)->pdgId() == (-6))
		  	  tbarPt = genPart.at(i)->pt();
	}

	double SFtop = exp(0.159-0.00141*topPt);
	double SFtbar = exp(0.159-0.00141*tbarPt);

	double weight=sqrt(SFtop*SFtbar);
  return weight;
}

//boost::shared_ptr<TH1D> EventWeightProvider::getPileUpHistogram(std::string pileUpEstimationFile) {
//	using namespace std;
//
//	if (!boost::filesystem::exists(pileUpEstimationFile))
//		throw "Exception in EventWeightProvider::getPileUpHistogram: could not find pile-up file '"
//				+ pileUpEstimationFile + "'";
//
//	cout << "Using pile-up estimation file " << pileUpEstimationFile << endl;
//	boost::scoped_ptr<TFile> file(TFile::Open(pileUpEstimationFile.c_str()));
//	boost::shared_ptr<TH1D> pileUp((TH1D*) file->Get("pileup")->Clone());
//	file->Close();
//
//	return pileUp;
//}

void EventWeightProvider::generate_weights() {

	pileUpWeights_Muon = generateWeights(Spring2016, estimatedPileUp_Muon);
	pileUpWeights_up_Muon = generateWeights(Spring2016, estimatedPileUp_up_Muon);
	pileUpWeights_down_Muon = generateWeights(Spring2016, estimatedPileUp_down_Muon);

	pileUpWeights_Electron = generateWeights(Spring2016, estimatedPileUp_Electron);
	pileUpWeights_up_Electron = generateWeights(Spring2016, estimatedPileUp_up_Electron);
	pileUpWeights_down_Electron = generateWeights(Spring2016, estimatedPileUp_down_Electron);

// 	cout << "Pile up weights" << endl;
// 	for (unsigned int index = 0; index < pileUpWeights.size(); ++index){
// 		cout << index << ": " << pileUpWeights.at(index) << endl;
// 	}
}

boost::array<double, NWEIGHTSSIZE> EventWeightProvider::generateWeights(
		const boost::array<double, NWEIGHTSSIZE> inputMC,
		const boost::shared_ptr<TH1D> dataEstimatedPileUp ) {
	boost::array<double, NWEIGHTSSIZE> weights;

	// Check that integral of dataEstimatedPileUp is 1
	if ( dataEstimatedPileUp->Integral() != 1 && dataEstimatedPileUp->Integral() != 0 ) {
		dataEstimatedPileUp->Scale( 1 / dataEstimatedPileUp->Integral() );
	}

	double s = 0.0;

	for (unsigned int npu = 0; npu < inputMC.size(); ++npu) {

		// +1-1 problem
		DATAdistribution[npu] = dataEstimatedPileUp->GetBinContent(npu+1);

		if (inputMC[npu] > 0) {
			weights[npu] = DATAdistribution[npu] / inputMC[npu];
		}
		else
			weights[npu] = 0;

		s += DATAdistribution[npu];
	}

	/**
	 * normalize weights such that the total sum of weights over the whole sample is 1.0,
	 * i.e., sum_i  result[i] * npu_probs[i] should be 1.0 (!)
	 */
	for (unsigned int npu = 0; npu < weights.size(); ++npu) {
		weights[npu] /= s;
	}

	return weights;
}

unsigned long EventWeightProvider::getNumberOfEventsWithTooHighPileUp() const {
	return numberOfEventsWithTooHighPileUp;
}

} // namespace BAT
