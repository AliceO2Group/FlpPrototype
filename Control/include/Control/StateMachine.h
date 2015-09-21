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


// data structure to hold information to setup node callbacks. It consists of:
// - a function to be called
// - a user-configurable context parameter which will be provided to the function
class NodeCallback {
public:
  enum NodeEventType {NodeCreated,NodeDestroyed,NodeUpdated};                       // the event type creating the callback
  void (*f_callback)(const std::string value, void *context, NodeCallback::NodeEventType event);  // function prototype for node update callback. function will be called with new node value, and 'context' pointer provided at subscribe time.
  void *context;                                                                    // parameter to be given to the function
};

/// creates an access to the central directory
class Directory {
  public:
  /// constructor
  Directory(DirectoryConfig cfg);

  /// destructor
  ~Directory();
  
  enum NodeOption {
    ephemeral=1, sequence=2
  };

  
  int CreateNode(const std::string node, const std::string value, int options=0);            // create a node in the directory tree (permanent by default, or persistent until process disconnects if isEphemeral set). node is assigned an initial value.
  int SubscribeNewChild(const std::string node, int purge=0, NodeCallback *callback=NULL);   // subsribe to new child nodes created below given node. If purge set, delete existing sub-nodes. If callback set, function will be called accordingly
  int SubscribeNode(const std::string node, NodeCallback *callback=NULL);                    // subscribe to node value update.
    
   
  int SetValue(const std::string node, const std::string value);                         // set value of a node
  int GetValue(const std::string node, std::string &value);                              // get value of a node
  
  private:
  DirectoryPrivate *dPtr; ///< private class data
};



class ControlPrivate;

/// a control object has a state, and receives commands.
class ControlObject {
  public:
  ControlObject(const std::string objectName, Directory * dir);     /// create a control object with given name, using given directory
  virtual ~ControlObject();                                         /// destroy a control object
  
  virtual void executeCommand(const std::string command);
  int getState(std::string &currentState);
  int setState(const std::string newState);
  
  private:
  ControlPrivate *dPtr; ///< private class data
};



/// subsribe a remote object state
/// send a command to remote object
class ControlClient {
  public:
  ControlClient(const std::string objectName, Directory *dir);
  ~ControlClient();
  
  int sendCommand(const std::string command);
  int getState(std::string &currentState);

  virtual void objectCreatedCallback();
  virtual void objectDestroyedCallback();
  virtual void objectStateChangedCallback(const std::string newState); // called when remote object state changes
  
  friend class ControlPrivate;
  protected:
  ControlPrivate *dPtr; ///< private class data
};

