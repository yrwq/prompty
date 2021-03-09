PROJECT = prompty
CFLAGS := $(shell pkg-config --cflags lua5.1 libgit2)
LDFLAGS := $(shell pkg-config --libs lua5.1 libgit2)
OBJECTS = src/script.o src/func.o src/main.o

CONF = "/home/${USER}/.config/prompty"
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	gcc $^ $(LDFLAGS) -o $@

config: $(PROJECT)
	mkdir -p $(CONF)
	cp init.lua $(CONF)/init.lua

install: $(PROJECT)
	mkdir -p $(BINDIR)
	install -Dm755 prompty $(BINDIR)

clean:
	rm -f $(PROJECT) $(OBJECTS)
