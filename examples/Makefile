# Makefile for GameLib examples
# Works on Windows (mingw32-make) and Linux / macOS
#
# Usage:
#   mingw32-make          # build all .cpp files in this directory
#   mingw32-make clean    # remove all generated executables
#
# On Linux/macOS, all targets use SDL2 backend (via sdl2-config).
# On Windows, files whose name contains "sdl" use SDL2 libs;
#   others use Win32 backend with zero external deps.
#   If SDL2 libs are missing, set CXXFLAGS/LDFLAGS to point to them.

CXX      := g++
CXXFLAGS := -std=c++11 -O2
SDL2_LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# --- Platform detection ---
UNAME_S := $(shell uname 2>/dev/null)

# Auto-discover all .cpp files in this directory
ALL_SRCS := $(wildcard *.cpp)

# Split: files with "sdl" in the name vs. the rest (findstring is more reliable than filter %sdl%)
SDL_SRCS := $(foreach f,$(ALL_SRCS),$(if $(findstring sdl,$(f))$(findstring SDL,$(f)),$(f)))
NOSDL_SRCS := $(foreach f,$(ALL_SRCS),$(if $(findstring sdl,$(f))$(findstring SDL,$(f)),,$(f)))

# Output extension
ifeq ($(OS),Windows_NT)
    SDL_EXES  := $(SDL_SRCS:%.cpp=%.exe)
    NOSDL_EXES := $(NOSDL_SRCS:%.cpp=%.exe)
else
    SDL_EXES  := $(SDL_SRCS:%.cpp=%)
    NOSDL_EXES := $(NOSDL_SRCS:%.cpp=%)
endif

ALL_EXES := $(NOSDL_EXES) $(SDL_EXES)

.PHONY: all clean

all: $(NOSDL_EXES) $(SDL_EXES)

ifeq ($(UNAME_S),Darwin)
# ===== macOS: all targets use SDL2 =====
$(NOSDL_EXES): %: %.cpp
	clang++ $(CXXFLAGS) $(shell sdl2-config --cflags) -o $@ $< $(LDFLAGS) $(SDL2_LIBS)

$(SDL_EXES): %: %.cpp
	clang++ $(CXXFLAGS) $(shell sdl2-config --cflags) -o $@ $< $(LDFLAGS) -mwindows $(SDL2_LIBS)

else ifeq ($(UNAME_S),Linux)
# ===== Linux: all targets use SDL2 =====
$(NOSDL_EXES): %: %.cpp
	$(CXX) $(CXXFLAGS) $(shell sdl2-config --cflags) -o $@ $< $(LDFLAGS) $(SDL2_LIBS)

$(SDL_EXES): %: %.cpp
	$(CXX) $(CXXFLAGS) $(shell sdl2-config --cflags) -o $@ $< $(LDFLAGS) -mwindows $(SDL2_LIBS)

else
# ===== Windows (mingw32-make) =====
# Non-SDL files: Win32 backend, zero deps
$(NOSDL_EXES): %.exe: %.cpp
	$(CXX) $(CXXFLAGS) -mwindows -static -o $@ $<

# SDL files: use SDL2 libs; .IGNORE prevents abort on failure
.IGNORE: $(SDL_EXES)
$(SDL_EXES): %.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) -mwindows $(SDL2_LIBS)
# Warn if SDL files exist but no external lib paths were provided
$(if $(SDL_SRCS),$(if $(LDFLAGS),,$(warning [SDL] If SDL targets fail to link, set CXXFLAGS/LDFLAGS to point to your SDL2 installation.)))
endif

clean:
	rm -f $(ALL_EXES)
