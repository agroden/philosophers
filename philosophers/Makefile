EXEC = philosophers
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -Wall -std=c++11
LIBS = -lpthread
BUILD_DIR = release

all: $(EXEC)

debug: CXXFLAGS += -DDEBUG -g
debug: BUILD_DIR = debug
debug: $(EXEC)

$(EXEC): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) -o $(BUILD_DIR)/$(EXEC) $(OBJS) $(LIBS) $(CXXFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(LIBS) $< -o $@

clean:
	$(RM) -rf debug release $(OBJS)
