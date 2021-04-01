PLATFORM_RELFLAGS += -fno-strict-aliasing  -fno-common -ffixed-r8 \
	 -msoft-float
PLATFORM_CPPFLAGS += -march=armv4t -mtune=arm926ej-s
PLATFORM_RELFLAGS +=$(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,))
