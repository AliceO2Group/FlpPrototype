SUBDIRS := $(patsubst %/.,%,$(wildcard */.))
CONFIGSUBDIRS = $(SUBDIRS:%=config-%)
INSTALLSUBDIRS = $(SUBDIRS:%=install-%)
CLEANSUBDIRS = $(SUBDIRS:%=clean-%)
CLOBBERSUBDIRS = $(SUBDIRS:%=clobber-%)

.PHONY: all clean package install test subdirs $(SUBDIRS) $(CONFIGSUBDIRS) $(INSTALLSUBDIRS) $(CLEANSUBDIRS) $(CLOBBERSUBDIRS)

default: all

all: configure build

build: configure $(SUBDIRS)

configure: $(CONFIGSUBDIRS)

install: $(INSTALLSUBDIRS)

clean: $(CLEANSUBDIRS)

clobber: $(CLOBBERSUBDIRS)

test: 
	@echo $(SUBDIRS)
	@echo $(CONFIGSUBDIRS)

$(SUBDIRS): 
	@echo Build $@
	@cd $@/build ; $(MAKE)

$(CONFIGSUBDIRS): 
	@echo Configure $(@:config-%=%)
	@cd $(@:config-%=%) ; mkdir -p build ; cd build ; cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug ..

$(INSTALLSUBDIRS): 
	@echo Install $(@:install-%=%)
	@cd $(@:install-%=%)/build ; make install

$(CLEANSUBDIRS):
	@echo Clean $(@:clean-%=%)
	@cd $(@:clean-%=%)/build ; make clean

$(CLOBBERSUBDIRS):
	@echo Delete build directory in $(@:clobber-%=%)
	@cd $(@:clobber-%=%) ; rm -rf build
