# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

# Require at least version 2.8.12 of CMake
cmake_minimum_required(VERSION 2.8.12)
project(InstrumentationEngine)

find_package(LibXml2 REQUIRED)

find_package(PkgConfig REQUIRED)

pkg_search_module(UUID REQUIRED uuid)

include_directories(. ../InstrumentationEngine.Lib ${LIBXML2_INCLUDE_DIR} ${UUID_INCLUDE_DIRS})

link_directories(/usr/local/opt/util-linux/lib/)
build_init(CPP InstrumentationEngine)

set(src_files
    ./stdafx.cpp
    ../InstrumentationEngine.Api/InstrumentationEngine_i.cpp
    ./AppDomainCollection.cpp
    ./AppDomainInfo.cpp
    ./ArrayType.cpp
    ./AssemblyInfo.cpp
    #./AssemblyInjector.cpp
    #./AtlModule.cpp
    ./CompositeType.cpp
    ./CorHeaders.cpp
    ./CorMethodMalloc.cpp
    ./CorProfilerFunctionControlWrapper.cpp
    ./CorProfilerInfoWrapper.cpp
    ./dllmain.cpp
    ./ExceptionClause.cpp
    ./ExceptionSection.cpp
    ./FunctionType.cpp
    ./GenericInstanceType.cpp
    ./GenericParameterType.cpp
    ./InlineSite.cpp
    ./InlineSiteMap.cpp
    ./Instruction.cpp
    ./InstructionFactory.cpp
    ./InstructionGraph.cpp
    ./LocalVariableCollection.cpp
    ./MethodInfo.cpp
    ./MethodJitInfo.cpp
    ./MethodLocal.cpp
    ./MethodParameter.cpp
    ./ModifierType.cpp
    ./ModuleInfo.cpp
    ./ProfilerManager.cpp
    ./ProfilerManagerForInstrumentationMethod.cpp
    ./SignatureBuilder.cpp
    ./SimpleType.cpp
    ./SingleRetDefaultInstrumentation.cpp
    #./StrongName.cpp
    ./TokenType.cpp
    ./Type.cpp
    ./TypeCreator.cpp
)

add_lib(${PROJECT_NAME}
    SHARED
    true # use_redefines
    false # hide_symbols
    ${src_files}
    )

# Force every object to be included even if it's unused.
SET (ATL atl)
SET (CORECLRPAL coreclrpal)
SET (LINUXPAL linux_pal)
#SET (LINUXPALRT palrt)


# For some reason linking against musl-libc we need libintl and on glibc we need libdl.
# They have nothing to do with one another, each is just a quirk of the environment.
if(CLR_CMAKE_PLATFORM_ALPINE_LINUX)
    target_link_libraries(${PROJECT_NAME} :libintl.so)
else()
    # NOTE: Explicitly link to the .so version of libdl.so to make sure we don't somehow accidentally
    # start statically linking to these, as these are LGPL components.
    target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
endif()

target_link_libraries(${PROJECT_NAME}
    pthread
    stdc++
    ${LIBXML2_LIBRARIES}
#    ${CORECLRPAL}
    ${LINUXPAL}
    ${ATL}
#    palrt
    InstrumentationEngine.Api
    InstrumentationEngine.Lib
    corguids
    /usr/local/opt/util-linux/lib/libuuid.dylib
    /usr/local/Cellar/llvm/11.0.0_1/lib/libunwind.dylib
    )

#ensure that the linux pal gets built before this
add_dependencies(${PROJECT_NAME} atl)
add_dependencies(${PROJECT_NAME} linux_pal)
