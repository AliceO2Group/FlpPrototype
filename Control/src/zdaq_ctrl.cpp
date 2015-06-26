#include "zdaq_ctrl.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <boost/uuid/uuid.hpp>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iostream>



static const char* state2String(int state){
  if (state == 0)
    return "CLOSED_STATE";
  if (state == ZOO_CONNECTING_STATE)
    return "CONNECTING_STATE";
  if (state == ZOO_ASSOCIATING_STATE)
    return "ASSOCIATING_STATE";
  if (state == ZOO_CONNECTED_STATE)
    return "CONNECTED_STATE";
  if (state == ZOO_EXPIRED_SESSION_STATE)
    return "EXPIRED_SESSION_STATE";
  if (state == ZOO_AUTH_FAILED_STATE)
    return "AUTH_FAILED_STATE";

  return "INVALID_STATE";
}

static const char* type2String(int state){
  if (state == ZOO_CREATED_EVENT)
    return "CREATED_EVENT";
  if (state == ZOO_DELETED_EVENT)
    return "DELETED_EVENT";
  if (state == ZOO_CHANGED_EVENT)
    return "CHANGED_EVENT";
  if (state == ZOO_CHILD_EVENT)
    return "CHILD_EVENT";
  if (state == ZOO_SESSION_EVENT)
    return "SESSION_EVENT";
  if (state == ZOO_NOTWATCHING_EVENT)
    return "NOTWATCHING_EVENT";

  return "UNKNOWN_EVENT_TYPE";
}

/*
void watcher(zhandle_t *zzh, int type, int state, const char *path,
             void* context)
{

    h->z_watcher (zzh, type, state, path);
}
*/




zdaqCtrl_service::zdaqCtrl_service(const char* serviceName, const char* DNS) {
    if (serviceName==NULL) {throw ("serviceName is NULL");}
    if (strlen(serviceName)>sizeof(info.name)-1) {throw ("serviceName too long");}
    if (DNS==NULL) {throw("DNS is NULL");}
    
    info.version=ZDAQ_CTRL_VERSION;
    
    strncpy(info.name,serviceName,sizeof(info.name));
    
    info.host[0]=0;
    info.host[sizeof(info.host)-1]=0;
    if (gethostname(info.host,sizeof(info.host))) {throw("gethostname() failed");}
    if (info.host[sizeof(info.host)-1]!=0) {throw("gethostname() truncated");}    
   
    info.port=0;
    
    bzero(info.uuid,sizeof(info.uuid));
    
    boost::uuids::uuid u = boost::uuids::random_generator()();
    std::vector<unsigned char> v(u.size());
    std::copy(u.begin(), u.end(), v.begin());
    int i;
    for (i=0;(i<v.size())&&(i<sizeof(info.uuid));i++) {
        info.uuid[i]=v[i];
    }
    //  const std::string tmp = boost::lexical_cast<std::string>(u);
    //  const char * value = tmp.c_str();
    //  printf("uuid=%s",value);
    
    z_shutdown=0;
    z_ok=0;
    bzero(&z_id,sizeof(z_id));
    //zh = zookeeper_init(DNS,  watcher, 30000, &myid, this, 0);
    //return;
    printf("this=%p\n",(void *)this);
    zh = zookeeper_init(DNS,  zdaqCtrl_service::z_watcher, 30000, &z_id, this, 0);
    if (!zh) {throw("zookeeper_init() failed");}
    
      for (int i=0;i<5;i++) {
        sleep(1);
        if (z_ok) {break;}
    }
     
    if (!z_ok) {throw("zookeeper init timeout");}
    int err;
    err=zoo_create( zh,info.name,(const char *)&info,sizeof(info),&ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,0,0);
    printf("zoo_create()=%d %s\n",err,zerror(err));
    if (err!=ZOK) {        
        //	      throw("zoo_acreate() failed");
    }
}

zdaqCtrl_service::~zdaqCtrl_service() {
    if (zh!=0) {
        zookeeper_close(zh);
    }
}


