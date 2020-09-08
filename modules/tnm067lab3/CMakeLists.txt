ivw_module(TNM067Lab3)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/lineintegralconvolution.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/vectorfieldinformation.h
)
ivw_group("Header Files" ${HEADER_FILES})

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/lineintegralconvolution.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/vectorfieldinformation.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

set(SHADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lineintegralconvolution.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lineintegralconvolution.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/vectorfieldinformation.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/vectorfieldinformation.vert
)
ivw_group("Shader Files" ${SHADER_FILES})

set(TEST_FILES
	#${CMAKE_CURRENT_SOURCE_DIR}/tests/tnm067lab3-test.cpp
)
ivw_add_unittest(${TEST_FILES})

ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)
ivw_folder(inviwo-module-tnm067lab3 TNM067)