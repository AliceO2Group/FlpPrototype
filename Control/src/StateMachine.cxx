#include <zookeeper.h>
#include <StateMachine.h>
#include <strings.h>
#include <unistd.h>
#include <boost/thread/mutex.hpp>

class DirectoryPrivate {
  public:
  DirectoryPrivate(const DirectoryConfig cfg);
  ~DirectoryPrivate();

  static void  z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);
 
  friend class Directory;
  friend class ControlObject;
  friend class ControlObjectPrivate;
  
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

std::string getObjectPathNode(std::string objectName) {
  return "/" + objectName;
}

std::string getObjectPathState(std::string objectName) {
  return "/" + objectName + "/state";
}

std::string getObjectPathCommand(std::string objectName) {
  return "/" + objectName + "/command";
}


int Directory::AddObject(std::string objectName) {
  dPtr->mMutex.lock();
  zoo_create( dPtr->zh,getObjectPathNode(objectName).c_str(),0,0,&ZOO_OPEN_ACL_UNSAFE, 0,0,0);
  zoo_create( dPtr->zh,getObjectPathState(objectName).c_str(),0,0,&ZOO_OPEN_ACL_UNSAFE,0,0,0);
  zoo_create( dPtr->zh,getObjectPathCommand(objectName).c_str(),0,0,&ZOO_OPEN_ACL_UNSAFE,ZOO_EPHEMERAL,0,0);
  dPtr->mMutex.unlock();
  return 0; 
}


class ControlObjectPrivate {
  public:
  ControlObjectPrivate(const std::string objectName, Directory *dir);
  ~ControlObjectPrivate();

  friend class ControlObject;
  protected:
  Directory *mDirectory;
  std::string mObjectName;
  std::string mPathCommand;
  std::string mPathState;

  std::string mCurrentState;
};

ControlObjectPrivate::ControlObjectPrivate(const std::string objectName, Directory *dir){
  mDirectory=dir;
  mObjectName=objectName;
  
  mPathCommand=getObjectPathState(objectName);
  mPathState=getObjectPathState(objectName); 
}

ControlObjectPrivate::~ControlObjectPrivate(){
}

ControlObject::ControlObject(const std::string objectName, Directory *dir){
  dPtr=new ControlObjectPrivate(objectName,dir);
  if (dPtr==NULL) {throw __LINE__;}

  dPtr->mDirectory->AddObject(objectName);
}

ControlObject::~ControlObject(){
  delete dPtr;
}

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

int ControlObject::setState(const std::string newState) {
  if (dPtr->mDirectory->SetValue(dPtr->mPathState,newState)==0) {
    dPtr->mCurrentState=newState;
  }
  return 0;
}

//returns locally cached state
int ControlObject::getState(std::string &currentState) {
  currentState=dPtr->mCurrentState;
  return 0;
}
