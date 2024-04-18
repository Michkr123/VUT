# Název cílového souboru (bez přípony)
TARGET = main.tex

# Seznam všech LaTeX souborů ve vašem projektu
TEX_FILES = $(wildcard *.tex)

all: $(TARGET).pdf

# První průchod
$(TARGET).pdf: $(TEX_FILES)
	pdflatex $(TARGET)
	@echo "První průchod dokončen."

# Druhý průchod
	pdflatex $(TARGET)
	@echo "Druhý průchod dokončen."
    
# Vyčištění pomocných souborů
clean:
	rm -f $(TARGET).aux $(TARGET).log $(TARGET).out $(TARGET).toc $(TARGET).pdf
	@echo "Vyčištění dokončeno."

.PHONY: all clean