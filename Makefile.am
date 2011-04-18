SUBDIRS = src
dist_doc_DATA = README lualatex-platform.html

lualatex-platform.html: README.rst
	$(RST2HTML) $< $@
