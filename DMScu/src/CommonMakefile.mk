## Hossein Moein
## July 17 2009

LOCAL_LIB_DIR = ../lib/$(BUILD_PLATFORM)
LOCAL_BIN_DIR = ../bin/$(BUILD_PLATFORM)
LOCAL_OBJ_DIR = ../obj/$(BUILD_PLATFORM)
LOCAL_INCLUDE_DIR = ../include
PROJECT_LIB_DIR = ../../../lib/$(BUILD_PLATFORM)
PROJECT_INCLUDE_DIR = ../../../include

# -----------------------------------------------------------------------------

SRCS =

HEADERS = $(LOCAL_INCLUDE_DIR)/DMScu_FixedSizeString.h \
          $(LOCAL_INCLUDE_DIR)/DMScu_PtrVector.h \
          $(LOCAL_INCLUDE_DIR)/DMScu_PtrVector.tcc

LIB_NAME =
TARGET_LIB =

TARGETS += $(LOCAL_BIN_DIR)/fixsizestr_tester \
           $(LOCAL_BIN_DIR)/ptrvec_tester

# -----------------------------------------------------------------------------

LFLAGS += -Bstatic -L$(LOCAL_LIB_DIR) -L$(PROJECT_LIB_DIR)

LIBS = $(LFLAGS) $(PLATFORM_LIBS)
INCLUDES += -I. -I$(LOCAL_INCLUDE_DIR) -I$(PROJECT_INCLUDE_DIR)
DEFINES = -D_REENTRANT -DDMS_INCLUDE_SOURCE \
          -DP_THREADS -D_POSIX_PTHREAD_SEMANTICS -DDMS_$(BUILD_DEFINE)__

# -----------------------------------------------------------------------------

# object file
#
LIB_OBJS =

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

FIXSIZESTR_TESTER_OBJ = $(LOCAL_OBJ_DIR)/fixsizestr_tester.o
$(LOCAL_BIN_DIR)/fixsizestr_tester: $(TARGET_LIB) $(FIXSIZESTR_TESTER_OBJ)
	$(CXX) -o $@ $(FIXSIZESTR_TESTER_OBJ) $(LIBS)

PTRVEC_TESTER_OBJ = $(LOCAL_OBJ_DIR)/ptrvec_tester.o
$(LOCAL_BIN_DIR)/ptrvec_tester: $(TARGET_LIB) $(PTRVEC_TESTER_OBJ)
	$(CXX) -o $@ $(PTRVEC_TESTER_OBJ) $(LIBS)

# -----------------------------------------------------------------------------

depend:
	makedepend $(CXXFLAGS) -Y $(SRCS)

clean:
	rm -f $(LIB_OBJS)

clobber:
	rm -f $(TARGETS) $(FIXSIZESTR_TESTER_OBJ) $(PTRVEC_TESTER_OBJ)

install_lib:

install_hdr:
	cp -pf $(HEADERS) $(PROJECT_INCLUDE_DIR)/.

# -----------------------------------------------------------------------------

## Local Variables:
## mode:Makefile
## tab-width:4
## End:
