################################################################################
# CopyResources
################################################################################
function(CopyResources src_dir dest_dir)
  file(GLOB files ${src_dir})

  foreach(item IN LISTS files)
  get_filename_component(dest_filename ${item} NAME)

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${item} 
      ${dest_dir}/${dest_filename}
  )
  endforeach()
endfunction()