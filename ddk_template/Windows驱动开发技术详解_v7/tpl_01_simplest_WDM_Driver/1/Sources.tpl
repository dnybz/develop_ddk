TARGETNAME=HelloWDM
TARGETTYPE=DRIVER
DRIVERTYPE=WDM
TARGETPATH=OBJ

INCLUDES=$(BASEDIR)\inc;\
         $(BASEDIR)\inc\ddk;\

SOURCES=HelloWDM.cpp\