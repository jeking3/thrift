#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

include(CMakeDependentOption)

set(THRIFT_COMPILER "" CACHE FILEPATH "External Thrift compiler to use during build")

# Additional components
option(BUILD_COMPILER "Build Thrift compiler" ON)

if(BUILD_COMPILER OR EXISTS ${THRIFT_COMPILER})
    set(HAVE_COMPILER ON)
endif()
option(BUILD_LIBRARIES "Build Thrift libraries" ON)
CMAKE_DEPENDENT_OPTION(BUILD_TESTING "Build with unit tests" ON "HAVE_COMPILER" OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TUTORIALS "Build Thrift tutorials" ON "HAVE_COMPILER" OFF)

# Libraries to build

# Each language library can be enabled or disabled using the WITH_<LANG> flag.
# By default CMake checks if the required dependencies for a language are present
# and enables the library if all are found. This means the default is to build as
# much as possible but leaving out libraries if their dependencies are not met.

CMAKE_DEPENDENT_OPTION(WITH_C_GLIB  "Build C glib Runtime Library"  ON "BUILD_LIBRARIES" OFF)
CMAKE_DEPENDENT_OPTION(WITH_CPP     "Build C++ Runtime Library"     ON "BUILD_LIBRARIES" OFF)
CMAKE_DEPENDENT_OPTION(WITH_HASKELL "Build Haskell Runtime Library" ON "BUILD_LIBRARIES" OFF)
CMAKE_DEPENDENT_OPTION(WITH_JAVA    "Build Java Runtime Library"    ON "BUILD_LIBRARIES" OFF)
CMAKE_DEPENDENT_OPTION(WITH_PYTHON  "Build Python Runtime Library"  ON "BUILD_LIBRARIES" OFF)

if(WITH_CPP)
    # If C++11 or later is the selected language standard then
    # we attempt to build with std:: items instead of boost::
    # items unless otherwise forced to use boost::
    if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "98")
        set(HAVE_CXX11 ON)
    endif()

	option(WITH_BOOST_STATIC "Build with Boost static link library" OFF)
	set(Boost_USE_STATIC_LIBS ${WITH_BOOST_STATIC})
	if (NOT WITH_BOOST_STATIC)
		add_definitions(-DBOOST_ALL_DYN_LINK)
		add_definitions(-DBOOST_TEST_DYN_LINK)
	endif()

    set(REQUIRED_BOOST_COMPONENTS)

	CMAKE_DEPENDENT_OPTION(WITH_BOOST_FUNCTIONAL "Use boost/tr1/functional.hpp (even under C++11 or later)" OFF "HAVE_CXX11" ON)
	if (WITH_BOOST_FUNCTIONAL)
		add_definitions(-DFORCE_BOOST_FUNCTIONAL)
	endif()

	CMAKE_DEPENDENT_OPTION(WITH_BOOST_SMART_PTR "Use boost/smart_ptr.hpp (even under C++11 or later)" OFF "HAVE_CXX11" ON)
	if (WITH_BOOST_SMART_PTR)
		add_definitions(-DFORCE_BOOST_SMART_PTR)
	endif()

    if (DEFINED WITH_BOOSTTHREADS)
        message(WARNING "Using older variable name WITH_BOOSTTHREADS, please use WITH_BOOST_THREAD")
        set(WITH_BOOST_THREAD ${WITH_BOOSTTHREADS})
    endif()
    
    if (DEFINED WITH_STDTHREADS)
        message(WARNING "Using older variable name WITH_STDTHREADS, please use WITH_STD_THREAD")
        set(WITH_STD_THREAD ${WITH_STDTHREADS})
    endif()
    
    # For C++11 or later, std::thread is selected
    # For C++98, boost:thread is selected
    # If you REALLY want to force use of pthreads directly,
    # you must set both options below to OFF, however note that
    # the pthread implementation deprecated; it may disappear...
	CMAKE_DEPENDENT_OPTION(WITH_BOOST_THREAD "Use boost/thread (even under C++11 or later)" OFF "HAVE_CXX11" ON)
	if (WITH_BOOST_THREAD)
		add_definitions(-DFORCE_BOOST_THREAD)
        list(APPEND REQUIRED_BOOST_COMPONENTS thread)
	endif()
	
	CMAKE_DEPENDENT_OPTION(WITH_STD_THREAD "Use std::thread" ON "NOT WITH_BOOST_THREAD" OFF)
    if((NOT HAVE_CXX11) AND WITH_STD_THREAD)
        message(FATAL_ERROR "Cannot enable std::thread without C++11 or later.")
    endif()
    if (WITH_BOOST_THREAD AND WITH_STD_THREAD)
        message(FATAL_ERROR "Cannot enable boost::thread and std::thread.")
    endif()
    
    if(ZLIB_LIBRARY)
        # FindZLIB.cmake does not normalize path so we need to do it ourselves.
        file(TO_CMAKE_PATH ${ZLIB_LIBRARY} ZLIB_LIBRARY)
    endif()

    find_package(Boost 1.53 QUIET)
    find_package(Libevent QUIET)
    find_package(OpenSSL QUIET)
    find_package(Qt4 QUIET COMPONENTS QtCore QtNetwork)
    find_package(Qt5 QUIET COMPONENTS Core Network)
    find_package(ZLIB QUIET)

    CMAKE_DEPENDENT_OPTION(WITH_LIBEVENT "Build with libevent support" ON "Libevent_FOUND" OFF)
    CMAKE_DEPENDENT_OPTION(WITH_OPENSSL "Build with OpenSSL support" ON "OPENSSL_FOUND" OFF)
    CMAKE_DEPENDENT_OPTION(WITH_QT4 "Build with Qt4 support" ON "QT4_FOUND" OFF)
    CMAKE_DEPENDENT_OPTION(WITH_QT5 "Build with Qt5 support" ON "Qt5_FOUND" OFF)
    CMAKE_DEPENDENT_OPTION(WITH_ZLIB "Build with ZLIB support" ON "ZLIB_FOUND" OFF)
