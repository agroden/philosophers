EXEC = philosophers
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS=-Wall -std=c++11
LIBS = -lpthread

all: $(EXEC)

debug: CXXFLAGS+=-DDEBUG -g
debug: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) -o $(EXEC) $(OBJS) $(LIBS) $(CXXFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(LIBS) $< -o $@

clean:
	$(RM) $(EXEC) $(OBJS)
