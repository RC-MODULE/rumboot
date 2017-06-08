function(add_source SRC)
        SET(PLATFORM_SOURCES "${PLATFORM_SOURCES};${CMAKE_SOURCE_DIR}/${SRC}" PARENT_SCOPE)
endfunction()

function(add_boot_source KEY SRC)
    if (${${KEY}})
        SET(PLATFORM_SOURCES "${PLATFORM_SOURCES};${CMAKE_SOURCE_DIR}/src/bootsrc/${SRC}" PARENT_SCOPE)
    endif()
endfunction()

macro(add_rumboot_test name_ binary baseclass)
    string(TOLOWER "${name_}" name)
    STRING(REGEX REPLACE "-" "_" name ${name})
    if (${RUMBOOT_PLATFORM} MATCHES "native")
      add_test(${name} ${binary} ${ARGN})
    else()
        file(APPEND ${RUMBOOT_MANIFEST}
        "${baseclass} test_rumboot_${name} ${binary}.bin")
        foreach(arg ${ARGN})
                file(APPEND ${RUMBOOT_MANIFEST} " ${arg}")
        endforeach()
        file(APPEND ${RUMBOOT_MANIFEST} "\n")
    endif()
endmacro()

macro(generate_stuff_for_target product)
  add_custom_command(
    OUTPUT ${product}.dmp
    COMMAND ${CROSS_COMPILE}-objdump -D ${CMAKE_DUMP_FLAGS} ${product} > ${product}.dmp
    COMMENT "Generating disassembly listing: ${product}.dmp"
    DEPENDS ${product}
  )

  add_custom_command(
    OUTPUT ${product}.bin
    COMMAND ${CROSS_COMPILE}-objcopy ${CMAKE_OBJCOPY_FLAGS} -O binary ${product} ${product}.bin
    COMMENT "Generating binary image ${product}.bin"
    DEPENDS ${product}
  )

  add_custom_target(
    objcopy-${product} ALL
    DEPENDS ${product}.bin ${product}.dmp
  )

endmacro()

macro(add_rumboot_target target)
    GET_FILENAME_COMPONENT(f ${target} NAME_WE)
    SET(product rumboot-${RUMBOOT_PLATFORM}-${CMAKE_BUILD_TYPE}-${f})
    add_executable(${product} ${target} $<TARGET_OBJECTS:rumboot>)
    target_link_libraries(${product} ${RUMBOOT_PRIMARY_LD_FLAGS} -Wl,-Map,${product}.map)
    install(TARGETS ${product} RUNTIME DESTINATION rumboot)

    generate_stuff_for_target(${product})
    install(FILES ${CMAKE_BINARY_DIR}/${product}.dmp DESTINATION rumboot)
    install(FILES ${CMAKE_BINARY_DIR}/${product}.bin DESTINATION rumboot)
    install(FILES ${CMAKE_BINARY_DIR}/${product}.map DESTINATION rumboot)

    rumboot_platform_generate_stuff_for_taget(${product})

    # Native platform is special - we do unit-testing!
    if (${RUMBOOT_PLATFORM} MATCHES "native")
      add_test(${product} ${product})
    endif()

    set(LAST_PRODUCT_NAME "${product}")
endmacro()


macro(add_platform_test NAME)

  #Production builds take a shitload of time
  if (${CMAKE_BUILD_TYPE} MATCHES "Production")
      set(TIMEOUT_OVERRIDE "TIMEOUT=4000ms")
  else()
      set(TIMEOUT_OVERRIDE "")
  endif()

  if (${RUMBOOT_PLATFORM} MATCHES "native")
    add_rumboot_test(${NAME} ${LAST_PRODUCT_NAME} null ${ARGN})
  else()
    add_rumboot_test(${RUMBOOT_PLATFORM}_${CMAKE_BUILD_TYPE}_${NAME} ${LAST_PRODUCT_NAME} ${ARGN} ${TIMEOUT_OVERRIDE})
  endif()
endmacro()

macro(add_platform_binary relname)
  set(fname ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/${relname})
  add_rumboot_target(${fname} ${ARGN})
