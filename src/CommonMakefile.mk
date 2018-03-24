## Hossein Moein
## March 17 2018

LOCAL_LIB_DIR = ../lib/$(BUILD_PLATFORM)
LOCAL_BIN_DIR = ../bin/$(BUILD_PLATFORM)
LOCAL_OBJ_DIR = ../obj/$(BUILD_PLATFORM)
LOCAL_INCLUDE_DIR = ../include
PROJECT_LIB_DIR = ../../../lib/$(BUILD_PLATFORM)
PROJECT_INCLUDE_DIR = ../../include

# -----------------------------------------------------------------------------

SRCS = BinaryItem.cc \
       ContainerItem.cc \
       DataItem.cc \
       DateItem.cc \
       DateTime.cc \
       DoubleItem.cc \
       FloatItem.cc \
       IntItem.cc \
       LongLongItem.cc \
       NullItem.cc \
       ReferenceItem.cc \
       ShortItem.cc \
       StopWatch.cc \
       StringItem.cc \
       SystemItem.cc \
       UCharItem.cc

HEADERS = $(LOCAL_INCLUDE_DIR)/BinaryItem.h \
          $(LOCAL_INCLUDE_DIR)/ContainerItem.h \
          $(LOCAL_INCLUDE_DIR)/DataItem.h \
          $(LOCAL_INCLUDE_DIR)/DateItem.h \
          $(LOCAL_INCLUDE_DIR)/DateTime.h \
          $(LOCAL_INCLUDE_DIR)/DoubleItem.h \
          $(LOCAL_INCLUDE_DIR)/FloatItem.h \
          $(LOCAL_INCLUDE_DIR)/IntItem.h \
          $(LOCAL_INCLUDE_DIR)/LongLongItem.h \
          $(LOCAL_INCLUDE_DIR)/NullItem.h \
          $(LOCAL_INCLUDE_DIR)/ReferenceItem.h \
          $(LOCAL_INCLUDE_DIR)/ShortItem.h \
          $(LOCAL_INCLUDE_DIR)/StopWatch.h \
          $(LOCAL_INCLUDE_DIR)/StringItem.h \
          $(LOCAL_INCLUDE_DIR)/SystemItem.h \
          $(LOCAL_INCLUDE_DIR)/UCharItem.h

LIB_NAME = DataItem
TARGET_LIB = $(LOCAL_LIB_DIR)/lib$(LIB_NAME).a

TARGETS = $(TARGET_LIB) \
          $(LOCAL_BIN_DIR)/conitm_tester \
          $(LOCAL_BIN_DIR)/sysitm_tester

# -----------------------------------------------------------------------------

LFLAGS += -Bstatic -L$(LOCAL_LIB_DIR) -L$(PROJECT_LIB_DIR)

LIBS = $(LFLAGS) -l$(LIB_NAME) $(PLATFORM_LIBS)
INCLUDES += -I. -I$(LOCAL_INCLUDE_DIR) -I$(PROJECT_INCLUDE_DIR)
DEFINES = -D_REENTRANT -DDMS_INCLUDE_SOURCE \
          -DP_THREADS -D_POSIX_PTHREAD_SEMANTICS -DDMS_$(BUILD_DEFINE)__

# -----------------------------------------------------------------------------

# object file
#
LIB_OBJS = $(LOCAL_OBJ_DIR)/BinaryItem.o \
           $(LOCAL_OBJ_DIR)/ContainerItem.o \
           $(LOCAL_OBJ_DIR)/DataItem.o \
           $(LOCAL_OBJ_DIR)/DateItem.o \
           $(LOCAL_OBJ_DIR)/DateTime.o \
           $(LOCAL_OBJ_DIR)/DoubleItem.o \
           $(LOCAL_OBJ_DIR)/FloatItem.o \
           $(LOCAL_OBJ_DIR)/IntItem.o \
           $(LOCAL_OBJ_DIR)/LongLongItem.o \
           $(LOCAL_OBJ_DIR)/NullItem.o \
           $(LOCAL_OBJ_DIR)/ReferenceItem.o \
           $(LOCAL_OBJ_DIR)/ShortItem.o \
           $(LOCAL_OBJ_DIR)/StopWatch.o \
           $(LOCAL_OBJ_DIR)/StringItem.o \
           $(LOCAL_OBJ_DIR)/SystemItem.o \
           $(LOCAL_OBJ_DIR)/UCharItem.o

# -----------------------------------------------------------------------------

# set up C++ suffixes and relationship between .cc and .o files
#
.SUFFIXES: .cc

$(LOCAL_OBJ_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.cc :
	$(CXX) $(CXXFLAGS) $< -o $@ -lm $(TLIB) -lg++

# -----------------------------------------------------------------------------

all: PRE_BUILD $(TARGETS)

PRE_BUILD:
	mkdir -p $(LOCAL_LIB_DIR)
	mkdir -p $(LOCAL_BIN_DIR)
	mkdir -p $(LOCAL_OBJ_DIR)
	mkdir -p $(PROJECT_LIB_DIR)
	mkdir -p $(PROJECT_INCLUDE_DIR)

$(TARGET_LIB): $(LIB_OBJS)
	ar -clrs $(TARGET_LIB) $(LIB_OBJS)

CONITM_TESTER_OBJ = $(LOCAL_OBJ_DIR)/conitm_tester.o
$(LOCAL_BIN_DIR)/conitm_tester: $(TARGET_LIB) $(CONITM_TESTER_OBJ) $(HEADERS)
	$(CXX) -o $@ $(CONITM_TESTER_OBJ) $(LIBS)

SYSITM_TESTER_OBJ = $(LOCAL_OBJ_DIR)/sysitm_tester.o
$(LOCAL_BIN_DIR)/sysitm_tester: $(TARGET_LIB) $(SYSITM_TESTER_OBJ) $(HEADERS)
	$(CXX) -o $@ $(SYSITM_TESTER_OBJ) $(LIBS)

# -----------------------------------------------------------------------------

depend:
	makedepend -f $(MAKEFILE_NAME) $(CXXFLAGS) -Y $(SRCS)

clobber:
	rm -f $(LIB_OBJS) $(TARGETS) $(CONITM_TESTER_OBJ) $(SYSITM_TESTER_OBJ)

install_lib:
	cp -pf $(TARGET_LIB) $(PROJECT_LIB_DIR)/.

install_hdr:
	cp -pf $(HEADERS) $(PROJECT_INCLUDE_DIR)/.

# -----------------------------------------------------------------------------

## Local Variables:
## mode:Makefile
## tab-width:4
## End:
