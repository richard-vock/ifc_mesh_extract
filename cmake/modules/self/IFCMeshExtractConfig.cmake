###############################################################################
# Find IFCMeshExtract
#
# This sets the following variables:
# IFCMESHEXTRACT_FOUND - True if IFCMeshExtract was found.
# IFCMESHEXTRACT_INCLUDE_DIRS - Directories containing the IFCMeshExtract include files.
# IFCMESHEXTRACT_LIBRARY_DIRS - Directories containing the IFCMeshExtract library.
# IFCMESHEXTRACT_LIBRARIES - IFCMeshExtract library files.

if(WIN32)
    find_path(IFCMESHEXTRACT_INCLUDE_DIR ifc_mesh_extract PATHS "/usr/include" "/usr/local/include" "/usr/x86_64-w64-mingw32/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)

    find_library(IFCMESHEXTRACT_LIBRARY_PATH ifc_mesh_extract PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib" NO_DEFAULT_PATHS)

    if(EXISTS ${IFCMESHEXTRACT_LIBRARY_PATH})
        get_filename_component(IFCMESHEXTRACT_LIBRARY ${IFCMESHEXTRACT_LIBRARY_PATH} NAME)
        find_path(IFCMESHEXTRACT_LIBRARY_DIR ${IFCMESHEXTRACT_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib" NO_DEFAULT_PATHS)
    endif()
else(WIN32)
    find_path(IFCMESHEXTRACT_INCLUDE_DIR ifc_mesh_extract PATHS "/usr/include" "/usr/local/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)
    find_library(IFCMESHEXTRACT_LIBRARY_PATH ifc_mesh_extract PATHS "/usr/lib" "/usr/local/lib" NO_DEFAULT_PATHS)

    if(EXISTS ${IFCMESHEXTRACT_LIBRARY_PATH})
        get_filename_component(IFCMESHEXTRACT_LIBRARY ${IFCMESHEXTRACT_LIBRARY_PATH} NAME)
        find_path(IFCMESHEXTRACT_LIBRARY_DIR ${IFCMESHEXTRACT_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" NO_DEFAULT_PATHS)
    endif()
endif(WIN32)

set(IFCMESHEXTRACT_INCLUDE_DIRS ${IFCMESHEXTRACT_INCLUDE_DIR})
set(IFCMESHEXTRACT_LIBRARY_DIRS ${IFCMESHEXTRACT_LIBRARY_DIR})
set(IFCMESHEXTRACT_LIBRARIES ${IFCMESHEXTRACT_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IFCMeshExtract DEFAULT_MSG IFCMESHEXTRACT_INCLUDE_DIR IFCMESHEXTRACT_LIBRARY IFCMESHEXTRACT_LIBRARY_DIR)

mark_as_advanced(IFCMESHEXTRACT_INCLUDE_DIR)
mark_as_advanced(IFCMESHEXTRACT_LIBRARY_DIR)
mark_as_advanced(IFCMESHEXTRACT_LIBRARY)
mark_as_advanced(IFCMESHEXTRACT_LIBRARY_PATH)
