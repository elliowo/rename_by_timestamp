PREFIX ?= /usr/local
LOCAL_BIN = ./bin
TARGET = rename_by_timestamp
SOURCE = src/rename_by_timestamp.c
BINARY = $(LOCAL_BIN)/$(TARGET)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2

all: $(BINARY)

$(BINARY): $(SOURCE)
	@mkdir -p $(LOCAL_BIN)
	@echo "Building $@"
	$(CC) $(CFLAGS) -o $@ $<

install: $(BINARY)
	@echo "Installing $@"
	install -m 755 $< $(PREFIX)/bin

uninstall:
	@if [ -f "$(PREFIX)/bin/$(TARGET)" ]; then \
		echo "Uninstalling $(TARGET)"; \
		rm -f "$(PREFIX)/bin/$(TARGET)"; \
	fi

clean:
	@rm -f $(BINARY)
	@echo "  ➜ Cleaned"

distclean: clean
	@rm -f *.o
