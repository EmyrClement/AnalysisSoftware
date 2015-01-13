/*
 * MTtbarAnalyser.cpp
 *
 *  Created on: 5 Sep 2011
 *      Author: kreczko
 */
//#include "../../interface/TopPairEventCandidate.h"
#include "../../interface/Analysers/MTtbarAnalyser.h"
#include "../../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include "../../interface/ReconstructionModules/ChiSquaredThreeJetsTopPairReconstruction.h"
#include "../../interface/ReconstructionModules/TopMassDifferenceTTbarReco.h"
#include "../../interface/BTagWeight.h"

#include <boost/scoped_ptr.hpp>

namespace BAT {

void MTtbarAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();

	analyseHadronicW(event);
}

void MTtbarAnalyser::analyseHadronicW(const EventPtr event) {

	// Get cleaned jets that aren't b tagged

	// for each jet i
	// for each jet j except i
	// Make new particle, W
	// Fill histograms

	if (topElectronPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedElectron = topElectronPlusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(selectedElectron));
		double efficiencyCorrection = event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(true, Globals::ElectronScaleFactorSystematic, event->runnumber());
		METPointer met = event->MET();
		JetCollection jets = topElectronPlusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topElectronPlusJetsRefSelection_->cleanedBJets(event);
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1.);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/Ref selection/FourJetChi2");
		}
	}

	if (topMuonPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedMuon = topMuonPlusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedMuon));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber());
		METPointer met = event->MET();
		JetCollection jets = topMuonPlusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topMuonPlusJetsRefSelection_->cleanedBJets(event);
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1.);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedMuon, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("MuonPlusJets/Ref selection/FourJetChi2");
		}
	}
}

void MTtbarAnalyser::fillHistograms(std::string subcollection, std::string suffix) {
	histMan_->setCurrentHistogramFolder("WAnalysis/" + subcollection);

	unsigned short numberOfSolutions = allSolutions.size();
	for (unsigned short index = 0; index < numberOfSolutions; ++index) {
		TtbarHypothesisPointer currentSolution = allSolutions.at(index);
		const ParticlePointer resonance = currentSolution->resonance;

		double hadronicWMass = currentSolution->hadronicW->mass();

		if (index == 0) {
			histMan_->H1D_BJetBinned("hadronicWMass" + suffix)->Fill(hadronicWMass, weight_ * scale_);
		}

		double normalisedWeight = weight_ * scale_ / numberOfSolutions;
		histMan_->H1D_BJetBinned("hadronicWMass_allSolutions" + suffix)->Fill(hadronicWMass, normalisedWeight);
	}
}

void MTtbarAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("WAnalysis");

	createHistogramsFor("MuonPlusJets/Ref selection");
	createHistogramsFor("ElectronPlusJets/Ref selection");
}

void MTtbarAnalyser::createHistogramsFor(std::string collection) {
	boost::array<std::string, 4> histTypes = { { "", "_allSolutions", } };

	histMan_->setCurrentHistogramFolder("WAnalysis/" + collection);

	for (unsigned short index = 0; index < histTypes.size(); ++index) {
		std::string suffix = histTypes.at(index);
		histMan_->addH1D_BJetBinned("hadronicWMass" + suffix, "hadronic W mass; m(W_{had}) [GeV]; events/1 GeV", 500, 0,
				500);
	}

}

WAnalyser::WAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {
}

WAnalyser::~WAnalyser() {
}

} /* namespace BAT */
