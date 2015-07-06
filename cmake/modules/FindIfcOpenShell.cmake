###############################################################################
# Find IfcOpenShell
#
# This sets the following variables:
# IFCOPENSHELL_FOUND - True if IfcOpenShell was found.
# IFCOPENSHELL_INCLUDE_DIRS - Directories containing the IfcOpenShell include files.
# IFCOPENSHELL_LIBRARY_DIRS - Directories containing the IfcOpenShell library.
# IFCOPENSHELL_LIBRARIES - IfcOpenShell library files.

find_path(IFCOPENSHELL_INCLUDE_DIR ifcgeom
    PATHS "/usr/include" "/usr/local/include" "$ENV{PROGRAMFILES}" "/usr/x86_64-w64-mingw32/include")

find_library(IFCOPENSHELL_GEOM_LIBRARY_PATH IfcGeom PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib")
find_library(IFCOPENSHELL_PARSE_LIBRARY_PATH IfcParse PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib")

if(EXISTS ${IFCOPENSHELL_GEOM_LIBRARY_PATH} AND EXISTS ${IFCOPENSHELL_PARSE_LIBRARY_PATH})
    get_filename_component(IFCOPENSHELL_GEOM_LIBRARY ${IFCOPENSHELL_GEOM_LIBRARY_PATH} NAME)
    get_filename_component(IFCOPENSHELL_PARSE_LIBRARY ${IFCOPENSHELL_PARSE_LIBRARY_PATH} NAME)
    find_path(IFCOPENSHELL_GEOM_LIBRARY_DIR ${IFCOPENSHELL_GEOM_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib")
    find_path(IFCOPENSHELL_PARSE_LIBRARY_DIR ${IFCOPENSHELL_PARSE_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib")
endif()
set(IFCOPENSHELL_LIBRARY ${IFCOPENSHELL_GEOM_LIBRARY} ${IFCOPENSHELL_PARSE_LIBRARY})

set(IFCOPENSHELL_INCLUDE_DIRS ${IFCOPENSHELL_INCLUDE_DIR})
set(IFCOPENSHELL_LIBRARY_DIRS ${IFCOPENSHELL_GEOM_LIBRARY_DIR} ${IFCOPENSHELL_PARSE_LIBRARY_DIR})
set(IFCOPENSHELL_LIBRARIES ${IFCOPENSHELL_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IfcOpenShell DEFAULT_MSG IFCOPENSHELL_INCLUDE_DIR IFCOPENSHELL_LIBRARY IFCOPENSHELL_LIBRARY_DIRS)

mark_as_advanced(IFCOPENSHELL_INCLUDE_DIR)
mark_as_advanced(IFCOPENSHELL_LIBRARY_DIR)
mark_as_advanced(IFCOPENSHELL_LIBRARY)
mark_as_advanced(IFCOPENSHELL_LIBRARY_PATH)
