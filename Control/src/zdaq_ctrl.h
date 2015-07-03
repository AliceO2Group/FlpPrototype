/* 
 * File:   zdaq_ctrl.h
 * Author: chapelan
 *
 * Created on July 25, 2013, 12:29 PM
 */

#ifndef ZDAQ_CTRL_H
#define	ZDAQ_CTRL_H

#include <stdint.h>
#include <zookeeper.h>
#include <vector>

#ifdef	__cplusplus
extern "C" {
#endif

    

// Zdaq control version 2 bytes: MAJOR+MINOR
#define ZDAQ_CTRL_VERSION 0xA101
// History of versions:
//        0xA101     v1.1    07/2013

// The structure to publish service information
// (how to reach, etc)
// Should be backward compatible when same MAJOR version.
//
typedef struct {
    uint16_t version;   // struct version number            
    char name[128];     // service name
    char host[128];     // host IP name    
    int  port;          // host TCP port
    char uuid[16];      // unique service id
} t_zdaqCtrl_srvinfo;




/* not used
class zdaqCtrl_service {
public:
    zdaqCtrl_service(const char *serviceName, const char* DNS);      // serviceName: name of the service to register. DNS: info to access to service directory (host/port).
    ~zdaqCtrl_service();
    
private:
    t_zdaqCtrl_srvinfo info;
        
    zhandle_t *zh;
    clientid_t z_id;
    static void z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);
    int z_shutdown;
    int z_ok;
};
*/


class zdaqCtrl_client {
public:
    zdaqCtrl_client(const char *objectName, const char* DNS);      // serviceName: name of the service to register. DNS: info to access to service directory (host/port).
    ~zdaqCtrl_client();
    
    const char* getState();     // retrieves the remote object state
    int execCommand(const char *command);       // send a command to remote object
    int getData(const char *key, char *value, int *valueSz);  // retrieve data published in object. valueSz initially set to size of 'value' buffer given, returned with actual data size and buffer set.
    
private:
       
    zhandle_t *zh;
    clientid_t z_id;
    static void z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);
    int z_shutdown;
    int z_ok;
    
    char state[128];
    char objname[128];
    
    char znode_state[128];    
    char znode_command[128];    
    
    static void z_watcher_state (zhandle_t *zzh, int type, int state, const char *path, void* context);

};

class zdaqCtrl_object {
public:
    zdaqCtrl_object(const char *objectName, const char* DNS);      // objectName: name of the service to register. DNS: info to access to service directory (host/port).
    ~zdaqCtrl_object();    
    
    int publishString(const char *key, const char *value);  // publish a value
    
private:       
    zhandle_t *zh;
    clientid_t z_id;
    static void z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);
    int z_shutdown;
    int z_ok;
    
    char state[128];
    char objname[128];
    
    char znode_state[128];
    char znode_command[128];
    char znode_data[128];
    
    int cmd_purge;
    static void z_watcher_cmd (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);
    
    int m_debug;
    
protected:    
    int setState(const char *newState);
    virtual int executeCommand(const char *command)=0;
    const char*getName();
    const char*getState();        
};

class testStateObj: public zdaqCtrl_object {
public:
    testStateObj(const char *objectName, const char* DNS);
protected:    
    int executeCommand(const char *command);
private:
    int ixcmd;
};



class zdaqCtrl_group;

/* A class to control an item in a group
 */
class zdaqCtrl_item {
public:
    zdaqCtrl_item(const char* name, zdaqCtrl_group *group);    // create an item associated to given object name in given group
    ~zdaqCtrl_item();
    int execCommand(const char *command);       // send a command to the object
    int debug;  // flag for debug messages
private:
     const zdaqCtrl_group *Ctrl_group;        // group associated to the object
     
    char state[128];    // current (known) state of the object
    char objname[128];  // object name (logical name)
    
    char znode_state[128];    // path to znode for object state in zookeeper
    char znode_command[128];  // path to znode for object command queue in zookeeper   
    
    friend class zdaqCtrl_group;
};

/* A class to control a list of objects
 */
class zdaqCtrl_group {
public:
    zdaqCtrl_group(const char* DNS);      // DNS: info to access to service directory (host/port).
    ~zdaqCtrl_group();

    int execCommand(const char *command);       // send a command to all items in group
    
    int addObject(const char *objectName);        // add a new object to group
    
    int debug;  // flag for debug messages
    private:
        int addItem(zdaqCtrl_item *i);     // add an item to the group
    
    std::vector<zdaqCtrl_item*> items;           // internal list of the items part of this group
    
    zhandle_t *zh;
    clientid_t z_id;
    static void z_watcher (zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);            // zookeeper callback for general connection handling
    static void z_watcher_item (zhandle_t *zzh, int type, int state, const char *path, void* watcherCtx);         // zookeeper callback for items (states, commands) in the group

    int z_shutdown;
    int z_ok;
    
    friend class zdaqCtrl_item;
};






#include <sys/time.h>

class myTimer {
  public:
    myTimer();
    ~myTimer();

    double getTime();
    int start();
    int stop();
    int reset();
    
    void test();
    
  private:
    double totalTime;
    struct timeval tv;
    int isRunning;
};





/*class to communicate with an object: get state and send command */

#ifdef	__cplusplus
}
#endif

#endif	/* ZDAQ_CTRL_H */

