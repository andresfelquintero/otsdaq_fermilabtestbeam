#include "otsdaq-fermilabtestbeam/DetectorConfiguration/FSSRDACsConfiguration.h"
#include "otsdaq/ConfigurationInterface/ConfigurationTree.h"

#include <iostream>

using namespace ots;

//==============================================================================
FSSRDACsConfiguration::FSSRDACsConfiguration(void)
{
	//////////////////////////////////////////////////////////////////////
	// WARNING: the names and the order MUST match the ones in the enum  //
	//////////////////////////////////////////////////////////////////////
	// FSSRDACsConfigurationInfo.xml
	//<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
	//<ROOT xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	// xsi:noNamespaceSchemaLocation="TableInfo.xsd">
	//  <CONFIGURATION Name="FSSRDACsConfiguration">
	//    <VIEW Name="FSSR_DACS_CONFIGURATION" Type="File,Database,DatabaseTest">
	//      <COLUMN Name="ROCName"       StorageName="ROC_NAME"       DataType="VARCHAR2"
	//      /> <COLUMN Name="PulserData"    StorageName="PULSER_DATA"    DataType="NUMBER"
	//      /> <COLUMN Name="PulserControl" StorageName="PULSER_CONTROL" DataType="NUMBER"
	//      /> <COLUMN Name="IntegratorVbn" StorageName="INTEGRATOR_VBN" DataType="NUMBER"
	//      /> <COLUMN Name="ShaperVbp2"    StorageName="SHAPER_VBP2"    DataType="NUMBER"
	//      /> <COLUMN Name="ShaperVbp1"    StorageName="SHAPER_VBP1"    DataType="NUMBER"
	//      /> <COLUMN Name="BLRestorer"    StorageName="BL_RESTORER"    DataType="NUMBER"
	//      />
	//      <COLUMN Name="VTn"           StorageName="VTN"	     	  DataType="NUMBER" />
	//      <COLUMN Name="VTp0"          StorageName="VTP0" DataType="NUMBER"   />
	//      <COLUMN Name="VTp1"          StorageName="VTP1" DataType="NUMBER"   />
	//      <COLUMN Name="VTp2"          StorageName="VTP2" DataType="NUMBER"   />
	//      <COLUMN Name="VTp3"          StorageName="VTP3" DataType="NUMBER"   />
	//      <COLUMN Name="VTp4"          StorageName="VTP4" DataType="NUMBER"   />
	//      <COLUMN Name="VTp5"          StorageName="VTP5" DataType="NUMBER"   />
	//      <COLUMN Name="VTp6"          StorageName="VTP6" DataType="NUMBER"   />
	//      <COLUMN Name="VTp7"          StorageName="VTP7" DataType="NUMBER"   />
	//      <COLUMN Name="ActiveLines"   StorageName="ACTIVE_LINES"   DataType="NUMBER" />
	//      <COLUMN Name="SendData"      StorageName="SEND_DATA"      DataType="NUMBER" />
	//      <COLUMN Name="RejectHits"    StorageName="REJECT_HITS"    DataType="NUMBER" />
	//      <COLUMN Name="DigContrReg"   StorageNanameToROCDACsMap_me="DIG_CONTR_REG"
	//      DataType="NUMBER"   /> <COLUMN Name="AqBCO"         StorageName="AQ_BCO"
	//      DataType="NUMBER"   />
	//    </VIEW>
	//  </CONFIGURATION>
	//</ROOT>

	// FIXME We need a FSSRDefinitions class and use it to get the registers addresses
	dacNameToDACAddress_["PulserData"]    = 1;
	dacNameToDACAddress_["PulserControl"] = 2;
	dacNameToDACAddress_["IntegratorVbn"] = 3;
	dacNameToDACAddress_["ShaperVbp2"]    = 4;
	dacNameToDACAddress_["ShaperVbp1"]    = 5;
	dacNameToDACAddress_["BLRestorer"]    = 6;
	dacNameToDACAddress_["VTn"]           = 7;
	dacNameToDACAddress_["VTp0"]          = 8;
	dacNameToDACAddress_["VTp1"]          = 9;
	dacNameToDACAddress_["VTp2"]          = 10;
	dacNameToDACAddress_["VTp3"]          = 11;
	dacNameToDACAddress_["VTp4"]          = 12;
	dacNameToDACAddress_["VTp5"]          = 13;
	dacNameToDACAddress_["VTp6"]          = 14;
	dacNameToDACAddress_["VTp7"]          = 15;
	dacNameToDACAddress_["ActiveLines"]   = 16;
	dacNameToDACAddress_["SendData"]      = 19;
	dacNameToDACAddress_["RejectHits"]    = 20;
	dacNameToDACAddress_["DigContrReg"]   = 27;
	dacNameToDACAddress_["AqBCO"]         = 30;
}

//==============================================================================
FSSRDACsConfiguration::~FSSRDACsConfiguration(void) {}

//==============================================================================
const ROCDACs FSSRDACsConfiguration::getROCDACs(
    const ConfigurationTree& dacsConfiguration)
{
	ROCDACs rocDACs;
	for(auto& it : dacNameToDACAddress_)
	{
		rocDACs.setDAC(it.first,
		               it.second,
		               dacsConfiguration.getNode(it.first).getValue<unsigned int>());
		// __COUT__ << "DAC. Name: " << it.first << " addr: " << it.second << " val: " <<
		// dacsConfiguration.getNode(it.first).getValue<unsigned int>() << " size: " <<
		// rocDACs.getDACList().size()<< std::endl;
	}
	//__COUT__ << "Size: " << rocDACs.getDACList().size()<< std::endl;
	return rocDACs;
}
