ivw_module(TNM067Lab2)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/hydrogengenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/marchingtetrahedra.h
)
ivw_group("Header Files" ${HEADER_FILES})

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/hydrogengenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/marchingtetrahedra.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


set(SHADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/glsl/tnm067lab2processor.frag
)
ivw_group("Shader Files" ${SHADER_FILES})

set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/hydrogen-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/tnm067lab2-unittest-main.cpp
)
ivw_add_unittest(${TEST_FILES})

ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)
ivw_folder(inviwo-module-tnm067lab2 TNM067)
