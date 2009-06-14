# $Id: Makefile,v 1.1 2009/03/06 22:16:07 gutsche Exp $

LATEX = pdflatex
LATEXFLAGS =

SOURCE = step09.pdf

ALLTEX = $(wildcard *.tex)
RERUN = '(No file .*\.toc|There were undefined references)'

.PHONY: all
all: $(SOURCE)

oli: $(SOURCE)
	open $(SOURCE)

%.pdf: %.tex $(wildcard *.tex) 
	@echo "====> LaTeX first pass"
	$(LATEX) $(LATEXFLAGS) $(<:.tex=)
	@if egrep -q $(RERUN) $*.log ; then echo "====> LaTeX rerun" && $(LATEX) $<; fi
	@if egrep -q $(RERUN) $*.log ; then echo "====> LaTeX rerun" && $(LATEX) $<; fi

.PHONY: clean
clean:
	rm -f $(SOURCE) *.aux *.dvi *.log *.qsl *.sol *.lof *.lot *.toc *~