void zdaqCtrl_service::z_watcher (zhandle_t *zzh, int type, int state, const char *path, void* context){
    /* Be careful using zh here rather than zzh - as this may be mt code
     * the client lib may call the watcher before zookeeper_init returns */

    /*
     * fprintf(stderr, "Watcher %s state = %s", type2String(type), state2String(state));
    if (path && strlen(path) > 0) {
      fprintf(stderr, " for path %s", path);
    }
    fprintf(stderr, "\n");
     */
     
        zdaqCtrl_service *h;
    h=(zdaqCtrl_service *)context;
    if (h==NULL) {return;}
    //printf("z_watcher(%p)\n",(void *)h);

    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            const clientid_t *id = zoo_client_id(zzh);
            if (h->z_id.client_id == 0 || h->z_id.client_id != id->client_id) {
                h->z_id = *id;
                //fprintf(stderr, "Got a new session id: 0x%llx\n", (long long)h->z_id.client_id);            
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
}






zdaqCtrl_client::zdaqCtrl_client(const char* objectName, const char* DNS) {
   if (objectName==NULL) {throw ("objectName is NULL");}
    if (DNS==NULL) {throw ("DNS");}
        
    int l;
    l=strlen(objectName);
    if (l>=sizeof(this->objname)) {
        l=sizeof(this->objname)-1;
    }
    strncpy(this->objname,objectName,l);
    this->objname[l]=0;
    if (DNS==NULL) {throw("DNS is NULL");}
    
    zh=0;
    z_shutdown=0;
    z_ok=0;
    bzero(&z_id,sizeof(z_id));

    zh = zookeeper_init(DNS,  zdaqCtrl_client::z_watcher, 30000, &z_id, this, 0);
    if (!zh) {throw("zookeeper_init() failed");}    
      for (int i=0;i<5000;i++) {
        usleep(1000);
        if (z_ok) {break;}
    } 
    if (!z_ok) {throw("zookeeper init timeout");}
    
    snprintf(znode_state,sizeof(znode_state),"%s/state",objname);
    snprintf(znode_command,sizeof(znode_command),"%s/command",objname);
    
    snprintf(this->state,sizeof(this->state),"UNKNOWN");
    
    int err;
    l=sizeof(this->state)-1;
    err=zoo_wget(zh, znode_state, zdaqCtrl_client::z_watcher_state, this, this->state, &l, NULL);
    if (err) {
        printf("Failed to get %s : %s",znode_state,zerror(err));
    } else {
        this->state[l]=0;
    }
    printf("client: %s = %s\n",this->znode_state,this->state);
}

zdaqCtrl_client::~zdaqCtrl_client() {
    if (zh!=0) {
        zookeeper_close(zh);
    }
}

int zdaqCtrl_client::execCommand(const char* command) {
    // add command to queue
    char zn[128];
    snprintf(zn,sizeof(zn),"%s/cmd-",this->znode_command);
    
    zoo_create( zh,zn,command,strlen(command)+1,&ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL | ZOO_SEQUENCE,0,0);    
    return 0;
}



void zdaqCtrl_client::z_watcher (zhandle_t *zzh, int type, int state, const char *path, void* context){
    /* Be careful using zh here rather than zzh - as this may be mt code
     * the client lib may call the watcher before zookeeper_init returns */

    /*
     * fprintf(stderr, "Watcher %s state = %s", type2String(type), state2String(state));
    if (path && strlen(path) > 0) {
      fprintf(stderr, " for path %s", path);
    }
    fprintf(stderr, "\n");
*/
    
        zdaqCtrl_client *h;
    h=(zdaqCtrl_client *)context;
    if (h==NULL) {return;}
    //printf("z_watcher(%p)\n",(void *)h);

    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            const clientid_t *id = zoo_client_id(zzh);
            if (h->z_id.client_id == 0 || h->z_id.client_id != id->client_id) {
                h->z_id = *id;
                fprintf(stderr, "Got a new session id: 0x%llx\n",
                        (long long)h->z_id.client_id);            
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
}



void zdaqCtrl_client::z_watcher_state (zhandle_t *zzh, int type, int state, const char *path, void* context){
    int err;
  
    
    zdaqCtrl_client *h;
    h=(zdaqCtrl_client *)context;
    if (h==NULL) {return;}
    
    //fprintf(stderr, "Watcher %s : %s state = %s\n", h->znode_state, type2String(type), state2String(state));
    
    if (type==ZOO_DELETED_EVENT) {
        snprintf(h->state,sizeof(h->state),"UNKOWN");
    }
    
    int l;
    l=sizeof(h->state)-1;
    err=zoo_wget(h->zh, h->znode_state, zdaqCtrl_client::z_watcher_state, h, h->state, &l, NULL);
    if (err) {
        printf("Failed to get %s : %s\n",h->znode_state,zerror(err));
    } else {
        h->state[l]=0;
    }
    printf("client: %s = %s\n",h->znode_state,h->state);
    fflush(stdout);
}




zdaqCtrl_object::zdaqCtrl_object(const char *objectName, const char* DNS){

    if (objectName==NULL) {throw ("objectName is NULL");}
    if (DNS==NULL) {throw ("DNS");}
        
    int l;
    l=strlen(objectName);
    if (l>=sizeof(this->objname)) {
        l=sizeof(this->objname)-1;
    }
    strncpy(this->objname,objectName,l);
    this->objname[l]=0;
    if (DNS==NULL) {throw("DNS is NULL");}
    

    m_debug=0;

    zh=0;
    z_shutdown=0;
    z_ok=0;
    bzero(&z_id,sizeof(z_id));

    setState("UNDEFINED");
    
    zh = zookeeper_init(DNS,  zdaqCtrl_object::z_watcher, 30000, &z_id, this, 0);
    if (!zh) {throw("zookeeper_init() failed");}    
      for (int i=0;i<500;i++) {
        usleep(10000);
        if (z_ok) {break;}
    }
     
    
    t_zdaqCtrl_srvinfo info;
    info.version=ZDAQ_CTRL_VERSION;    
    strncpy(info.name,objname,sizeof(info.name));    
    info.host[0]=0;
    info.host[sizeof(info.host)-1]=0;
    if (gethostname(info.host,sizeof(info.host))) {throw("gethostname() failed");}
    if (info.host[sizeof(info.host)-1]!=0) {throw("gethostname() truncated");}
    info.port=0;
    bzero(info.uuid,sizeof(info.uuid));    
    boost::uuids::uuid u = boost::uuids::random_generator()();
    std::vector<unsigned char> v(u.size());
    std::copy(u.begin(), u.end(), v.begin());
    int i;
    for (i=0;(i<v.size())&&(i<sizeof(info.uuid));i++) {
        info.uuid[i]=v[i];
    }

    
    if (!z_ok) {throw("zookeeper init timeout");}
    int err;
    zoo_create( zh,objname,0,0,&ZOO_OPEN_ACL_UNSAFE, 0,0,0);
    // check if aname exists / create
    
    snprintf(znode_state,sizeof(znode_state),"%s/state",objname);
    snprintf(znode_command,sizeof(znode_command),"%s/command",objname);
    snprintf(znode_data,sizeof(znode_data),"%s/data",objname);
    
    zoo_create( zh,znode_state,0,0,&ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,0,0);
    zoo_create( zh,znode_command,0,0,&ZOO_OPEN_ACL_UNSAFE, 0,0,0);
    zoo_create( zh,znode_data,0,0,&ZOO_OPEN_ACL_UNSAFE, 0,0,0);
    
    
    String_vector s;     
    cmd_purge=1;
    if (0==zoo_wget_children(zh, znode_command,  zdaqCtrl_object::z_watcher_cmd, this, &s)){
        // cleanup pending commands
        for (int i=0;i<s.count;i++) {
              char zn[128];
              snprintf(zn,sizeof(zn),"%s/%s",znode_command,s.data[i]);
              if (m_debug) {
                  printf("%s purging cmd %d = %s\n",objname,i,zn);
              }
              err=zoo_delete(zh,zn,-1);
              //printf("err=%d %s",err,zerror(err));
        }
    }
    cmd_purge=0;
}

zdaqCtrl_object::~zdaqCtrl_object() {
    setState("DEAD");
    if (zh!=0) {
        zookeeper_close(zh);
        zh=0;
    }
}

int zdaqCtrl_object::setState(const char *newState) {
    int l;
    int err=0;
    if (newState==NULL) {
        setState("UNDEFINED");
        return -1;
    }
    l=strlen(newState);
    if (l>=sizeof(this->state)) {
        err=1;
        l=sizeof(this->state)-1;
    }
    strncpy(this->state,newState,l);
    this->state[l]=0;
    
    if (m_debug) {printf("Object %s: newstate = %s\n",objname,newState);}
    if (zh!=0) {
        err=zoo_set(zh, znode_state, this->state, l, -1);
        if (err) {
            printf("Failed to publish %s : %s\n",znode_state,zerror(err));
        }
    }
    return err;
}

void zdaqCtrl_object::z_watcher (zhandle_t *zzh, int type, int state, const char *path, void* context){
    /* Be careful using zh here rather than zzh - as this may be mt code
     * the client lib may call the watcher before zookeeper_init returns */

    /*
        printf("Object %s: newstate = %s\n",objname,newState);}
        fprintf(stderr, "Watcher %s state = %s", type2String(type), state2String(state));

        if (path && strlen(path) > 0) {
            fprintf(stderr, " for path %s", path);            
        }
        fprintf(stderr, "\n");
    */

    zdaqCtrl_object *h;
    h=(zdaqCtrl_object *)context;
    if (h==NULL) {return;}
    if (h->m_debug) {
        printf("z_watcher(%p)\n",(void *)h);
    }

    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            const clientid_t *id = zoo_client_id(zzh);
            if (h->z_id.client_id == 0 || h->z_id.client_id != id->client_id) {
                h->z_id = *id;
                if (h->m_debug) {
                    fprintf(stderr, "Got a new session id: 0x%llx\n", (long long)h->z_id.client_id);            
                }
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
}


void zdaqCtrl_object::z_watcher_cmd (zhandle_t *zzh, int type, int state, const char *path, void* context){
    zdaqCtrl_object *h;
    h=(zdaqCtrl_object *)context;
    if (h==NULL) {return;}
    
    //    printf("WatcherCMD %s: %s %s\n",h->objname, type2String(type), state2String(state));
    

    String_vector s;
    
    while (h->cmd_purge) {
        usleep(100);
    }
    
    if (type==ZOO_CHILD_EVENT) {
      if (0==zoo_wget_children(zzh, h->znode_command,  zdaqCtrl_object::z_watcher_cmd, h,
        &s)){
          // printf("WatcherCMD: %d pending actions\n",s.count);
          
          // todo: reorder queue
          for (int i=0;i<s.count;i++) {
              // printf("WatcherCMD : found cmd %d = %s\n",i,s.data[i]);

              int err;
              char zn[128];
              snprintf(zn,sizeof(zn),"%s/%s",h->znode_command,s.data[i]);
              char val[128];
              int sz=sizeof(val);
              err=zoo_get(zzh, zn, 0, val, &sz, NULL);
              printf("%s command=%s (%d bytes)\n",h->getName(),val,sz);
              h->executeCommand(val);
              if (err) {
                     printf("Failed to get cmd : %s\n",zerror(err));
              }
              zoo_delete(zzh,zn,-1);
          }      
      }
    }

}



testStateObj::testStateObj(const char *objectName, const char* DNS): zdaqCtrl_object(objectName,DNS) {
    ixcmd=0;
    setState("READY");    
};

int testStateObj::executeCommand(const char *command) {
    ixcmd++;
    char newState[128];
    snprintf(newState,sizeof(newState),"state%04d",ixcmd);
    printf("Object %s: command=%s newstate = %s\n",getName(),command,newState);
    setState(newState);
    return 0;
}

const char* zdaqCtrl_object::getName(){
    return this->objname;
}

const char* zdaqCtrl_object::getState(){
    return this->state;
}



int zdaqCtrl_object::publishString(const char *key, const char *value){
    if (key==NULL) {return -1;}
    if (value==NULL) {value="";}
    printf("publish\n");
    char path[256];
    snprintf(path,sizeof(path),"%s/%s",this->znode_data,key);
    int l;
    l=strlen(value)+1;
    
    int err=0;
    if (zoo_exists(zh, path, 0, NULL)==ZOK) {
        err=zoo_set(zh, path, value,l, -1);
    } else {
        err=zoo_create( zh,path,value,l,&ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,0,0);
    }
    printf("Create %s : %s\n",path,zerror(err));
    
    return err;
}

int zdaqCtrl_client::getData(const char *key, char *value, int *valueSz) {
    if (key==NULL) {return -1;}
    if (value==NULL) {return -1;}
    if (valueSz==NULL) {return -1;}
    if (*valueSz<=0) { return -1;}
    
    char path[256];
    snprintf(path,sizeof(path),"%s/data/%s",this->objname,key);
        
    int err;
    err=zoo_get(zh, path, 0, value, valueSz, NULL);
    if (err==ZOK) {return 0;}
    return -1;
}




zdaqCtrl_item::zdaqCtrl_item(const char* objectName, zdaqCtrl_group *group){
    debug=0;
    if (group==NULL) {
        throw __LINE__;
    }
    this->Ctrl_group=group;
    
    if (objectName==NULL) {throw ("objectName is NULL");}
    int l;
    l=strlen(objectName);
    if (l>=sizeof(this->objname)) {
        l=sizeof(this->objname)-1;
    }
    strncpy(this->objname,objectName,l);
    this->objname[l]=0;

    snprintf(znode_state,sizeof(znode_state),"%s/state",objname);
    snprintf(znode_command,sizeof(znode_command),"%s/command",objname);  
    snprintf(this->state,sizeof(this->state),"UNKNOWN");      
    
    group->addItem(this);
}
zdaqCtrl_item::~zdaqCtrl_item() {
}

int zdaqCtrl_item::execCommand(const char *command) {
    if (command==NULL) {
        return -1;
    }
    
    // add command to queue
    char zn[128];
    snprintf(zn,sizeof(zn),"%s/cmd-",this->znode_command);
    
    zoo_create( this->Ctrl_group->zh,zn,command,strlen(command)+1,&ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL | ZOO_SEQUENCE,0,0);    

    return 0;
};


zdaqCtrl_group::zdaqCtrl_group(const char* DNS){
    if (DNS==NULL) {throw("DNS is NULL");}
    debug=0;
    zh=0;
    z_shutdown=0;
    z_ok=0;
    bzero(&z_id,sizeof(z_id));
zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
    zh = zookeeper_init(DNS,  zdaqCtrl_group::z_watcher, 30000, &z_id, this, 0);
    if (!zh) {throw("zookeeper_init() failed");}    
      for (int i=0;i<5000;i++) {
        usleep(1000);
        if (z_ok) {break;}
    } 
    if (!z_ok) {throw("zookeeper init timeout");}
}

zdaqCtrl_group::~zdaqCtrl_group(){
  if (zh!=0) {
    zookeeper_close(zh);
    // zookeper_close() waits pending callbacks (if any) to complete before returning. (feature tested 19/08/2013)
    // at this stage, we are sure no zdaqCtrl_group::z_watcher callback is being / will be executed, the object can be deleted safely.
  }
  for (int i=0;i<items.size();i++) {
      delete items[i];
  }
}
int zdaqCtrl_group::execCommand(const char *command) {
    int i;
    for (i=0;i<items.size();i++){
        items[i]->execCommand(command);
    }
    return 0;
}
void zdaqCtrl_group::z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx){
    /* add lock to prevent delete while in there */
    zdaqCtrl_group *h;
    h=(zdaqCtrl_group *)watcherCtx;
    if (h==NULL) {return;}
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            const clientid_t *id = zoo_client_id(zzh);
            if (h->z_id.client_id == 0 || h->z_id.client_id != id->client_id) {
                h->z_id = *id;
                //fprintf(stderr, "Got a new session id: 0x%llx\n",(long long)h->z_id.client_id);            
            }
            h->z_ok=1;
        } else if (state == ZOO_AUTH_FAILED_STATE) {
            //fprintf(stderr, "Authentication failure\n");
            h->z_ok=0;
        } else if (state == ZOO_EXPIRED_SESSION_STATE) {
            //fprintf(stderr, "Session expired\n");
            h->z_ok=0;
        } else {
            h->z_ok=0;
        }
    } else {
        printf("unhandled ZOO type %d\n",type);    
    }     
}
int zdaqCtrl_group::addItem(zdaqCtrl_item *i) {
    this->items.push_back(i);
    int ix;
    ix=this->items.size();
    
    z_watcher_item(this->zh, ZOO_CHANGED_EVENT, ZOO_CONNECTED_STATE, i->znode_state, i);  // initiate state update callback
    
    return ix;
}


