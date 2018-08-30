cmake_minimum_required(VERSION 2.6)

############################################################################
#
#  Gc - graph cut optimization library
#
#  Copyright (C) 2008-2010   Centre for Biomedical Image Analysis (CBIA)
# 
#############################################################################
#
# FILE: cross platform CMake configuration script
#
# Ondrej Danek <ondrej.danek@gmail.com>
#
# Gc - graph cut optimization library
#
#############################################################################

# project name
project(Gc)

# set the compilation way
set(CMAKE_VERBOSE_MAKEFILE ON CACHE STRING "Tracing the compilation process." FORCE)

############################################################################
# basic decisions
############################################################################

# only dynamic version of the library is supported
set(BUILD_SHARED_LIBS ON FORCE)

############################################################################
# debug/release
############################################################################

# set the debug/release version
if(UNIX)
    option(GC_DEBUG_VERSION "Shall I add debug information?" ON)
    if(GC_DEBUG_VERSION) 
	   set(CMAKE_BUILD_TYPE "DEBUG" CACHE STATIC "Compilation for debug or release version?" FORCE)
    else(GC_DEBUG_VERSION)
	   set(CMAKE_BUILD_TYPE "RELEASE" CACHE STATIC "Compilation for debug or release version?" FORCE)	
    endif(GC_DEBUG_VERSION)
endif(UNIX)

#########################################################################
# compilation details
#########################################################################

# export symbols
if(WIN32)
    # Turn off some annoying warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo /wd4251 /wd4275")    
    # Fast math
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2 /Oi /fp:fast")
    
    add_definitions(-DGC_SHOULD_EXPORT) 
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
endif(WIN32)

