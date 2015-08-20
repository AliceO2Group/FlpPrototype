#include <zookeeper.h>
#include <StateMachine.h>
#include <strings.h>
#include <unistd.h>
#include <boost/thread/mutex.hpp>



// implementation of private
class DirectoryPrivate {
  public:
  DirectoryPrivate(const DirectoryConfig cfg);
  ~DirectoryPrivate();

  static void  z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);

  static void z_watcher_subNode (zhandle_t *zzh, int type, int state, const char *path, void* watcherContext);
  static void z_watcher_node (zhandle_t *zzh, int type, int state, const char *path, void* watcherContext);
  
  friend class Directory;
  friend class ControlObject;
  friend class ControlPrivate;
  
  protected:
  zhandle_t *zh;
  clientid_t z_id;
  int z_shutdown;
  int z_ok; 
  DirectoryConfig dirConfig;
  boost::mutex mMutex;
};

DirectoryPrivate::DirectoryPrivate(const DirectoryConfig cfg) {
  zh=0;
  z_shutdown=0;
  z_ok=0;
  bzero(&z_id,sizeof(z_id));
    zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
    zh = zookeeper_init(cfg.zookeeperServer.c_str(),  DirectoryPrivate::z_watcher, 30000, &z_id, this, 0);
    if (!zh) {throw("zookeeper_init() failed");}    
      for (int i=0;i<5000;i++) {
        usleep(1000);
        if (z_ok) {break;}
    } 
    if (!z_ok) {throw("zookeeper init timeout");}
    printf("init done!\n");
}

DirectoryPrivate::~DirectoryPrivate() {
  z_shutdown=1;
  if (z_id.client_id!=0) {
    zookeeper_close(zh);
  }
}

void DirectoryPrivate::z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx){

    DirectoryPrivate *h;
    h=(DirectoryPrivate *)watcherCtx;
    if (h==NULL) {return;}
    if (h->z_shutdown) {return;}
    h->mMutex.lock();
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            const clientid_t *id = zoo_client_id(zzh);
            if (h->z_id.client_id == 0 || h->z_id.client_id != id->client_id) {
                h->z_id = *id;
                //fprintf(stderr, "Got a new session id: 0x%llx\n",(long long)h->z_id.client_id);            
            }
            h->z_ok=1;
        } else if (state == ZOO_AUTH_FAILED_STATE) {
            fprintf(stderr, "Authentication failure. Shutting down...\n");
            zookeeper_close(zzh);
            h->z_shutdown=1;
            h->zh=0;
            h->z_ok=0;
        } else if (state == ZOO_EXPIRED_SESSION_STATE) {
            fprintf(stderr, "Session expired. Shutting down...\n");
            zookeeper_close(zzh);
            h->z_shutdown=1;
            h->zh=0;
            h->z_ok=0;
        } else {
            h->z_ok=0;
        }
    }
    h->mMutex.unlock();
}


DirectoryConfig::DirectoryConfig(const char* zkServer) {
  zookeeperServer=zkServer;
}

DirectoryConfig::~DirectoryConfig() {
}


Directory::Directory(DirectoryConfig cfg) {
  dPtr=new DirectoryPrivate(cfg);  
  if (dPtr==NULL) {
    throw __LINE__;
  }


}

Directory::~Directory(){
  delete dPtr;
}

std::string getObjectPathNode(const std::string objectName) {
  return "/oo/" + objectName;
}

std::string getObjectPathState(const std::string objectName) {
  return "/oo/" + objectName + "/state";
}

std::string getObjectPathCommand(const std::string objectName) {
  return "/oo/" + objectName + "/command";
}


