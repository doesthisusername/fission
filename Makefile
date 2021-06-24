SRC := source
EXT := external
DAT := data
BLD := build
BIN := bin

ifeq ($(OS),Windows_NT)
else
	UNAME_S := $(shell uname -s)
	GLFW3 := $(shell pkg-config --libs glfw3)
	ifeq ($(UNAME_S),Darwin)
		LIBS := $(GLFW3) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm -lGLEW -L/usr/local/lib
	else
		LIBS := $(GLFW3) -lGL -lm -lGLU -lGLEW -ldl -lpthread
	endif
endif

CC := cc -flto
CFLAGS := -I$(SRC) -I$(EXT) -I$(BLD) -I. -Wall -O0 -g
LDFLAGS := -L$(BLD) \
	-llivesplit_core $(LIBS)

.PHONY: clean all

all: $(BIN)/fission

clean:
	rm -rfv $(BLD) $(BIN)

$(BIN):
	@mkdir -p $(BIN)

$(BLD):
	@mkdir -p $(BLD)

# yeah, this runs too often
$(BIN)/$(DAT): $(BIN)
	@echo "CP      "$(BIN)/$(DAT)
	@cp -r $(DAT) $(BIN)

$(BLD)/%.o: $(SRC)/%.c $(BLD)/livesplit_core.h $(BLD)
	@echo "CC      "$<
	@mkdir -p $(@D)
	@$(CC) -g -c -o $@ $< $(CFLAGS)

FISSION_O := $(patsubst $(SRC)/%.c,$(BLD)/$(notdir %.o),$(shell find $(SRC) -type f -name *.c))
$(BIN)/fission: $(FISSION_O) $(BLD)/liblivesplit_core.a $(BLD) $(BIN) $(BIN)/$(DAT)
	@echo "LD      "$@
	@$(CC) -g -o $@ $(FISSION_O) $(LDFLAGS)

$(BLD)/liblivesplit_core.a: $(BLD)
	@cd $(EXT)/livesplit-core; \
	cargo build --release -p staticlib; \
	cp target/release/liblivesplit_core.a ../../$(BLD)/liblivesplit_core.a; \
	cd -

$(BLD)/livesplit_core.h: $(BLD)/liblivesplit_core.a $(BLD)
	@cd $(EXT)/livesplit-core/capi/bind_gen; \
	cargo run; \
	cp ../bindings/livesplit_core.h ../../../../$(BLD)/livesplit_core.h; \
	cd -
