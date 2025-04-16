////////////////////////////////////////////////////////////////////////
// Class:       EventDataReceiverProducer
// Plugin Type: producer (art v2_11_01)
// File:        EventDataReceiverProducer_module.cc
//
// Generated at Mon Jul  1 08:54:54 2019 by Eric Flumerfelt using cetskelgen
// from cetlib version v3_03_00.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "artdaq-core/Data/Fragment.hh"
#include "artdaq-core/Utilities/TimeUtils.hh"
#include "artdaq/DAQdata/TCPConnect.hh"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "otsdaq-fermilabtestbeam/ArtModules/detail/DataRequestMessage.hh"
#include "otsdaq-fermilabtestbeam/Overlays/FragmentType.hh"

#include <memory>

class EventDataReceiverProducer;

class EventDataReceiverProducer : public art::EDProducer
{
  public:
	explicit EventDataReceiverProducer(fhicl::ParameterSet const& p);
	// The compiler-generated destructor is fine for non-base
	// classes without bare pointers or other resource use.

	// Plugins should not be copied or assigned.
	EventDataReceiverProducer(EventDataReceiverProducer const&) = delete;
	EventDataReceiverProducer(EventDataReceiverProducer&&)      = delete;
	EventDataReceiverProducer& operator=(EventDataReceiverProducer const&) = delete;
	EventDataReceiverProducer& operator=(EventDataReceiverProducer&&) = delete;

	// Required functions.
	void produce(art::Event& e) override;

	// Selected optional functions.
	void beginJob() override;
	void beginRun(art::Run& r) override;
	void beginSubRun(art::SubRun& sr) override;
	void endJob() override;
	void endRun(art::Run& r) override;
	void endSubRun(art::SubRun& sr) override;

  private:
	// Declare member data here.
	int               fd_;
	int               port_;
	std::string       address_;
	std::atomic<bool> offset_set_;
	int               event_number_offset_;
	double            timeout_;
	bool              wantMWPC_;
	bool              wantSTIB_;
	bool              wantCherenkov_;
	bool              wantUDP_;

	std::unique_ptr<otsftbf::DataResponseHeader> sendRequest_(uint32_t eventNum);
	artdaq::Fragment                             readFragment_();
};

EventDataReceiverProducer::EventDataReceiverProducer(fhicl::ParameterSet const& p)
    : fd_(-1)
    , port_(p.get<int>("request_port", 44555))
    , address_(p.get<std::string>("request_address", "127.0.0.1"))
    , offset_set_(false)
    , event_number_offset_(0)
    , timeout_(p.get<double>("server_timeout_s", 10.0))
    , wantMWPC_(p.get<bool>("receive_mwpc", false))
    , wantSTIB_(p.get<bool>("receive_stib", false))
    , wantCherenkov_(p.get<bool>("receive_cherenkov", false))
    , wantUDP_(p.get<bool>("receive_udpdata", false))
{
	// Call appropriate produces<>() functions here.
	if(wantMWPC_)
	{
		produces<artdaq::Fragments>("mwpc_data");
	}
	if(wantSTIB_)
	{
		produces<artdaq::Fragments>("stib_data");
	}
	if(wantCherenkov_)
	{
		produces<artdaq::Fragments>("cherenkov_data");
	}
	if(wantUDP_)
	{
		produces<artdaq::Fragments>("udp_data");
	}
}

