#ifndef _ots_FirmwareSequence_h
#define _ots_FirmwareSequence_h

#include <vector>

namespace ots
{
template<typename T>
class FirmwareSequence
{
  public:
	typedef std::vector<std::pair<T, std::vector<T> > > Sequence;  // It is a vector of
	                                                               // pairs of addresses
	                                                               // and data to send to
	                                                               // the Firmware
	typedef typename Sequence::iterator       iterator;
	typedef typename Sequence::const_iterator const_iterator;

	FirmwareSequence(void) { ; }
	virtual ~FirmwareSequence(void) { ; }

	// Getters
	const Sequence& getSequence(void) const { return theSequence_; }

	inline void pushBack(std::pair<T, std::vector<T> > entry)
	{
		theSequence_.push_back(entry);
	}
	inline void pushBack(T address, const std::vector<T>& data)
	{
		theSequence_.push_back(std::pair<T, std::vector<T> >(address, data));
	}
	inline void pushBack(T address, const T& data)
	{
		theSequence_.push_back(
		    std::pair<T, std::vector<T> >(address, std::vector<T>(1, data)));
	}
	void clear(void) { theSequence_.clear(); }

	iterator begin(void) { return theSequence_.begin(); }
	iterator end(void) { return theSequence_.end(); }

	const const_iterator begin(void) const { return theSequence_.begin(); }
	const const_iterator end(void) const { return theSequence_.end(); }

  private:
	Sequence theSequence_;
};
}

#endif
