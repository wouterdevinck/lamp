PROJ    = lamp
OSC     = 16000000UL
LFUSE   = 0xdf
DEVICE  = atmega328
PROGDEV = m328
PROGVAR = usbasp

G++     = avr-g++
SIZE    = avr-size
OBJCOPY = avr-objcopy
PROG    = avrdude

SRCDIR  = src/mcu-basic/
COREDIR = src/mcu/core/
STLDIR  = src/mcu-basic/stl/
OUTDIR  = build/mcu-basic/

C++FLAGS = -Os
C++FLAGS += -Wall
C++FLAGS += -std=c++11
C++FLAGS += -DF_CPU=$(OSC)
C++FLAGS += -DBASIC
C++FLAGS += -mmcu=$(DEVICE)
C++FLAGS += -I$(STLDIR)
C++FLAGS += -I$(COREDIR)
C++FLAGS += -fno-exceptions

OBJECTS = \
	$(patsubst $(SRCDIR)%.cpp,$(OUTDIR)%.o,$(wildcard $(SRCDIR)*.cpp)) \
	$(patsubst $(COREDIR)%.cpp,$(OUTDIR)%.o,$(wildcard $(COREDIR)*.cpp))

.PHONY: all
all: $(OUTDIR)$(PROJ).hex

$(OUTDIR)%.hex: $(OUTDIR)%.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(OUTDIR)%.elf: $(OBJECTS)
	$(G++) $(C++FLAGS) $(OBJECTS) --output $@ 
	$(SIZE) --format=avr --mcu=$(DEVICE) $@

$(OUTDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p $(OUTDIR)
	$(G++) $< $(C++FLAGS) -c -o $@

$(OUTDIR)%.o: $(COREDIR)%.cpp
	mkdir -p $(OUTDIR)
	$(G++) $< $(C++FLAGS) -c -o $@

.PHONY: clean
clean:
	rm -rf $(OUTDIR)

.PHONY: list
list:
	echo $(OBJECTS)

.PHONY: flash
flash:
	$(PROG) -p $(PROGDEV) -c $(PROGVAR) -B 5 -U flash:w:$(OUTDIR)$(PROJ).hex

.PHONY: fuses
fuses:
	$(PROG) -p $(PROGDEV) -c $(PROGVAR) -B 5 -U lfuse:w:$(LFUSE):m
