##############################################################################n
# Find OpenCascade
#
# This sets the following variables:
# OPENCASCADE_FOUND - True if OpenCascade was found.
# OPENCASCADE_INCLUDE_DIRS - Directories containing the OpenCascade include files.
# OPENCASCADE_LIBRARY_DIRS - Directories containing the OpenCascade library.
# OPENCASCADE_LIBRARIES - OpenCascade library files.

find_path(OPENCASCADE_INCLUDE_DIR "gp_Pnt.hxx" PATHS "/usr/include" "/usr/local/include" "$ENV{PROGRAMFILES}" "/usr/x86_64-w64-mingw32/include/oce" PATH_SUFFIXES "oce")
set(OPENCASCADE_INCLUDE_DIRS ${OPENCASCADE_INCLUDE_DIR})

#set(OPENCASCADE_LIB_SEARCH "TKernel" "TKMath" "TKBRep" "TKGeomBase" "TKGeomAlgo" "TKG3d" "TKG2d" "TKShHealing" "TKTopAlgo" "TKMesh" "TKPrim" "TKBool" "TKBO" "TKFillet" "TKSTEP" "TKSTEPBase" "TKSTEPAttr" "TKXSBase" "TKSTEP209" "TKIGES" "TKOffset")
set(OPENCASCADE_LIB_SEARCH "TKernel" "TKMath" "TKBRep" "TKGeomBase" "TKGeomAlgo" "TKG3d" "TKG2d" "TKShHealing" "TKTopAlgo" "TKMesh" "TKPrim" "TKBool" "TKBO" "TKFillet" "TKOffset")
#set(OPENCASCADE_LIB_SEARCH "FWOSPlugin" "PTKernel" "TKAdvTools" "TKBin" "TKBinL" "TKBinTObj" "TKBinXCAF" "TKBO" "TKBool" "TKBRep" "TKCAF" "TKCDF" "TKernel" "TKFeat" "TKFillet" "TKG2d" "TKG3d" "TKGeomAlgo" "TKGeomBase" "TKHLR" "TKIGES" "TKLCAF" "TKMath" "TKMesh" "TKOffset" "TKPCAF" "TKPLCAF" "TKPrim" "TKPShape" "TKService" "TKShapeSchema" "TKShHealing" "TKStdLSchema" "TKStdSchema" "TKSTEP209" "TKSTEPAttr" "TKSTEPBase" "TKSTEP" "TKSTL" "TKTObj" "TKTopAlgo" "TKV3d" "TKVRML" "TKXCAF" "TKXCAFSchema" "TKXDEIGES" "TKXDESTEP" "TKXMesh" "TKXml" "TKXmlL" "TKXmlTObj" "TKXmlXCAF" "TKXSBase" )

set(OPENCASCADE_LIBRARY_DIRS "")
set(OPENCASCADE_LIBRARIES "")
foreach(ITEM IN LISTS OPENCASCADE_LIB_SEARCH)
    unset(ITEM_PATH CACHE)

    unset(ITEM_LIBRARY CACHE)
    find_library(ITEM_PATH ${ITEM} PATHS "/usr/lib" "/usr/local/lib" "$ENV{PROGRAMFILES}" "/usr/x86_64-w64-mingw32/Win64/lib")
    if(EXISTS ${ITEM_PATH})
        get_filename_component(ITEM_LIBRARY ${ITEM_PATH} NAME)
        find_path(ITEM_LIBRARY_DIR ${ITEM_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/Win64/lib")
        set(OPENCASCADE_LIBRARY_DIRS ${ITEM_LIBRARY_DIR})
        LIST(APPEND OPENCASCADE_LIBRARIES ${ITEM_LIBRARY})
    endif()
endforeach()

message(STATUS "libs: ${OPENCASCADE_LIBRARIES}")
message(STATUS "lib dirs: ${OPENCASCADE_LIBRARY_DIRS}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenCascade DEFAULT_MSG OPENCASCADE_INCLUDE_DIRS OPENCASCADE_LIBRARIES OPENCASCADE_LIBRARY_DIRS)

mark_as_advanced(OPENCASCADE_INCLUDE_DIR)
mark_as_advanced(OPENCASCADE_LIBRARY_DIR)
mark_as_advanced(OPENCASCADE_LIBRARY)
mark_as_advanced(OPENCASCADE_LIBRARY_PATH)
