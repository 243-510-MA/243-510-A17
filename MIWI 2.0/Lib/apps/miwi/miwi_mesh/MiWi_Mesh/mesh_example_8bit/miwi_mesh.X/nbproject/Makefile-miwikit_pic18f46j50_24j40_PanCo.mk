#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-miwikit_pic18f46j50_24j40_PanCo.mk)" "nbproject/Makefile-local-miwikit_pic18f46j50_24j40_PanCo.mk"
include nbproject/Makefile-local-miwikit_pic18f46j50_24j40_PanCo.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=miwikit_pic18f46j50_24j40_PanCo
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=--mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../../../framework/driver/mrf_miwi/src/drv_mrf_miwi_mesh_24j40.c ../src/system_config/miwikit_pic18f46j50_24j40/delay.c ../src/system_config/miwikit_pic18f46j50_24j40/lcd.c ../src/system_config/miwikit_pic18f46j50_24j40/spi.c ../src/system_config/miwikit_pic18f46j50_24j40/symbol.c ../src/system_config/miwikit_pic18f46j50_24j40/system.c ../src/mainCoordinator.c ../src/Bridge.c ../src/Main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1 ${OBJECTDIR}/_ext/1255583909/delay.p1 ${OBJECTDIR}/_ext/1255583909/lcd.p1 ${OBJECTDIR}/_ext/1255583909/spi.p1 ${OBJECTDIR}/_ext/1255583909/symbol.p1 ${OBJECTDIR}/_ext/1255583909/system.p1 ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1 ${OBJECTDIR}/_ext/1360937237/Bridge.p1 ${OBJECTDIR}/_ext/1360937237/Main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1.d ${OBJECTDIR}/_ext/1255583909/delay.p1.d ${OBJECTDIR}/_ext/1255583909/lcd.p1.d ${OBJECTDIR}/_ext/1255583909/spi.p1.d ${OBJECTDIR}/_ext/1255583909/symbol.p1.d ${OBJECTDIR}/_ext/1255583909/system.p1.d ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1.d ${OBJECTDIR}/_ext/1360937237/Bridge.p1.d ${OBJECTDIR}/_ext/1360937237/Main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1 ${OBJECTDIR}/_ext/1255583909/delay.p1 ${OBJECTDIR}/_ext/1255583909/lcd.p1 ${OBJECTDIR}/_ext/1255583909/spi.p1 ${OBJECTDIR}/_ext/1255583909/symbol.p1 ${OBJECTDIR}/_ext/1255583909/system.p1 ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1 ${OBJECTDIR}/_ext/1360937237/Bridge.p1 ${OBJECTDIR}/_ext/1360937237/Main.p1

