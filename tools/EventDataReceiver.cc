
#include <boost/program_options.hpp>
#include "fhiclcpp/make_ParameterSet.h"
namespace bpo = boost::program_options;

#include "artdaq-core/Data/ContainerFragment.hh"
#include "artdaq/DAQdata/Globals.hh"
#include "artdaq/DAQdata/TCPConnect.hh"
#include "otsdaq-fermilabtestbeam/ArtModules/detail/DataRequestMessage.hh"
#include "artdaq-core/Utilities/configureMessageFacility.hh"

int main(int argc, char* argv[])
{
	artdaq::configureMessageFacility("EventDataReceiver");

	std::ostringstream descstr;
	descstr << argv[0] << " <-c <config-file>>";
	bpo::options_description desc(descstr.str());
	desc.add_options()("config,c", bpo::value<std::string>(), "Configuration file.")(
	    "help,h", "produce help message");
	bpo::variables_map vm;
	try
	{
		bpo::store(bpo::command_line_parser(argc, argv).options(desc).run(), vm);
		bpo::notify(vm);
	}
	catch(bpo::error const& e)
	{
		std::cerr << "Exception from command line processing in " << argv[0] << ": "
		          << e.what() << "\n";
		return -1;
	}
	if(vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 1;
	}
	if(!vm.count("config"))
	{
		std::cerr << "Exception from command line processing in " << argv[0]
		          << ": no configuration file given.\n"
		          << "For usage and an options list, please do '" << argv[0] << " --help"
		          << "'.\n";
		return 2;
	}

	fhicl::ParameterSet pset;
	if(getenv("FHICL_FILE_PATH") == nullptr)
	{
		std::cerr
		    << "INFO: environment variable FHICL_FILE_PATH was not set. Using \".\"\n";
		setenv("FHICL_FILE_PATH", ".", 0);
	}
	cet::filepath_lookup_after1 lookup_policy("FHICL_FILE_PATH");
	pset = fhicl::ParameterSet::make(vm["config"].as<std::string>(), lookup_policy);

	int rc = 0;

	int socket = TCPConnect(pset.get<std::string>("hostname", "localhost").c_str(),
	                        pset.get<int>("port", 44555));

	uint32_t spillNum = 0;

	while(true)
	{
		otsftbf::DataRequestMessage req(spillNum, true, true);
		write(socket, (void*)&req, sizeof(req));

		otsftbf::DataResponseHeader res;
		read(socket, &res, sizeof(res));

		if(res.isValid())
		{
			if(res.response_fragment_count > 0)
			{
				spillNum++;
				for(uint64_t ii = 0; ii < res.response_fragment_count; ++ii)
				{
					artdaq::Fragment frag;

					read(socket,
					     frag.headerAddress(),
					     sizeof(artdaq::detail::RawFragmentHeader));
					frag.autoResize();
					read(socket,
					     frag.headerAddress() +
					         artdaq::detail::RawFragmentHeader::num_words(),
					     frag.sizeBytes() -
					         artdaq::detail::RawFragmentHeader::num_words() *
					             sizeof(artdaq::RawDataType));

					artdaq::ContainerFragment container(frag);
					std::cout << "Received ContainerFragment, sz="
					          << std::to_string(frag.sizeBytes()) << ", container count="
					          << std::to_string(container.block_count())
					          << ", fragment type="
					          << std::to_string(container.fragment_type()) << std::endl;
				}
			}
			else
			{
				if(spillNum < res.first_event_in_buffer)
				{
					spillNum = res.first_event_in_buffer;
				}
				else
				{
					usleep(10000);
				}
			}
		}
	}

	return rc;
}
