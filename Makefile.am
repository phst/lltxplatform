SUBDIRS = src
dist_doc_DATA = README

if HAVE_RST2HTML
dist_doc_DATA += lualatex-platform.html
lualatex-platform.html: README.rst
	$(RST2HTML) $< $@
endif
