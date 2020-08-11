PROJ = lamp

SRCDIR = src/fpga/
SIMDIR = test/fpga/
OUTDIR = build/fpga/

.PHONY: all
all: $(OUTDIR)$(PROJ).bin

$(OUTDIR)%.json: $(SRCDIR)%.v
	mkdir -p $(OUTDIR)
	yosys -v 3 -p 'synth_ice40 -top $(PROJ) -json $@ -dsp' $<

$(OUTDIR)%.asc: $(SRCDIR)%.pcf $(OUTDIR)%.json
	nextpnr-ice40 --up5k --package sg48 --json $(word 2,$^) --pcf $< --asc $@ --freq 2

$(OUTDIR)%.bin: $(OUTDIR)%.asc
	icepack $< $@

$(OUTDIR)%.vvp: $(SIMDIR)%_tb.v $(SRCDIR)*.v
	mkdir -p $(OUTDIR)
	iverilog -I $(SRCDIR) -o $@ $<

$(OUTDIR)%.vcd: $(OUTDIR)%.vvp
	vvp $< -n

$(OUTDIR)$(PROJ).hash: $(SRCDIR)*.v  $(SRCDIR)*.pcf
	cat $^ | sha256sum | head -c 64 > $@

.PHONY: lint
lint: 
	verilator --lint-only -I$(SRCDIR) --top-module $(PROJ) $(SRCDIR)$(PROJ).v

.PHONY: simulate
simulate: $(OUTDIR)$(PROJ).vcd

.PHONY: hash
hash: $(OUTDIR)$(PROJ).hash

.PHONY: clean
clean:
	rm -rf $(OUTDIR)

.PHONY: show-simulation
show-simulation: $(OUTDIR)$(PROJ).vcd
	gtkwave $<

.PHONY: flash
flash: $(OUTDIR)$(PROJ).bin
	iceprogduino $<