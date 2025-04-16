#ifndef _ots_ROCDACs_h_
#define _ots_ROCDACs_h_

#include <map>
#include <string>
#include <utility>

namespace ots
{
typedef std::map<std::string, std::pair<unsigned int, unsigned int> > DACList;

class ROCDACs
{
  public:
	ROCDACs(void) { ; }
	virtual ~ROCDACs(void) { ; }
	void clear(void) { theDACList_.clear(); }

	// Setters
	// void setDAC(std::string name, std::pair<unsigned int, unsigned int> aDAC)
	// {theDACList_[name] = std::make_pair(aDAC.first,aDAC.second);}  void
	// setDAC(std::string name, unsigned int address, unsigned int value)
	// {theDACList_[name].first = address; theDACList_[name].second = value;}
	void setDAC(std::string name, unsigned int address, unsigned int value)
	{
		theDACList_[name] = std::pair<unsigned int, unsigned int>(address, value);
	}

	// Getters
	const std::pair<unsigned int, unsigned int>& getDAC(std::string name) const
	{
		return theDACList_.find(name)->second;
	}
	unsigned int getDACAddress(std::string name) const
	{
		return theDACList_.find(name)->second.first;
	}
	unsigned int getDACValue(std::string name) const
	{
		return theDACList_.find(name)->second.second;
	}
	const DACList getDACList(void) const { return theDACList_; }

  protected:
	DACList theDACList_;
};
}
#endif
