EXEEXT = .exe
CXXTESTGEN = cxxtestgen.pl
CPPFLAGS   = -I/cygdrive/e/Dev/include

# Paths
# MODULE_ROOT:sh	= dirname `pwd`
MODULE_ROOT	= $(shell dirname `pwd`)
TU_SRC_ROOT	= $(MODULE_ROOT)/tu
TU_OBJ_ROOT	= $(MODULE_ROOT)/obj/tu
TU_BIN_ROOT	= $(MODULE_ROOT)/bin/tu

# external command to list all the test units in directory $tu_src_root
TU_COMMAND	= ls ${TU_SRC_ROOT}/test_*.hpp &2>/dev/null
# TU_SOURCES	= $(TU_COMMAND:sh) # store the result of the ext. command
TU_SOURCES	= $(shell $(TU_COMMAND)) # store the result of the ext. command

# ---------------------------------------------------------------------------
# règle de construction des objets à partir des sources
# ---------------------------------------------------------------------------
# build lists of files (.hpp, .cpp, .o, executables) from $(TU_SOURCES)
TU_TRANSL_UNITS	=  ${TU_SOURCES:${TU_SRC_ROOT}/%.hpp=${TU_SRC_ROOT}/%.cpp}
TU_OBJECTS	=  ${TU_SOURCES:${TU_SRC_ROOT}/%.hpp=${TU_OBJ_ROOT}/%.o}
TU_TARGETS	=  ${TU_SOURCES:${TU_SRC_ROOT}/%.hpp=${TU_BIN_ROOT}/%$(EXEEXT)}

$(TU_OBJ_ROOT):
	mkdir -p "$@"

$(TU_BIN_ROOT):
	mkdir -p "$@"

.DEFAULT: all
all: ${TU_OBJ_ROOT} ${TU_BIN_ROOT} $(TU_TARGETS)

.PHONY: clean distclan print 

## ==========================================================================
# Test units
# ---------------------------------------------------------------------------
# cible générique de génération des unités de traduction pour les test-unitaires
# ---------------------------------------------------------------------------
${TU_SRC_ROOT}/test_%.cpp : ${TU_SRC_ROOT}/test_%.hpp
	@echo "Convert TU to .cpp"
	$(CXXTESTGEN) -error-printer -o $@ $^

# ---------------------------------------------------------------------------
${TU_OBJ_ROOT}/%.obj : ${TU_SRC_ROOT}/%.cpp
	@echo ".cpp -> .obj"
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $^

# ---------------------------------------------------------------------------
${TU_OBJ_ROOT}/test_Moves.cpp: ../src/Moves.hpp
	touch $@

${TU_BIN_ROOT}/test_Moves$(EXEEXT) : ${TU_OBJ_ROOT}/test_Moves.obj
	@echo "-> .exe"
	$(CXX) $(LDFLAGS) -o $@ ${TU_OBJ_ROOT}/test_Moves.obj 
	$@


# ---------------------------------------------------------------------------
# cibles de nettoyage
# ---------------------------------------------------------------------------
clean: 
	rm -f  ${TU_OBJECTS}
	rm -f  ${TU_TRANSL_UNITS}
	rm -f  ${TU_BIN_ROOT}/core
	rm -f  ${TU_INC_ROOT}/*~ ${TU_INC_ROOT}/*% ${TU_INC_ROOT}/*.bak
	rm -f  ${TU_SRC_ROOT}/*~ ${TU_SRC_ROOT}/*% ${TU_SRC_ROOT}/*.bak
	find ${MODULE_ROOT} -name SunWS_cache -type d -prune -exec rm -rf {} \;

distclean: clean
	rm -f ${TU_TARGETS}

# ---------------------------------------------------------------------------
print:
	@echo "MODULE_ROOT 	= $(MODULE_ROOT)"
	@echo "TU_SRC_ROOT	= $(TU_SRC_ROOT)"
	@echo "TU_SOURCES	= $(TU_SOURCES)"
	@echo "TU_TRANSL_UNITS	= $(TU_TRANSL_UNITS)"
	@echo "TU_OBJECTS	= $(TU_OBJECTS)"
	@echo "TU_TARGETS	= $(TU_TARGETS)"
# ---------------------------------------------------------------------------
# vim: set noexpandtab:
