# -----------------------------------------------------------------------------
# 
#                              M A K E F I L E
# 
# zum Uebungsbetriebssystem OOStuBS der Lehrveranstaltung 'Betriebssystembau'.
# -----------------------------------------------------------------------------
#
# Durch Eingabe von 'make' werden die Quelltexte des Systems kompiliert. 'make
# bootdisk' schreibt das System-Image auf einen USB-Stick, von dem das System
# dann gebootet werden kann. Mit 'make clean' wird im Verzeichnis aufgeraeumt.
# Die Targets 'qemu', 'qemu-gdb', 'gdb', 'ddd' starten den QEMU-Emulator mit
# bzw. ohne GDB-Stub und starten den GDB- bzw. den DDD-Debugger, um sich dann
# mit dem GDB-Stub zu verbinden. Auf Poolrechnern koennen mit 'vorgabe2' bis
# 'vorgabe7' die Vorgaben fuer eine neue Aufgabe in das aktuelle
# Arbeitsverzeichnis eingepflegt werden.
#
# -----------------------------------------------------------------------------

MAKEFLAGS += --no-builtin-rules

.SUFFIXES:

# -----------------------------------------------------------------------------
# Liste der Quelltexte:

SRC_DIR = src
BIN_DIR = bin
DEP_DIR = dep
ISO_DIR = iso
LIB_DIR = lib
INITRD_DIR = initrd
INCLUDE_DIRS = -I $(LIB_DIR)/initrd/lib

STARTUP_SOURCE = startup.s
CC_SOURCES = $(shell find $(SRC_DIR) -name "*.cc")
C_SOURCES = $(shell find $(SRC_DIR) -name "*.c")
ASM_SOURCES = $(shell find $(SRC_DIR) -name "*.s")

DRIVE_HD ?= /dev/sdb

# Einstellungen in Abhaengigkeit vom Generierungssystem:
#
#    ASMOBJFORMAT: Ausgabeformat fuer den Assembler. Das Format muss dem
#                  jeweiligen Format des verwendeten C++ Compilers angepasst
#                  werde, damit gemischte Objektdateien gelinkt werden koennen.
#    BIN_DIR:      Verzeichnis, in dem die Objektdateien abgelegt werden
#                  sollen.
#    DEP_DIR:       Verzeichnis, in dem die Abhaengigkeitsdateien abgelegt werden
#                  sollen
#    DRIVE_HD:     Festplatten-/USB-Device, auf das das System-Image
#                  geschrieben wird (Target: bootdisk)
#    DELETE:       Name des Kommandos zum Loeschen von Dateien
#    ASM:          Zu benutzender Assembler
#    CC/CXX:       Zu benutzender C/C++-Compiler
#    CFLAGS:       Flags fuer den C-Compileraufruf
#    CXXFLAGS:     Flags fuer den C++-Compileraufruf
#    QEMU:         Architekturspezifischer Name des QEMU-Programms
#    MKRESCUE:     Name des GRUB mkrescue-Programms

VERBOSE = @
ASMOBJFORMAT = elf64
# ACHTUNG: ein falsch angegebenes Laufwerk kann dazu fuehren, dass Daten auf dem
# spezifizierten Laufwerk verloren gehen! Nicht mit root-Rechten ausfuehren!
#DRIVE_HD = /dev/sdz
DELETE = rm
ASM = nasm
MKINITRD = $(LIB_DIR)/initrd/mkinitrd
QEMU ?= qemu-system-x86_64
CC ?= gcc
CXX ?= g++
USER_MACROS ?=
CFLAGS := $(CFLAGS) -O2 -m64 -mno-red-zone -mno-sse -Wall -Werror -Wno-write-strings -fno-stack-protector -nostdlib -I $(SRC_DIR) $(INCLUDE_DIRS) -g $(USER_MACROS) #-DDEBUG
CXXFLAGS := $(CFLAGS) -Wall -Wno-non-virtual-dtor -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti -fno-exceptions

