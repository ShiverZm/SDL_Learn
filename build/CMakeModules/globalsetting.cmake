## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(Dart)

 # ����Ԥ����ͷ
# Target����������Ԥ����ͷ����ĿTarget��PrecompiledHeader��PrecompiledSource�ֱ���ͷ�ļ���·��
MACRO(ADD_PRECOMPILED_HEADER
    Target PrecompiledHeader PrecompiledSource)
 
  IF(MSVC)
    ADD_MSVC_PRECOMPILED_HEADER(${PrecompiledHeader}
      ${PrecompiledSource})
  ENDIF(MSVC)
 
  IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_GCC_PRECOMPILED_HEADER(${Target} ${PrecompiledHeader})
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDMACRO(ADD_PRECOMPILED_HEADER)
 
# ������һ��Ԥ����ͷ�Ժ�Ϳ��Ե����������ָ��Դ�ļ���ʹ����
# SourcesVar��һ��������Ҫʹ�ø�Ԥ����ͷ��Դ�ļ��б�
MACRO(USE_PRECOMPILED_HEADER SourcesVar)
  IF(MSVC)
    USE_MSVC_PRECOMPILED_HEADER(${SourcesVar})
  ENDIF(MSVC)
 
  IF(CMAKE_COMPILER_IS_GNUCXX)
    USE_GCC_PRECOMPILED_HEADER(${SourcesVar})
  ENDIF()
ENDMACRO(USE_PRECOMPILED_HEADER)
 
# ����Ϊvisual studio����pch
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource)
  GET_FILENAME_COMPONENT(PrecompiledBasename
    ${PrecompiledHeader} NAME_WE)
 
  # �õ�pch�ļ����ļ���
  SET(PrecompiledBinary
    "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
 
  SET(PCH_BIN_PATH
    ${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch
    CACHE INTERNAL "the path of the precompiled binary")
  SET(PCH_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/${PrecompiledHeader}
    CACHE INTERNAL "the path of the precompiled header")
 
  # Ϊvisual studio������������pch�ı���������
  SET_SOURCE_FILES_PROPERTIES(
    ${PrecompiledSource}
    PROPERTIES COMPILE_FLAGS
    "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
    OBJECT_OUTPUTS "${PrecompiledBinary}")
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)
 
MACRO(USE_MSVC_PRECOMPILED_HEADER SourcesVar)
  SET(Sources ${${SourcesVar}})
 
  message("using pch: ${PCH_BIN_PATH} for ${${SourcesVar}}")
 
  set(PrecompiledBinary ${PCH_BIN_PATH})
 
  # ����Ҫʹ��pch��Դ�ļ����ñ���������
  SET_SOURCE_FILES_PROPERTIES(${Sources}
    PROPERTIES COMPILE_FLAGS
    "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
    OBJECT_DEPENDS "${PrecompiledBinary}")
ENDMACRO(USE_MSVC_PRECOMPILED_HEADER)
 
# �ĺ��������ж�gcc�Ƿ�֧��Ԥ����ͷ
IF(CMAKE_COMPILER_IS_GNUCXX)
  EXEC_PROGRAM(
    ${CMAKE_CXX_COMPILER}
    ARGS                    --version
    OUTPUT_VARIABLE _compiler_output)
  STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1"
    gcc_compiler_version ${_compiler_output})
  #MESSAGE("GCC Version: ${gcc_compiler_version}")
  IF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    SET(PCHSupport_FOUND TRUE)
  ELSE(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    IF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
      SET(PCHSupport_FOUND TRUE)
    ENDIF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
  ENDIF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
 
MACRO(USE_GCC_PRECOMPILED_HEADER SourcesVar)
  SET(Sources ${${SourcesVar}})
 
  # ͨ��-include����������Դ�ļ�ǿ�ư���Ԥ����ͷ
  FOREACH(source ${_sourcesVar})
    SET_SOURCE_FILES_PROPERTIES(${source}
      PROPERTIES
      COMPILE_FLAGS "-include ${_name} -Winvalid-pch"
      OBJECT_DEPENDS "${PCH_PATH}")
  ENDFOREACH(source)
ENDMACRO(USE_GCC_PRECOMPILED_HEADER)
 
MACRO(ADD_GCC_PRECOMPILED_HEADER TargetName PrecompileHeader)
  SET(_compile_FLAGS ${CMAKE_CXX_FLAGS})
 
  SET(_input "${CMAKE_CURRENT_SOURCE_DIR}/${PrecompileHeader}")
  MESSAGE("creating pch: ${_input}")
  GET_FILENAME_COMPONENT(_name ${_input} NAME)
  GET_FILENAME_COMPONENT(_path ${_input} PATH)
 
  # ���ݲ�ͬ�ı�����������Ԥ����ͷ�������ļ����ļ���������debug.c++, release.c++
  SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
  IF(CMAKE_BUILD_TYPE)
    SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.c++")
    LIST(APPEND _compile_FLAGS ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}})
  ELSE(CMAKE_BUILD_TYPE)
    SET(_output "${_outdir}/default.c++")
  ENDIF(CMAKE_BUILD_TYPE)
 
  IF(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    LIST(APPEND _compile_FLAGS "-DQT_DEBUG")
  ENDIF()
 
  # �Զ������ļ���
  ADD_CUSTOM_COMMAND(
    OUTPUT ${_outdir}
    COMMAND mkdir ${_outdir} # TODO: {CMAKE_COMMAND} -E ...
    )
 
  GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
 
  # ȷ������Ԥ����ͷ���ļ��������а���Ŀ¼����ǰ��
  SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path FALSE)
  FOREACH(item ${_directory_flags})
    IF(${item}  STREQUAL ${_path} AND NOT
        _CMAKE_CURRENT_BINARY_DIR_included_before_path )
      MESSAGE(FATAL_ERROR
        "This is the ADD_PRECOMPILED_HEADER macro. "
        "CMAKE_CURREN_BINARY_DIR has to mentioned at INCLUDE_DIRECTORIES's argument list before ${_path}, where ${_name} is located"
        )
    ENDIF(${item}  STREQUAL ${_path} AND NOT
      _CMAKE_CURRENT_BINARY_DIR_included_before_path )
 
    IF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
      SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path TRUE)
    ENDIF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
 
    LIST(APPEND _compile_FLAGS "-I${item}")
  ENDFOREACH(item)
 
  GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
  #LIST(APPEND _compile_FLAGS "-fPIC")
  LIST(APPEND _compile_FLAGS ${_directory_flags})
 
  SEPARATE_ARGUMENTS(_compile_FLAGS)
  #MESSAGE("_compiler_FLAGS: ${_compile_FLAGS}")
 
  # ����ͷ�ļ���binaryĿ¼
  ADD_CUSTOM_COMMAND(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    COMMAND ${CMAKE_COMMAND} -E copy  ${_input}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    )
 
  SET(PCH_PATH ${CMAKE_CURRENT_BINARY_DIR}/${_name} CACHE INTERNAL
    "the path of the precompiled header")
 
  #MESSAGE("command : ${CMAKE_COMMAND} -E copy  ${_input}
  #  ${CMAKE_CURRENT_BINARY_DIR}/${_name}")
 
  # �����������Ԥ����ͷ������
  ADD_CUSTOM_COMMAND(
    OUTPUT ${_output}
    COMMAND ${CMAKE_CXX_COMPILER}
    ${_compile_FLAGS}
    -x c++-header
    -o ${_output}
    ${_input}
    DEPENDS ${_input} ${_outdir} ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    )
  ADD_CUSTOM_TARGET(${TargetName}_gch
    DEPENDS ${_output}
    )
  ADD_DEPENDENCIES(${TargetName} ${TargetName}_gch )
