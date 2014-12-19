/*
 * SelectionOutputReader.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef SELECTIONOUTPUTREADER_H_
#define SELECTIONOUTPUTREADER_H_
#include "VariableReader.h"
#include "../RecoObjects/SelectionOutputInfo.h"

namespace BAT {

class SelectionOutputReader {
public:
	SelectionOutputReader();
	SelectionOutputReader(TChainPointer input, bool isElectronChannel);
	virtual ~SelectionOutputReader();
	const SelectionOutputInfo& getSelectionOutputInfo();
	void initialise();

private:
	VariableReader<unsigned int> numberJetsReader;
	VariableReader<unsigned int> numberBJetsReader;
	VariableReader<unsigned int> signalLeptonIndexReader;
	VariableReader<MultiUIntPointer> cleanedJetsIndexReader;

	void readSelectionOutputInfo();

	SelectionOutputInfo selectionOutputInfo;

};

}

#endif /* SELECTIONOUTPUTREADER_H_ */
