.PHONY : all tab fig clean

all :
	latexmk -silent -f manual.tex >/dev/null

tab :
	cd tab; ./rng_engine.pl
	cd tab; ./rng_distribution.pl

fig :
	cd prg; ./make.sh pf.pdf
	cp prg/pf.pdf fig/pf.pdf

clean:
	make -C prg clean
	latexmk -c manual.tex
