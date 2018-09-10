#                              COPYRIGHT
#
#Copyright 1993-2018, Lawrence Livermore National Security, LLC. All
#rights reserved.  This work was produced at the Lawrence Livermore
#National Laboratory (LLNL) under contract no. DE-AC52-07NA27344
#(Contract 44) between the U.S. Department of Energy (DOE) and Lawrence
#Livermore National Security, LLC (LLNS) for the operation of
#LLNL. Copyright is reserved to Lawrence Livermore National Security,
#LLC for purposes of controlled dissemination, commercialization
#through formal licensing, or other disposition under terms of Contract
#44; DOE policies, regulations and orders; and U.S. statutes. The
#rights of the Federal Government are reserved under Contract 44.
#
#                              DISCLAIMER
#
#This work was prepared as an account of work sponsored by an agency of
#the United States Government. Neither the United States Government nor
#Lawrence Livermore National Security, LLC nor any of their employees,
#makes any warranty, express or implied, or assumes any liability or
#responsibility for the accuracy, completeness, or usefulness of any
#information, apparatus, product, or process disclosed, or represents
#that its use would not infringe privately-owned rights. Reference
#herein to any specific commercial products, process, or service by
#trade name, trademark, manufacturer or otherwise does not necessarily
#constitute or imply its endorsement, recommendation, or favoring by
#the United States Government or Lawrence Livermore National Security,
#LLC. The views and opinions of authors expressed herein do not
#necessarily state or reflect those of the United States Government or
#Lawrence Livermore National Security, LLC, and shall not be used for
#advertising or product endorsement purposes.
#
#                        LICENSING REQUIREMENTS
#
#Any use, reproduction, modification, or distribution of this software
#or documentation for commercial purposes requires a license from
#Lawrence Livermore National Security, LLC. Contact: Lawrence Livermore
#National Laboratory, Industrial Partnerships Office, P.O. Box 808,
#L-795, Livermore, CA 94551.
#
#The Government is granted for itself and others acting on its behalf a
#paid-up, nonexclusive, irrevocable worldwide license in this data to
#reproduce, prepare derivative works, and perform publicly and display
#publicly.
#
#                                ACCESS
#This code is both Official Use Only (OUO) and Export Controlled.  By
#DOE Order 471.3. OUO may be exempt from public release under the
#Freedom of Information Act (5 U.S.C 552), Exemption 2, Circumvention
#of Statute. Access is on a need-to-know basis and individuals with
#access must treat it as such.  It may NOT be shared with non
#US-persons unless licensed by the Department of State.  Violations may
#result in administrative, civil, or criminal penalties.

cmake_minimum_required(VERSION 3.1)
if("${CMAKE_VERSION}" VERSION_GREATER 3.3)
    cmake_policy(SET CMP0057 NEW)
endif()

message(STATUS "Using cmake version ${CMAKE_VERSION}")

#--------------------------------------------------------------------------------
# BLT
#--------------------------------------------------------------------------------
if (DEFINED BLT_SOURCE_DIR)
    # Support having a shared BLT outside of the repository if given a BLT_SOURCE_DIR

    if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
        message(FATAL_ERROR "Given BLT_SOURCE_DIR does not contain SetupBLT.cmake")
    endif()
else()
    # Use internal BLT if no BLT_SOURCE_DIR is given

    set(BLT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/blt" CACHE PATH "")
    if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
        message(FATAL_ERROR
            "The BLT submodule is not present. "
            "Run the following two commands in your git repository: \n"
            "    git submodule init\n"
            "    git submodule update" )
    endif()
endif()

set(ENABLE_GTEST OFF CACHE BOOL "")
set(ENABLE_FRUIT OFF CACHE BOOL "")

include(${BLT_SOURCE_DIR}/SetupBLT.cmake)

# ... # ################################
# ... # # Include standard build system logic and options
# ... # ################################
# ... # include(cmake/CMakeBasics.cmake)
#
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static libraries")
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

#------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------
set(SNLS_HEADERS
    SNLS_port.h
    SNLS_lup_solve.h
    SNLS_TrDLDenseG.h)

set(SNLS_SOURCES
    SNLS_lup_solve.cc
    SNLS_TrDLDenseG.cxx)

#------------------------------------------------------------------------------
# Defines
#------------------------------------------------------------------------------
set(SNLS_DEFINES)

if(ENABLE_MSLIB_BUILD)
    list(APPEND SNLS_DEFINES HAVE_MSLIB)
endif(ENABLE_MSLIB_BUILD)

#------------------------------------------------------------------------------
# Includes
#------------------------------------------------------------------------------
set(SNLS_INCLUDES ${HEADER_INCLUDE_DIR}
                  ${CMAKE_SOURCE_DIR}/snls)

#------------------------------------------------------------------------------
# Build Targets
#------------------------------------------------------------------------------
if(BUILD_SHARED_LIBS)
    blt_add_library(NAME        snls_shared
                    OUTPUT_NAME snls
                    DEFINES     ${SNLS_DEFINES}
                    INCLUDES    ${SNLS_INCLUDES}
                    DEPENDS_ON  ${SNLS_DEPENDS}
                    HEADERS     ${SNLS_HEADERS}
                    SOURCES     ${SNLS_SOURCES}
                    SHARED      TRUE)
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    blt_add_library(NAME        snls_static
                    OUTPUT_NAME snls
                    DEFINES     ${SNLS_DEFINES}
                    INCLUDES    ${SNLS_INCLUDES}
                    DEPENDS_ON  ${SNLS_DEPENDS}
                    HEADERS     ${SNLS_HEADERS}
                    SOURCES     ${SNLS_SOURCES}
                    SHARED      FALSE)
endif(BUILD_STATIC_LIBS)

#------------------------------------------------------------------------------
# Testing
#------------------------------------------------------------------------------

if(ENABLE_TESTS)
    if (BUILD_STATIC_LIBS)
        set(SNLSTEST_DEPENDS snls_static)
    else()
        set(SNLSTEST_DEPENDS snls_shared)
    endif()

    blt_add_executable(NAME       snlstest
                       SOURCES    SNLS_testdriver.cc
                       OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                       DEPENDS_ON ${SNLSTEST_DEPENDS})
    blt_add_test(NAME    snlstest
                 COMMAND snlstest)
endif(ENABLE_TESTS)

#------------------------------------------------------------------------------
# Install files
#------------------------------------------------------------------------------

set(SNLS_INSTALLED_HEADERS
    SNLS_lup_solve.h
    SNLS_TrDLDenseG.h)

install(FILES ${SNLS_INSTALLED_HEADERS} DESTINATION include)

if(BUILD_SHARED_LIBS)
    install(TARGETS snls_shared DESTINATION lib)
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    install(TARGETS snls_static DESTINATION lib)
endif(BUILD_STATIC_LIBS)