int Directory::CreateNode(const std::string nodeName, const std::string value, int options) {
  int err;
  int zoo_options=0;
  if (options & Directory::NodeOption::ephemeral) {
    zoo_options |= ZOO_EPHEMERAL;
  }
  if (options & Directory::NodeOption::sequence) {
    zoo_options |= ZOO_EPHEMERAL | ZOO_SEQUENCE;
  }
  //printf("create %s = %s\n",nodeName.c_str(),value.c_str());
  char buf[128]="";
  dPtr->mMutex.lock();
  err=zoo_create( dPtr->zh,nodeName.c_str(),value.c_str(),value.length(),&ZOO_OPEN_ACL_UNSAFE,zoo_options,buf,128);
  dPtr->mMutex.unlock();  
  if (err!=ZOK) {
    printf("zookeeper_create(%s): %s\n",nodeName.c_str(),zerror(err));
  } else {
    //printf("%s created (val=%s, %d bytes)\n",buf,value.c_str(),value.length());
  }
  return 0;
}

/*
int Directory::DeleteChildren(const std::string node) {
  String_vector s;
  int err;
  err=zoo_get_children(dPtr->zh, node.c_str(), 0, &s);
  for (int i=0;i<s.count;i++) {
      std::string childNode;
      childNode=node + "/" + s.data[i];      
      err=zoo_delete(dPtr->zh,childNode.c_str(),-1);
      printf("deleting %s = %d\n",childNode.c_str(),err);
  }
  return 0;
}
*/


int Directory::SetValue(const std::string node ,const std::string value) {
  int l=value.length();
  int err=0;
  dPtr->mMutex.lock();  
  if (dPtr->zh!=0) {
    err=zoo_set(dPtr->zh, node.c_str(), value.c_str(), l, -1);
    if (err) {
      printf("Failed to publish %s = %s : %s\n",node.c_str(), value.c_str(),zerror(err));
    }
  }
  dPtr->mMutex.unlock();
}



void DirectoryPrivate::z_watcher_subNode (zhandle_t *zzh, int type, int state, const char *path, void* context){
    String_vector s;
    
    if (type==ZOO_CHILD_EVENT) {
      if (ZOK==zoo_wget_children(zzh, path, DirectoryPrivate::z_watcher_subNode, context, &s)) {
          // todo: why are we called with s.count=0 ???
          //printf("%s : %d commands received\n",path,s.count);
          // todo: reorder queue
          for (int i=0;i<s.count;i++) {
              std::string node(path);
              std::string childNode;
              childNode=node + "/" + s.data[i];      
              //printf("handling command %s\n",childNode.c_str());
              int err;
              char val[128]="";
              int sz=sizeof(val)-1;
              err=zoo_get(zzh, childNode.c_str(), 0, val, &sz, NULL);
              val[sz]=0;
              if (err==ZOK) {
                //printf("%s command=%s (%d bytes)\n",childNode.c_str(),val,sz);
                std::string command(val,sz);
                //executeCommand(command);
                if (context!=NULL) {
                  NodeCallback *exe=(NodeCallback *)context;
                  exe->f_callback(command,exe->context, NodeCallback::NodeEventType::NodeCreated);
                }
              }
              err=zoo_delete(zzh,childNode.c_str(),-1);
              if (err=ZOK) {
                //printf("%s deleted\n",childNode.c_str());
              }
          }      
      }
    }
}


int Directory::SubscribeNewChild(const std::string node, int purge, NodeCallback *callback) {
  String_vector s;
  int err;
  err=zoo_wget_children(dPtr->zh, node.c_str(), DirectoryPrivate::z_watcher_subNode, callback, &s); 
  if (err!=ZOK) {
    return __LINE__;
  }
  // should delete or process existing nodes?
  for (int i=0;i<s.count;i++) {
      std::string childNode;
      childNode=node + "/" + s.data[i];      
      // todo ...
      if (purge) {
        err=zoo_delete(dPtr->zh,childNode.c_str(),-1);
        //printf("deleting %s = %d\n",childNode.c_str(),err);
      }
  }
  return 0;
}


// for initial callback setup, call with type=0 for "no type" event c.f. /opt/zookeeper-3.4.6/src/c/src/zk_adaptor.h

