###########################################################################
## Makefile generated for component 'cvxEDA'. 
## 
## Makefile     : cvxEDA_rtw.mk
## Generated on : Sun Jul 16 22:34:03 2023
## Final product: .\cvxEDA.lib
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# MODELLIB                Static library target

PRODUCT_NAME              = cvxEDA
MAKEFILE                  = cvxEDA_rtw.mk
MATLAB_ROOT               = C:\PROGRA~1\MATLAB\R2020b
MATLAB_BIN                = C:\PROGRA~1\MATLAB\R2020b\bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)\win64
START_DIR                 = C:\Users\chenguanghao\Desktop\ESDC\Algo\cvxEDa\codegen\lib\cvxEDA
TGT_FCN_LIB               = ISO_C
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = .
COMPILER_COMMAND_FILE     = cvxEDA_rtw_comp.rsp
CMD_FILE                  = cvxEDA_rtw.rsp
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
NODEBUG                   = 1
MODELLIB                  = cvxEDA.lib

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          Microsoft Visual C++ 2019 v16.0 | nmake (64-bit Windows)
# Supported Version(s):    16.0
# ToolchainInfo Version:   2020b
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# NODEBUG
# cvarsdll
# cvarsmt
# conlibsmt
# ldebug
# conflags
# cflags

#-----------
# MACROS
#-----------

MW_EXTERNLIB_DIR    = $(MATLAB_ROOT)\extern\lib\win64\microsoft
MW_LIB_DIR          = $(MATLAB_ROOT)\lib\win64
CPU                 = AMD64
APPVER              = 5.02
CVARSFLAG           = $(cvarsmt)
CFLAGS_ADDITIONAL   = -D_CRT_SECURE_NO_WARNINGS
CPPFLAGS_ADDITIONAL = -EHs -D_CRT_SECURE_NO_WARNINGS /wd4251
LIBS_TOOLCHAIN      = $(conlibs)

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Microsoft Visual C Compiler
CC = cl

# Linker: Microsoft Visual C Linker
LD = link

# C++ Compiler: Microsoft Visual C++ Compiler
CPP = cl

# C++ Linker: Microsoft Visual C++ Linker
CPP_LD = link