# all warnings under gcc
if(UNIX)
	# gnu compiler
	if(CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -mfpmath=sse -msse2")
	endif(CMAKE_COMPILER_IS_GNUCXX)
endif(UNIX)

#########################################################################
#
# Gc library
#  
#########################################################################

########################
#  Project source files
########################

# set the list of source files
set(GC_SOURCES

    # Core
    Gc/Core.h
    Gc/Type.h
    
    # System
    Gc/System/Exception.h
    Gc/System/OutOfMemoryException.h
    Gc/System/ArgumentException.h
    Gc/System/IndexOutOfRangeException.h
    Gc/System/InvalidOperationException.h
    Gc/System/OverflowException.h
    Gc/System/NotImplementedException.h
    Gc/System/Format.cpp
    Gc/System/Format.h
    Gc/System/Log.h   
    Gc/System/Log.cpp    
    Gc/System/Collection/Tuple.h
    Gc/System/Collection/Array.h
    Gc/System/Collection/Pair.h
    Gc/System/Collection/ArrayBuilder.h
    Gc/System/Collection/IArrayMask.h
    Gc/System/Collection/BoolArrayMask.h    
    Gc/System/Collection/EmptyArrayMask.h
    Gc/System/Time/StopWatch.h         
    Gc/System/Time/StopWatch.cpp
    Gc/System/Algo/Basic.h
    Gc/System/Algo/Function.h
    Gc/System/Algo/Predicate.h
    Gc/System/Algo/Range.h    
    Gc/System/Algo/Iterator.h    
    Gc/System/Algo/Sort/Heap.h    
    
    # Math
    Gc/Math/Basic.h
    Gc/Math/Constant.h
    Gc/Math/ConvergenceException.h
    Gc/Math/Algebra/Number.h
    Gc/Math/Algebra/Vector.h
    Gc/Math/Algebra/Hyperplane.h
    Gc/Math/Algebra/Matrix.h
    Gc/Math/Algebra/SquareMatrix.h
    Gc/Math/Algebra/Eigenvectors.cpp
    Gc/Math/Algebra/Eigenvectors.h        
    Gc/Math/Geometry/ConvexHull.cpp
    Gc/Math/Geometry/ConvexHull.h
    Gc/Math/Geometry/Voronoi.cpp
    Gc/Math/Geometry/Voronoi.h
        
    # Maximum Flow
    Gc/Flow/Type.h
    Gc/Flow/IMaxFlow.h        
    Gc/Flow/IGridMaxFlow.h
    Gc/Flow/General/FordFulkerson.cpp    
    Gc/Flow/General/FordFulkerson.h
    Gc/Flow/General/EdmondsKarp.cpp    
    Gc/Flow/General/EdmondsKarp.h
    Gc/Flow/General/Dinitz.cpp    
    Gc/Flow/General/Dinitz.h
    Gc/Flow/General/BoykovKolmogorov.cpp
    Gc/Flow/General/BoykovKolmogorov.h    
    Gc/Flow/General/PushRelabel/Fifo.cpp    
    Gc/Flow/General/PushRelabel/Fifo.h
    Gc/Flow/General/PushRelabel/FifoGap.cpp    
    Gc/Flow/General/PushRelabel/FifoGap.h    
    Gc/Flow/General/PushRelabel/HighestLevel.cpp    
    Gc/Flow/General/PushRelabel/HighestLevel.h    
    Gc/Flow/General/Kohli.cpp
    Gc/Flow/General/Kohli.h
    Gc/Flow/Grid/CommonBase.h
    Gc/Flow/Grid/CommonBase.cpp    
    Gc/Flow/Grid/Kohli.cpp
    Gc/Flow/Grid/Kohli.h
    Gc/Flow/Grid/ZengDanek.cpp
    Gc/Flow/Grid/ZengDanek.h
    Gc/Flow/Grid/DanekLabels.cpp
    Gc/Flow/Grid/DanekLabels.h
    Gc/Flow/Grid/PushRelabel/HighestLevel.cpp    
    Gc/Flow/Grid/PushRelabel/HighestLevel.h    
    Gc/Flow/Grid/PushRelabel/Fifo.cpp    
    Gc/Flow/Grid/PushRelabel/Fifo.h    
        
    # Energy
    Gc/Energy/Neighbourhood.h
    Gc/Energy/IEnergy.h
    Gc/Energy/IGridEnergy.h
    Gc/Energy/Potential/Metric/RiemannianDanek.cpp
    Gc/Energy/Potential/Metric/RiemannianDanek.h
    Gc/Energy/Potential/Metric/RiemannianBoykov.cpp
    Gc/Energy/Potential/Metric/RiemannianBoykov.h    
    Gc/Energy/Min/Grid/AlphaBetaSwap.h
    Gc/Energy/Min/Grid/AlphaBetaSwap.cpp
    Gc/Energy/Min/Grid/AlphaExpansion.h
    Gc/Energy/Min/Grid/AlphaExpansion.cpp
    Gc/Energy/Min/Grid/LabelingEnergy.h
        
    # Data
    Gc/Data/Image.cpp
    Gc/Data/Image.h
    Gc/Data/BitField.h
    Gc/Data/BorderIterator.h
    
    # Algo
    Gc/Algo/Segmentation/Tools.cpp
    Gc/Algo/Segmentation/Tools.h
    Gc/Algo/Segmentation/Mask.h
    Gc/Algo/Segmentation/ChanVese.cpp
    Gc/Algo/Segmentation/ChanVese.h
    Gc/Algo/Segmentation/RoussonDeriche.cpp
    Gc/Algo/Segmentation/RoussonDeriche.h
    Gc/Algo/Segmentation/MumfordShah.cpp
    Gc/Algo/Segmentation/MumfordShah.h
    Gc/Algo/Geometry/DistanceTransform.cpp
    Gc/Algo/Geometry/DistanceTransform.h
    Gc/Algo/Geometry/SimplePoint.cpp
    Gc/Algo/Geometry/SimplePoint.h
    Gc/Algo/Clustering/KMeans.cpp
    Gc/Algo/Clustering/KMeans.h
    )

########################
#  Add project
########################

set(GC_LIBRARY_NAME_RELEASE "Gc" CACHE STRING "The name of the library.")
set(GC_LIBRARY_NAME_DEBUG "Gcd" CACHE STRING "The name of debug library.")
add_library(Gc ${GC_SOURCES})
set_target_properties(Gc PROPERTIES VERSION 1.0.0 
    RELEASE_OUTPUT_NAME ${GC_LIBRARY_NAME_RELEASE} 
    DEBUG_OUTPUT_NAME ${GC_LIBRARY_NAME_DEBUG})

########################
#  Install project
########################

# install header files
install(DIRECTORY Gc DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)

# install the library
install(TARGETS Gc
	  RUNTIME DESTINATION bin
	  LIBRARY DESTINATION lib
	  ARCHIVE DESTINATION lib)