void DirectoryPrivate::z_watcher_node (zhandle_t *zzh, int type, int state, const char *path, void* context) { 
    //printf("callback %p:%p %s type %d\n",path,context,path,type);
    NodeCallback *exe=(NodeCallback *)context;
    std::string nodeValue;
    
    if (type==ZOO_CHANGED_EVENT) {
         char buffer[1024];
         int bufferSize=sizeof(buffer)/sizeof(char);
         struct Stat stat;
         // get new value (and subscribe to changed/deleted at the same time)
         if (ZOK==zoo_wget(zzh, path, DirectoryPrivate::z_watcher_node, context, buffer, &bufferSize, &stat)) {         

           if (bufferSize>0) {
             nodeValue=std::string(buffer,bufferSize);
           }
           if (exe!=NULL) {
             exe->f_callback(nodeValue, exe->context, NodeCallback::NodeEventType::NodeUpdated);
           }           
           return;
         }
    } else if (type==ZOO_CREATED_EVENT) {
         if (exe!=NULL) {
           exe->f_callback(nodeValue, exe->context, NodeCallback::NodeEventType::NodeCreated);
         }   
    } else if (type==ZOO_DELETED_EVENT) {
         if (exe!=NULL) {
           exe->f_callback(nodeValue, exe->context, NodeCallback::NodeEventType::NodeDestroyed);
         }
    }
    
    // subscribe again
    // this will trigger a callback on create/delete/change events
    struct Stat s;
    int err;
    err=zoo_wexists(zzh, path, DirectoryPrivate::z_watcher_node, context, &s);
    //printf("zoo_wexists %p=%d\n",path,err);
    if ((err==ZOK)&&(type==0)) {
      // if initial subscribe, subscribe again to get a 1st value (otherwise will trigger only on next change)
      z_watcher_node(zzh,ZOO_CHANGED_EVENT,0,path,context);
    }
}


int Directory::SubscribeNode(const std::string node, NodeCallback *callback) {
  const char *p=node.c_str();
  //printf("subscribe %s\n",p);
  DirectoryPrivate::z_watcher_node(dPtr->zh,0,0,p,callback);
  return 0;
//   return getNodeValue(dPtr->zh, node.c_str(), callback);
}



class ControlPrivate {
  public:
  ControlPrivate(const std::string objectName, Directory *dir);
  ~ControlPrivate();

  friend class ControlObject;
  friend class ControlClient;
  protected:
  Directory *mDirectory;
  std::string mObjectName;

  std::string mPathNode;
  std::string mPathCommand;
  std::string mPathState;

  std::string mCurrentState;
  //todo
  int mPathExists;
  
  static void commandCallback(const std::string nodeValue, void *context, NodeCallback::NodeEventType event); // to handle command received by object
  NodeCallback cbCommand;
  static void stateCallback(const std::string nodeValue, void *context, NodeCallback::NodeEventType event); // to handle node state change
  NodeCallback cbState;
};


ControlPrivate::ControlPrivate(const std::string objectName, Directory *dir){
  mDirectory=dir;
  mObjectName=objectName;
  
  mPathNode=getObjectPathNode(objectName);
  mPathCommand=getObjectPathCommand(objectName);
  mPathState=getObjectPathState(objectName);
  
  mCurrentState="";
  mPathExists=0;
  
  cbCommand.f_callback=NULL;
  cbCommand.context=NULL;

  cbState.f_callback=NULL;
  cbState.context=NULL;
}

ControlPrivate::~ControlPrivate(){
  // TODO: cancel subscribe
}