ENDMACRO(ADD_GCC_PRECOMPILED_HEADER)


#INCLUDE (MSVCPCH)


if (WIN32)
    set(myoutputdirectory ${your_source_file_SOURCE_DIR}/output/win/32)
elseif (CMAKE_COMPILER_IS_GNUCC)
    set(myoutputdirectory ${your_source_file_SOURCE_DIR}/output/linux/32)
elseif(APPLE)
    set(myoutputdirectory ${your_source_file_SOURCE_DIR}/output/mac/32)
endif (WIN32)

#
SET (COMMON_COMPILER_OPTIONS "")
SET (EXT_DIR ${CMAKE_CURRENT_BINARY_DIR}/../ext)
SET (COMMON_DIR ${CMAKE_CURRENT_BINARY_DIR}/..)

if (WIN32)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_RELEASE_MD
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_RELEASE_MD
        )
	if (CMAKE_X64)
		SET (Curlib_LIB ${EXT_DIR}/curlib/win64)	
		SET (Tag_LIB ${EXT_DIR}/taglib/win64)	
		SET (OpenSSL_LIB ${EXT_DIR}/openssl/win64)	
		SET (FreeImage_LIB ${EXT_DIR}/freeimage/win64)		
		SET (BOOST_LIB ${EXT_DIR}/boost/win64)
		ADD_DEFINITIONS(-DXW64)
	else(CMAKE_X64)
		SET (Curlib_LIB ${EXT_DIR}/curlib/win32)	
		SET (Tag_LIB ${EXT_DIR}/taglib/win32)	
		SET (OpenSSL_LIB ${EXT_DIR}/openssl/win32)	
		SET (FreeImage_LIB ${EXT_DIR}/freeimage/win32)		
		SET (BOOST_LIB ${EXT_DIR}/boost/win32)
	endif (CMAKE_X64)
	SET(CMAKE_CXX_FLAGS_DEBUG "/MDd /Od /Zi" CACHE STRING "" FORCE)
	SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /D NDEBUG" CACHE STRING "" FORCE)
	SET(CMAKE_CXX_FLAGS_RELEASE_MD "/MD /O2 /D NDEBUG" CACHE STRING "" FORCE)
	SET (LINK_FLAGS "${LINK_FLAGS}  /DEBUG")
else(WIN32)
	if (CMAKE_X64)
		SET (Curlib_LIB ${EXT_DIR}/curlib/mac64)	
		SET (OpenSSL_LIB ${EXT_DIR}/openssl/mac64)	
		SET (Tag_LIB ${EXT_DIR}/taglib/mac64)	
		SET (BOOST_LIB ${EXT_DIR}/boost/mac64)
		SET (FreeImage_LIB ${EXT_DIR}/freeimage/mac64) 
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		ADD_DEFINITIONS(-DXW64)
	else(CMAKE_X64)
		SET (Curlib_LIB ${EXT_DIR}/curlib/mac)	
		SET (OpenSSL_LIB ${EXT_DIR}/openssl/mac)	
		SET (Tag_LIB ${EXT_DIR}/taglib/mac)	
		SET (BOOST_LIB ${EXT_DIR}/boost/mac)
		SET (FreeImage_LIB ${EXT_DIR}/freeimage/mac) 
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
	endif (CMAKE_X64)

endif (WIN32)

SET (COMMON_LIB ${COMMON_DIR}/lib)
SET (COMMON_INCLUDE ${COMMON_DIR}/include)
