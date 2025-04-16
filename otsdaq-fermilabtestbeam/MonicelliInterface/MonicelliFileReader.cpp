#include "otsdaq-fermilabtestbeam/MonicelliInterface/MonicelliFileReader.h"
#include "otsdaq/Macros/CoutMacros.h"

#include "otsdaq-fermilabtestbeam/MonicelliInterface/Event.h"
#include "otsdaq-fermilabtestbeam/MonicelliInterface/EventHeader.h"
#include "otsdaq-fermilabtestbeam/MonicelliInterface/Geometry.h"

#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>

using namespace ots;

//========================================================================================================================
MonicelliFileReader::MonicelliFileReader(void)
    : argv(1)
    , argc(new char*[1])
    , theApp_("MonicelliFileReaderApplication", &argv, argc)
    , theGeoFile_(0)
    , theEventsFile_(0)
    , inputGeometryTree_(0)
    , inputEventTree_(0)
    , inputEventHeader_(0)
    , theEvent_(new monicelli::Event())
    , theEventHeader_(new monicelli::EventHeader())
    , theGeometry_(new monicelli::Geometry())
    , theEventBranch_(0)
    , theEventTree_(0)
{
}

//========================================================================================================================
MonicelliFileReader::~MonicelliFileReader(void)
{
	delete theEvent_;
	delete theGeometry_;
	delete theEventHeader_;
}

//========================================================================================================================
bool MonicelliFileReader::openGeoFile(std::string fileName)
{
	inputGeometryTree_ = 0;
	closeGeoFile();
	__COUT__ << "Opening file " << fileName
	          << __E__;
	TFile* theGeoFile_ = TFile::Open(fileName.c_str(), "read");
	if(!theGeoFile_->IsOpen())
	{
		__COUT__ << "Can't open file "
		          << fileName << __E__;
		return false;
	}

	const std::string geometryTreeName = "Geometry";
	if((TTree*)theGeoFile_->Get(geometryTreeName.c_str()))
	{
		inputGeometryTree_ = (TTree*)theGeoFile_->Get(geometryTreeName.c_str());
		inputGeometryTree_->SetBranchAddress("GeometryBranch", &theGeometry_);
		inputGeometryTree_->GetEntry(0);
	}
	return (inputGeometryTree_ != 0);
}

//========================================================================================================================
bool MonicelliFileReader::openEventsFile(std::string fileName)
{
	inputEventTree_   = 0;
	inputEventHeader_ = 0;

	closeEventsFile();
	__COUT__ << "Opening file " << fileName
	          << __E__;
	TFile* theEventsFile_ = TFile::Open(fileName.c_str(), "read");
	if(!theEventsFile_->IsOpen())
	{
		__COUT__ << "Can't open file "
		          << fileName << __E__;
		return false;
	}

	std::string eventsTreeName   = fileName + "Events";
	std::string eventsHeaderName = fileName + "Header";
	if((inputEventTree_ = (TTree*)theEventsFile_->Get(eventsTreeName.c_str())))
	{
		inputEventTree_->SetBranchAddress("EventBranch", &theEvent_);
		inputEventTree_->GetEntry(0);

		if((inputEventHeader_ = (TTree*)theEventsFile_->Get(eventsHeaderName.c_str())))
		{
			inputEventHeader_->SetBranchAddress("EventHeader", &theEventHeader_);
			inputEventHeader_->GetEntry(0);
		}
	}

	if(!inputEventTree_)
	{
		__COUT__ << "Can't find any tree in "
		          << fileName << __E__;
		return false;
		;
	}
	return true;
}

//========================================================================================================================
void MonicelliFileReader::closeGeoFile(void)
{
	if(theGeoFile_)
	{
		theGeoFile_->Close();
		theGeoFile_ = 0;
	}
}

//========================================================================================================================
void MonicelliFileReader::closeEventsFile(void)
{
	if(theEventsFile_)
	{
		theEventsFile_->Close();
		theEventsFile_ = 0;
	}
}

//========================================================================================================================
unsigned int MonicelliFileReader::getNumberOfEvents(void)
{
	if(inputEventTree_)
		return inputEventTree_->GetEntries();
	return 0;
}

//========================================================================================================================
void MonicelliFileReader::readEvent(unsigned int event)
{
	if(inputEventTree_)
		inputEventTree_->GetEntry(event);
}

//========================================================================================================================
monicelli::Event* MonicelliFileReader::getEventPointer(void) { return theEvent_; }

//========================================================================================================================
monicelli::EventHeader* MonicelliFileReader::getEventHeaderPointer(void)
{
	return theEventHeader_;
}

//========================================================================================================================
monicelli::Geometry* MonicelliFileReader::getGeometryPointer(void)
{
	return theGeometry_;
}