endmacro()

macro(add_platform_image target)
  set(image_srcs "")
  foreach(f ${ARGN})
    LIST(APPEND image_srcs src/platform/${RUMBOOT_PLATFORM}/images/${f})
  endforeach()
  LIST(APPEND image_srcs ${RUMBOOT_PLATFORM_REQUIRED_FILES})

  add_executable(${target} ${image_srcs})
  target_link_libraries(${target} ${RUMBOOT_SECONDARY_LD_FLAGS})
  generate_stuff_for_target(${target})
endmacro()



macro(pack_platform_image target suffix epv)
  add_custom_command(
    OUTPUT ${target}_${suffix}.img
    COMMAND rumboot-packimage `${CMAKE_SOURCE_DIR}/scripts/get_ep_mm7705 ${epv} ${target}.dmp`
            -t ${ARGN} -i ${target}.bin -o ${target}_${suffix}.img
    COMMENT "Generating packed loader image ${target}_${suffix}.img"
    DEPENDS ${target}.bin
  )

  add_custom_command(
    OUTPUT ${target}_${suffix}.sd
    COMMAND dd status=none if=/dev/zero of=${target}_${suffix}.sd bs=1024 count=8
    COMMAND dd status=none if=${target}_${suffix}.img of=${target}_${suffix}.sd bs=1024 seek=8
    COMMENT "Creating SD card image ${target}_${suffix}.sd"
    DEPENDS ${target}_${suffix}.img
  )

  add_custom_command(
    OUTPUT ${target}_${suffix}.spi
    COMMAND dd status=none if=/dev/zero of=${target}_${suffix}.spi bs=1K count=4096
    COMMAND dd status=none if=${target}_${suffix}.img of=${target}_${suffix}.spi conv=notrunc
    COMMENT "Creating SPI flash image ${target}_${suffix}.spi"
    DEPENDS ${target}_${suffix}.img
  )

  add_custom_command(
    OUTPUT ${target}_${suffix}.im0
    COMMAND dd status=none if=/dev/zero of=${target}_${suffix}.spi bs=1K count=256
    COMMAND dd status=none if=${target}_${suffix}.img of=${target}_${suffix}.im0 conv=notrunc
    COMMENT "Creating IM0 padded image ${target}_${suffix}.im0"
    DEPENDS ${target}_${suffix}.img
  )

  add_custom_command(
    OUTPUT ${target}_${suffix}.hx
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/gen_hx_file.sh ${target}_${suffix}.img > ${target}_${suffix}.hx
    COMMENT "Creating HX image ${target}_${suffix}.hx"
    DEPENDS ${target}_${suffix}.img
  )

  add_custom_command(
    OUTPUT ${target}_${suffix}.edcl_frames
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/gen_eth_frames.pl ${target}_${suffix}.hx ${target}_${suffix}.edcl_frames
    COMMENT "Creating edcl frames file ${target}_${suffix}.edcl_frames"
    DEPENDS ${target}_${suffix}.hx
  )

  add_custom_target(
    pack-${target}_${suffix} ALL
    DEPENDS ${target}_${suffix}.spi ${target}_${suffix}.sd ${target}_${suffix}.img ${target}_${suffix}.im0 ${target}_${suffix}.edcl_frames
  )

  install(FILES ${CMAKE_BINARY_DIR}/${target}_${suffix}.img DESTINATION rumboot/images)
  install(FILES ${CMAKE_BINARY_DIR}/${target}_${suffix}.sd DESTINATION rumboot/images)
  install(FILES ${CMAKE_BINARY_DIR}/${target}_${suffix}.spi DESTINATION rumboot/images)
  install(FILES ${CMAKE_BINARY_DIR}/${target}_${suffix}.im0 DESTINATION rumboot/images)
  install(FILES ${CMAKE_BINARY_DIR}/${target}_${suffix}.edcl_frames DESTINATION rumboot/images)
  install(FILES ${CMAKE_BINARY_DIR}/${target}_${suffix}.hx DESTINATION rumboot/images)
endmacro()