void EventDataReceiverProducer::produce(art::Event& e)
{
	auto drh = sendRequest_(e.event());

	if(drh == nullptr)
		return;

	if(!offset_set_)
	{
		assert(drh->response_fragment_count == 0);

		event_number_offset_ = drh->first_event_in_buffer - e.event();

		drh = sendRequest_(e.event());

		if(drh == nullptr)
			return;
	}

	std::unique_ptr<artdaq::Fragments> mwpc_fragments, stib_fragments,
	    cherenkov_fragments, udp_fragments;
	for(size_t ii = 0; ii < drh->response_fragment_count; ++ii)
	{
		auto frag = readFragment_();
		if(wantMWPC_ && (frag.type() == otsftbf::FragmentType::MWPC ||
		                 frag.type() == otsftbf::FragmentType::MWPCEVT))
		{
			mwpc_fragments->push_back(frag);
		}
		else if(wantSTIB_ && (frag.type() == otsftbf::FragmentType::STIB ||
		                      frag.type() == otsftbf::FragmentType::STIBEVT))
		{
			stib_fragments->push_back(frag);
		}
		else if(wantCherenkov_ && frag.type() == otsftbf::FragmentType::CAMAC)
		{
			cherenkov_fragments->push_back(frag);
		}
		else if(wantUDP_ && frag.type() == otsftbf::FragmentType::UDP)
		{
			udp_fragments->push_back(frag);
		}
	}
	if(wantMWPC_)
		e.put(std::move(mwpc_fragments), "mwpc_data");
	if(wantSTIB_)
		e.put(std::move(stib_fragments), "stib_data");
	if(wantCherenkov_)
		e.put(std::move(cherenkov_fragments), "cherenkov_data");
	if(wantUDP_)
		e.put(std::move(udp_fragments), "udp_data");
}

void EventDataReceiverProducer::beginJob()
{
	// Implementation of optional member function here.
	offset_set_ = false;
}

void EventDataReceiverProducer::beginRun(art::Run& r)
{
	// Implementation of optional member function here.
	offset_set_ = false;
}

void EventDataReceiverProducer::beginSubRun(art::SubRun& sr)
{
	// Implementation of optional member function here.
	offset_set_ = false;
}

void EventDataReceiverProducer::endJob()
{
	// Implementation of optional member function here.
	offset_set_ = false;
}

void EventDataReceiverProducer::endRun(art::Run& r)
{
	// Implementation of optional member function here.
	offset_set_ = false;
}

void EventDataReceiverProducer::endSubRun(art::SubRun& sr)
{
	// Implementation of optional member function here.
	offset_set_ = false;
}

std::unique_ptr<otsftbf::DataResponseHeader> EventDataReceiverProducer::sendRequest_(
    uint32_t eventNum)
{
	auto start_time = std::chrono::steady_clock::now();
	// Implementation of required member function here.
	while(fd_ == -1 && artdaq::TimeUtils::GetElapsedTime(start_time) < timeout_)
	{
		TLOG(TLVL_TRACE) << "Attempting to connect to Event Data Server";
		fd_ = TCPConnect(address_.c_str(), port_, 0, 0);
		if(fd_ == -1)
		{
			usleep(100000);
		}
		else
		{
			TLOG(TLVL_DEBUG) << "Event Data Server Connected";
			offset_set_ = false;
		}
	}

	if(fd_ == -1)
	{
		TLOG(TLVL_ERROR) << "Socket not connected after timeout of " << timeout_
		                 << " s. Aborting!";
		return nullptr;
	}

	auto eventNumber = offset_set_ ? eventNum + event_number_offset_ : 0;
	otsftbf::DataRequestMessage drm(
	    eventNumber, wantMWPC_, wantSTIB_, wantCherenkov_, wantUDP_);

	write(fd_, &drm, sizeof(drm));

	std::unique_ptr<otsftbf::DataResponseHeader> drh(new otsftbf::DataResponseHeader());
	read(fd_, drh.get(), sizeof(otsftbf::DataResponseHeader));

	if(!drh->isValid())
	{
		TLOG(TLVL_ERROR) << "Received invalid DataResponseHeader!";
		close(fd_);
		fd_ = -1;
		return nullptr;
	}

	return drh;
}

artdaq::Fragment EventDataReceiverProducer::readFragment_()
{
	artdaq::Fragment output;

	read(fd_, output.headerAddress(), sizeof(artdaq::detail::RawFragmentHeader));
	output.autoResize();
	read(fd_,
	     output.headerAddress() + sizeof(artdaq::detail::RawFragmentHeader),
	     output.sizeBytes() - sizeof(artdaq::detail::RawFragmentHeader));

	return output;
}

DEFINE_ART_MODULE(EventDataReceiverProducer)
