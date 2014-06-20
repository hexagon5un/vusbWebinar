all: vusb_slides.pdf 

%.tex: %.txt
	utils/makeSlides.py $<

%.pdf: %.tex
	pdflatex -shell-escape $<

handout:  
	utils/makeSlides.py vusb_slides.txt handout
	pdflatex -shell-escape vusb_slides.tex 

clean: 
	-rm *.aux *.log *.dvi *~ *.toc *.bbl *.blg *.snm *.nav *.out *.pyg *.pdf *.tex

ps: *.dvi
	dvips *.dvi -o

docs:
	./makeHTMLs
	./makePDFs      



