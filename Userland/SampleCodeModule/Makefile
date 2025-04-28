include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES_C=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard [^_]*.asm)
OBJDIR=obj
OBJECTS_C=$(addprefix $(OBJDIR)/, $(SOURCES_C:.c=.o))
OBJECTS_ASM=$(addprefix $(OBJDIR)/, $(SOURCES_ASM:.asm=.o))

all: $(OBJDIR) $(MODULE)

# Crear el directorio obj si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Regla para crear el binario final a partir de archivos C y ASM
$(MODULE): $(OBJECTS_C) $(OBJECTS_ASM) $(OBJDIR)/_loader.o
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld $(OBJDIR)/_loader.o $(OBJECTS_C) $(OBJECTS_ASM) -o ../$(MODULE)
	$(LD) -o ../0000-sampleCodeModule.elf --oformat=elf64-x86-64 -T sampleCodeModule.ld $(OBJDIR)/_loader.o $(OBJECTS_C) $(OBJECTS_ASM)


# Regla para compilar archivos .c a .o
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

# Regla para compilar archivos .asm a .o
$(OBJDIR)/%.o: %.asm | $(OBJDIR)
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)

.PHONY: all clean print


En shell:
