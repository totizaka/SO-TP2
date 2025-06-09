include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin

SOURCES_C=$(wildcard [^_]*.c) $(wildcard tests/*.c) $(wildcard shell/*.c) $(wildcard modules/*.c) $(wildcard lib/*.c)
SOURCES_ASM=$(wildcard [^_]*.asm) $(wildcard lib/*.asm)

OBJDIR=obj
OBJECTS_C=$(addprefix $(OBJDIR)/, $(SOURCES_C:.c=.o))
OBJECTS_ASM=$(addprefix $(OBJDIR)/, $(SOURCES_ASM:.asm=.o))

all: $(OBJDIR) $(MODULE)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(MODULE): $(OBJDIR)/_loader.o $(OBJECTS_C) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -I../../Shared -T sampleCodeModule.ld $(OBJDIR)/_loader.o $(OBJECTS_C) $(OBJECTS_ASM) -o ../$(MODULE)
	$(LD) -o ../0000-sampleCodeModule.elf --oformat=elf64-x86-64 -T sampleCodeModule.ld $(OBJDIR)/_loader.o $(OBJECTS_C) $(OBJECTS_ASM)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(GCC) $(GCCFLAGS) -I./include -I../../Shared -c $< -o $@

$(OBJDIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) ../$(MODULE) ../0000-sampleCodeModule.elf

.PHONY: all clean print


En shell:
