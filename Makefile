############ Tools #########################

CC = sdcc
AS = sdas8051
LD = sdld
HEX_TOOL = packihx
BIN_TOOL = makebin

############# Project Settings ##############

OUTPUT = Race_stop_watch

ASM_SRCS +=

C_SRCS += main.c

INCLUDE_PATHS +=

########## Compiler/Linker Flags ############

ASMFLAGS += -l -o

CFLAGS += -mmcs51 -c 

LDFLAGS +=

#############################################

OBJ_DIR = build

C_FILES = $(notdir $(C_SRCS))
C_OBJS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.rel))
C_PATHS = $(sort $(dir $(C_SRCS)))
vpath %.c $(C_PATHS)

ASM_OBJS = $(ASM_SRCS:.asm=.rel)
ASM_PATHS = $(sort $(dir $(ASM_SRCS)))
vpath %.asm $(ASM_PATHS)

OBJS = $(C_OBJS) $(ASM_OBJS) 

IHX = $(OBJ_DIR)/$(OUTPUT).ihx

HEX = $(OUTPUT).hex
BIN = $(OUTPUT).bin

############## Make targets #################

all: $(HEX) $(IHX) $(BIN)

$(BIN) : $(HEX)
	@echo [ BIN ] $@
	@$(BIN_TOOL) -p "$<" "$@"

$(HEX) : $(IHX)
	@echo [ HEX ] $@
	@$(HEX_TOOL) "$<" > "$@" 

$(OBJ_DIR):
	@echo [ MKDIR ] $@
	@mkdir "$@"

$(IHX): $(OBJ_DIR) $(OBJS)
	@echo [ LD ] $@
	@$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.rel: %.c
	@echo [ CC ] $< 
	@$(CC) $(CFLAGS) $(addprefix -I,$(INCLUDEPATHS)) -o "$@" "$<"

%.rel: %.asm
	@echo [ AS ] $< 
	@$(AS) $(ASMFLAGS) $<

clean:
	rm -rf $(OBJ_DIR) $(HEX) $(BIN) $(ASM_OBJS) *.rel *.lst *.rst


