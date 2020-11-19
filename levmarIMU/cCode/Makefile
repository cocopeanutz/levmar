# tool macros
CC := gcc
CCFLAG := # FILL: compile flags
CCOBJFLAG := $(CCFLAG) -c

# path macros
BIN_PATH := $(CURDIR)/build/bin
OBJ_PATH := $(CURDIR)/build/obj
SRC_PATH := $(CURDIR)/src

# compile macros
TARGET_NAME := imuLevmar
LIB_TARGET_NAME := accLevmar
TARGET := $(BIN_PATH)/$(TARGET_NAME)
MAIN_SRC := # FILL: src file contains `main()`

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) $(CCFLAG) -o $@ $?

$(STATIC_LIB_TARGET): $(OBJ)
	$(CC) $(CCFLAG) -o $@ $?

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAG) -o $@ $<

# phony rules
.PHONY: all
all: $(TARGET) $(STATIC_LIB_TARGET)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)
