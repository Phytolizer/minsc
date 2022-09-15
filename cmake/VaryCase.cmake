macro(vary_case FILEPATH OUTPATH CASE TARGET)
  set(_filepath "${FILEPATH}")
  cmake_path(GET _filepath STEM _filestem)
  set(_outpath "${OUTPATH}")
  cmake_path(
    ABSOLUTE_PATH _outpath BASE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  )
  set(_outfilepath "${_outpath}/${_filestem}_${CASE}.inc")
  cmake_path(ABSOLUTE_PATH _filepath OUTPUT_VARIABLE _infilepath)
  add_custom_target(
    "${TARGET}_${_filestem}_${CASE}"
    DEPENDS varycase::varycase "${_infilepath}"
    COMMAND varycase::varycase ${CASE} "${_infilepath}" -o "${_outfilepath}"
  )
  if(TARGET ${TARGET})
    target_include_directories(
      ${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
    )
    add_dependencies(${TARGET} "${TARGET}_${_filestem}_${CASE}")
  endif()
endmacro()
