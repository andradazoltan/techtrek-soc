################################################################################
# Key paths and settings
################################################################################
CFLAGS += -std=c++11 
CXX = g++ ${CFLAGS} 
ODIR  = ./build${D}
SDIR  = ./src
MKDIR = mkdir -p

OUTPUTNAME = hikercam${D}
OUTDIR = .

################################################################################
# Dependencies
################################################################################
# Spinnaker deps
SPINNAKER_LIB = -lSpinnaker${D} ${SPIN_DEPS}

################################################################################
# Master inc/lib/obj/dep settings
################################################################################
_OBJ = main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ2 = Centroid.o HikerCam.o PeopleTracker.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))

INC += -I/usr/include/spinnaker
INC += -I./include/camera
LIB += -Wl,-Bdynamic ${SPINNAKER_LIB}
LIB += -Wl,-rpath-link=./lib -pthread

################################################################################
# Rules/recipes
################################################################################
# Final binary
${OUTPUTNAME}: ${OBJ2} ${OBJ}
	${CXX} -o ${OUTPUTNAME} ${OBJ2} ${OBJ} ${LIB}

# Intermediate object files
${OBJ2}: ${ODIR}/%.o : ${SDIR}/camera/%.cpp
	@${MKDIR} ${ODIR}
	${CXX} ${CFLAGS} ${INC} -Wall -D LINUX -c $< -o $@
${OBJ}: ${ODIR}/%.o : ${SDIR}/%.cpp
	@${MKDIR} ${ODIR}
	${CXX} ${CFLAGS} ${INC} -Wall -D LINUX -c $< -o $@


# Clean up intermediate objects
clean_obj:
	rm -f ${OBJ}
	@echo "intermediate objects cleaned up!"

# Clean up everything.
clean: clean_obj
	rm -f ${OUTDIR}/${OUTPUTNAME}
	@echo "all cleaned up!"
