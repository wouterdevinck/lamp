PROJ = lamp
DEVICE = -d 5k -P sg48

SRCDIR = src/fpga/
SIMDIR = test/fpga/
OUTDIR = build/fpga/

.PHONY: all
all: $(OUTDIR)$(PROJ).bin

$(OUTDIR)%.blif: $(SRCDIR)%.v
	mkdir -p $(OUTDIR)
	yosys -v 3 -p 'synth_ice40 -top $(PROJ) -blif $@' $<

$(OUTDIR)%.asc: $(SRCDIR)%.pcf $(OUTDIR)%.blif
	arachne-pnr -o $@ -p $^ $(DEVICE)

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