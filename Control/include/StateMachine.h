/// \file  StateMachine.h
/// \brief An interface to implement distributed state machines
///
///
/// \author Sylvain Chapeland, CERN
#include <string>



/// class for private data in Directory class
class DirectoryPrivate;



/// configuration parameters for central directory access
class DirectoryConfig {
  public:

  /// constructor.
  /// \param zookeeperServer : zookeeper connection string, as accepted by zookeeper_init(), i.e. comma-separated list of hostname:port pairs
  DirectoryConfig(
    const char* zookeeperServer = ""
  );

  /// destructor
  ~DirectoryConfig();

  /// read directory configuration from file
  int fromFile(const char *configFile);

  friend class DirectoryPrivate;
  protected:
  std::string zookeeperServer;  ///< path to the server(s), c.f. zookeeper_init() 1st parameter
};



/// creates an access to the central directory
class Directory {
  public:
  /// constructor
  Directory(DirectoryConfig cfg);

  /// destructor
  ~Directory();
  
  
  int AddObject(std::string obj);
  int SetValue(std::string node, std::string value);
  int SendCommand(std::string obj,std::string command);
  
  private:
  DirectoryPrivate *dPtr; ///< private class data
};



class ControlObjectPrivate;

/// a control object has a state, and receives commands.
class ControlObject {
  public:
  ControlObject(const std::string objectName, Directory * dir);     /// create a control object with given name, using given directory
  ~ControlObject();                                               /// destroy a control object
  
  int commandCallback(const std::string command);
  int getState(std::string &currentState);
  int setState(const std::string newState);
  
  private:
  ControlObjectPrivate *dPtr; ///< private class data
};


class ControlClient;
/// subsribe a remote object state
/// send a command to remote object