void zdaqCtrl_group::z_watcher_item (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx){
    int err;
  
    zdaqCtrl_item *i;
    i=(zdaqCtrl_item *)watcherCtx;
    if (i==NULL) {return;}
    
    //fprintf(stderr, "Watcher %s : %s state = %s\n", path, type2String(type), state2String(state));
    
    if (type==ZOO_DELETED_EVENT) {
        snprintf(i->state,sizeof(i->state),"UNKOWN");
    }
    if (type==ZOO_CREATED_EVENT) {
        snprintf(i->state,sizeof(i->state),"UNKOWN");
    }

    int l=0;
    l=sizeof(i->state)-1;
    err=zoo_wget(zzh, i->znode_state, zdaqCtrl_group::z_watcher_item, i, i->state, &l, NULL);
    if (err) {
        if (err==ZNONODE) {
            // set watch for when objects comes online
             zoo_wexists(zzh, i->znode_state, zdaqCtrl_group::z_watcher_item, i, NULL);
        }
        //printf("Failed to get %s : %s\n",i->znode_state,zerror(err));
    } else {
        i->state[l]=0;
    }
    if (l==0) {
      snprintf(i->state,sizeof(i->state),"UNKOWN");
    }
    if (i->debug) {
        printf("State update: %s = %s\n",i->znode_state,i->state);
    }
    fflush(stdout);
}

