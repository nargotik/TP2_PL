SUBDIRS := $(wildcard src/*/.)

all: $(SUBDIRS)
$(SUBDIRS):
	@[ -f $@/Makefile ] && $(MAKE) -C $@ || echo "WARNING: Nao Existe Makefile \"$@/Makefile\""
clean:
	find bin/. ! -name 'readme.md' -type f -exec rm -f {} +
run_import:
	./bin/import < test_file.tsv
run_display:
	./bin/display

.PHONY: $(TOPTARGETS) $(SUBDIRS)

