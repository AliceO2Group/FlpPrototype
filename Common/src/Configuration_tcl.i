// File : Configuration.i
// Wrapper interface for SWIG

%module Configuration

%include <std_string.i>

%{
#include <Common/Configuration.h>
#include <stdio.h>
%};

%typemap(in) (const std::string param) {
  int str_len;
  char *str_param;  
    
  str_param=Tcl_GetStringFromObj($input,&str_len);
  if (str_param==NULL) {return TCL_ERROR;}
  $1=std::string(str_param);
}



%include "exception.i"

%exception {
  try {
    $action
  } catch (const std::string& e) {
    SWIG_exception(SWIG_RuntimeError, e.c_str());
  }
}



class ConfigFile {
   public:
     ConfigFile();
     ~ConfigFile();
     void load(const std::string path);
     template <class T> T getValue(const std::string);
};

%template(getValue) ConfigFile::getValue<std::string>;

     

/*

load ./Configuration.so
set a [ConfigFile]
$a load "file:/tmp/configDummy.cfg"
$a getValue "readout.rate"
exit

swig -c++ -tcl8 Configuration.i
g++ -c Configuration_wrap.cxx -fPIC -I../include -std=c++14
g++ -c Configuration.cxx -fPIC -I../include -std=c++14
g++ -shared Configuration_wrap.o Configuration.o -o Configuration.so

load libtclConfiguration.so Configuration

*/
