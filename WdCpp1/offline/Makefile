INC_DIR:= ./include/
SRC_DIR:= ./src/
EXE_DIR:= ./bin/
SRCS:=$(wildcard $(SRC_DIR)*.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
LIBS:=-lpthread

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:= $(EXE_DIR)test.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
