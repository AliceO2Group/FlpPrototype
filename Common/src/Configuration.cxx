/// \file Configuration.h
/// \brief C++ interface to retrieve configuration parameters.
///
/// \author Sylvain Chapeland, CERN

#include "Common/Configuration.h"
#include <iostream>
#include <vector>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;

ConfigFilePrivate::ConfigFilePrivate()
{
}

ConfigFilePrivate::~ConfigFilePrivate()
{
}


ConfigFile::ConfigFile()
{
  dPtr = new ConfigFilePrivate();
  if (dPtr == NULL) {
    std::stringstream ss;
    ss << "new() error in " << __FILE__ << " line " << __LINE__;
    throw ss.str();
  }
}

ConfigFile::~ConfigFile()
{
  if (dPtr != NULL) {
    delete dPtr;
  }
}

#define PREFIX_FILE "file:"
#define SUFFIX_FILE_INI {".ini", ".cfg"}

void ConfigFile::load(const std::string path)
{
  if (path.length() == 0) { throw std::string("Invalid argument"); }

  //
  // open location according to prefix
  //


  // filesystem file
  if (boost::algorithm::starts_with(path.c_str(), PREFIX_FILE)) {
    const char *filename;
    filename = &path[strlen(PREFIX_FILE)];

    // TODO: filter out comments in file with boost filtering_stream

    //
    // parse file according to suffix
    //

    // INI file
    std::vector<const char*> suffix_ini SUFFIX_FILE_INI;
    for (auto suffix : suffix_ini) {
      if (boost::algorithm::ends_with(filename, suffix)) {
        try {
          boost::property_tree::ini_parser::read_ini(filename, dPtr->pt);
        }
        catch (boost::property_tree::ini_parser::ini_parser_error perr) {
          std::stringstream ss;
          if (perr.line()) {
            ss << perr.message() << " in " << perr.filename() << " line " << perr.line();
          } else {
            ss << perr.message() << " " << perr.filename();
          }
          throw ss.str();
        }
        return;
      }
    }
    throw std::string("Invalid type in file name");
  } else {
    throw std::string("Invalid path prefix");
  }
}

void getValueSpecialization(void) {
  ConfigFile f;
  const std::string s("");
  int vInt;
  f.getValue(s,vInt);
  vInt=f.getValue<int>(s);
  float vFloat;
  f.getValue("",vFloat);
  vFloat=f.getValue<float>(s);
  std::string vString;
  f.getValue("",vString);
  vString=f.getValue<std::string>(s);
}


//template void ConfigFile::getValue<float>(std::string, float&);





// http://stackoverflow.com/questions/4586768/how-to-iterate-a-boost-property-tree
string indent(int level) {
  string s; 
  for (int i=0; i<level; i++) s += "  ";
  return s; 
}

void printTree (boost::property_tree::ptree &pt, int level) {
  if (pt.empty()) {
    cerr << "\""<< pt.data()<< "\"";
  } else {
    if (level) cerr << endl; 
    cerr << indent(level) << "{" << endl;     
    for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end();) {
      cerr << indent(level+1) << "\"" << pos->first << "\": "; 
      printTree(pos->second, level + 1); 
      ++pos; 
      if (pos != pt.end()) {
        cerr << ","; 
      }
      cerr << endl;
    } 
    cerr << indent(level) << " }";     
  }
  return; 
}

void ConfigFile::print() {
  printTree(dPtr->pt,0);
}







ConfigFileBrowser::Iterator::Iterator(ConfigFileBrowser *_bPtr, ConfigFileBrowser::Iterator::t_Iterator _it, ConfigFileBrowser::Iterator::t_Iterator _itEnd)
:bPtr(_bPtr),it(_it),itEnd(_itEnd) {
  // skip initial elements if filter defined
  findNext();
}

// iterate until matching sub-tree name found, in case a filter is defined
void ConfigFileBrowser::Iterator::findNext() {
  int l=bPtr->filter.length();
  if (l) {
    while (it!=itEnd) {
      std::string s((*it).first);
      if (!s.compare(0,l,bPtr->filter)) {
         break;
      }
      ++it;
    }
  }
}

const std::string & ConfigFileBrowser::Iterator::operator*() {
  return (*it).first;
}

ConfigFileBrowser::Iterator& ConfigFileBrowser::Iterator::operator++() {
   ++it;
   findNext();      
   return *this;
}

bool ConfigFileBrowser::Iterator::operator!=(const ConfigFileBrowser::Iterator& _it) const {
  return it!=_it.it;
}

  


ConfigFileBrowser::ConfigFileBrowser(ConfigFile *_p, std::string _filter) {
  p=_p;
  filter=_filter;
}

ConfigFileBrowser::~ConfigFileBrowser() {
}

ConfigFileBrowser::Iterator ConfigFileBrowser::begin() {
  return {this,p->dPtr->pt.begin(),p->dPtr->pt.end()};
}

ConfigFileBrowser::Iterator ConfigFileBrowser::end() {
  return Iterator(this,p->dPtr->pt.end(),p->dPtr->pt.end());
}
 
