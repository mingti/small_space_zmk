#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <stdio.h>

// 定义一个延时工作队列
static struct k_work_delayable name_update_work;

// 真正执行改名的核心函数
static void update_name_handler(struct k_work *work) {
    bt_addr_le_t addrs[1];
    size_t count = 1;
    
    // 获取当前主控的蓝牙 MAC 地址
    bt_id_get(addrs, &count);
    if (count > 0) {
        char bt_name[32];
        
        // 提取 MAC 地址的最后两个字节作为后缀
        snprintf(bt_name, sizeof(bt_name), "%s_%02X%02X", 
                 CONFIG_ZMK_KEYBOARD_NAME, 
                 addrs[0].a.val[1], addrs[0].a.val[0]);
        
        // 强行更新蓝牙名称
        bt_set_name(bt_name);
    }
}

// 系统启动时触发的初始化函数
static int init_custom_bt_name(void) {
    // 初始化延时任务
    k_work_init_delayable(&name_update_work, update_name_handler);
    // 设定在系统启动 1500 毫秒 (1.5秒) 后执行改名任务，确保抢在 ZMK 初始化之后
    k_work_schedule(&name_update_work, K_MSEC(1500));
    return 0;
}

// 放在 APPLICATION 级别执行
SYS_INIT(init_custom_bt_name, APPLICATION, 99);