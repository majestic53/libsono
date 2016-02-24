# libsono
# Copyright (C) 2016 David Jolly
# ----------------------
#
# libsono is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libsono is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

JOB_SLOTS=4
DIR_BIN=./bin/
DIR_BUILD=./build/
DIR_EXT=./src/ext/
DIR_LIB=./src/lib/
DIR_LOG=./log/
DIR_SRC=./src/
DIR_TOOL=./src/tool/
EXE=sonoctl
LOG_MEM=val_err.log
LOG_STAT=stat_err.log
LOG_CLOC=cloc_stat.log

all: build

build: clean _init _lib _tool _ext

clean:
	rm -rf $(DIR_BIN)
	rm -rf $(DIR_BUILD)
	rm -rf $(DIR_LOG)

_ext:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING EXTERNALS'
	@echo '============================================'
	cd $(DIR_EXT) && make ##all

_init:
	mkdir $(DIR_BIN)
	mkdir $(DIR_BUILD)
	mkdir $(DIR_LOG)

_lib: 
	@echo ''
	@echo '============================================'
	@echo 'BUILDING LIBRARIES'
	@echo '============================================'
	cd $(DIR_LIB) && make build -j $(JOB_SLOTS)
	cd $(DIR_LIB) && make archive

_tool:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING EXECUTABLES'
	@echo '============================================'
	cd $(DIR_TOOL) && make ##all

### TESTING ###

test: _static _mem

_mem:
	@echo ''
	@echo '============================================'
	@echo 'RUNNING MEMORY TEST'
	@echo '============================================'
	valgrind $(DIR_BIN)$(EXE) --leak-check=full --verbose 2> $(DIR_LOG)$(LOG_MEM)

_static:
	@echo ''
	@echo '============================================'
	@echo 'RUNNING STATIC ANALYSIS'
	@echo '============================================'
	cppcheck --enable=all --std=c++11 $(DIR_SRC) 2> $(DIR_LOG)$(LOG_STAT)

### STATISTICS ###

stat: _lines

_lines:
	@echo ''
	@echo '============================================'
	@echo 'CALCULATING LINE COUNT'
	@echo '============================================'
	cloc $(DIR_SRC) > $(DIR_LOG)$(LOG_CLOC)

