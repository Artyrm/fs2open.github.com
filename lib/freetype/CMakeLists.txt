if (UNIX AND NOT FSO_BUILD_INCLUDED_LIBS)
  # Try to use the system libraries on Unix where it may be available
  find_package(Freetype)

  if (FREETYPE_FOUND)
    add_library(freetype INTERFACE)
    target_link_libraries(freetype INTERFACE ${FREETYPE_LIBRARIES})
    target_include_directories(freetype INTERFACE "${FREETYPE_INCLUDE_DIRS}")
    return()
  endif()
endif()

# This is based on the CMakeLists.txt found in the FreeType source

include(CheckIncludeFile)

# Add local cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/builds/cmake)

set(VERSION_MAJOR "2")
set(VERSION_MINOR "7")
set(VERSION_PATCH "1")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config")

# Create the configuration file
if (UNIX)
  check_include_file("unistd.h" HAVE_UNISTD_H)
  check_include_file("fcntl.h" HAVE_FCNTL_H)
  check_include_file("stdint.h" HAVE_STDINT_H)

  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/builds/unix/ftconfig.in" FTCONFIG_H)
  if (HAVE_UNISTD_H)
    string(REGEX REPLACE "#undef +(HAVE_UNISTD_H)" "#define \\1" FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  if (HAVE_FCNTL_H)
    string(REGEX REPLACE "#undef +(HAVE_FCNTL_H)" "#define \\1" FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  if (HAVE_STDINT_H)
    string(REGEX REPLACE "#undef +(HAVE_STDINT_H)" "#define \\1" FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  string(REPLACE "/undef " "#undef " FTCONFIG_H "${FTCONFIG_H}")
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h-new" "${FTCONFIG_H}")
else ()
  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/freetype/config/ftconfig.h" FTCONFIG_H)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h-new" "${FTCONFIG_H}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h-new"
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h")


# Create the options file
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/freetype/config/ftoption.h" FTOPTION_H)

string(REGEX REPLACE "/\\* +(#define +FT_CONFIG_OPTION_SYSTEM_ZLIB) +\\*/" "\\1" FTOPTION_H "${FTOPTION_H}")

string(REGEX REPLACE "/\\* +(#define +FT_CONFIG_OPTION_USE_PNG) +\\*/" "\\1" FTOPTION_H "${FTOPTION_H}")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftoption.h-new" "${FTOPTION_H}")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftoption.h-new"
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftoption.h")



file(GLOB PUBLIC_HEADERS "include/ft2build.h" "include/freetype/*.h")
file(GLOB PUBLIC_CONFIG_HEADERS "include/freetype/config/*.h")
file(GLOB PRIVATE_HEADERS "include/freetype/internal/*.h")


set(BASE_SRCS
  src/autofit/autofit.c
  src/base/ftbase.c
  src/base/ftbbox.c
  src/base/ftbdf.c
  src/base/ftbitmap.c
  src/base/ftcid.c
  src/base/ftfntfmt.c
  src/base/ftfstype.c
  src/base/ftgasp.c
  src/base/ftglyph.c
  src/base/ftgxval.c
  src/base/ftinit.c
  src/base/ftlcdfil.c
  src/base/ftmm.c
  src/base/ftotval.c
  src/base/ftpatent.c
  src/base/ftpfr.c
  src/base/ftstroke.c
  src/base/ftsynth.c
  src/base/ftsystem.c
  src/base/fttype1.c
  src/base/ftwinfnt.c
  src/bdf/bdf.c
  src/bzip2/ftbzip2.c
  src/cache/ftcache.c
  src/cff/cff.c
  src/cid/type1cid.c
  src/gzip/ftgzip.c
  src/lzw/ftlzw.c
  src/pcf/pcf.c
  src/pfr/pfr.c
  src/psaux/psaux.c
  src/pshinter/pshinter.c
  src/psnames/psnames.c
  src/raster/raster.c
  src/sfnt/sfnt.c
  src/smooth/smooth.c
  src/truetype/truetype.c
  src/type1/type1.c
  src/type42/type42.c
  src/winfonts/winfnt.c
)

if (WIN32)
  set(BASE_SRCS ${BASE_SRCS} builds/windows/ftdebug.c)
else ()
  set(BASE_SRCS ${BASE_SRCS} src/base/ftdebug.c)
endif ()

add_library(freetype STATIC
  ${PUBLIC_HEADERS}
  ${PUBLIC_CONFIG_HEADERS}
  ${PRIVATE_HEADERS}
  ${BASE_SRCS}
)

# Disable warnings if building from source
suppress_warnings(freetype)

# Compiler definitions for building the library
target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY)

# Specify library include directories
target_include_directories(freetype PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(freetype BEFORE PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")

target_include_directories(freetype INTERFACE $<INSTALL_INTERFACE:include/freetype2>)

target_link_libraries(freetype PRIVATE zlib)
target_link_libraries(freetype PRIVATE png)

# eof
