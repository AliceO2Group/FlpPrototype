/// \file    launchReceiver.cxx
/// \brief   Example of a FairMQDevice receiver
///
/// \author  Vasco Barroso, CERN

#include "ControlFairMQ/Receiver.h"
#include "ControlFairMQ/Version.h"
#include <boost/program_options.hpp>
#include <iostream>
#include "runFairMQDevice.h"

using AliceO2::InfoLogger::InfoLogger;
namespace program_options = boost::program_options;
namespace ControlFairMQ = AliceO2::ControlFairMQ;

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description &options)
{
  options.add_options()
    ("version,v", "Show program name/version banner and exit.")
    ("revision", "Print the revision number");
  // TODO add back the possibility to get the version number (in the Device?)
}

FairMQDevicePtr getDevice(const FairMQProgOptions & /*config*/)
{
  return new ControlFairMQ::Core::Receiver();
}