void ControlPrivate::stateCallback(const std::string nodeValue, void *context, NodeCallback::NodeEventType event) {
  const char *evT="unknown";
  switch(event) {
  case NodeCallback::NodeEventType::NodeCreated: evT="NodeCreated"; break;
  case NodeCallback::NodeEventType::NodeDestroyed: evT="NodeDestroyed"; break;
  case NodeCallback::NodeEventType::NodeUpdated: evT="NodeUpdated"; break;
  }
  if (context==NULL) return;
  ControlClient *o=(ControlClient *)(context);
  printf("event=%s node=%s value=%s\n",evT,o->dPtr->mObjectName.c_str(),nodeValue.c_str());

  const std::string state=nodeValue;
  // further decoding can be done here
   
  o->dPtr->mCurrentState=state;

  if (event==NodeCallback::NodeEventType::NodeUpdated) {
    o->objectStateChangedCallback(state);
  } else if (event==NodeCallback::NodeEventType::NodeCreated) {
    o->objectCreatedCallback();
  } else if (event==NodeCallback::NodeEventType::NodeDestroyed) {
    o->objectDestroyedCallback();    
  }

}



void ControlPrivate::commandCallback(const std::string nodeValue, void *context, NodeCallback::NodeEventType event){
  if (context!=NULL) {
    ControlObject *o=(ControlObject *)context;
    const std::string command=nodeValue;
    // further decoding can be done here, e.g. in case of parameters
    o->executeCommand(command);
  }
}

ControlObject::ControlObject(const std::string objectName, Directory *dir){
  dPtr=new ControlPrivate(objectName,dir);
  if (dPtr==NULL) {throw __LINE__;}

  int err;
  err=dPtr->mDirectory->CreateNode(dPtr->mPathNode,"");
  err=dPtr->mDirectory->CreateNode(dPtr->mPathCommand,"");
  err=dPtr->mDirectory->CreateNode(dPtr->mPathState,"",1);  
  if (err) {throw err;}
  
  dPtr->cbCommand.f_callback=&ControlPrivate::commandCallback;
  dPtr->cbCommand.context=this;
  
  // subscribe to commands and purge existing ones
  dPtr->mDirectory->SubscribeNewChild(dPtr->mPathCommand, 1, &dPtr->cbCommand);  
}

ControlObject::~ControlObject(){
  delete dPtr;
}

void ControlObject::executeCommand(const std::string command) {
  printf("%s : executing %s\n",dPtr->mObjectName.c_str(),command.c_str());
}

int ControlObject::setState(const std::string newState) {
  if (dPtr->mDirectory->SetValue(dPtr->mPathState,newState)==0) {
    dPtr->mCurrentState=newState;
    printf("%s : set new state:= %s\n",dPtr->mObjectName.c_str(),newState.c_str());
  }
  return 0;
}

//returns locally cached state
int ControlObject::getState(std::string &currentState) {
  currentState=dPtr->mCurrentState;
  return 0;
}


ControlClient::ControlClient(const std::string objectName, Directory *dir) {
  dPtr=new ControlPrivate(objectName,dir);
  if (dPtr==NULL) {throw __LINE__;}
  
  //subscribe to state
  dPtr->cbState.f_callback=&ControlPrivate::stateCallback;
  dPtr->cbState.context=this; 
  dPtr->mDirectory->SubscribeNode(dPtr->mPathState, &dPtr->cbState);
}

ControlClient::~ControlClient(){
  delete dPtr;
}
  
int ControlClient::sendCommand(const std::string command) {
  std::string zcmd=dPtr->mPathCommand + "/cmd-";
  std::cout << zcmd << " -> " << command << std::endl;
  return dPtr->mDirectory->CreateNode(zcmd,command,Directory::NodeOption::sequence);
}

int ControlClient::getState(std::string &currentState) {
  currentState=dPtr->mCurrentState;
  return 0;
}


void ControlClient::objectStateChangedCallback(const std::string newState) {
  printf("%s => updated, new state=%s\n",dPtr->mObjectName.c_str(),newState.c_str());
  return;
}

void ControlClient::objectCreatedCallback() {
  printf("%s => created\n",dPtr->mObjectName.c_str());
  return;
}

void ControlClient::objectDestroyedCallback() {
  printf("%s => destroyed\n",dPtr->mObjectName.c_str());
  return;
}