# Archiver: Microsoft Visual C/C++ Archiver
AR = lib

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)\mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: NMAKE Utility
MAKE = nmake


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -Zi
C_OUTPUT_FLAG       = -Fo
LDDEBUG             = /DEBUG
OUTPUT_FLAG         = -out:
CPPDEBUG            = -Zi
CPP_OUTPUT_FLAG     = -Fo
CPPLDDEBUG          = /DEBUG
OUTPUT_FLAG         = -out:
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = -out:
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @ren
RUN                 = @cmd /C

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = /nologo
CFLAGS               = $(cflags) $(CVARSFLAG) $(CFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPPFLAGS             = /TP $(cflags) $(CVARSFLAG) $(CPPFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPP_LDFLAGS          = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
CPP_SHAREDLIB_LDFLAGS  = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                         -dll -def:$(DEF_FILE)
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                       -dll -def:$(DEF_FILE)



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = .\cvxEDA.lib
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=cvxEDA

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)\cvxEDA_data.c $(START_DIR)\rt_nonfinite.c $(START_DIR)\rtGetNaN.c $(START_DIR)\rtGetInf.c $(START_DIR)\cvxEDA_initialize.c $(START_DIR)\cvxEDA_terminate.c $(START_DIR)\cvxEDA.c $(START_DIR)\sparse.c $(START_DIR)\colon.c $(START_DIR)\conv.c $(START_DIR)\minOrMax.c $(START_DIR)\bsxfun.c $(START_DIR)\repmat.c $(START_DIR)\sparse1.c $(START_DIR)\mtimes.c $(START_DIR)\horzcat.c $(START_DIR)\speye.c $(START_DIR)\vertcat.c $(START_DIR)\sum.c $(START_DIR)\quadprog.c $(START_DIR)\xnrm2.c $(START_DIR)\locBsearch.c $(START_DIR)\insertionsort.c $(START_DIR)\introsort.c $(START_DIR)\heapsort.c $(START_DIR)\fillIn.c $(START_DIR)\setProblemType.c $(START_DIR)\driver.c $(START_DIR)\RemoveDependentEq_.c $(START_DIR)\xgeqp3.c $(START_DIR)\xzgeqp3.c $(START_DIR)\xzlarfg.c $(START_DIR)\xzlarf.c $(START_DIR)\xgerc.c $(START_DIR)\computeQ_.c $(START_DIR)\factorQRE.c $(START_DIR)\countsort.c $(START_DIR)\RemoveDependentIneq_.c $(START_DIR)\feasibleX0ForWorkingSet.c $(START_DIR)\factorQR.c $(START_DIR)\xgemv.c $(START_DIR)\maxConstraintViolation.c $(START_DIR)\phaseone.c $(START_DIR)\computeFval.c $(START_DIR)\linearForm_.c $(START_DIR)\iterate.c $(START_DIR)\computeGrad_StoreHx.c $(START_DIR)\computeFval_ReuseHx.c $(START_DIR)\xrotg.c $(START_DIR)\deleteColMoveEnd.c $(START_DIR)\compute_deltax.c $(START_DIR)\factor.c $(START_DIR)\partialColLDL3_.c $(START_DIR)\fullColLDL2_.c $(START_DIR)\solve.c $(START_DIR)\xgemm.c $(START_DIR)\compute_lambda.c $(START_DIR)\feasibleratiotest.c $(START_DIR)\ratiotest.c $(START_DIR)\addBoundToActiveSetMatrix_.c $(START_DIR)\computeFirstOrderOpt.c $(START_DIR)\PresolveWorkingSet.c $(START_DIR)\cvxEDA_emxutil.c $(START_DIR)\cvxEDA_emxAPI.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = cvxEDA_data.obj rt_nonfinite.obj rtGetNaN.obj rtGetInf.obj cvxEDA_initialize.obj cvxEDA_terminate.obj cvxEDA.obj sparse.obj colon.obj conv.obj minOrMax.obj bsxfun.obj repmat.obj sparse1.obj mtimes.obj horzcat.obj speye.obj vertcat.obj sum.obj quadprog.obj xnrm2.obj locBsearch.obj insertionsort.obj introsort.obj heapsort.obj fillIn.obj setProblemType.obj driver.obj RemoveDependentEq_.obj xgeqp3.obj xzgeqp3.obj xzlarfg.obj xzlarf.obj xgerc.obj computeQ_.obj factorQRE.obj countsort.obj RemoveDependentIneq_.obj feasibleX0ForWorkingSet.obj factorQR.obj xgemv.obj maxConstraintViolation.obj phaseone.obj computeFval.obj linearForm_.obj iterate.obj computeGrad_StoreHx.obj computeFval_ReuseHx.obj xrotg.obj deleteColMoveEnd.obj compute_deltax.obj factor.obj partialColLDL3_.obj fullColLDL2_.obj solve.obj xgemm.obj compute_lambda.obj feasibleratiotest.obj ratiotest.obj addBoundToActiveSetMatrix_.obj computeFirstOrderOpt.obj PresolveWorkingSet.obj cvxEDA_emxutil.obj cvxEDA_emxAPI.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CFLAGS = $(CFLAGS) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS = $(CPPFLAGS) $(CPPFLAGS_BASIC)

###########################################################################
## INLINED COMMANDS
###########################################################################


!include $(MATLAB_ROOT)\rtw\c\tools\vcdefs.mak


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute set_environment_variables


all : build
	@cmd /C "@echo ### Successfully generated all binary outputs."


build : set_environment_variables prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


set_environment_variables : 
	@set INCLUDE=$(INCLUDES);$(INCLUDE)
	@set LIB=$(LIB)


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@cmd /C "@echo ### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS) -out:$(PRODUCT) @$(CMD_FILE)
	@cmd /C "@echo ### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{C:\Users\chenguanghao\Desktop\ESDC\Algo\cvxEDa}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{C:\Users\chenguanghao\Desktop\ESDC\Algo\cvxEDa}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


cvxEDA_data.obj : $(START_DIR)\cvxEDA_data.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\cvxEDA_data.c


rt_nonfinite.obj : $(START_DIR)\rt_nonfinite.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\rt_nonfinite.c


rtGetNaN.obj : $(START_DIR)\rtGetNaN.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\rtGetNaN.c


rtGetInf.obj : $(START_DIR)\rtGetInf.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\rtGetInf.c


cvxEDA_initialize.obj : $(START_DIR)\cvxEDA_initialize.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\cvxEDA_initialize.c


cvxEDA_terminate.obj : $(START_DIR)\cvxEDA_terminate.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\cvxEDA_terminate.c


cvxEDA.obj : $(START_DIR)\cvxEDA.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\cvxEDA.c


sparse.obj : $(START_DIR)\sparse.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\sparse.c


colon.obj : $(START_DIR)\colon.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\colon.c


conv.obj : $(START_DIR)\conv.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\conv.c


minOrMax.obj : $(START_DIR)\minOrMax.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\minOrMax.c


bsxfun.obj : $(START_DIR)\bsxfun.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\bsxfun.c


repmat.obj : $(START_DIR)\repmat.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\repmat.c


sparse1.obj : $(START_DIR)\sparse1.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\sparse1.c


mtimes.obj : $(START_DIR)\mtimes.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\mtimes.c


horzcat.obj : $(START_DIR)\horzcat.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\horzcat.c


speye.obj : $(START_DIR)\speye.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\speye.c


vertcat.obj : $(START_DIR)\vertcat.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\vertcat.c


sum.obj : $(START_DIR)\sum.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\sum.c


quadprog.obj : $(START_DIR)\quadprog.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\quadprog.c


xnrm2.obj : $(START_DIR)\xnrm2.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xnrm2.c


locBsearch.obj : $(START_DIR)\locBsearch.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\locBsearch.c


insertionsort.obj : $(START_DIR)\insertionsort.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\insertionsort.c


introsort.obj : $(START_DIR)\introsort.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\introsort.c


heapsort.obj : $(START_DIR)\heapsort.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\heapsort.c


fillIn.obj : $(START_DIR)\fillIn.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\fillIn.c


setProblemType.obj : $(START_DIR)\setProblemType.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\setProblemType.c


driver.obj : $(START_DIR)\driver.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\driver.c


RemoveDependentEq_.obj : $(START_DIR)\RemoveDependentEq_.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\RemoveDependentEq_.c


xgeqp3.obj : $(START_DIR)\xgeqp3.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xgeqp3.c


xzgeqp3.obj : $(START_DIR)\xzgeqp3.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xzgeqp3.c


xzlarfg.obj : $(START_DIR)\xzlarfg.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xzlarfg.c


xzlarf.obj : $(START_DIR)\xzlarf.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xzlarf.c


xgerc.obj : $(START_DIR)\xgerc.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xgerc.c


computeQ_.obj : $(START_DIR)\computeQ_.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\computeQ_.c


factorQRE.obj : $(START_DIR)\factorQRE.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\factorQRE.c


countsort.obj : $(START_DIR)\countsort.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\countsort.c


RemoveDependentIneq_.obj : $(START_DIR)\RemoveDependentIneq_.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\RemoveDependentIneq_.c


feasibleX0ForWorkingSet.obj : $(START_DIR)\feasibleX0ForWorkingSet.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\feasibleX0ForWorkingSet.c


factorQR.obj : $(START_DIR)\factorQR.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\factorQR.c


xgemv.obj : $(START_DIR)\xgemv.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xgemv.c


maxConstraintViolation.obj : $(START_DIR)\maxConstraintViolation.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\maxConstraintViolation.c


phaseone.obj : $(START_DIR)\phaseone.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\phaseone.c


computeFval.obj : $(START_DIR)\computeFval.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\computeFval.c


linearForm_.obj : $(START_DIR)\linearForm_.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\linearForm_.c


iterate.obj : $(START_DIR)\iterate.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\iterate.c


computeGrad_StoreHx.obj : $(START_DIR)\computeGrad_StoreHx.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\computeGrad_StoreHx.c


computeFval_ReuseHx.obj : $(START_DIR)\computeFval_ReuseHx.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\computeFval_ReuseHx.c


xrotg.obj : $(START_DIR)\xrotg.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xrotg.c


deleteColMoveEnd.obj : $(START_DIR)\deleteColMoveEnd.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\deleteColMoveEnd.c


compute_deltax.obj : $(START_DIR)\compute_deltax.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\compute_deltax.c


factor.obj : $(START_DIR)\factor.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\factor.c


partialColLDL3_.obj : $(START_DIR)\partialColLDL3_.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\partialColLDL3_.c


fullColLDL2_.obj : $(START_DIR)\fullColLDL2_.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\fullColLDL2_.c


solve.obj : $(START_DIR)\solve.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\solve.c


xgemm.obj : $(START_DIR)\xgemm.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\xgemm.c


compute_lambda.obj : $(START_DIR)\compute_lambda.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\compute_lambda.c


feasibleratiotest.obj : $(START_DIR)\feasibleratiotest.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\feasibleratiotest.c


ratiotest.obj : $(START_DIR)\ratiotest.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\ratiotest.c


addBoundToActiveSetMatrix_.obj : $(START_DIR)\addBoundToActiveSetMatrix_.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\addBoundToActiveSetMatrix_.c


computeFirstOrderOpt.obj : $(START_DIR)\computeFirstOrderOpt.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\computeFirstOrderOpt.c


PresolveWorkingSet.obj : $(START_DIR)\PresolveWorkingSet.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\PresolveWorkingSet.c


cvxEDA_emxutil.obj : $(START_DIR)\cvxEDA_emxutil.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\cvxEDA_emxutil.c


cvxEDA_emxAPI.obj : $(START_DIR)\cvxEDA_emxAPI.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\cvxEDA_emxAPI.c


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(COMPILER_COMMAND_FILE) $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@cmd /C "@echo ### PRODUCT = $(PRODUCT)"
	@cmd /C "@echo ### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@cmd /C "@echo ### BUILD_TYPE = $(BUILD_TYPE)"
	@cmd /C "@echo ### INCLUDES = $(INCLUDES)"
	@cmd /C "@echo ### DEFINES = $(DEFINES)"
	@cmd /C "@echo ### ALL_SRCS = $(ALL_SRCS)"
	@cmd /C "@echo ### ALL_OBJS = $(ALL_OBJS)"
	@cmd /C "@echo ### LIBS = $(LIBS)"
	@cmd /C "@echo ### MODELREF_LIBS = $(MODELREF_LIBS)"
	@cmd /C "@echo ### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@cmd /C "@echo ### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@cmd /C "@echo ### CFLAGS = $(CFLAGS)"
	@cmd /C "@echo ### LDFLAGS = $(LDFLAGS)"
	@cmd /C "@echo ### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### CPPFLAGS = $(CPPFLAGS)"
	@cmd /C "@echo ### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@cmd /C "@echo ### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### ARFLAGS = $(ARFLAGS)"
	@cmd /C "@echo ### MEX_CFLAGS = $(MEX_CFLAGS)"
	@cmd /C "@echo ### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@cmd /C "@echo ### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@cmd /C "@echo ### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@cmd /C "@echo ### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@cmd /C "@echo ### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@cmd /C "@echo ### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	@if exist $(PRODUCT) $(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