# Source Files
SOURCEFILES=../../../../../../framework/driver/mrf_miwi/src/drv_mrf_miwi_mesh_24j40.c ../src/system_config/miwikit_pic18f46j50_24j40/delay.c ../src/system_config/miwikit_pic18f46j50_24j40/lcd.c ../src/system_config/miwikit_pic18f46j50_24j40/spi.c ../src/system_config/miwikit_pic18f46j50_24j40/symbol.c ../src/system_config/miwikit_pic18f46j50_24j40/system.c ../src/mainCoordinator.c ../src/Bridge.c ../src/Main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-miwikit_pic18f46j50_24j40_PanCo.mk dist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46J50
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1: ../../../../../../framework/driver/mrf_miwi/src/drv_mrf_miwi_mesh_24j40.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1308774647" 
	@${RM} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1  ../../../../../../framework/driver/mrf_miwi/src/drv_mrf_miwi_mesh_24j40.c 
	@-${MV} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.d ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/delay.p1: ../src/system_config/miwikit_pic18f46j50_24j40/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/delay.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/delay.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/delay.p1  ../src/system_config/miwikit_pic18f46j50_24j40/delay.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/delay.d ${OBJECTDIR}/_ext/1255583909/delay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/delay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/lcd.p1: ../src/system_config/miwikit_pic18f46j50_24j40/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/lcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/lcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/lcd.p1  ../src/system_config/miwikit_pic18f46j50_24j40/lcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/lcd.d ${OBJECTDIR}/_ext/1255583909/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/spi.p1: ../src/system_config/miwikit_pic18f46j50_24j40/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/spi.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/spi.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/spi.p1  ../src/system_config/miwikit_pic18f46j50_24j40/spi.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/spi.d ${OBJECTDIR}/_ext/1255583909/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/symbol.p1: ../src/system_config/miwikit_pic18f46j50_24j40/symbol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/symbol.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/symbol.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/symbol.p1  ../src/system_config/miwikit_pic18f46j50_24j40/symbol.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/symbol.d ${OBJECTDIR}/_ext/1255583909/symbol.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/symbol.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/system.p1: ../src/system_config/miwikit_pic18f46j50_24j40/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/system.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/system.p1  ../src/system_config/miwikit_pic18f46j50_24j40/system.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/system.d ${OBJECTDIR}/_ext/1255583909/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1: ../src/mainCoordinator.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1  ../src/mainCoordinator.c 
	@-${MV} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.d ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/Bridge.p1: ../src/Bridge.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Bridge.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Bridge.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1360937237/Bridge.p1  ../src/Bridge.c 
	@-${MV} ${OBJECTDIR}/_ext/1360937237/Bridge.d ${OBJECTDIR}/_ext/1360937237/Bridge.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360937237/Bridge.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/Main.p1: ../src/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1360937237/Main.p1  ../src/Main.c 
	@-${MV} ${OBJECTDIR}/_ext/1360937237/Main.d ${OBJECTDIR}/_ext/1360937237/Main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360937237/Main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1: ../../../../../../framework/driver/mrf_miwi/src/drv_mrf_miwi_mesh_24j40.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1308774647" 
	@${RM} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1  ../../../../../../framework/driver/mrf_miwi/src/drv_mrf_miwi_mesh_24j40.c 
	@-${MV} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.d ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1308774647/drv_mrf_miwi_mesh_24j40.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/delay.p1: ../src/system_config/miwikit_pic18f46j50_24j40/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/delay.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/delay.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/delay.p1  ../src/system_config/miwikit_pic18f46j50_24j40/delay.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/delay.d ${OBJECTDIR}/_ext/1255583909/delay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/delay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/lcd.p1: ../src/system_config/miwikit_pic18f46j50_24j40/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/lcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/lcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/lcd.p1  ../src/system_config/miwikit_pic18f46j50_24j40/lcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/lcd.d ${OBJECTDIR}/_ext/1255583909/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/spi.p1: ../src/system_config/miwikit_pic18f46j50_24j40/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/spi.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/spi.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/spi.p1  ../src/system_config/miwikit_pic18f46j50_24j40/spi.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/spi.d ${OBJECTDIR}/_ext/1255583909/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/symbol.p1: ../src/system_config/miwikit_pic18f46j50_24j40/symbol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/symbol.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/symbol.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/symbol.p1  ../src/system_config/miwikit_pic18f46j50_24j40/symbol.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/symbol.d ${OBJECTDIR}/_ext/1255583909/symbol.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/symbol.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1255583909/system.p1: ../src/system_config/miwikit_pic18f46j50_24j40/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1255583909" 
	@${RM} ${OBJECTDIR}/_ext/1255583909/system.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1255583909/system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1255583909/system.p1  ../src/system_config/miwikit_pic18f46j50_24j40/system.c 
	@-${MV} ${OBJECTDIR}/_ext/1255583909/system.d ${OBJECTDIR}/_ext/1255583909/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1255583909/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1: ../src/mainCoordinator.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1  ../src/mainCoordinator.c 
	@-${MV} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.d ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360937237/mainCoordinator.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/Bridge.p1: ../src/Bridge.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Bridge.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Bridge.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1360937237/Bridge.p1  ../src/Bridge.c 
	@-${MV} ${OBJECTDIR}/_ext/1360937237/Bridge.d ${OBJECTDIR}/_ext/1360937237/Bridge.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360937237/Bridge.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/Main.p1: ../src/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1360937237/Main.p1  ../src/Main.c 
	@-${MV} ${OBJECTDIR}/_ext/1360937237/Main.d ${OBJECTDIR}/_ext/1360937237/Main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360937237/Main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../MiWi_Mesh_Library/Libraries/dist/PIC18_ADDR_8/production/EightBit_MiWiMesh24GhzLib.lpp  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.map  -D__DEBUG=1 --debugger=icd3  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"        $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ..\MiWi_Mesh_Library\Libraries\dist\PIC18_ADDR_8\production\EightBit_MiWiMesh24GhzLib.lpp 
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../MiWi_Mesh_Library/Libraries/dist/PIC18_ADDR_8/production/EightBit_MiWiMesh24GhzLib.lpp 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.map  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -DMRF24J40_COMPILE -P -N255 -I"../src" -I"../src/system_config/miwikit_pic18f46j50_24j40" -I"../../../../../../framework" --warn=6 --asmlist -DXPRJ_miwikit_pic18f46j50_24j40_PanCo=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/miwi_mesh.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ..\MiWi_Mesh_Library\Libraries\dist\PIC18_ADDR_8\production\EightBit_MiWiMesh24GhzLib.lpp 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/miwikit_pic18f46j50_24j40_PanCo
	${RM} -r dist/miwikit_pic18f46j50_24j40_PanCo

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
