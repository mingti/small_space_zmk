#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <stdio.h>

// 这个函数会在系统启动时自动运行
static int init_custom_bt_name(void) {
    bt_addr_le_t addrs[1];
    size_t count = 1;
    
    // 获取当前主控的蓝牙 MAC 地址
    bt_id_get(addrs, &count);
    if (count > 0) {
        char bt_name[32];
        
        // ZMK 底层的 MAC 地址在 addrs[0].a.val 中是倒序（小端）存储的
        // val[0] 是地址的最后两位，val[1] 是倒数三四位
        // 我们提取 MAC 地址的最后两个字节作为 4 位随机码后缀
        snprintf(bt_name, sizeof(bt_name), "%s_%02X%02X", 
                 CONFIG_ZMK_KEYBOARD_NAME, 
                 addrs[0].a.val[1], addrs[0].a.val[0]);
        
        // 调用底层 API 更新蓝牙名称
        bt_set_name(bt_name);
    }
    return 0;
}

// 设定为 APPLICATION 级别，优先级设为 99，确保在蓝牙堆栈初始化完毕后再执行改名
SYS_INIT(init_custom_bt_name, APPLICATION, 99);