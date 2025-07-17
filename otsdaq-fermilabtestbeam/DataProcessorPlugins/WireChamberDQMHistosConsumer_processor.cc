#include "otsdaq/Macros/ProcessorPluginMacros.h"
#include "otsdaq-fermilabtestbeam/DataProcessorPlugins/WireChamberDataSaverConsumer.h"

using namespace ots;

//========================================================================================================================
WireChamberDataSaverConsumer::WireChamberDataSaverConsumer(
    std::string              supervisorApplicationUID,
    std::string              bufferUID,
    std::string              processorUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       configurationPath)
    : WorkLoop(processorUID)
    , RawDataSaverConsumerBase(supervisorApplicationUID,
                               bufferUID,
                               processorUID,
                               theXDAQContextConfigTree,
                               configurationPath)
{
	NUMBER_OF_TDCs = 16;  // FIXME occurs here and TTBFWireChamber_interface.cc
}

//========================================================================================================================
WireChamberDataSaverConsumer::~WireChamberDataSaverConsumer(void) {}
//========================================================================================================================
void WireChamberDataSaverConsumer::convertSpillData(const std::string& spillData)
{
	{
		char msg[100];

		std::stringstream ss;
		ss << "physics Response: ";
		for(unsigned int i = 0; i < spillData.size(); ++i)
		{
			sprintf(msg, "0x%2.2x", ((unsigned int)spillData[i]) & 0x0FF);
			ss << msg << " ";
		}
		//__COUT__ << "\n" << ss.str() << std::endl;
	}

	// Unpack Controller Header
	unsigned int words, spillLinkStatus, spillTDCStatus, spillTriggerCount, minute,
	    second, day, hour, year, month, spillCount, totalWordCount, index;
	unsigned int       tmpInt;
	const unsigned int wordSz = 2;
	unsigned int       it;
	bool               doneFlag = false;

	for(it = 0; it < spillData.size() - (wordSz - 1) && !doneFlag; it += wordSz)
	{
		tmpInt = 0;
		for(unsigned int j = 0; j < wordSz; ++j)
			tmpInt |= (((unsigned int)spillData[it + j]) & 0x0FF)
			          << (8 * (wordSz - 1 - j));

		switch(it / wordSz)  // word index
		{
		case 0:  // word count
			totalWordCount = tmpInt << 16;
			break;
		case 1:  // word count
			totalWordCount |= tmpInt;
			break;
		case 2:  // spill count
			spillCount = tmpInt;
			break;
		case 3:  // year and month
			year  = tmpInt >> 8;
			month = tmpInt & 0x00FF;
			break;
		case 4:  // day and hour
			day  = tmpInt >> 8;
			hour = tmpInt & 0x00FF;
			break;
		case 5:  // minute and second
			minute = tmpInt >> 8;
			second = tmpInt & 0x00FF;
			break;
		case 6:  // spillTriggerCount 32 bit number
			spillTriggerCount = tmpInt << 16;
			break;
		case 7:  // spillTriggerCount
			spillTriggerCount |= tmpInt;
			break;
		case 8:  // spillTDCStatus
			spillTDCStatus = tmpInt;
			break;
		case 9:  // spillLinkStatus
			spillLinkStatus = tmpInt;
			doneFlag        = true;  // done with header
			words           = it / wordSz + 1;
			break;
		default:;
		}
	}
	__COUT__ << "////////////////////////HEADER////////////////////// " << std::endl;
	__COUT__ << "wordCount " << totalWordCount << std::endl;

	outFile_ << "SPILL\t" << spillCount << std::endl;
	// outFile_ << SDATE
	__COUT__ << "Date: " << month << "/" << day << "/" << year << std::endl;
	__COUT__ << "Time: " << hour << ":" << minute << ":" << second << std::endl;
	__COUT__ << "spillTriggerCount " << spillTriggerCount << std::endl;
	__COUT__ << "spillTDCStatus " << spillTDCStatus << std::endl;
	__COUT__ << "spillLinkStatus " << spillLinkStatus << std::endl;
	__COUT__ << "words  " << words << " = " << it / 2 << std::endl;
	__COUT__ << "//////////////////////////////////////////////////// " << std::endl;

	// Unpack TDC Spill Header
	// unsigned int tdcSpillWordCount, tdcNumber, tdcSpillTriggerCount, tdcSpillStatus,
	// tdcWords = 1;
	unsigned int tdcHeaderIndex = 0;
	// unsigned int numberOfTDCs = 16;
	// while(it < /*spillData.size()-(wordSz-1)*/)//FIXME Tries to loop through all the
	// data, need to stop after header is finished; Need to detect TDC Header Size
	while(tdcHeaderIndex < NUMBER_OF_TDCs)
	{
		unsigned int startIt = it;
		doneFlag             = false;
		struct TDCHeaderStruct tdcHeader;
		for(; it < spillData.size() - (wordSz - 1) && !doneFlag; it += wordSz)
		{
			tmpInt = 0;
			for(unsigned int j = 0; j < wordSz; ++j)
				tmpInt |= (((unsigned int)spillData[it + j]) & 0x0FF)
				          << (8 * (wordSz - 1 - j));

			switch((it - startIt) / wordSz)  // word index
			{
			case 0:  // tdcSpillWordCount 32 bit number
				tdcHeader.tdcSpillWordCount = tmpInt << 16;
				break;
			case 1:  // tdcSpillWordCount
				tdcHeader.tdcSpillWordCount |= tmpInt;
				break;
			case 2:  // tdcNumber
				tdcHeader.tdcNumber = tmpInt;
				break;
			case 3:
				tdcHeader.tdcSpillTriggerCount = tmpInt << 16;
				break;
			case 4:
				tdcHeader.tdcSpillTriggerCount |= tmpInt;
				break;
			case 5:
				tdcHeader.tdcSpillStatus = tmpInt;
				doneFlag                 = true;
				tdcHeader.tdcWords       = (it - startIt) / wordSz + 1;
				break;
			default:
				break;
			}
		}
		tdcHeader.tdcHeaderIndex = tdcHeaderIndex;

		__COUT__ << "/////////////////// TDC HEADER////////////////////// " << std::endl;
		//		__COUT__ << "tdcHeaderIndex " 		<< tdcHeader.tdcHeaderIndex 		<<
		// std::endl;
		__COUT__ << "tdcSpillWordCount " << tdcHeader.tdcSpillWordCount << std::endl;
		__COUT__ << "tdcNumber " << tdcHeader.tdcNumber << std::endl;
		__COUT__ << "tdcSpillTriggerCount " << tdcHeader.tdcSpillTriggerCount
		         << std::endl;
		__COUT__ << "tdcSpillStatus " << tdcHeader.tdcSpillStatus << std::endl;
		__COUT__ << "tdcWords " << tdcHeader.tdcWords << std::endl;
		__COUT__ << "//////////////////////////////////////////////////// " << std::endl;

		++tdcHeaderIndex;
	}

	__COUT__ << "/////////////////// END OF DATA////////////////////// " << std::endl;
	__COUT__ << "//////////////////////////////////////////////////// " << std::endl;
	__COUT__ << "///////// unpacking tdc events ///////////////////// " << std::endl;
	__COUT__ << "spillTriggerCount: " << spillTriggerCount << std::endl;
	__COUT__ << "Current data index: " << it << std::endl;
	__COUT__ << "Spill data size: " << spillData.size() << std::endl;
	for(unsigned int triggerIndex = 0; triggerIndex < spillTriggerCount; ++triggerIndex)
	{
		__COUT__ << "Trigger Index: " << triggerIndex << std::endl;

		for(unsigned int numTDCsIndex = 0; numTDCsIndex < NUMBER_OF_TDCs; ++numTDCsIndex)
		{
			__COUT__ << "numTDCsIndex: " << numTDCsIndex << std::endl;

			unsigned int startIt = it;
			doneFlag             = false;
			struct TDCEvent tdcEvent;
			for(; it < spillData.size() - (wordSz - 1) && !doneFlag; it += wordSz)
			{
				tmpInt = 0;
				for(unsigned int j = 0; j < wordSz; ++j)
					tmpInt |= (((unsigned int)spillData[it + j]) & 0x0FF)
					          << (8 * (wordSz - 1 - j));

				switch((it - startIt) / wordSz)  // word index
				{
				case 0:  // tdcEvent Word Count
					tdcEvent.wordCount = tmpInt;
					break;
				case 1:  // tdcEvent tdcNumber
					tdcEvent.tdcNumber = tmpInt;
					break;
				case 2:  // tdcEvent Event Status
					tdcEvent.eventStatus = tmpInt;
					break;
				case 3:  // tdcEvent Trigger Number (32 bit number)
					tdcEvent.triggerNumber = tmpInt << 16;
					break;
				case 4:
					tdcEvent.triggerNumber |= tmpInt;
					break;
				case 5:  // tdcEvent Trigger Type
					tdcEvent.triggerType = tmpInt;
					break;
				case 6:  // tdcEvent Controller Timestamp
					tdcEvent.controllerEventTimeStamp = tmpInt;
					break;
				case 7:  // tdcEvent Event TimeStamp (32 bit number)
					tdcEvent.tdcEventTimeStamp = tmpInt << 16;
					break;
				case 8:
					tdcEvent.tdcEventTimeStamp |= tmpInt;
					doneFlag           = true;
					tdcEvent.dataWords = (it - startIt) / wordSz + 1;
					tdcEvent.words     = 0;
					break;
				default:
					break;
				}
			}

			//			__COUT__ << "/////////////////// TDC Event////////////////////// "
			//<< 	std::endl;
			//			__COUT__ << "wordCount " 				<< tdcEvent.wordCount
			//<<  std::endl;
			__COUT__ << "tdcNumber " << tdcEvent.tdcNumber << std::endl;
			//			__COUT__ << "eventStatus " 				<< tdcEvent.eventStatus
			//<<  std::endl;
			__COUT__ << "triggerNumber " << tdcEvent.triggerNumber << std::endl;
			//			__COUT__ << "triggerType " 				<< tdcEvent.triggerType
			//<<  std::endl;
			//			__COUT__ << "controllerEventTimeStamp " <<
			// tdcEvent.controllerEventTimeStamp	<< std::endl;
			//			__COUT__ << "tdcEventTimeStamp " 		<<
			// tdcEvent.tdcEventTimeStamp
			//<< std::endl;
			//			__COUT__ << "////////////////////////////////////////////////////
			//"
			//<< std::endl;

			__COUT__ << "////////////////////tdcData/////////////////" << std::endl;
			__COUT__ << "dataWords " << tdcEvent.dataWords << std::endl;
			__COUT__ << "wordCount " << tdcEvent.wordCount << std::endl;

			for(; tdcEvent.dataWords < tdcEvent.wordCount &&
			      it < spillData.size() - (wordSz - 1);
			    it += wordSz)
			{
				tmpInt = 0;
				for(unsigned int j = 0; j < wordSz; ++j)
					tmpInt |= (((unsigned int)spillData[it + j]) & 0x0FF)
					          << (8 * (wordSz - 1 - j));

				tdcEvent.tdcData.push_back(tmpInt);
				++tdcEvent.dataWords;
				std::cout << tmpInt << ", ";
			}
			__COUT__ << "////////////////End of tdcData/////////////" << std::endl;
			__COUT__ << "dataWords " << tdcEvent.dataWords << std::endl;
			__COUT__ << "///////////////////////////////////////////" << std::endl;

			vectorOfTDCEvents_.push_back(tdcEvent);
		}
	}
}
//========================================================================================================================
// void WireChamberDataSaverConsumer::writeDataPaw(std::string label, int firstValue, int
// secondValue, int thirdValue, std::string txt);
//{
//	return;
//}
//========================================================================================================================
// void WireChamberDataSaverConsumer::writeHeader(void)
//{
//	unsigned int version = 0x00010001;
//	outFile_.write("stib",4);
//	outFile_.write( (char*)&version,4);
//}

void WireChamberDataSaverConsumer::openFile(std::string runNumber)
{
	currentRunNumber_ = runNumber;
	//	std::string fileName =   "Run" + runNumber + "_" + processorUID_ + "_Raw.dat";
	std::stringstream fileName;
	fileName << filePath_ << "/" << fileRadix_ << "_Run" << runNumber;
	// if split file is there then subrunnumber must be set!
	if(maxFileSize_ > 0)
		fileName << "_" << currentSubRunNumber_;
	fileName << ".dat";
	std::cout << __COUT_HDR_FL__ << "Saving file: " << fileName.str() << std::endl;
	outFile_.open(fileName.str().c_str(), std::ios::out);
	if(!outFile_.is_open())
	{
		__SS__ << "Can't open file " << fileName.str() << std::endl;
		__COUT_ERR__ << "\n" << ss.str();
		throw std::runtime_error(ss.str());
	}
}

//========================================================================================================================
void WireChamberDataSaverConsumer::save(const std::string& data)
{
	convertSpillData(data);
}

DEFINE_OTS_PROCESSOR(WireChamberDataSaverConsumer)