endif()
CMAKE_DEPENDENT_OPTION(BUILD_CPP "Build C++ library" ON "BUILD_LIBRARIES;WITH_CPP" OFF)
CMAKE_DEPENDENT_OPTION(WITH_PLUGIN "Build compiler plugin support" OFF "BUILD_COMPILER;BUILD_CPP;Boost_FOUND" OFF)

if(WITH_C_GLIB)
    find_package(GLIB QUIET COMPONENTS gobject)
endif()
CMAKE_DEPENDENT_OPTION(BUILD_C_GLIB "Build C (GLib) library" ON
                       "BUILD_LIBRARIES;WITH_C_GLIB;GLIB_FOUND" OFF)

if(BUILD_CPP)
    if(BUILD_TESTING)
        list(APPEND REQUIRED_BOOST_COMPONENTS chrono date_time filesystem program_options thread unit_test_framework)
    endif()
    if(REQUIRED_BOOST_COMPONENTS)
        message(STATUS "Required Boost Components: ${REQUIRED_BOOST_COMPONENTS}")
        find_package(Boost 1.53 REQUIRED COMPONENTS ${REQUIRED_BOOST_COMPONENTS})
    endif()
elseif(BUILD_C_GLIB AND BUILD_TESTING)
    find_package(Boost 1.53 REQUIRED)
endif()

# Java
if(ANDROID)
    find_package(Gradle QUIET)
    CMAKE_DEPENDENT_OPTION(BUILD_JAVA "Build Java library" ON
                           "BUILD_LIBRARIES;WITH_JAVA;GRADLE_FOUND" OFF)
else()
    find_package(Gradlew QUIET)
    find_package(Java QUIET)
    CMAKE_DEPENDENT_OPTION(BUILD_JAVA "Build Java library" ON
                           "BUILD_LIBRARIES;WITH_JAVA;JAVA_FOUND;GRADLEW_FOUND" OFF)
endif()

# Python
find_package(PythonInterp QUIET) # for Python executable
find_package(PythonLibs QUIET) # for Python.h
CMAKE_DEPENDENT_OPTION(BUILD_PYTHON "Build Python library" ON
                       "BUILD_LIBRARIES;WITH_PYTHON;PYTHONLIBS_FOUND" OFF)

# Haskell
find_package(GHC QUIET)
find_package(Cabal QUIET)
CMAKE_DEPENDENT_OPTION(BUILD_HASKELL "Build GHC library" ON
                       "BUILD_LIBRARIES;WITH_HASKELL;GHC_FOUND;CABAL_FOUND" OFF)

# Common library options
option(WITH_SHARED_LIB "Build shared libraries" ON)
option(WITH_STATIC_LIB "Build static libraries" ON)
if (NOT WITH_SHARED_LIB AND NOT WITH_STATIC_LIB)
    message(FATAL_ERROR "Cannot build with both shared and static outputs disabled!")
