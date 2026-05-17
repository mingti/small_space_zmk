if (CONFIG_BT)
    zephyr_library()
    # 将我们马上要写的 C 源码文件加入编译队列
    zephyr_library_sources(custom_bt_name.c)
endif()