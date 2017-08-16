DIR_INC=./include
DIR_SRC=./src
DIR_BIN=./bin
DIR_CMT=./cmt

CC=g++
TARGET=$(DIR_BIN)/test
SRC = $(wildcard $(DIR_SRC)/*.cxx)
OBJ = $(patsubst %.cxx,$(DIR_CMT)/%.o,$(notdir $(SRC)))

$(TARGET):$(OBJ)
	$(CC)  -o  $@ -L /opt/cactus/lib -lpthread -lboost_filesystem -lboost_regex -lboost_system -lboost_thread -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal $^
$(DIR_CMT)/%.o : $(DIR_SRC)/%.cxx
	$(CC) -g -c $^ -o $@ -I /opt/cactus/include  -I /home/lijin/ipbus/ipbus_suite4/include
.PHONY : clean
clean:
	rm $(TARGET) $(OBJ)
