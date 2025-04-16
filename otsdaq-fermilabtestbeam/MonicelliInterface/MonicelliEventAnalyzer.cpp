#include "otsdaq-fermilabtestbeam/MonicelliInterface/MonicelliEventAnalyzer.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq-fermilabtestbeam/MonicelliInterface/MonicelliFileReader.h"
#include "otsdaq-fermilabtestbeam/MonicelliInterface/Visual3DEvent.h"

#include "otsdaq-fermilabtestbeam/MonicelliInterface/Detector.h"
#include "otsdaq-fermilabtestbeam/MonicelliInterface/Event.h"
#include "otsdaq-fermilabtestbeam/MonicelliInterface/Geometry.h"

using namespace ots;

//========================================================================================================================
MonicelliEventAnalyzer::MonicelliEventAnalyzer(void) {}

//========================================================================================================================
MonicelliEventAnalyzer::~MonicelliEventAnalyzer(void) {}

//========================================================================================================================
void MonicelliEventAnalyzer::load(std::string fileName)
{
	theVisualEvents_.clear();
	if(!theReader_.openEventsFile(fileName))
		return;
	if(!theReader_.openGeoFile(fileName.replace(fileName.find('.') + 1, 4, "geo")))
		return;

	theMonicelliEvent_    = theReader_.getEventPointer();
	theMonicelliHeader_   = theReader_.getEventHeaderPointer();
	theMonicelliGeometry_ = theReader_.getGeometryPointer();

	unsigned int numberOfEvents = theReader_.getNumberOfEvents();
	__COUT__ << "Number of events: " << numberOfEvents << __E__;
	theVisualEvents_.resize(numberOfEvents);
	for(unsigned int event = 0; event < numberOfEvents && event < 1000; event++)
	// for(unsigned int event=0; event<10; event++)
	{
		theReader_.readEvent(event);
		this->analyzeEvent(event);
	}
	theReader_.closeGeoFile();
	theReader_.closeEventsFile();
	__COUT__ << "Done!" << __E__;
}

//========================================================================================================================
const Visual3DEvents& MonicelliEventAnalyzer::getEvents(void) { return theVisualEvents_; }

//========================================================================================================================
void MonicelliEventAnalyzer::analyzeEvent(unsigned int event)
{
	// std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Event: " << event <<
	// __E__;

	monicelli::Event::clustersMapDef&  clusters = theMonicelliEvent_->getClusters();
	monicelli::Event::fittedTracksDef& fittedTracks =
	    theMonicelliEvent_->getFittedTracks();
	// monicelli::Event::chi2VectorDef             & chi2                     =
	// theMonicelliEvent_->getFittedTracksChi2()     ;
	// monicelli::Event::fittedTracksCovarianceDef & fittedTrackCovariance    =
	// theMonicelliEvent_->getFittedTracksCovariance();
	// monicelli::Event::trackCandidatesDef        & trackPoints              =
	// theMonicelliEvent_->getTrackCandidates ()     ;

	if(fittedTracks.size() == 0)
		return;

	for(unsigned int tr = 0; tr < fittedTracks.size(); tr++)
	{
		ROOT::Math::SVector<double, 4> tParameters = fittedTracks[tr];
		VisualTrack                    tmpVisualTrack;
		tmpVisualTrack.slopeX     = tParameters[0];
		tmpVisualTrack.interceptX = tParameters[1] * 10;
		tmpVisualTrack.slopeY     = tParameters[2];
		tmpVisualTrack.interceptY = tParameters[3] * 10;
		// chi2[tr] ;
		theVisualEvents_[event].addTrack(tmpVisualTrack);
	}

	for(monicelli::Event::clustersMapDef::iterator itClusters = clusters.begin();
	    itClusters != clusters.end();
	    itClusters++)
		for(monicelli::Event::aClusterMapDef::iterator itClusterMap =
		        itClusters->second.begin();
		    itClusterMap != itClusters->second.end();
		    itClusterMap++)
		{
			VisualHit tmpVisualHit;
			tmpVisualHit.x      = itClusterMap->second["x"];
			tmpVisualHit.y      = itClusterMap->second["y"];
			tmpVisualHit.z      = 0;
			tmpVisualHit.charge = itClusterMap->second["charge"];
			// std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Local  X: " <<
			// tmpVisualHit.x << " Y: " << tmpVisualHit.y << " Z: " << tmpVisualHit.z <<
			// __E__;
			monicelli::Detector* detector =
			    theMonicelliGeometry_->getDetector(itClusters->first);
			detector->fromLocalToGlobal(
			    &(tmpVisualHit.x), &(tmpVisualHit.y), &(tmpVisualHit.z));
			theVisualEvents_[event].addHit(
			    tmpVisualHit * 10);  // To become um
			                         // std::cout << __COUT_HDR_FL__ <<
			                         // __PRETTY_FUNCTION__ << "Global X: " <<
			                         // tmpVisualHit.x << " Y: " << tmpVisualHit.y << " Z:
			                         // " << tmpVisualHit.z << __E__;
		}
}