ifneq ($(shell which grub-mkrescue 2> /dev/null),)
MKRESCUE = grub-mkrescue
endif
ifneq ($(shell which grub2-mkrescue 2> /dev/null),)
MKRESCUE = grub2-mkrescue
endif

# -------------------------------------------------------------------------
# Namen der Unterverzeichnisse mit den Quelltexten

VPATH = $(sort $(dir $(STARTUP_SOURCE) $(CC_SOURCES) $(C_SOURCES) $(ASM_SOURCES)))

# -------------------------------------------------------------------------
# Listen mit den Objektdateien, die beim Kompilieren entstehen:

FIRST_OBJECT = $(addprefix $(BIN_DIR)/,$(patsubst %.s,%.s.o, $(STARTUP_SOURCE)))
C_OBJECTS = $(addsuffix .o,$(C_SOURCES:$(SRC_DIR)/%=%))
CC_OBJECTS = $(addsuffix .o,$(CC_SOURCES:$(SRC_DIR)/%=%))

DEP_FILES = $(patsubst %.o,$(DEP_DIR)/%.d,$(C_OBJECTS))
DEP_FILES += $(patsubst %.o,$(DEP_DIR)/%.d,$(CC_OBJECTS))

ASM_OBJECTS = $(patsubst %.s,%.s.o, $(ASM_SOURCES))
OBJPRE = $(addprefix $(BIN_DIR)/,$(ASM_OBJECTS) $(C_OBJECTS) $(CC_OBJECTS))

# --------------------------------------------------------------------------
# Default target: Das Image fuer USB-Sticks und QEMU

all: bootdisk.iso
	cp bootdisk.iso ~/shared/
	cp $(ISO_DIR)/boot/system ~/shared/

$(MKINITRD): $(shell find $(LIB_DIR)/initrd)
	$(MAKE) -C $(LIB_DIR)/initrd/

$(BIN_DIR)/initrd.bin: $(MKINITRD) $(shell find $(INITRD_DIR))
	cd $(INITRD_DIR) && find . -type f | xargs ../$(MKINITRD) -v7 ../$(BIN_DIR)/initrd.bin

# --------------------------------------------------------------------------
# Regeln zur Erzeugung der Abhaengigkeitsdateien

$(DEP_DIR)/%.c.d : %.c
	@echo "DEP		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CC) $(CFLAGS) -MM -MT $(BIN_DIR)/$*.c.o -MF $@ $<

$(DEP_DIR)/%.cc.d : %.cc
	@echo "DEP		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) $(CXXFLAGS) -MM -MT $(BIN_DIR)/$*.cc.o -MF $@ $<

# --------------------------------------------------------------------------
# Regeln zur Erzeugung der Objektdateien

$(BIN_DIR)/%.c.o : %.c
	@echo "CC		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CC) -c $(CFLAGS) -o $@ $<

$(BIN_DIR)/%.cc.o : %.cc
	@echo "CXX		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) -S $(CXXFLAGS) -o $@.asm $<
	$(VERBOSE) $(CXX) -c $(CXXFLAGS) -o $@ $<

$(BIN_DIR)/%.s.o : %.s
	@echo "ASM		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(ASM) -f $(ASMOBJFORMAT) -i ./$(SRC_DIR)/ -o $@ $<

# --------------------------------------------------------------------------
# Gelinktes System

objects: $(OBJPRE)

$(BIN_DIR)/system: $(FIRST_OBJECT) $(OBJPRE)
	@echo "LD		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) $(CXXFLAGS) -static -z max-page-size=0x1000 -e startup -T sections -o $(BIN_DIR)/system $(FIRST_OBJECT) $(OBJPRE)

# --------------------------------------------------------------------------
# 'bootdisk.iso' besteht aus dem System und einem Bootloader (GRUB) mit
# Bootblock und Setup-Code. Das System wird per Multiboot gestartet, so dass
# es sich nicht um den Wechsel in den Protected Mode oder den Setup aus dem
# BIOS heraus kuemmern muss.

