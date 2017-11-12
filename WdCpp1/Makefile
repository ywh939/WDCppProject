INC_DIR:= ./inc/ ./inc/reactor/ ./inc/threadpool/ ./inc/timer/ 
SRC_DIR:= ./src/ ./src/reactor/ ./src/threadpool/ ./src/timer/
EXE_DIR:= ./bin/
SRCS:=$(wildcard ./src/*.cc) $(wildcard ./src/reactor/*.cc) $(wildcard ./src/threadpool/*.cc) $(wildcard ./src/timer/*.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
LIBS:=-lpthread

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:= $(EXE_DIR)server

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
