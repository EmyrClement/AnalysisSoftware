/*
 * WAnalyser.h
 *
 *  Created on: 12 Jan 2015
 *      Author: ejclemen
 */

#ifndef WANALYSER_H_
#define WANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class MTtbarAnalyser: public BAT::BasicAnalyser {
public:
	MTtbarAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "WAnalysis");
	virtual ~MTtbarAnalyser();
	void analyse(const EventPtr);

	void createHistograms();

protected:
	void analyseHadronicW(const EventPtr);
	void fillHistograms(std::string subcollection, std::string suffix = "");

	void createHistogramsFor(std::string collection);
};

} /* namespace BAT */
#endif /* WANALYSER_H_ */
