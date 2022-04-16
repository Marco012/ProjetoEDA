list(APPEND IMGUI_PUBLIC_HEADERS
        "imgui.h"
        )

list(APPEND IMGUI_PRIVATE_HEADERS
        "imgui_internal.h"
        "imconfig.h"
        "stb_rect_pack.h"
        "stb_textedit.h"
        "stb_truetype.h"
        )

list(APPEND IMGUI_SOURCES
        "imgui.cpp"
        "imgui_draw.cpp"
        "imgui_widgets.cpp"
        "imgui_tables.cpp"
        ${CRYPTO_PUBLIC_HEADERS}
        ${CRYPTO_PRIVATE_HEADERS}
        )

list(APPEND IMGUI_PUBLIC_INCLUDE_DIRS
        ${IMGUI_SRC_DIR}
        )

list(APPEND IMGUI_PUBLIC_LIBRARIES)

set(NAME imgui)
add_library(${NAME} STATIC ${IMGUI_SOURCES})
target_link_libraries(${NAME} PUBLIC ${IMGUI_PUBLIC_LIBRARIES})
target_include_directories(${NAME}
        PUBLIC ${IMGUI_PUBLIC_INCLUDE_DIRS}
        PRIVATE ${IMGUI_PRIVATE_INCLUDE_DIRS}
        )
