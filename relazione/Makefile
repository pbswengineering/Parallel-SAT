#
# Parallel SAT
# di Paolo Bernardi
#
# Makefile della documentazione
#

### Definizione delle regole

all: relazione_parallel-sat.pdf

grafico-%.png: %.gp
	gnuplot $^

relazione_parallel-sat.pdf: relazione_parallel-sat.tex unipg.png grafico-tempi.png grafico-speed-up.png grafico-efficienza.png
	pdflatex relazione_parallel-sat.tex
	pdflatex relazione_parallel-sat.tex

.PHONY: clean help

clean:
	rm -f *.pdf *.dvi *.aux *.log grafico-*.png *.toc *.out *~

help:
	@echo
	@echo "     Makefile della relazione Parallel SAT"
	@echo "              di Paolo Bernardi"
	@echo
	@echo "Le regole che puoi eseguire con GNU make sono le seguenti:"
	@echo
	@echo "all -> regola predefinita; crea la documentazione in formato"
	@echo "       PDF."
	@echo
	@echo "clean -> rimuove i file intermedi creati dal processo di"
	@echo "         compilazione della documentazione ed il file PDF."
	@echo

# That's all, folks!

