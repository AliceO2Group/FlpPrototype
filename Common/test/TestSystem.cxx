#include "Common/System.h"
#include <atomic>
#include <signal.h>
#include <boost/filesystem/operations.hpp>

#define BOOST_TEST_MODULE System
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


/*
void setSigIntHandler(void(*function)(int));
bool isSigIntHandlerSet();
std::string executeCommand(const std::string& command);
void makeParentDirectories(const std::string& path);
void touchFile(const std::string& path);
std::string getFileSystemType(const std::string& path);
std::pair<bool, std::string> isFileSystemTypeAnyOf(const std::string& path, const std::set<std::string>& types);
void assertFileSystemType(const std::string&, const std::set<std::string>& validTypes, std::string name);
*/

using namespace AliceO2::Common;

namespace
{
std::atomic<bool> sSignalRaised { false };
void sigIntHandler(int)
{
  sSignalRaised = true;
}
} // Anyonymous namespace

BOOST_AUTO_TEST_CASE(TestSetSigIntHandler)
{
  BOOST_CHECK(sSignalRaised.load() == false);
  BOOST_CHECK(System::isSigIntHandlerSet() == false);

  System::setSigIntHandler(&sigIntHandler);
  BOOST_CHECK(System::isSigIntHandlerSet() == true);

  raise(SIGINT);
  BOOST_CHECK(sSignalRaised.load() == true);
}

BOOST_AUTO_TEST_CASE(TestExecuteCommand)
{
  BOOST_CHECK(System::executeCommand("echo 'Hello test!'") == "Hello test!\n");
}

BOOST_AUTO_TEST_CASE(TestTouchFile)
{
  auto path = "/tmp/touched_file";
  boost::filesystem::remove(path);
  System::touchFile("/tmp/touched_file");
  BOOST_CHECK(boost::filesystem::exists(path));
}

BOOST_AUTO_TEST_CASE(TestGetFileSystemType)
{
  BOOST_CHECK(System::getFileSystemType("/sys") == "sysfs");
  BOOST_CHECK(System::getFileSystemType("/proc") == "proc");
  BOOST_CHECK(System::getFileSystemType("/dev") == "devtmpfs");
  BOOST_CHECK(System::getFileSystemType("/dev/shm") == "tmpfs");
}

BOOST_AUTO_TEST_CASE(TestIsFileSystemTypeAnyOf)
{
  BOOST_CHECK(System::isFileSystemTypeAnyOf("/sys", {"sysfs", "ext4", "tmpfs"}).first == true);
  BOOST_CHECK(System::isFileSystemTypeAnyOf("/sys", {"blahfs", "ext42"}).first == false);
}
