# UMoria portable Makefile (no sudo, user-agnostic)
# -------------------------------------------------
# Usage:
#   make
#   make install              # installs to ~/.local (or override BINDIR/LIBDIR)
#   make run
#   make uninstall
#   make help / make doctor

# ---- Config (override on command line if you want) ----
BINARY   ?= moria
CC       ?= gcc
CFLAGS   ?= -O2 -Wall -std=gnu89 -Wno-implicit-function-declaration -Wno-int-conversion
LDLIBS   ?= -lncurses
# Locate the bundled data directory robustly (repo root or parent)
FILES_DIR ?= $(firstword $(wildcard files ../files))

# Install locations (user-agnostic, no sudo)
PREFIX   ?= $(HOME)/.local
BINDIR   ?= $(PREFIX)/bin

# Prefer XDG if present, else ~/.local/share/moria
ifeq ($(XDG_DATA_HOME),)
  LIBDIR ?= $(HOME)/.local/share/moria
else
  LIBDIR ?= $(XDG_DATA_HOME)/moria
endif

# Inject the runtime data dir at compile time and override legacy macros
CPPFLAGS ?= -DLIBDIR=\"$(LIBDIR)\" -include local_paths.h

# ---- Sources / Objects ----
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

# ---- Phony targets ----
.PHONY: all clean install uninstall run help doctor

# ---- Build ----
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# ---- Install (no sudo, no chown, no set[ug]id) ----
# Idempotent and robust: makes dirs writable during copy, then sets final perms.
install: $(BINARY)
	@echo "Installing $(BINARY) -> $(BINDIR)"
	@echo "Installing data      -> $(LIBDIR)"
	# 1) Create dirs with sane perms
	mkdir -p "$(BINDIR)" "$(LIBDIR)"
	chmod 755 "$(BINDIR)" "$(LIBDIR)"
	# 2) Install binary, ensure no setuid/setgid bits
	install -m 0755 "$(BINARY)" "$(BINDIR)/$(BINARY)"
	chmod a-s "$(BINDIR)/$(BINARY)" || true
	# 3) Copy bundled data if present (works from repo root or unix/)
	if [ -n "$(FILES_DIR)" ] && [ -d "$(FILES_DIR)" ]; then \
  	chmod u+w "$(LIBDIR)"; \
  	cp -af "$(FILES_DIR)"/. "$(LIBDIR)/"; \
	fi
	# 4) Ensure runtime files exist (scores is writable, others readonly)
	: >"$(LIBDIR)/scores"
	[ -e "$(LIBDIR)/hours" ] || : >"$(LIBDIR)/hours"
	[ -e "$(LIBDIR)/news"  ] || : >"$(LIBDIR)/news"
	chmod 644 "$(LIBDIR)/scores"
	chmod 444 "$(LIBDIR)/hours" "$(LIBDIR)/news" 2>/dev/null || true
	# 5) Static help/docs read-only if present
	chmod 444 $(LIBDIR)/*.hlp $(LIBDIR)/COPYING 2>/dev/null || true
	@echo "OK."

# ---- Uninstall everything installed by this Makefile ----
uninstall:
	@echo "Removing $(BINDIR)/$(BINARY) (if present)"
	rm -f "$(BINDIR)/$(BINARY)"
	@echo "Removing data dir $(LIBDIR)"
	rm -rf "$(LIBDIR)"
	@echo "Uninstall complete."

# ---- Run the installed game ----
run:
	@if [ -x "$(BINDIR)/$(BINARY)" ]; then \
	  exec "$(BINDIR)/$(BINARY)"; \
	else \
	  echo "Binary not found at $(BINDIR)/$(BINARY). Run 'make install' first."; \
	  exit 1; \
	fi

# ---- Cleanup ----
clean:
	rm -f $(OBJS) $(BINARY)

# ---- Help ----
help:
	@echo "UMoria Makefile targets:"
	@echo "  make           - Build the game binary"
	@echo "  make install   - Install locally to \$$BINDIR (default: \$$HOME/.local/bin)"
	@echo "                   and copy game data to \$$LIBDIR (default: XDG_DATA_HOME/moria or \$$HOME/.local/share/moria)"
	@echo "  make run       - Run the installed game"
	@echo "  make uninstall - Remove the installed binary and data"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make doctor    - Quick env/path/data check"
	@echo ""
	@echo "Variables you can override:"
	@echo "  BINDIR=/custom/bin    LIBDIR=/custom/data/moria   CFLAGS=...   LDLIBS=..."

# ---- Quick sanity checks (paths, libs, data files) ----
doctor:
	@echo "== Paths =="; \
	  command -v $(BINARY) >/dev/null 2>&1 && which $(BINARY) || echo "(not on PATH)"; \
	  echo "BINDIR=$(BINDIR)"; echo "LIBDIR=$(LIBDIR)"; \
	 echo "== Binary =="; \
	  ls -l "$(BINDIR)/$(BINARY)" 2>/dev/null || echo "No binary in BINDIR"; \
	 echo "== Linked curses =="; \
	  ldd "$(BINDIR)/$(BINARY)" 2>/dev/null | grep -E 'curses|not found' || true; \
	 echo "== Data dir =="; \
	  ls -l "$(LIBDIR)" 2>/dev/null || echo "No data dir"; \
	 echo "== SGID/SUID on binary (should be off) =="; \
	  ls -l "$(BINDIR)/$(BINARY)" 2>/dev/null | sed -n '1p' || true

l_clean:
	rm -r *.o
	rm -f moria

l_gitclean:
	mv unix.c DONOTDELETE
	rm -r *.c
	rm -r *.h
	mv DONOTDELETE unix.c
