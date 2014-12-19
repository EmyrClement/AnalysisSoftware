/*
 * SelectionOutputReader.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Emyr Clement
 */

#include "../../interface/Readers/SelectionOutputReader.h"
#include "../../interface/GlobalVariables.h"
#include <iostream>

using namespace std;

namespace BAT {

SelectionOutputReader::SelectionOutputReader() :
	numberJetsReader(),
	numberBJetsReader(),
	signalLeptonIndexReader(),
	cleanedJetsIndexReader(),
	selectionOutputInfo() {

}

SelectionOutputReader::SelectionOutputReader(TChainPointer input, bool isElectronChannel ) :
	numberJetsReader(),
	numberBJetsReader(),
	signalLeptonIndexReader(),
	cleanedJetsIndexReader(),
	selectionOutputInfo() {

	if ( isElectronChannel ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.signalElectronIndex");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedJetIndex");
	}
	else {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.signalMuonIndex");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsSelection.cleanedJetIndex");		
	}
}

void SelectionOutputReader::initialise() {
	numberJetsReader.initialise();
	numberBJetsReader.initialise();
	signalLeptonIndexReader.initialise();
	cleanedJetsIndexReader.initialise();
}

const SelectionOutputInfo& SelectionOutputReader::getSelectionOutputInfo() {
	readSelectionOutputInfo();
	return selectionOutputInfo;
}

void SelectionOutputReader::readSelectionOutputInfo() {
	selectionOutputInfo.setNumberOfJets( numberJetsReader.getVariable() );
	selectionOutputInfo.setNumberOfBJets( numberBJetsReader.getVariable() );

	std::vector<unsigned int> cleanedJetIndices;
	for (unsigned int index = 0; index < cleanedJetsIndexReader.size(); index++) {
		cleanedJetIndices.push_back( cleanedJetsIndexReader.getUIntVariableAt(index) );
	}
	selectionOutputInfo.setCleanedJetIndex( cleanedJetIndices );

	selectionOutputInfo.setSignalLeptonIndex( signalLeptonIndexReader.getVariable() );
}
SelectionOutputReader::~SelectionOutputReader() {
}

}
