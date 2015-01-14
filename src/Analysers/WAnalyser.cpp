/*
 * WAnalyser.cpp
 *
 *  Created on: 12 Jan 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/WAnalyser.h"

#include <boost/scoped_ptr.hpp>

namespace BAT {

void WAnalyser::analyse(const EventPtr event) {
	// weight_ = event->weight();

}

void WAnalyser::analyseHadronicW(const EventPtr event, const JetCollection jets, const JetCollection bjets) {

	// Get cleaned jets that aren't b tagged
	JetCollection jetsWithoutBs;
	for ( unsigned int jetIndex=0; jetIndex < jets.size(); ++jetIndex ) {
		bool isBJet = false;
		JetPointer thisJet = jets[jetIndex];
		for ( unsigned int bJetIndex=0; bJetIndex < bjets.size(); ++bJetIndex ) {
			JetPointer thisBJet = bjets[bJetIndex];
			if ( thisJet == thisBJet ) {
				isBJet = true;
				break;
			}
		}
		if ( !isBJet ) jetsWithoutBs.push_back( thisJet );
	}

	// Get each jet pair combination and form a W candidate
	for ( unsigned int jet1Index=0; jet1Index < jets.size()-1; ++jet1Index ) {
		for ( unsigned int jet2Index=jet1Index+1; jet2Index < jets.size(); ++jet2Index ) {
			JetPointer jet1 = jets[jet1Index];
			JetPointer jet2 = jets[jet2Index];

			if (jet1->pt()<=30 || jet2->pt()<=30 ) continue;

			Particle hadronicW(*jet1 + *jet2);

			histMan_->setCurrentHistogramFolder(histogramFolder_);
			histMan_->H1D("hadronicWMass")->Fill(hadronicW.mass() , weight_);
			histMan_->H1D("jetPt")->Fill(jet1->pt() , weight_);
			histMan_->H1D("jetPt")->Fill(jet2->pt() , weight_);
		}
	}
}

void WAnalyser::fillHistograms(std::string subcollection, std::string suffix) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	// unsigned short numberOfSolutions = allSolutions.size();
	// for (unsigned short index = 0; index < numberOfSolutions; ++index) {
	// 	TtbarHypothesisPointer currentSolution = allSolutions.at(index);
	// 	const ParticlePointer resonance = currentSolution->resonance;

	// 	double hadronicWMass = currentSolution->hadronicW->mass();

	// 	if (index == 0) {
	// 		histMan_->H1D_BJetBinned("hadronicWMass" + suffix)->Fill(hadronicWMass, weight_ * scale_);
	// 	}

	// 	double normalisedWeight = weight_ * scale_ / numberOfSolutions;
	// 	histMan_->H1D_BJetBinned("hadronicWMass_allSolutions" + suffix)->Fill(hadronicWMass, normalisedWeight);
	// }
}

void WAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	histMan_->addH1D("hadronicWMass", "hadronic W mass; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt", "jet pt; p_{t} [GeV]; events/1 GeV", 500, 0, 500);

}

WAnalyser::WAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {
}

WAnalyser::~WAnalyser() {
}

} /* namespace BAT */
