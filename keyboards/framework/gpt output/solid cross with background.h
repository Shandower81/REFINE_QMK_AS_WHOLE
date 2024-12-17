#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
#    if defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR) || defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS_WITH_BACKGROUND_COLOR)

#        ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR
RGB_MATRIX_EFFECT(SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR)
#        endif

#        ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS_WITH_BACKGROUND_COLOR
RGB_MATRIX_EFFECT(SOLID_REACTIVE_MULTICROSS_WITH_BACKGROUND_COLOR)
#        endif

#        ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static HSV SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick, HSV bkgrnd_hsv) {
    uint16_t effect = tick + dist;
    dx              = dx < 0 ? dx * -1 : dx;
    dy              = dy < 0 ? dy * -1 : dy;
    dx              = dx * 16 > 255 ? 255 : dx * 16;
    dy              = dy * 16 > 255 ? 255 : dy * 16;
    effect += dx > dy ? dy : dx;
    if (effect > 255) effect = 255;
#            ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
    hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
#            endif
    hsv.v = qadd8(hsv.v, 255 - effect);
    return hsv;
}

static HSV get_background_color(uint16_t tick) {
    // Define the background color logic
    HSV bkgrnd_hsv;
#        ifdef ENABLE_RAINBOW_BACKGROUND
    bkgrnd_hsv.h = tick % 256; // Rainbow effect
    bkgrnd_hsv.s = 255;
    bkgrnd_hsv.v = 128;
#        else
    bkgrnd_hsv.h = background_hsv.h; // Static background color
    bkgrnd_hsv.s = background_hsv.s;
    bkgrnd_hsv.v = background_hsv.v;
#        endif
    return bkgrnd_hsv;
}

#        ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR
bool SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR(effect_params_t* params) {
    HSV bkgrnd_hsv = get_background_color(g_rgb_timer);
    return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, 
        [bkgrnd_hsv](HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
        return SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR_math(hsv, dx, dy, dist, tick, bkgrnd_hsv);
});
}
#        endif

#        ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS_WITH_BACKGROUND_COLOR
bool SOLID_REACTIVE_MULTICROSS_WITH_BACKGROUND_COLOR(effect_params_t* params) {
    HSV bkgrnd_hsv = get_background_color(g_rgb_timer);
    return effect_runner_reactive_splash(0, params, 
        [bkgrnd_hsv](HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
        return SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR_math(hsv, dx, dy, dist, tick, bkgrnd_hsv);
});
}
#        endif

#        endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#    endif     // !defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS_WITH_BACKGROUND_COLOR) || defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS_WITH_BACKGROUND_COLOR)
#endif         // RGB_MATRIX_KEYREACTIVE_ENABLED
