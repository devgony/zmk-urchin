if(CONFIG_ZMK_PROFILE_LAYER_SYNC)
    zephyr_library()
    zephyr_library_sources(src/profile_layer_sync.c)
    # ZMK app headers (zmk/event_manager.h etc.) are not propagated to module
    # libraries; CMAKE_SOURCE_DIR is zmk/app in a Zephyr build.
    zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
endif()
