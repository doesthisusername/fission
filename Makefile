SRC := source
EXT := external
BLD := build

ifeq ($(OS),Windows_NT)
else
	UNAME_S := $(shell uname -s)
	GLFW3 := $(shell pkg-config --libs glfw3)
	ifeq ($(UNAME_S),Darwin)
		LIBS := $(GLFW3) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm -lGLEW -L/usr/local/lib
	else
		LIBS := $(GLFW3) -lGL -lm -lGLU -lGLEW
	endif
endif

CC := cc -flto
CFLAGS := -I$(SRC) -I$(EXT) -I. -Wall -O3
LDFLAGS := -L$(BLD) \
	-llivesplit_core $(LIBS)

.PHONY: clean all

all: fission

clean:
	rm -rfv $(BLD) fission

$(BLD):
	@mkdir -p $(BLD)

$(BLD)/%.o: $(SRC)/%.c $(BLD)/livesplit_core.h $(BLD)
	@echo "CC      "$<
	@mkdir -p $(@D)
	@$(CC) -c -o $@ $< $(CFLAGS)

FISSION_O := $(patsubst $(SRC)/%.c,$(BLD)/$(notdir %.o),$(shell find $(SRC) -type f -name *.c))
fission: $(FISSION_O) $(BLD)/liblivesplit_core.a $(BLD)
	@echo "LD      "$@
	@$(CC) -o $@ $(FISSION_O) $(LDFLAGS)

$(BLD)/liblivesplit_core.a: $(BLD)
	@cd $(EXT)/livesplit-core; \
	cargo build --release -p staticlib; \
	cp target/release/liblivesplit_core.a ../../$(BLD)/liblivesplit_core.a; \
	cd -

$(BLD)/livesplit_core.h: $(BLD)/liblivesplit_core.a $(BLD)
	@cd $(EXT)/livesplit-core/capi/bind_gen; \
	cargo run; \
	cp ../bindings/livesplit_core.h ../../../../$(BLD)/liveplit_core.h; \
	cd -
