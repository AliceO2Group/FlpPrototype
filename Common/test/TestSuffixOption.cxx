#include "Common/SuffixOption.h"
#include <boost/program_options.hpp>

#define BOOST_TEST_MODULE SuffixOption
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace AliceO2::Common;
namespace po = boost::program_options;

BOOST_AUTO_TEST_CASE(TestSuffixOption)
{
  int64_t numberA = 0;
  SuffixNumber<int64_t> numberB = 0;
  double numberC = 0;

  po::options_description options;
  options.add_options()("numA", SuffixOption<int64_t>::make(&numberA)->required(), "");
  options.add_options()("numB", SuffixOption<int64_t>::make(&numberB)->default_value("123k"), "");
  options.add_options()("numC", SuffixOption<double>::make(&numberC), "");

  //  Mock arguments
  std::vector<const char*> args = { "/test", "--numA=1Mi", "--numB=123", "--numC=-1k", };
  po::variables_map map;
  po::store(po::parse_command_line(args.size(), args.data(), options), map);
  po::notify(map);

  BOOST_CHECK(numberA == 1*1024*1024);
  BOOST_CHECK(numberB.getNumber() == 123);
  BOOST_CHECK(numberC == -1000.0);
}

