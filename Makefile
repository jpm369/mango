CXX = g++
MSTOOLKIT = MSToolkit
HARDKLOR = hardklor
override CXXFLAGS +=  -O3 -Wall -Wextra -static -Wno-char-subscripts -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D__LINUX__ -I$(MSTOOLKIT)/include
EXECNAME = mango.exe
OBJS = mango.o  mango_Process.o mango_ProcessManager.o mango_Interfaces.o
DEPS = mango.h Common.h mango_Data.h mango_DataInternal.h mango_ProcessManager.h mango_Interfaces.h

LIBS = -L$(MSTOOLKIT) -lmstoolkitlite -lm -L/usr/local/lib 
ifdef MSYSTEM
   LIBS += -lws2_32
endif



mango.exe: $(OBJS)
#	git submodule init; git submodule update
	cd $(MSTOOLKIT) ; make lite 
	cd $(HARDKLOR) ; make; 
	${CXX} $(CXXFLAGS) $(OBJS) $(LIBS) -o ${EXECNAME}

mango.o: mango.cpp $(DEPS)
#	git submodule init; git submodule update
	${CXX} ${CXXFLAGS} mango.cpp -c

mango_Process.o: mango_Process.cpp Common.h mango_Process.h mango.h Common.h mango_Data.h mango_DataInternal.h
#	git submodule init; git submodule update
	${CXX} ${CXXFLAGS} mango_Process.cpp -c

mango_ProcessManager.o:  mango_ProcessManager.cpp Common.h mango_Data.h mango_DataInternal.h mango_Process.h mango_ProcessManager.h mango_Interfaces.h
#	git submodule init; git submodule update
	${CXX} ${CXXFLAGS} mango_ProcessManager.cpp -c

mango_Interfaces.o:  mango_Interfaces.cpp Common.h mango_Data.h mango_DataInternal.h mango_Process.h mango_ProcessManager.h mango_Interfaces.h
	${CXX} ${CXXFLAGS} mango_Interfaces.cpp -c

clean:
	rm -f *.o ${EXECNAME}
	cd $(MSTOOLKIT) ; make clean