int zdaqCtrl_group::addObject(const char *objectName) {
    zdaqCtrl_item *i;
    i=new zdaqCtrl_item(objectName,this);
    if (i==NULL) {return -1;}    
    return 0;
}





/* class to measure time */
using namespace std;

myTimer::myTimer() {
  totalTime=0;
  isRunning=0;
}
myTimer::~myTimer() {
}
int myTimer::start() {
  if (isRunning) {return -1;}
  gettimeofday(&tv,NULL);  
  isRunning=1;
  return 0;
}
int myTimer::stop() {
  if (!isRunning) {return -1;}
  struct timeval tv2;
  gettimeofday(&tv2,NULL);
  totalTime+=(tv2.tv_sec-tv.tv_sec)*1.0+(tv2.tv_usec-tv.tv_usec)/1000000.0;
  isRunning=0;
  return 0;
}
double myTimer::getTime() {
  return totalTime;
}
int myTimer::reset() {
  if (isRunning) {
    gettimeofday(&tv,NULL);
  } else {
    totalTime=0;
  }
  return 0;
}
void myTimer::test() {
  int ni=10000000;
  cout << "Testing myTimer()" << endl;
  if (isRunning) isRunning=0;
  this->reset();
  myTimer tt;
  tt.start();
  for (int i=0;i<ni;i++) {
    this->start();
    this->stop();
  }
  tt.stop();
  cout << "myTimer start/stop cycle (selfstat) = " << 2*this->getTime()/ni << " seconds" << endl;  
  cout << "myTimer start/stop cycle (measured) = " << tt.getTime()/ni << " seconds" << endl;  
}
