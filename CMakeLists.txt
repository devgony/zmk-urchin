if(CONFIG_ZMK_PROFILE_LAYER_SYNC)
    zephyr_library()
    zephyr_library_sources(src/profile_layer_sync.c)
endif()
