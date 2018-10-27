PROJ = lamp
DEVICE = atmega328

SRCDIR = src/mcu-basic/
OUTDIR = build/mcu-basic/

.PHONY: all
all: $(OUTDIR)$(PROJ).hex

$(OUTDIR)%.o: $(SRCDIR)%.c
	mkdir -p $(OUTDIR)
	avr-gcc -g -Os -mmcu=$(DEVICE) -o $@ -c $<

$(OUTDIR)%.elf: $(OUTDIR)%.o
	avr-gcc -g -mmcu=$(DEVICE) -o $@ $<
	avr-size --format=avr --mcu=$(DEVICE) $@

$(OUTDIR)%.hex: $(OUTDIR)%.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: clean
clean:
	rm -rf $(OUTDIR)