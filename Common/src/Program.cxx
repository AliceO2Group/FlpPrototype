/// \file Program.cxx
/// \brief Implementation of the Program class.
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#include "Common/Program.h"
#include <sys/ioctl.h>
#include <iomanip>
#include <iostream>
#include <boost/version.hpp>
#include "Common/Exception.h"
#include "Common/Version.h"
#include "Common/System.h"

using std::cout;
using std::endl;
namespace po = boost::program_options;

namespace AliceO2
{
namespace Common
{
namespace
{
  const std::string HELP_SWITCH = "help";
  const std::string VERBOSE_SWITCH = "verbose";
  const std::string VERSION_SWITCH = "version";

  po::options_description createOptionsDescription()
  {
    // Get size of the terminal, the amount of columns is used for formatting the options
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    po::options_description optionsDescription("Allowed options", w.ws_col, w.ws_col/2);
    optionsDescription.add_options()("help", "Produce help message");
    return optionsDescription;
  }
}

std::atomic<bool> Program::sFlagSigInt(false);

void Program::sigIntHandler(int)
{
  sFlagSigInt = true;
}

Program::Program()
    : mVerbose(false)
{
}

Program::~Program()
{
}

void Program::printHelp (const po::options_description& optionsDescription)
{
  const auto& description = getDescription();
  cout << "#### " << description.name << "\n"
  << description.description << '\n'
  << '\n'
  << optionsDescription
  << '\n'
  << "Example:\n"
  << "  " << description.usage << '\n';
}

int Program::execute(int argc, char** argv)
{
  System::setSigIntHandler(sigIntHandler);

  auto optionsDescription = createOptionsDescription();

  // We add a verbose switch
  optionsDescription.add_options()
      (VERBOSE_SWITCH.c_str(), "Verbose output")
      (VERSION_SWITCH.c_str(), "Display RORC library version");

  // Subclass will add own options
  addOptions(optionsDescription);

  try {
    // Parse options and get the resulting map of variables
    po::variables_map variablesMap;
    try {
      po::store(po::parse_command_line(argc, argv, optionsDescription), variablesMap);
      if (variablesMap.count(HELP_SWITCH)) {
        printHelp(optionsDescription);
        return 0;
      }
      po::notify(variablesMap);
    }
    catch (const po::unknown_option& e) {
      BOOST_THROW_EXCEPTION(ProgramOptionException()
          << ErrorInfo::Message("Unknown option '" + e.get_option_name() + "'"));
    }

    if (variablesMap.count(VERSION_SWITCH)) {
      cout << "Common lib " << Version::getString() << '\n' << "Revision " << Version::getRevision()
          << '\n';
      return 0;
    }

    mVerbose = bool(variablesMap.count(VERBOSE_SWITCH));

    // Start the actual program
    run(variablesMap);
  }
  catch (const ProgramOptionException& e) {
    auto message = boost::get_error_info<ErrorInfo::Message>(e);
    std::cout << "Program options invalid: " << *message << "\n\n";
    printHelp(optionsDescription);
  }
  catch (const po::error& e) {
    std::cout << "Program options error: " << e.what() << "\n\n";
    printHelp(optionsDescription);
  }
  catch (const std::exception& e) {
#if (BOOST_VERSION >= 105400)
    std::cout << "Error: " << e.what() << '\n' << boost::diagnostic_information(e, isVerbose()) << '\n';
#else
#pragma message "BOOST_VERSION < 105400"
    std::cout << "Error: " << e.what() << '\n' << boost::diagnostic_information(e) << '\n';
#endif
  }

  return 0;
}

} // namespace Common
} // namespace AliceO2
