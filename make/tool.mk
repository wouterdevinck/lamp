SRCDIR  = tool/
OUTDIR  = build/tool/

SCRIPT_DIR  = make

.PHONY: all
all:
	$(SCRIPT_DIR)/build-tool.sh $(SRCDIR) $(OUTDIR)