bootdisk.iso : $(BIN_DIR)/system $(BIN_DIR)/initrd.bin
	@echo "BUILD		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) cp $(BIN_DIR)/system $(ISO_DIR)/boot/system
	$(VERBOSE) cp $(BIN_DIR)/initrd.bin $(ISO_DIR)/boot/initrd.bin
	$(VERBOSE) $(MKRESCUE) -o $@ $(ISO_DIR) > $(BIN_DIR)/mk_log.txt

# --------------------------------------------------------------------------
# 'clean' loescht das generierte System, die Objektdateien und die
# Abhaengigkeitsdateien

clean:	
	@echo "RM		$(BIN_DIR)"
	$(VERBOSE) rm -rf $(BIN_DIR)
	@echo "RM		$(DEP_DIR)"
	$(VERBOSE) rm -rf $(DEP_DIR)
	@echo "RM       bootdisk.iso"
	$(VERBOSE) rm -f bootdisk.iso
	$(MAKE) -C $(LIB_DIR)/initrd clean

# --------------------------------------------------------------------------
# 'bootdisk' erzeugt zunaechst das System, falls das noch nicht geschehen ist.
# Danach wird das System auf das spezifizierte Laufwerk geschrieben, welches
# sowohl eine Festplatte als auch ein USB-Stick sein kann.
# ACHTUNG: ein falsch angegebenes Laufwerk kann dazu fuehren, dass Daten auf dem
# spezifizierten Laufwerk verloren gehen! Nicht mit root-Rechten ausfuehren!

bootdisk: bootdisk.iso
	@echo "CP		$<"
	$(VERBOSE) cp $< $(DRIVE_HD)

# --------------------------------------------------------------------------
# 'qemu' ruft den qemu-Emulator mit dem System auf.

qemu: bootdisk.iso
	$(QEMU) -drive file=bootdisk.iso,format=raw -k en-us -soundhw all

# --------------------------------------------------------------------------
# 'qemu-smp' ruft den qemu-Emulator mit SMP-Support und 2 CPUs mit dem System auf.

qemu-smp: bootdisk.iso
	$(QEMU) -drive file=bootdisk.iso,format=raw -k en-us -smp 2 -soundhw all

# --------------------------------------------------------------------------
# 'qemu-gdb' ruft den qemu-Emulator mit aktiviertem GDB-Stub mit dem System
# auf, sodass es per GDB oder DDD inspiziert werden kann.

qemu-gdb: bootdisk.iso
	$(VERBOSE) echo "target remote localhost:$(shell echo $$(( $$(id -u) + 1023 )))" > /tmp/gdbcommands.$(shell id -u)
	$(QEMU) -drive file=bootdisk.iso,format=raw -k en-us -soundhw all -gdb tcp::$(shell echo $$(( $$(id -u) + 1023 )))

# --------------------------------------------------------------------------
# 'gdb' startet den GDB-Debugger und verbindet sich mit dem GDB-Stub des vorher
# gestarteten QEMU.
# Achtung: gdb unterstuetzt derzeit (2017) keine Systeme, die zur Laufzeit
# vom Protected Mode in den Long Mode wechseln. Der gdb darf daher erst
# gestartet werden, wenn das Betriebssystem in qemu bereits erfolgreich in den
# Long Mode gewechselt ist -- ein Start mit `qemu -S` ist nicht moeglich.

gdb:
	gdb -x /tmp/gdbcommands.$(shell id -u) $(BIN_DIR)/system

# --------------------------------------------------------------------------
# 'ddd' startet den DDD-Debugger und verbindet sich mit dem GDB-Stub des vorher
# gestarteten QEMU.

ddd:
	ddd --gdb -x /tmp/gdbcommands.$(shell id -u) $(BIN_DIR)/system

# --------------------------------------------------------------------------
# Einbindung der Abhaengigkeitsdateien

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP_FILES)
endif

.PHONY: all clean bootdisk gdb ddd qemu qemu-smp qemu-gdb objects