endif()

#NOTE: C++ compiler options are defined in the lib/cpp/CMakeLists.txt

# Visual Studio only options
if(MSVC)
option(WITH_MT "Build using MT instead of MD (MSVC only)" OFF)
endif(MSVC)

macro(MESSAGE_DEP flag summary)
if(NOT ${flag})
  message(STATUS "   - ${summary}")
endif()
endmacro(MESSAGE_DEP flag summary)

macro(PRINT_CONFIG_SUMMARY)
message(STATUS "----------------------------------------------------------")
message(STATUS "Thrift version:                               ${thrift_VERSION} (${thrift_VERSION_MAJOR}.${thrift_VERSION_MINOR}.${thrift_VERSION_PATCH})")
message(STATUS "Thrift package version:                       ${PACKAGE_VERSION}")
message(STATUS "Build configuration Summary")
message(STATUS "  Build Thrift compiler:                      ${BUILD_COMPILER}")
message(STATUS "  Build compiler plugin support:              ${WITH_PLUGIN}")
message(STATUS "  Build Thrift libraries:                     ${BUILD_LIBRARIES}")
message(STATUS "  Build Thrift unit tests:                    ${BUILD_TESTING}")
MESSAGE_DEP(HAVE_COMPILER "Disabled because BUILD_THRIFT=OFF and no valid THRIFT_COMPILER is given")
message(STATUS " Language libraries:")
message(STATUS "  Build C++ library:                          ${BUILD_CPP}")
MESSAGE_DEP(WITH_CPP "Disabled by WITH_CPP=OFF")
message(STATUS "    C++ Language Level:                       ${CXX_LANGUAGE_LEVEL}")
message(STATUS "    Use boost instead of std for:")
message(STATUS "      functional                              ${WITH_BOOST_FUNCTIONAL}")
message(STATUS "      smart_ptr                               ${WITH_BOOST_SMART_PTR}")
message(STATUS "      thread                                  ${WITH_BOOST_THREAD}")
if(NOT WITH_BOOST_THREAD AND NOT WITH_STD_THREAD)
message(STATUS "    Using pthread implementation:             ON <<<<<<<<<<<<<<<<<<<<<<<< DEPRECATED")
endif()
message(STATUS "  Build with Boost static link library:       ${WITH_BOOST_STATIC}")
message(STATUS "  Build C (GLib) library:                     ${BUILD_C_GLIB}")
MESSAGE_DEP(WITH_C_GLIB "Disabled by WITH_C_GLIB=OFF")
MESSAGE_DEP(GLIB_FOUND "GLib missing")
message(STATUS "  Build Java library:                         ${BUILD_JAVA}")
MESSAGE_DEP(WITH_JAVA "Disabled by WITH_JAVA=OFF")
if(ANDROID)
    MESSAGE_DEP(GRADLE_FOUND "Gradle missing")
else()
    MESSAGE_DEP(JAVA_FOUND "Java Runtime missing")
    MESSAGE_DEP(GRADLEW_FOUND "Gradle Wrapper missing")
endif()
message(STATUS "  Build Python library:                       ${BUILD_PYTHON}")
MESSAGE_DEP(WITH_PYTHON "Disabled by WITH_PYTHON=OFF")
MESSAGE_DEP(PYTHONLIBS_FOUND "Python libraries missing")
message(STATUS "  Build Haskell library:                      ${BUILD_HASKELL}")
MESSAGE_DEP(WITH_HASKELL "Disabled by WITH_HASKELL=OFF")
MESSAGE_DEP(GHC_FOUND "GHC missing")
MESSAGE_DEP(CABAL_FOUND "Cabal missing")
message(STATUS " Library features:")
message(STATUS "  Build shared libraries:                     ${WITH_SHARED_LIB}")
message(STATUS "  Build static libraries:                     ${WITH_STATIC_LIB}")
message(STATUS "  Build with libevent support:                ${WITH_LIBEVENT}")
message(STATUS "  Build with OpenSSL support:                 ${WITH_OPENSSL}")
message(STATUS "  Build with Qt4 support:                     ${WITH_QT4}")
message(STATUS "  Build with Qt5 support:                     ${WITH_QT5}")
message(STATUS "  Build with ZLIB support:                    ${WITH_ZLIB}")
message(STATUS "----------------------------------------------------------")
endmacro(PRINT_CONFIG_SUMMARY)
