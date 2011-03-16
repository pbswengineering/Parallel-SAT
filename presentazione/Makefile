TARGET=presentazione_parallel-sat
IMAGES=$(wildcard *.png)

.PHONY: all clean

all: $(TARGET).pdf

$(TARGET).pdf: $(TARGET).tex $(IMAGES)
	pdflatex $(TARGET).tex
	pdflatex $(TARGET).tex

clean:
	rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.lot *.lof *.out *.bbl *.blg *.lol *.pdf *.backup *.snm *.nav *.vrb *.info
