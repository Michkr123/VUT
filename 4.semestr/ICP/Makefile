# 
# ICP Project
#
# File: Makefile
#
# Description: Makefile to compile and run program. It can generate doxygen documentation and pack source files to .zip.
# 
# Authors: Michal Blažek <xblaze38>
#

PROJECT=src/ICP_project
ZIP_NAME=xblaze38-xmicha94.zip

compile: 
	qmake $(PROJECT).pro -o src/Makefile
	$(MAKE) -C src

all: compile run

run:
	$(PROJECT)

doxygen:
	doxygen doxygenConfig

pack: clean
	zip -r $(ZIP_NAME) src/ examples/ doc/ doxygenConfig README.txt Makefile class_diagram.pdf -x src/.qmake.stash

clean:
	rm -rf src/*.o src/Makefile src/ICP_project src/moc_* src/ui_* src/obj doc/* tmpSavedScene.json src/ICP_project.pro.user $(ZIP_NAME)