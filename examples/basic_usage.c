/**
 * @file basic_usage.c
 * @brief Otto Emoji GIF组件基础使用示例
 *
 * 这个示例展示了如何使用Otto Emoji GIF组件在LVGL显示屏上显示表情。
 */

#include "esp_log.h"
#include "lvgl.h"
#include "otto_emoji_gif.h"

static const char* TAG = "otto_gif_example";

// 全局GIF对象
static lv_obj_t* emotion_gif = NULL;
static lv_timer_t* emotion_timer = NULL;

// 表情序列
static const char* emotion_sequence[] = {"staticstate", "happy", "sad", "anger", "scare", "buxue"};
static int current_emotion_index = 0;

/**
 * @brief 定时器回调函数，用于自动切换表情
 */
static void emotion_timer_callback(lv_timer_t* timer) {
    if (emotion_gif == NULL)
        return;

    // 获取当前表情
    const char* emotion_name = emotion_sequence[current_emotion_index];
    const lv_img_dsc_t* emotion = otto_emoji_gif_get_by_name(emotion_name);

    if (emotion != NULL) {
        lv_gif_set_src(emotion_gif, emotion);
        ESP_LOGI(TAG, "切换到表情: %s", emotion_name);
    } else {
        ESP_LOGW(TAG, "未找到表情: %s，使用默认表情", emotion_name);
        lv_gif_set_src(emotion_gif, &staticstate);
    }

    // 移动到下一个表情
    current_emotion_index =
        (current_emotion_index + 1) % (sizeof(emotion_sequence) / sizeof(emotion_sequence[0]));
}

/**
 * @brief 创建表情显示界面
 */
void create_emotion_display(lv_obj_t* parent) {
    ESP_LOGI(TAG, "Otto Emoji GIF组件版本: %s", otto_emoji_gif_get_version());
    ESP_LOGI(TAG, "支持的表情数量: %d", otto_emoji_gif_get_count());

    // 创建容器
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(container, lv_color_black(), 0);
    lv_obj_center(container);

    // 创建标题标签
    lv_obj_t* title_label = lv_label_create(container);
    lv_label_set_text(title_label, "Otto Robot Emotions");
    lv_obj_set_style_text_color(title_label, lv_color_white(), 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    // 创建GIF显示对象
    emotion_gif = lv_gif_create(container);
    lv_obj_set_size(emotion_gif, 200, 200);
    lv_obj_center(emotion_gif);

    // 设置初始表情
    lv_gif_set_src(emotion_gif, &staticstate);

    // 创建表情名称标签
    lv_obj_t* emotion_label = lv_label_create(container);
    lv_label_set_text(emotion_label, "staticstate");
    lv_obj_set_style_text_color(emotion_label, lv_color_white(), 0);
    lv_obj_align(emotion_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    // 创建定时器，每3秒切换一次表情
    emotion_timer = lv_timer_create(emotion_timer_callback, 3000, NULL);

    ESP_LOGI(TAG, "表情显示界面创建完成");
}

/**
 * @brief 手动设置表情
 */
void set_emotion_by_name(const char* emotion_name) {
    if (emotion_gif == NULL) {
        ESP_LOGE(TAG, "GIF对象未初始化");
        return;
    }

    const lv_img_dsc_t* emotion = otto_emoji_gif_get_by_name(emotion_name);
    if (emotion != NULL) {
        lv_gif_set_src(emotion_gif, emotion);
        ESP_LOGI(TAG, "手动设置表情: %s", emotion_name);
    } else {
        ESP_LOGW(TAG, "未找到表情: %s", emotion_name);
    }
}

/**
 * @brief 停止自动切换表情
 */
void stop_auto_emotion_switch(void) {
    if (emotion_timer != NULL) {
        lv_timer_del(emotion_timer);
        emotion_timer = NULL;
        ESP_LOGI(TAG, "已停止自动切换表情");
    }
}

/**
 * @brief 开始自动切换表情
 */
void start_auto_emotion_switch(void) {
    if (emotion_timer == NULL) {
        emotion_timer = lv_timer_create(emotion_timer_callback, 3000, NULL);
        ESP_LOGI(TAG, "已开始自动切换表情");
    }
}

/**
 * @brief 演示所有表情
 */
void demo_all_emotions(void) {
    ESP_LOGI(TAG, "开始演示所有表情...");

    const char* all_emotions[] = {"staticstate", "happy", "sad", "anger", "scare", "buxue"};
    int emotion_count = sizeof(all_emotions) / sizeof(all_emotions[0]);

    for (int i = 0; i < emotion_count; i++) {
        set_emotion_by_name(all_emotions[i]);
        vTaskDelay(pdMS_TO_TICKS(2000));  // 等待2秒
    }

    ESP_LOGI(TAG, "表情演示完成");
}