.PHONY: all debug release clean
.DEFAULT_GOAL = debug

OBJECTDIR = ./objects
OBJECTDIR_RELEASE = ./$(OBJECTDIR)/release
OBJECTDIR_DEBUG = ./$(OBJECTDIR)/debug


#COMPILER
CC=gcc

CFLAGS =\
	-c\
	-std=c99\
	-Wall\
	-Wextra\
	-Wpedantic\
	-fsanitize=undefined\
	-fsanitize-undefined-trap-on-error
CFLAGS_RELEASE = -DNDEBUG
CFLAGS_DEBUG = -g


#LINKER
LD=gcc

LDFLAGS =\
	-fsanitize=undefined\
	-fsanitize-undefined-trap-on-error
LDFLAGS_RELEASE = -Wl,-Map -Wl,release/$(TARGET_NAME).map
LDFLAGS_DEBUG = -Wl,-Map -Wl,debug/$(TARGET_NAME).map


OBJECTS_RELEASE = $(addprefix $(OBJECTDIR_RELEASE)/, $(SOURCES:.c=.o) )
OBJECTS_DEBUG = $(addprefix $(OBJECTDIR_DEBUG)/, $(SOURCES:.c=.o) )
DEPENDS_RELEASE = $(addprefix $(OBJECTDIR_RELEASE)/, $(SOURCES:.c=.d) )
DEPENDS_DEBUG = $(addprefix $(OBJECTDIR_DEBUG)/, $(SOURCES:.c=.d) )

-include $(DEPENDS_RELEASE)
-include $(DEPENDS_DEBUG)

$(OBJECTDIR_RELEASE)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(INCLUDES) $(CFLAGS) $(CFLAGS_RELEASE) $(DEFINES) -MMD -MP -o $@ -l $(@D) $<

$(OBJECTDIR_DEBUG)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(INCLUDES) $(CFLAGS) $(CFLAGS_DEBUG) $(DEFINES) -MMD -MP -o $@ -l $(@D) $<

release/$(TARGET_NAME).exe: $(OBJECTS_RELEASE)
	mkdir -p release
	$(LD) $(OBJECTS_RELEASE) $(LDFLAGS) $(LDFLAGS_RELEASE) -o $@

debug/$(TARGET_NAME).exe: $(OBJECTS_DEBUG)
	mkdir -p debug
	$(LD) $(OBJECTS_DEBUG) $(LDFLAGS) $(LDFLAGS_DEBUG) -o $@

debug: debug/$(TARGET_NAME).exe
release: release/$(TARGET_NAME).exe
all: clean debug

clean:
	rm -rfd $(OBJECTDIR)
	rm -rfd release
	rm -rfd debug
