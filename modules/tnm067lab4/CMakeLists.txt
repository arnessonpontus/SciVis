ivw_module(TNM067Lab4)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/jacobian.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/glyphrenderer.h
)
ivw_group("Header Files" ${HEADER_FILES})

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/jacobian.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/glyphrenderer.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

set(SHADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/tensor_glyphrenderer.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/tensor_glyphrenderer.geom
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/tensor_glyphrenderer.vert
)
ivw_group("Shader Files" ${SHADER_FILES})

set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/jacobian-test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/tnm067lab4-unittest-main.cpp
)
ivw_add_unittest(${TEST_FILES})

ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)
ivw_folder(inviwo-module-tnm067lab4 TNM067)
