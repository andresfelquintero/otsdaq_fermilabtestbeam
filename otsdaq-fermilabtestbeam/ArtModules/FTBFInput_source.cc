#include "art/Framework/IO/Sources/Source.h"
#include "artdaq/ArtModules/detail/SharedMemoryReader.hh"
#include "otsdaq-fermilabtestbeam/Overlays/FragmentType.hh"
#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/IO/Sources/SourceTraits.h"


#include <string>
using std::string;

namespace otsftbf
{
std::map<artdaq::Fragment::type_t, std::string> makeFragmentTypeMap()
{
	auto output = artdaq::Fragment::MakeSystemTypeMap();
	for(auto name : names)
	{
		output[toFragmentType(name)] = name;
	}
	return output;
}
}

namespace art
{
/**
 * \brief  Specialize an art source trait to tell art that we don't care about
 * source.fileNames and don't want the files services to be used.
 */
template<>
struct Source_generator<artdaq::detail::SharedMemoryReader<otsftbf::makeFragmentTypeMap>>
{
	static constexpr bool value =
	    true;  ///< Used to suppress use of file services on art Source
};
}

namespace otsftbf
{
/**
 * \brief DemoInput is an art::Source using the detail::RawEventQueueReader class
 */
typedef art::Source<artdaq::detail::SharedMemoryReader<otsftbf::makeFragmentTypeMap>>
    FTBFInput;
}

DEFINE_ART_INPUT_SOURCE(otsftbf::FTBFInput)