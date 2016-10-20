/// \file Configuration.h
/// \brief C++ interface to retrieve configuration parameters.
///
/// \author Sylvain Chapeland, CERN

#ifndef SRC_CONFIGURATION_H_
#define SRC_CONFIGURATION_H_

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

class ConfigFilePrivate
{
  public:
    ConfigFilePrivate();
    ~ConfigFilePrivate();

    friend class ConfigFile;
    friend class ConfigFileBrowser;

  protected:
    boost::property_tree::ptree pt;

};

class ConfigFile
{
  public:
    ConfigFile();

    ~ConfigFile();

    // explicitely disable automatically generated methods
    // disable copy constructor
    ConfigFile(const ConfigFile&) =delete;
    // disable copy assignment operator
    ConfigFile& operator=(const ConfigFile&) =delete;


    /// Load the configuration from given path
    /// \param path  Path to configuration data.
    ///              Example: file:/configDir/example.cfg
    ///              Accepted prefix:
    ///                 file:     for a file accessible from the filesystem
    ///              Accepted suffix, to define file format (TODO: separate doc for file formats):
    ///                 .ini, .cfg    see example.cfg
    /// \exception   Throws a <std::string> exception on error.
    void load(const std::string path);


    /// Get the configuration value for given key path (by reference)
    /// \param key   Key name (possibly hierarchical)
    /// \param value Result value found (possible types: int, float, std::string), by reference (this variable is modified in case of success)
    /// \returns     Nothing
    /// \exception   Throws a <std::string> exception on error.
    template<typename T>
    void getValue(const std::string key, T &value)
    {
      try {
        value = dPtr->pt.get<T>(key);
      }
      catch (const boost::property_tree::ptree_error &e) {
        throw std::string(e.what());
      }
    }


    /// Get the configuration value for given key path (by result)
    /// \param key   Key name (possibly hierarchical)
    /// \returns     Result value found (possible types: int, float, std::string)
    /// \exception   Throws a <std::string> exception on error.
    template<typename T>
    T getValue(const std::string key)
    {
      T res;
      getValue(key, res);
      return res;
    }


    /// Print configuration tree
    void print();

  private:
    ConfigFilePrivate *dPtr;

  friend class ConfigFileBrowser;
};





/// Helper class to iterate over 1st level sub-tree of config file with a for range-based loop.
///
/// \example        To print section names starting with "part" found in a .ini file:
/// \example          for (auto sectionName : ConfigFileBrowser (&myConfigFile,"part") { std::cout << "Found section : " << sectionName << std::endl;}
class ConfigFileBrowser {

 
public:
  /// Constructor
  /// \param cfgPtr   Pointer to a ConfigFile object
  /// \param filter   Optional string to be matched with sub-tree name. Non-matching elements will be skipped. Only sub-tree names starting by this string will be returned.
  ConfigFileBrowser(ConfigFile *cfgPtr, std::string filter="");
  ~ConfigFileBrowser();
  
  class Iterator;
  Iterator begin();
  Iterator end();
 
private:
  ConfigFile *p;
  std::string filter;
};


/// Internal class used to implement ConfigFileBrowser iterator
class ConfigFileBrowser::Iterator {

   typedef boost::property_tree::ptree::iterator t_Iterator;

   private:
   ConfigFileBrowser *bPtr;
   t_Iterator it;
   t_Iterator itEnd;
   void findNext();

   public: 
   Iterator(ConfigFileBrowser *_bPtr, Iterator::t_Iterator _it, Iterator::t_Iterator _itEnd);
   const std::string & operator*();
   Iterator& operator++();
   bool operator!=(const Iterator& _it) const;
};





#endif /* SRC_CONFIGURATION_H_ */

