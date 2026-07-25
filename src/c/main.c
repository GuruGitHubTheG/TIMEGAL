#include <pebble.h>
#include <stdlib.h>
#include <time.h>

// Include auto-generated message keys from CloudPebble
#include "message_keys.auto.h"

// ================== DEBUG SETTINGS ==================
#define ENABLE_DEBUG 0

#if ENABLE_DEBUG
    // ── Standard debug options ──────────────────────────────
    #define DEBUG_SHOW_INFO       false
    #define DEBUG_FORCE_ANIM      -1
    #define DEBUG_FORCE_TIME      true
    #define DEBUG_TIME_HOUR       12
    #define DEBUG_TIME_MINUTE     34
    #define DEBUG_FORCE_BT_ANIM   true

    // ── Force settings (true = override, false = use stored) ─
    // Animation
    #define FORCE_Frequency                false
    #define FORCE_Frequency_VALUE          "1"

    #define FORCE_AnimateOnFlick           false
    #define FORCE_AnimateOnFlick_VALUE     0

    #define FORCE_BluetoothDisconnectAnimation  false
    #define FORCE_BluetoothDisconnectAnimation_VALUE 0

    // Display (non‑Emery)
    #define FORCE_HudContent               false
    #define FORCE_HudContent_VALUE         "1"

    #define FORCE_CurrentRight             false
    #define FORCE_CurrentRight_VALUE       0

    #define FORCE_FlashCurrentWhileCharging false
    #define FORCE_FlashCurrentWhileCharging_VALUE 1

    #define FORCE_ShowBattery              false
    #define FORCE_ShowBattery_VALUE        1

    // Display (Emery only)
    #define FORCE_HudContentLeft           false
    #define FORCE_HudContentLeft_VALUE     "1"

    #define FORCE_HudContentRight          false
    #define FORCE_HudContentRight_VALUE    "2"

    #define FORCE_FlashSide                false
    #define FORCE_FlashSide_VALUE          "1"

    // Haptics/Sound (new)
    #define FORCE_VibrateOnAnimationType   false
    #define FORCE_VibrateOnAnimationType_VALUE "1"
    #define FORCE_VibrateOnBTDisconnectType false
    #define FORCE_VibrateOnBTDisconnectType_VALUE "1"
    #define FORCE_VibrateEveryHourType     false
    #define FORCE_VibrateEveryHourType_VALUE "2"
    #define FORCE_VibrateDuringQuietTime   false
    #define FORCE_VibrateDuringQuietTime_VALUE 1
    #define FORCE_PlaySoundOnAnimation     false
    #define FORCE_PlaySoundOnAnimation_VALUE 1
    #define FORCE_PlaySoundOnBTDisconnect  false
    #define FORCE_PlaySoundOnBTDisconnect_VALUE 1
    #define FORCE_PlaySoundEveryHour       false
    #define FORCE_PlaySoundEveryHour_VALUE 1
    #define FORCE_SoundDuringQuietTime     false
    #define FORCE_SoundDuringQuietTime_VALUE 0
    #define FORCE_SuppressStartupVibes     false
    #define FORCE_SuppressStartupVibes_VALUE 1
    #define FORCE_SuppressStartupSounds    false
    #define FORCE_SuppressStartupSounds_VALUE 1

    // Clock
    #define FORCE_ClockMode                false
    #define FORCE_ClockMode_VALUE          "0"

    #define FORCE_LeadingZeros             false
    #define FORCE_LeadingZeros_VALUE       1

    #define FORCE_AMPM_Position            false
    #define FORCE_AMPM_Position_VALUE      "0"

    // Date
    #define FORCE_DateFormat               false
    #define FORCE_DateFormat_VALUE         "0"

    #define FORCE_DateSeparator            false
    #define FORCE_DateSeparator_VALUE      "0"

    #define FORCE_MonthFormat              false
    #define FORCE_MonthFormat_VALUE        "0"

    #define FORCE_DateLeadingZeros         false
    #define FORCE_DateLeadingZeros_VALUE   0
#endif

// ================== SETTINGS ==================
#define SETTINGS_KEY 1

typedef struct {
    // Animation
    int frequency;
    int clock_mode;
    bool leading_zeros;
    int ampm_position;
    bool animate_on_flick;
    bool bt_animation;
    
    // Display
    bool show_battery;
    bool current_right;
    int date_format;
    int date_separator;
    int month_format;
    bool date_leading_zeros;
    bool flash_while_charging;
    int hud_content;            // 0=off,1=CURRENT,2=WEEKDAY,3=BATTERY,4=SECONDS
    int hud_content_left;
    int hud_content_right;
    int flash_side;
    bool player_mode;           // true = PLAYER1 / PLAYER2 headings
    
    // Haptics
    int vibrate_on_anim_type;
    int vibrate_on_bt_type;
    int vibrate_every_hour_type;
    bool vibrate_during_quiet_time;
    bool suppress_startup_vibes;
    
    // Sound
    bool play_sound_on_anim;
    bool play_sound_on_bt;
    bool play_sound_every_hour;
    bool sound_during_quiet_time;
    bool suppress_startup_sounds;
} ClaySettings;

static ClaySettings s_settings;

// ================== CONFIGURATION ==================
#define NUM_BACKGROUNDS  7

// ================== FRAME DURATION & RESOURCES (main) ==================
// ---------- CHANGED: Diorite now uses Aplite's frames & timing ----------
#if defined(PBL_PLATFORM_APLITE) || defined(PBL_PLATFORM_DIORITE)
    #define FRAME_DURATION_MS 160
    static const uint32_t s_frame_resources[] = {
        RESOURCE_ID_F1,  RESOURCE_ID_F3,  RESOURCE_ID_F5,  RESOURCE_ID_F7,
        RESOURCE_ID_F9,  RESOURCE_ID_F11, RESOURCE_ID_F13
    };
    #define NUM_FRAMES       7
    #define FRAME_LABEL      "Odd (1-13)"

#elif defined(PBL_PLATFORM_GABBRO)
    #define FRAME_DURATION_MS 224
    #define NUM_FRAMES       4
    #define FRAME_LABEL      "Gabbro (blank,6,9,13)"
    static const uint32_t s_frame_resources[NUM_FRAMES] = {
        0,                   // frame 0 – blank
        RESOURCE_ID_F6,      // frame 1
        RESOURCE_ID_F9,      // frame 2
        RESOURCE_ID_F13      // frame 3
    };
#else
    #define FRAME_DURATION_MS 80
    static const uint32_t s_frame_resources[] = {
        RESOURCE_ID_F0,  RESOURCE_ID_F1,  RESOURCE_ID_F2,  RESOURCE_ID_F3,
        RESOURCE_ID_F4,  RESOURCE_ID_F5,  RESOURCE_ID_F6,  RESOURCE_ID_F7,
        RESOURCE_ID_F8,  RESOURCE_ID_F9,  RESOURCE_ID_F10, RESOURCE_ID_F11,
        RESOURCE_ID_F12, RESOURCE_ID_F13
    };
    #define NUM_FRAMES       14
    #define FRAME_LABEL      "All (0-13)"
#endif

// ================== BT ANIMATION RESOURCES ==================
// ---------- CHANGED: Diorite now uses Aplite's BT frames & timing ----------
#if defined(PBL_PLATFORM_GABBRO)
    #define BT_FRAME_DURATION_MS   160
#elif defined(PBL_PLATFORM_APLITE) || defined(PBL_PLATFORM_DIORITE)
    #define BT_FRAME_DURATION_MS   267
#else
    #define BT_FRAME_DURATION_MS   (FRAME_DURATION_MS * 2)
#endif

#if defined(PBL_PLATFORM_APLITE) || defined(PBL_PLATFORM_DIORITE)
    static const uint32_t s_bt_frame_resources[] = {
        RESOURCE_ID_BT_F0,
        RESOURCE_ID_BT_F2,
        RESOURCE_ID_BT_F4,
        RESOURCE_ID_BT_F6,
        RESOURCE_ID_BT_F8,
        RESOURCE_ID_BT_F10
    };
    #define BT_NUM_FRAMES 6
#else
    static const uint32_t s_bt_frame_resources[] = {
        RESOURCE_ID_BT_F0, RESOURCE_ID_BT_F1, RESOURCE_ID_BT_F2,
        RESOURCE_ID_BT_F3, RESOURCE_ID_BT_F4, RESOURCE_ID_BT_F5,
        RESOURCE_ID_BT_F6, RESOURCE_ID_BT_F7, RESOURCE_ID_BT_F8,
        RESOURCE_ID_BT_F9, RESOURCE_ID_BT_F10
    };
    #define BT_NUM_FRAMES 11
#endif

// ================== FONT & SPACING ==================
#define CHAR_HEIGHT 24
#define LETTER_GAP 1
#define DIGIT_GAP 2
#define COLON_GAP 2
#define AM_PM_GAP 8

// ================== RESOURCES ==================
static const uint32_t s_bg_resources[NUM_BACKGROUNDS] = {
    RESOURCE_ID_A0, RESOURCE_ID_A1, RESOURCE_ID_A2, RESOURCE_ID_A3,
    RESOURCE_ID_A4, RESOURCE_ID_A5, RESOURCE_ID_A6
};

static const uint32_t s_digit_resources[10] = {
    RESOURCE_ID_FONT_0, RESOURCE_ID_FONT_1, RESOURCE_ID_FONT_2,
    RESOURCE_ID_FONT_3, RESOURCE_ID_FONT_4, RESOURCE_ID_FONT_5,
    RESOURCE_ID_FONT_6, RESOURCE_ID_FONT_7, RESOURCE_ID_FONT_8,
    RESOURCE_ID_FONT_9
};

static const uint32_t s_letter_resources[3] = {
    RESOURCE_ID_FONT_A, RESOURCE_ID_FONT_P, RESOURCE_ID_FONT_M
};

static const uint32_t s_colon_resource = RESOURCE_ID_FONT_COLON;

// ================== GLOBAL VARIABLES ==================
static Window *s_main_window;
static BitmapLayer *s_bg_layer;
static BitmapLayer *s_fg_layer;
static GBitmap *s_bg_bitmap = NULL;
static GBitmap *s_fg_bitmap = NULL;
static AppTimer *s_timer = NULL;

static int s_current_frame = 0;
static bool s_is_animating = false;
static int s_current_bg_index = 0;
static int s_last_played = -1;

static Layer *s_clock_layer;
static GFont s_arcade_font;
static int s_char_width = 0;

static AppTimer *s_flash_timer = NULL;
static bool s_flash_visible = true;

static bool s_bt_animating = false;
static int  s_bt_current_frame = 0;
static int  s_prev_bg_index = 0;
static AppTimer *s_bt_timer = NULL;

static bool s_startup_pending = true;
static bool s_startup_animation_pending = false;   // NEW: startup race fix

static int s_phone_battery = -1;
static bool s_phone_battery_known = false;

static int s_last_hour = -1;

static AppTimer *s_delayed_vibe_timer = NULL;

static bool s_bt_disconnected = false;   // NEW: tracks disconnected state for reconnect animation

#if ENABLE_DEBUG
    static TextLayer *s_debug_layer;
    static char s_debug_text[128];
    static struct tm s_fake_time;
    static bool s_fake_time_toggle = false;
    static AppTimer *s_debug_timer = NULL;
#endif

// ================== FORWARD DECLARATIONS ==================
static void start_animation(void);
static void start_animation_silent(void);
static void start_animation_with_current_bg(bool is_startup);
static void startup_timer_callback(void *data);
#if ENABLE_DEBUG
    static void update_debug_text(void);
#endif
static void cleanup_bitmaps(void);
static void start_flash_timer(void);
static void stop_flash_timer(void);
static void bt_animation_start(void);
static void bt_animation_stop(void);
static void play_feedback(int vibe_type, bool play_sound, bool allow_quiet_override, uint32_t sound_resource);
#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
static void play_wav_resource(uint32_t resource_id);
#endif

// ================== SETTINGS HELPERS ==================
static void prv_default_settings() {
    // Animation
    s_settings.frequency = 1;
    s_settings.clock_mode = 0;
    s_settings.leading_zeros = true;
    s_settings.ampm_position = 0;
    s_settings.animate_on_flick = false;
    s_settings.bt_animation = false;
    
    // Display
    s_settings.show_battery = true;
    s_settings.current_right = false;
    s_settings.date_format = 0;
    s_settings.date_separator = 0;
    s_settings.month_format = 0;
    s_settings.date_leading_zeros = true;
    s_settings.flash_while_charging = true;
    s_settings.hud_content = 1;
    s_settings.hud_content_left = 1;
    s_settings.hud_content_right = 2;
    s_settings.flash_side = 1;
    s_settings.player_mode = false;
    
    // Haptics
    s_settings.vibrate_on_anim_type = 0;
    s_settings.vibrate_on_bt_type = 0;
    s_settings.vibrate_every_hour_type = 0;
    s_settings.vibrate_during_quiet_time = false;
    s_settings.suppress_startup_vibes = true;
    
    // Sound
    s_settings.play_sound_on_anim = false;
    s_settings.play_sound_on_bt = false;
    s_settings.play_sound_every_hour = false;
    s_settings.sound_during_quiet_time = false;
    s_settings.suppress_startup_sounds = false;
}

#if ENABLE_DEBUG
static void apply_forced_debug_settings(void) {
    if (FORCE_Frequency)              s_settings.frequency = atoi(FORCE_Frequency_VALUE);
    if (FORCE_AnimateOnFlick)         s_settings.animate_on_flick = FORCE_AnimateOnFlick_VALUE;
    if (FORCE_BluetoothDisconnectAnimation) s_settings.bt_animation = FORCE_BluetoothDisconnectAnimation_VALUE;

    if (FORCE_HudContent)             s_settings.hud_content = atoi(FORCE_HudContent_VALUE);
    if (FORCE_CurrentRight)           s_settings.current_right = FORCE_CurrentRight_VALUE;
    if (FORCE_FlashCurrentWhileCharging) s_settings.flash_while_charging = FORCE_FlashCurrentWhileCharging_VALUE;
    if (FORCE_ShowBattery)            s_settings.show_battery = FORCE_ShowBattery_VALUE;

    if (FORCE_HudContentLeft)         s_settings.hud_content_left = atoi(FORCE_HudContentLeft_VALUE);
    if (FORCE_HudContentRight)        s_settings.hud_content_right = atoi(FORCE_HudContentRight_VALUE);
    if (FORCE_FlashSide)              s_settings.flash_side = atoi(FORCE_FlashSide_VALUE);

    if (FORCE_VibrateOnAnimationType) s_settings.vibrate_on_anim_type = atoi(FORCE_VibrateOnAnimationType_VALUE);
    if (FORCE_VibrateOnBTDisconnectType) s_settings.vibrate_on_bt_type = atoi(FORCE_VibrateOnBTDisconnectType_VALUE);
    if (FORCE_VibrateEveryHourType)   s_settings.vibrate_every_hour_type = atoi(FORCE_VibrateEveryHourType_VALUE);
    if (FORCE_VibrateDuringQuietTime) s_settings.vibrate_during_quiet_time = FORCE_VibrateDuringQuietTime_VALUE;
    if (FORCE_PlaySoundOnAnimation)   s_settings.play_sound_on_anim = FORCE_PlaySoundOnAnimation_VALUE;
    if (FORCE_PlaySoundOnBTDisconnect) s_settings.play_sound_on_bt = FORCE_PlaySoundOnBTDisconnect_VALUE;
    if (FORCE_PlaySoundEveryHour)     s_settings.play_sound_every_hour = FORCE_PlaySoundEveryHour_VALUE;
    if (FORCE_SoundDuringQuietTime)   s_settings.sound_during_quiet_time = FORCE_SoundDuringQuietTime_VALUE;
    if (FORCE_SuppressStartupVibes)   s_settings.suppress_startup_vibes = FORCE_SuppressStartupVibes_VALUE;
    if (FORCE_SuppressStartupSounds)  s_settings.suppress_startup_sounds = FORCE_SuppressStartupSounds_VALUE;

    if (FORCE_ClockMode)              s_settings.clock_mode = atoi(FORCE_ClockMode_VALUE);
    if (FORCE_LeadingZeros)           s_settings.leading_zeros = FORCE_LeadingZeros_VALUE;
    if (FORCE_AMPM_Position)          s_settings.ampm_position = atoi(FORCE_AMPM_Position_VALUE);

    if (FORCE_DateFormat)             s_settings.date_format = atoi(FORCE_DateFormat_VALUE);
    if (FORCE_DateSeparator)          s_settings.date_separator = atoi(FORCE_DateSeparator_VALUE);
    if (FORCE_MonthFormat)            s_settings.month_format = atoi(FORCE_MonthFormat_VALUE);
    if (FORCE_DateLeadingZeros)       s_settings.date_leading_zeros = FORCE_DateLeadingZeros_VALUE;
}
#endif

static void prv_save_settings() {
    persist_write_data(SETTINGS_KEY, &s_settings, sizeof(s_settings));
}

static void prv_load_settings() {
    prv_default_settings();
    if (persist_exists(SETTINGS_KEY)) {
        persist_read_data(SETTINGS_KEY, &s_settings, sizeof(s_settings));
    }
#if ENABLE_DEBUG
    apply_forced_debug_settings();
#endif
}

// ================== FEEDBACK HELPER ==================
static bool is_quiet_time_active(void) {
    return quiet_time_is_active();
}

static const uint32_t s_timegal_vibe_pattern[] = {
    7, 131, 7, 134, 7, 130, 8, 133, 7, 131, 7, 129, 7, 163, 144, 11, 108, 11, 700
};
#define TIMEGAL_VIBE_SEGMENTS (sizeof(s_timegal_vibe_pattern) / sizeof(uint32_t))

static void timegal_vibration_timer_callback(void *data) {
    s_delayed_vibe_timer = NULL;
    VibePattern pat = {
        .durations = s_timegal_vibe_pattern,
        .num_segments = TIMEGAL_VIBE_SEGMENTS,
    };
    vibes_enqueue_custom_pattern(pat);
}

static void play_vibration(int vibe_type) {
    if (s_delayed_vibe_timer) {
        app_timer_cancel(s_delayed_vibe_timer);
        s_delayed_vibe_timer = NULL;
    }

    if (vibe_type == 4) {
        s_delayed_vibe_timer = app_timer_register(7, timegal_vibration_timer_callback, NULL);
        return;
    }

    switch (vibe_type) {
        case 1: vibes_short_pulse(); break;
        case 2: vibes_long_pulse(); break;
        case 3: vibes_double_pulse(); break;
        case 0: 
        default: break;
    }
}

static void play_feedback(int vibe_type, bool play_sound, bool allow_quiet_override, uint32_t sound_resource) {
    bool quiet = is_quiet_time_active();
    
    if (quiet && !allow_quiet_override) {
        return;
    }
    
    if (vibe_type > 0 && vibe_type <= 4) {
        play_vibration(vibe_type);
    }
    
    #if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
        if (play_sound) {
            play_wav_resource(sound_resource);
        }
    #endif
}

// ================== BACKGROUND SELECTION ==================
static int get_next_background() {
#if ENABLE_DEBUG
    if (DEBUG_FORCE_ANIM >= 0 && DEBUG_FORCE_ANIM < NUM_BACKGROUNDS) {
        return DEBUG_FORCE_ANIM;
    }
#endif
    int bg = s_last_played;
    while (bg == s_last_played) {
        bg = rand() % NUM_BACKGROUNDS;
    }
    s_last_played = bg;
    return bg;
}

// ================== LOAD FUNCTIONS ==================
static bool load_background(int bg_index) {
    if (s_bg_bitmap) {
        gbitmap_destroy(s_bg_bitmap);
        s_bg_bitmap = NULL;
    }
    s_bg_bitmap = gbitmap_create_with_resource(s_bg_resources[bg_index]);
    if (!s_bg_bitmap) return false;
    bitmap_layer_set_bitmap(s_bg_layer, s_bg_bitmap);
    return true;
}

static bool load_foreground_frame(int frame_index) {
    if (frame_index >= NUM_FRAMES) return false;
    uint32_t resource_id = s_frame_resources[frame_index];
    if (s_fg_bitmap) {
        gbitmap_destroy(s_fg_bitmap);
        s_fg_bitmap = NULL;
    }
    if (resource_id == 0) {
        bitmap_layer_set_bitmap(s_fg_layer, NULL);
        return true;
    }
    s_fg_bitmap = gbitmap_create_with_resource(resource_id);
    if (!s_fg_bitmap) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to load frame %d", frame_index);
        return false;
    }
    bitmap_layer_set_bitmap(s_fg_layer, s_fg_bitmap);
    return true;
}

static void load_last_frame() {
    int last_frame = NUM_FRAMES - 1;
    load_foreground_frame(last_frame);
    s_current_frame = last_frame;
}

// ================== BT ANIMATION LOAD / CONTROL ==================
static bool bt_load_background(void) {
    if (s_bg_bitmap) {
        gbitmap_destroy(s_bg_bitmap);
        s_bg_bitmap = NULL;
    }
    s_bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_BG);
    if (!s_bg_bitmap) return false;
    bitmap_layer_set_bitmap(s_bg_layer, s_bg_bitmap);
    return true;
}

static bool bt_load_frame(int index) {
    if (index >= BT_NUM_FRAMES) return false;
    if (s_fg_bitmap) {
        gbitmap_destroy(s_fg_bitmap);
        s_fg_bitmap = NULL;
    }
    s_fg_bitmap = gbitmap_create_with_resource(s_bt_frame_resources[index]);
    if (!s_fg_bitmap) return false;
    bitmap_layer_set_bitmap(s_fg_layer, s_fg_bitmap);
    return true;
}

static void bt_animation_timer_callback(void *data) {
    s_bt_current_frame++;
    if (s_bt_current_frame < BT_NUM_FRAMES) {
        if (bt_load_frame(s_bt_current_frame)) {
            s_bt_timer = app_timer_register(BT_FRAME_DURATION_MS,
                                            bt_animation_timer_callback, NULL);
        } else {
            // Abort animation on frame load failure
            APP_LOG(APP_LOG_LEVEL_ERROR, "BT frame %d failed to load, aborting", s_bt_current_frame);
            s_bt_timer = NULL;
            bt_animation_stop();
        }
    } else {
        s_bt_timer = NULL;
        s_bt_animating = false;   // sequence finished
        APP_LOG(APP_LOG_LEVEL_INFO, "BT Animation finished");
    }
}

static void bt_animation_start(void) {
    if (s_bt_timer) {
        app_timer_cancel(s_bt_timer);
        s_bt_timer = NULL;
    }
    s_bt_animating = false;

    if (s_fg_bitmap) {
        gbitmap_destroy(s_fg_bitmap);
        s_fg_bitmap = NULL;
        bitmap_layer_set_bitmap(s_fg_layer, NULL);
    }

#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
    play_feedback(s_settings.vibrate_on_bt_type, 
                  s_settings.play_sound_on_bt,
                  s_settings.vibrate_during_quiet_time,
                  RESOURCE_ID_BT_SOUND);
#else
    play_feedback(s_settings.vibrate_on_bt_type, 
                  s_settings.play_sound_on_bt,
                  s_settings.vibrate_during_quiet_time,
                  0);
#endif

    if (s_timer) {
        app_timer_cancel(s_timer);
        s_timer = NULL;
    }
    s_is_animating = false;
    s_prev_bg_index = s_current_bg_index;

    s_bt_animating = true;

    if (!bt_load_background()) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "BT background failed to load");
        s_bt_animating = false;
        return;
    }
    layer_mark_dirty(bitmap_layer_get_layer(s_bg_layer));

    if (s_settings.frequency == 2) {
        if (!bt_load_frame(BT_NUM_FRAMES - 1)) {
            bt_load_frame(0);
        }
        s_bt_current_frame = BT_NUM_FRAMES - 1;
        return;
    }

    if (!bt_load_frame(0)) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "BT frame 0 failed to load");
        s_bt_animating = false;
        return;
    }

    s_bt_current_frame = 0;
    s_bt_timer = app_timer_register(BT_FRAME_DURATION_MS,
                                    bt_animation_timer_callback, NULL);
}

static void bt_animation_stop(void) {
    if (s_bt_timer) {
        app_timer_cancel(s_bt_timer);
        s_bt_timer = NULL;
    }

    if (!load_background(s_prev_bg_index)) {
        load_background(0);
        s_current_bg_index = 0;
    } else {
        s_current_bg_index = s_prev_bg_index;
    }

    if (s_settings.frequency == 2) {
        load_last_frame();
    } else {
        if (s_fg_bitmap) {
            gbitmap_destroy(s_fg_bitmap);
            s_fg_bitmap = NULL;
            bitmap_layer_set_bitmap(s_fg_layer, NULL);
        }
    }

    s_bt_animating = false;
}

// ================== BLUETOOTH CALLBACK (FIXED) ==================
static void bt_connection_handler(bool connected) {
    if (!connected) {
        // Store the disconnected state so we can react on reconnect
        s_bt_disconnected = true;
        if (s_settings.bt_animation && !s_bt_animating) {
            bt_animation_start();
        }
    } else {
        // Reconnected: always stop any BT animation and restore normal background,
        // then play main anime if not disabled.
        if (s_bt_disconnected) {
            s_bt_disconnected = false;
            // Stop BT animation (safe to call even if already finished)
            bt_animation_stop();
            // Restart main animation if it's not completely off
            if (s_settings.frequency != 2) {
                start_animation_with_current_bg(false);
            }
        }
    }
}

// ================== FONT HELPERS (v3.0.0 style – no persistent caching) ==================
static uint8_t get_char_width(int index, bool is_digit) {
    if (is_digit) {
        if (index == 1) return 11;
        if (index == 4) return 14;
        return 13;
    } else {
        return 19;
    }
}

// ================== DRAW CLOCK ==================
static void draw_character(GContext *ctx, uint32_t resource_id, int x, int y) {
    GBitmap *bitmap = gbitmap_create_with_resource(resource_id);
    if (!bitmap) return;
    GRect bounds = gbitmap_get_bounds(bitmap);
    int y_offset = (CHAR_HEIGHT - bounds.size.h) / 2;
    graphics_draw_bitmap_in_rect(ctx, bitmap, GRect(x, y + y_offset, bounds.size.w, bounds.size.h));
    gbitmap_destroy(bitmap);
}

static void draw_time_string(GContext *ctx, struct tm *t) {
    int hour = t->tm_hour;
    int minute = t->tm_min;
    bool is_pm = false;
    int display_hour = hour;

    int clock_mode = s_settings.clock_mode;
    if (clock_mode == 0) {
        bool is_24h = clock_is_24h_style();
        clock_mode = is_24h ? 2 : 1;
    }

    if (clock_mode == 1) {
        if (hour >= 12) {
            is_pm = true;
            display_hour = (hour > 12) ? hour - 12 : 12;
        } else {
            display_hour = (hour == 0) ? 12 : hour;
        }
    } else {
        display_hour = hour;
        is_pm = (hour >= 12);
    }

    bool show_ampm = (s_settings.ampm_position != 2);
    bool ampm_left = (s_settings.ampm_position == 0);

    int h_tens = display_hour / 10;
    int h_ones = display_hour % 10;
    int m_tens = minute / 10;
    int m_ones = minute % 10;

    Layer *root_layer = window_get_root_layer(s_main_window);
    GRect bounds = layer_get_bounds(root_layer);
    int screen_w = bounds.size.w;
    int screen_h = bounds.size.h;

    int total_width = 0;

    if (show_ampm && ampm_left) {
        int letter = is_pm ? 1 : 0;
        total_width += get_char_width(letter, false);
        total_width += LETTER_GAP;
        total_width += get_char_width(2, false);
        total_width += AM_PM_GAP;
    }

    if (s_settings.leading_zeros || display_hour >= 10) {
        total_width += get_char_width(h_tens, true);
        total_width += DIGIT_GAP;
    }
    total_width += get_char_width(h_ones, true);
    total_width += COLON_GAP;
    total_width += 5;
    total_width += COLON_GAP;
    total_width += get_char_width(m_tens, true);
    total_width += DIGIT_GAP;
    total_width += get_char_width(m_ones, true);

    if (show_ampm && !ampm_left) {
        int letter = is_pm ? 1 : 0;
        total_width += AM_PM_GAP;
        total_width += get_char_width(letter, false);
        total_width += LETTER_GAP;
        total_width += get_char_width(2, false);
    }

    int x = (screen_w - total_width) / 2;
    int y = (screen_h * 3 / 4) - (CHAR_HEIGHT / 2);

    if (show_ampm && ampm_left) {
        int letter = is_pm ? 1 : 0;
        draw_character(ctx, s_letter_resources[letter], x, y);
        x += get_char_width(letter, false) + LETTER_GAP;
        draw_character(ctx, s_letter_resources[2], x, y);
        x += get_char_width(2, false) + AM_PM_GAP;
    }

    if (s_settings.leading_zeros || display_hour >= 10) {
        draw_character(ctx, s_digit_resources[h_tens], x, y);
        x += get_char_width(h_tens, true) + DIGIT_GAP;
    }
    draw_character(ctx, s_digit_resources[h_ones], x, y);
    x += get_char_width(h_ones, true) + COLON_GAP;
    draw_character(ctx, s_colon_resource, x, y);
    x += 5 + COLON_GAP;
    draw_character(ctx, s_digit_resources[m_tens], x, y);
    x += get_char_width(m_tens, true) + DIGIT_GAP;
    draw_character(ctx, s_digit_resources[m_ones], x, y);
    x += get_char_width(m_ones, true);

    if (show_ampm && !ampm_left) {
        int letter = is_pm ? 1 : 0;
        x += AM_PM_GAP;
        draw_character(ctx, s_letter_resources[letter], x, y);
        x += get_char_width(letter, false) + LETTER_GAP;
        draw_character(ctx, s_letter_resources[2], x, y);
    }
}

static struct tm *get_display_time(void) {
#if ENABLE_DEBUG
    if (DEBUG_FORCE_TIME) {
        if (s_fake_time_toggle) {
            static struct tm s_tmp_time;
            s_tmp_time = s_fake_time;
            s_tmp_time.tm_min++;
            if (s_tmp_time.tm_min >= 60) {
                s_tmp_time.tm_min = 0;
                s_tmp_time.tm_hour++;
                if (s_tmp_time.tm_hour >= 24) {
                    s_tmp_time.tm_hour = 0;
                }
            }
            return &s_tmp_time;
        } else {
            return &s_fake_time;
        }
    }
#endif
    time_t now = time(NULL);
    return localtime(&now);
}

// Format small clock string (Fix #9: respect leading zeros)
static void format_small_clock(char *buffer, size_t size, struct tm *t) {
    int hour = t->tm_hour;
    int minute = t->tm_min;
    bool is_pm = false;
    int display_hour = hour;

    int clock_mode = s_settings.clock_mode;
    if (clock_mode == 0) {
        bool is_24h = clock_is_24h_style();
        clock_mode = is_24h ? 2 : 1;
    }

    if (clock_mode == 1) {
        if (hour >= 12) {
            is_pm = true;
            display_hour = (hour > 12) ? hour - 12 : 12;
        } else {
            display_hour = (hour == 0) ? 12 : hour;
        }
    } else {
        display_hour = hour;
        is_pm = (hour >= 12);
    }

    bool show_ampm = (s_settings.ampm_position != 2);
    bool ampm_left = (s_settings.ampm_position == 0);

    char hour_str[3];
    if (s_settings.leading_zeros) {
        snprintf(hour_str, sizeof(hour_str), "%02d", display_hour);
    } else {
        snprintf(hour_str, sizeof(hour_str), "%d", display_hour);
    }

    if (show_ampm && ampm_left) {
        snprintf(buffer, size, "%c%s:%02d", is_pm ? 'P' : 'A', hour_str, minute);
    } else if (show_ampm && !ampm_left) {
        snprintf(buffer, size, "%s:%02d%c", hour_str, minute, is_pm ? 'P' : 'A');
    } else {
        snprintf(buffer, size, "%s:%02d", hour_str, minute);
    }
}

// ------------------------- HUD drawing helper (with custom heading) ---------------------------
static void draw_hud_block(GContext *ctx, struct tm *t, GFont font,
                           int content_type, int x_start, int top_y,
                           int block_width, int font_height,
                           GColor heading_color, GColor date_color,
                           bool allow_flash, const char *custom_heading) {
    if (content_type == 0) return;

    char heading[16] = {0};
    char second_line[16] = {0};
    bool has_second = false;

    if (custom_heading && custom_heading[0] != '\0') {
        strncpy(heading, custom_heading, sizeof(heading) - 1);
    } else {
        if (content_type == 1) {
            strcpy(heading, "CURRENT");
        } else if (content_type == 2) {
            strcpy(heading, "WEEKDAY");
        } else if (content_type == 3) {
            strcpy(heading, "BATTERY");
        } else if (content_type == 4) {
            strcpy(heading, "SECONDS");
        }
    }

    if (content_type == 1) {
        char sep;
        switch (s_settings.date_separator) {
            case 1: sep = '/'; break;
            case 2: sep = '-'; break;
            case 3: sep = ' '; break;
            default: sep = '.'; break;
        }
        char month_str[12] = {0};
        if (s_settings.month_format == 1) {
            static const char *mon_abbr[] = {"JAN","FEB","MAR","APR","MAY","JUN",
                                             "JUL","AUG","SEP","OCT","NOV","DEC"};
            snprintf(month_str, sizeof(month_str), "%s", mon_abbr[t->tm_mon]);
        } else {
            if (s_settings.date_leading_zeros)
                snprintf(month_str, sizeof(month_str), "%02d", t->tm_mon + 1);
            else
                snprintf(month_str, sizeof(month_str), "%d", t->tm_mon + 1);
        }
        if (s_settings.date_format == 1) {
            if (s_settings.date_leading_zeros)
                snprintf(second_line, sizeof(second_line), "%02d%c%s", t->tm_mday, sep, month_str);
            else
                snprintf(second_line, sizeof(second_line), "%d%c%s", t->tm_mday, sep, month_str);
        } else {
            snprintf(second_line, sizeof(second_line), "%s%c%02d", month_str, sep, t->tm_mday);
        }
        has_second = true;
    } else if (content_type == 2) {
        static const char *wdays[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
        strcpy(second_line, wdays[t->tm_wday]);
        has_second = true;
    } else if (content_type == 3) {
        if (s_phone_battery_known) {
            snprintf(second_line, sizeof(second_line), "%d%%", s_phone_battery);
        } else {
            strcpy(second_line, "N/A");
        }
        has_second = true;
    } else if (content_type == 4) {
        snprintf(second_line, sizeof(second_line), "%d SEC.", t->tm_sec);
        has_second = true;
    }

    bool draw_heading = true;
    if (allow_flash) {
        BatteryChargeState charge_state = battery_state_service_peek();
        if (charge_state.is_charging) {
            draw_heading = s_flash_visible;
        }
    }

    if (heading[0] && draw_heading) {
        graphics_context_set_text_color(ctx, GColorBlack);
        graphics_draw_text(ctx, heading, font,
                           GRect(x_start + 1, top_y + 1, block_width, font_height),
                           GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
        graphics_context_set_text_color(ctx, heading_color);
        graphics_draw_text(ctx, heading, font,
                           GRect(x_start, top_y, block_width, font_height),
                           GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
    }

    if (has_second) {
        int second_y = top_y + font_height;
        graphics_context_set_text_color(ctx, GColorBlack);
        graphics_draw_text(ctx, second_line, font,
                           GRect(x_start + 1, second_y + 1, block_width, font_height),
                           GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
        graphics_context_set_text_color(ctx, date_color);
        graphics_draw_text(ctx, second_line, font,
                           GRect(x_start, second_y, block_width, font_height),
                           GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
    }
}

static void clock_layer_update_callback(Layer *layer, GContext *ctx) {
    graphics_context_set_compositing_mode(ctx, GCompOpSet);

    struct tm *t = get_display_time();

    GRect full_bounds = layer_get_bounds(layer);
    GRect unobstructed = layer_get_unobstructed_bounds(layer);
    bool quick_view = (unobstructed.size.h < full_bounds.size.h);

    int screen_w = full_bounds.size.w;
    int screen_h = full_bounds.size.h;

    if (!quick_view) {
        draw_time_string(ctx, t);
    }

    GFont font = s_arcade_font ? s_arcade_font : fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
    GRect big_box = GRect(0, 0, 1000, 100);
    GSize sample_size = graphics_text_layout_get_content_size("GAL", font, big_box,
                                     GTextOverflowModeWordWrap, GTextAlignmentLeft);
    int font_height = sample_size.h;

    int clock_y = (screen_h * 3 / 4) - (CHAR_HEIGHT / 2);
    int clock_bottom = clock_y + CHAR_HEIGHT;
    int midpoint = clock_bottom + (screen_h - clock_bottom) / 2;
    int batt_text_y_full = midpoint - (font_height / 2);
    if (batt_text_y_full < 0) batt_text_y_full = 0;
    if (batt_text_y_full + font_height > screen_h) batt_text_y_full = screen_h - font_height;

    int bottom_gap_full = screen_h - (batt_text_y_full + font_height);

    int top_extra_y    = 0;
    int bottom_extra_y = 0;
    int horiz_extra    = 0;
    bool center_round  = false;

#if defined(PBL_ROUND)
    #if PBL_DISPLAY_WIDTH == 260
        horiz_extra    = 6;
    #elif PBL_DISPLAY_WIDTH == 180
        center_round = true;
    #endif
#endif

    int top_y = bottom_gap_full + top_extra_y;

    int batt_y = batt_text_y_full;
#if !defined(PBL_PLATFORM_GABBRO)
    if (quick_view) {
        int unobstructed_bottom = unobstructed.origin.y + unobstructed.size.h;
        batt_y = unobstructed_bottom - font_height - bottom_gap_full;
        if (batt_y < 0) batt_y = 0;
    }
#endif

    int bottom_gap = screen_h - (batt_y + font_height);

#ifdef PBL_COLOR
    GColor red   = GColorFromRGB(196, 29, 42);
    GColor green = GColorFromRGB(89, 243, 100);
    GColor yellow = GColorFromRGB(251, 255, 93);
#else
    GColor red   = GColorWhite;
    GColor green = GColorWhite;
    GColor yellow = GColorWhite;
#endif

    GSize cur_size = graphics_text_layout_get_content_size("CURRENT", font, big_box,
                                     GTextOverflowModeWordWrap, GTextAlignmentLeft);
    GSize wday_size = graphics_text_layout_get_content_size("WED.", font, big_box,
                                     GTextOverflowModeWordWrap, GTextAlignmentLeft);
    GSize week_size = graphics_text_layout_get_content_size("WEEKDAY", font, big_box,
                                     GTextOverflowModeWordWrap, GTextAlignmentLeft);

    int block_width = cur_size.w;
    if (wday_size.w > block_width) block_width = wday_size.w;
    if (week_size.w > block_width) block_width = week_size.w;

    int gap = (s_char_width > 0) ? s_char_width : 12;
    int extra_pixels = (s_char_width > 0) ? (s_char_width * horiz_extra) : (12 * horiz_extra);

    bool is_emery = false;
    bool is_chalk = false;
#if defined(PBL_PLATFORM_EMERY)
    is_emery = true;
#elif defined(PBL_PLATFORM_CHALK)
    is_chalk = true;
#endif

    if (is_emery) {
        const char *heading_left = NULL;
        const char *heading_right = NULL;
        if (s_settings.player_mode) {
            heading_left = "PLAYER1";
            heading_right = "PLAYER2";
        }

        int x_left = gap + extra_pixels;
        int x_right = screen_w - gap - extra_pixels - block_width;

        bool left_flash = (s_settings.hud_content_left == 1) && (s_settings.flash_side == 1);
        bool right_flash = (s_settings.hud_content_right == 1) && (s_settings.flash_side == 2);

        draw_hud_block(ctx, t, font, s_settings.hud_content_left,
                       x_left, top_y, block_width, font_height,
                       red, green, left_flash, heading_left);
        draw_hud_block(ctx, t, font, s_settings.hud_content_right,
                       x_right, top_y, block_width, font_height,
                       red, green, right_flash, heading_right);
    } else {
        int content = s_settings.hud_content;
        if (content != 0) {
            int x_start;
            bool is_right = false;
            
            if (center_round || is_chalk) {
                x_start = (screen_w - block_width) / 2;
            } else {
                if (s_settings.current_right) {
                    x_start = screen_w - gap - extra_pixels - block_width;
                    is_right = true;
                } else {
                    x_start = gap + extra_pixels;
                }
            }

            const char *custom_heading = NULL;
            if (s_settings.player_mode) {
                if (center_round || is_chalk) {
                    custom_heading = "PLAYER1";
                } else if (is_right) {
                    custom_heading = "PLAYER2";
                } else {
                    custom_heading = "PLAYER1";
                }
            }

            bool flash_enabled = (content == 1) ? s_settings.flash_while_charging : false;

            draw_hud_block(ctx, t, font, content,
                           x_start, top_y, block_width, font_height,
                           red, green, flash_enabled, custom_heading);
        }
    }

    if (s_settings.show_battery) {
        BatteryChargeState state = battery_state_service_peek();
        int battery = state.charge_percent;
        int digit;
        if (battery == 0) digit = 0;
        else if (battery == 100) digit = 9;
        else digit = battery / 10;

        char gal_text[] = "GAL ";
        char digit_text[4];
        snprintf(digit_text, sizeof(digit_text), "%d", digit);

        GSize gal_size = graphics_text_layout_get_content_size(gal_text, font, big_box,
                                        GTextOverflowModeWordWrap, GTextAlignmentLeft);
        GSize digit_size = graphics_text_layout_get_content_size(digit_text, font, big_box,
                                        GTextOverflowModeWordWrap, GTextAlignmentLeft);
        int total_width = gal_size.w + digit_size.w;

        int right_gap = (s_char_width > 0) ? s_char_width : 12;
        int batt_y_final = batt_y - bottom_extra_y;
        if (batt_y_final < 0) batt_y_final = 0;

        int x_start;
        if (center_round) {
            x_start = (screen_w - total_width) / 2;
        } else {
            int extra_pixels_right = (s_char_width > 0) ? (s_char_width * horiz_extra) : (12 * horiz_extra);
            x_start = screen_w - right_gap - extra_pixels_right - total_width;
        }

        char full_text[8];
        snprintf(full_text, sizeof(full_text), "GAL %d", digit);

        graphics_context_set_text_color(ctx, GColorBlack);
        graphics_draw_text(ctx, full_text, font,
                           GRect(x_start + 1, batt_y_final + 1, total_width, font_height),
                           GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
        graphics_context_set_text_color(ctx, yellow);
        graphics_draw_text(ctx, gal_text, font,
                           GRect(x_start, batt_y_final, gal_size.w, font_height),
                           GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
        graphics_context_set_text_color(ctx, green);
        graphics_draw_text(ctx, digit_text, font,
                           GRect(x_start + gal_size.w, batt_y_final, digit_size.w, font_height),
                           GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);

        if (quick_view) {
            char small_clock_str[16];
            format_small_clock(small_clock_str, sizeof(small_clock_str), t);

            GSize clock_size = graphics_text_layout_get_content_size(small_clock_str, font, big_box,
                                            GTextOverflowModeWordWrap, GTextAlignmentLeft);
            int clock_width = clock_size.w;

            int clock_x = gap + extra_pixels;
            int clock_y_pos = batt_y_final;

#ifdef PBL_COLOR
            GColor clock_color = GColorFromRGB(215, 43, 175);
#else
            GColor clock_color = GColorWhite;
#endif

            graphics_context_set_text_color(ctx, GColorBlack);
            graphics_draw_text(ctx, small_clock_str, font,
                               GRect(clock_x + 1, clock_y_pos + 1, clock_width, font_height),
                               GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
            graphics_context_set_text_color(ctx, clock_color);
            graphics_draw_text(ctx, small_clock_str, font,
                               GRect(clock_x, clock_y_pos, clock_width, font_height),
                               GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
        }
    }
}

// ================== BATTERY & FLASH ==================
static void flash_timer_callback(void *data) {
    s_flash_visible = !s_flash_visible;
    layer_mark_dirty(s_clock_layer);
    s_flash_timer = app_timer_register(s_flash_visible ? 300 : 200, flash_timer_callback, NULL);
}

static void start_flash_timer(void) {
    if (s_flash_timer) return;
    s_flash_visible = true;
    layer_mark_dirty(s_clock_layer);
    s_flash_timer = app_timer_register(300, flash_timer_callback, NULL);
}

static void stop_flash_timer(void) {
    if (s_flash_timer) {
        app_timer_cancel(s_flash_timer);
        s_flash_timer = NULL;
    }
    s_flash_visible = true;
    layer_mark_dirty(s_clock_layer);
}

static void battery_callback(BatteryChargeState state) {
    layer_mark_dirty(s_clock_layer);
    if (s_settings.flash_while_charging && state.is_charging) {
        start_flash_timer();
    } else {
        stop_flash_timer();
    }
}

// ================== PCM SOUND PLAYER (Emery/Flint only) ==================
#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)

static uint8_t *s_active_wav = NULL;

static void wav_finished_callback(SpeakerFinishReason reason, void *ctx) {
    free(ctx);
    s_active_wav = NULL;
}

static bool parse_wav_header(const uint8_t *data, size_t size,
                              uint32_t *data_offset, uint32_t *data_size,
                              uint16_t *bits_per_sample, uint32_t *sample_rate) {
    if (size < 44) return false;
    if (*(uint32_t*)data != 0x46464952 || *(uint32_t*)(data + 8) != 0x45564157)
        return false;

    size_t offset = 12;
    *data_offset = 0;
    *data_size = 0;

    for (int chunk_idx = 0; chunk_idx < 100; chunk_idx++) {
        if (offset + 8 > size) break;

        uint32_t chunk_id = *(uint32_t*)(data + offset);
        uint32_t chunk_size = *(uint32_t*)(data + offset + 4);

        if (chunk_id == 0x20746d66) { // "fmt "
            if (offset + 8 + 16 > size) return false;
            uint16_t audio_format = *(uint16_t*)(data + offset + 8);
            uint16_t num_channels   = *(uint16_t*)(data + offset + 10);
            *sample_rate            = *(uint32_t*)(data + offset + 12);
            *bits_per_sample        = *(uint16_t*)(data + offset + 22);

            if (audio_format != 1) return false;
            if (num_channels != 1) return false;
            if (*bits_per_sample != 8 && *bits_per_sample != 16) return false;
        } else if (chunk_id == 0x61746164) { // "data"
            if (offset + 8 + chunk_size > size) return false;
            *data_offset = offset + 8;
            *data_size = chunk_size;
            return true;
        }

        offset += 8 + ((chunk_size + 1) & ~1);
    }

    return (*data_offset != 0 && *data_size != 0);
}

static void play_wav_resource(uint32_t resource_id) {
    if (s_active_wav != NULL) {
        speaker_set_finish_callback(NULL, NULL);
        speaker_stop();
        free(s_active_wav);
        s_active_wav = NULL;
    }

    ResHandle rh = resource_get_handle(resource_id);
    size_t res_size = resource_size(rh);
    if (res_size == 0 || res_size > 65536) return;

    uint8_t *wav_data = malloc(res_size);
    if (!wav_data) return;

    resource_load_byte_range(rh, 0, wav_data, res_size);

    uint32_t data_offset = 0, data_size = 0;
    uint16_t bits_per_sample = 0;
    uint32_t sample_rate = 0;

    if (!parse_wav_header(wav_data, res_size, &data_offset, &data_size,
                          &bits_per_sample, &sample_rate)) {
        free(wav_data);
        return;
    }

    if (data_offset + data_size > res_size) {
        free(wav_data);
        return;
    }

    if (data_size > 16384) {
        data_size = 16384;
    }

    if (bits_per_sample == 8) {
        uint8_t *samples = wav_data + data_offset;
        for (uint32_t i = 0; i < data_size; i++) {
            samples[i] ^= 0x80;
        }
    }

    SpeakerPcmFormat format;
    if (sample_rate == 8000) {
        format = (bits_per_sample == 16) ? SpeakerPcmFormat_8kHz_16bit : SpeakerPcmFormat_8kHz_8bit;
    } else if (sample_rate == 16000) {
        format = (bits_per_sample == 16) ? SpeakerPcmFormat_16kHz_16bit : SpeakerPcmFormat_16kHz_8bit;
    } else {
        free(wav_data);
        return;
    }

    SpeakerSample sample = {
        .data = wav_data + data_offset,
        .num_bytes = data_size,
        .format = format,
        .base_midi_note = 60,
        .loop = false
    };

    SpeakerNote note = {
        .midi_note = 60,
        .waveform = 0,
        .duration_ms = 30000,
        .velocity = 127
    };

    SpeakerTrack track = {
        .notes = &note,
        .num_notes = 1,
        .sample = &sample
    };

    speaker_set_finish_callback(wav_finished_callback, wav_data);
    if (speaker_play_tracks(&track, 1, 80)) {
        s_active_wav = wav_data;
    } else {
        free(wav_data);
        speaker_set_finish_callback(NULL, NULL);
    }
}
#endif

// ================== DEBUG FUNCTIONS ==================
#if ENABLE_DEBUG
static void update_debug_text() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int secs = 0;
    char label[32] = "";
    switch (s_settings.frequency) {
        case 0: secs = 3600 - (t->tm_min * 60 + t->tm_sec);
            snprintf(label, sizeof(label), "Next: %dh%02dm%02ds", secs/3600, (secs%3600)/60, secs%60); break;
        case 1: secs = 60 - t->tm_sec; if (secs == 60) secs = 0;
            snprintf(label, sizeof(label), "Next: %ds", secs); break;
        case 2: snprintf(label, sizeof(label), "Next: Off"); break;
        case 3: snprintf(label, sizeof(label), "Next: Startup"); break;
        default: snprintf(label, sizeof(label), "Next: ???"); break;
    }
    char forced[32] = "";
    if (DEBUG_FORCE_ANIM >= 0 && DEBUG_FORCE_ANIM < NUM_BACKGROUNDS) {
        snprintf(forced, sizeof(forced), " [Forced: %d]", DEBUG_FORCE_ANIM);
    }
    int display = s_current_frame + 1;
    if (display > NUM_FRAMES) display = NUM_FRAMES;
    snprintf(s_debug_text, sizeof(s_debug_text), "BG: %d%s  Frame: %d/%d (%s)\n%s",
             s_current_bg_index, forced, display, NUM_FRAMES, FRAME_LABEL, label);
    text_layer_set_text(s_debug_layer, s_debug_text);
}

static void debug_timer_callback(void *data) {
    s_fake_time_toggle = !s_fake_time_toggle;
    layer_mark_dirty(s_clock_layer);

    if (DEBUG_FORCE_BT_ANIM) {
        if (!s_bt_animating) {
            APP_LOG(APP_LOG_LEVEL_INFO, "Debug: Starting BT animation");
            bt_animation_start();
        } else {
            APP_LOG(APP_LOG_LEVEL_INFO, "Debug: BT animation already active, skipping");
        }
    } else {
        APP_LOG(APP_LOG_LEVEL_INFO, "Debug: Starting regular animation");
        start_animation();
    }

    s_debug_timer = app_timer_register(5000, debug_timer_callback, NULL);
}
#endif

// ================== ANIMATION ENGINE ==================
static void timer_callback(void *data) {
    if (!s_is_animating) return;
    s_current_frame++;
    if (s_current_frame < NUM_FRAMES) {
        if (load_foreground_frame(s_current_frame)) {
            #if ENABLE_DEBUG
                update_debug_text();
            #endif
            s_timer = app_timer_register(FRAME_DURATION_MS, timer_callback, NULL);
        } else {
            s_is_animating = false;
            s_timer = NULL;
            APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to load frame %d", s_current_frame);
        }
    } else {
        s_is_animating = false;
        s_timer = NULL;
        #if ENABLE_DEBUG
            update_debug_text();
        #endif
        APP_LOG(APP_LOG_LEVEL_INFO, "Animation finished");
    }
}

static void start_animation() {
    if (s_bt_animating) return;
    if (s_timer) {
        app_timer_cancel(s_timer);
        s_timer = NULL;
    }
    s_is_animating = false;

#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
    play_feedback(s_settings.vibrate_on_anim_type,
                  s_settings.play_sound_on_anim,
                  s_settings.vibrate_during_quiet_time,
                  RESOURCE_ID_ANIME_SOUND);
#else
    play_feedback(s_settings.vibrate_on_anim_type,
                  s_settings.play_sound_on_anim,
                  s_settings.vibrate_during_quiet_time,
                  0);
#endif

    int bg = get_next_background();
    if (!load_background(bg)) {
        if (!load_background(s_current_bg_index)) {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to load background");
            return;
        }
    } else {
        s_current_bg_index = bg;
    }

    s_current_frame = 0;
    if (!load_foreground_frame(0)) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to load first frame");
        return;
    }
    s_is_animating = true;
    #if ENABLE_DEBUG
        update_debug_text();
    #endif
    s_timer = app_timer_register(FRAME_DURATION_MS, timer_callback, NULL);
}

static void start_animation_silent() {
    if (s_bt_animating) return;
    if (s_timer) {
        app_timer_cancel(s_timer);
        s_timer = NULL;
    }
    s_is_animating = false;

    int bg = get_next_background();
    if (!load_background(bg)) {
        if (!load_background(s_current_bg_index)) {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to load background");
            return;
        }
    } else {
        s_current_bg_index = bg;
    }

    s_current_frame = 0;
    if (!load_foreground_frame(0)) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to load first frame");
        return;
    }
    s_is_animating = true;
    #if ENABLE_DEBUG
        update_debug_text();
    #endif
    s_timer = app_timer_register(FRAME_DURATION_MS, timer_callback, NULL);
}

static void start_animation_with_current_bg(bool is_startup) {
    s_startup_animation_pending = false;   // clear race flag

    if (s_bt_animating) return;
    if (s_timer) {
        app_timer_cancel(s_timer);
        s_timer = NULL;
    }
    s_is_animating = false;

    int vibe = s_settings.vibrate_on_anim_type;
    bool sound = s_settings.play_sound_on_anim;
    if (is_startup) {
        if (s_settings.suppress_startup_vibes) vibe = 0;
        if (s_settings.suppress_startup_sounds) sound = false;
    }

#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
    play_feedback(vibe, sound, s_settings.vibrate_during_quiet_time, RESOURCE_ID_ANIME_SOUND);
#else
    play_feedback(vibe, sound, s_settings.vibrate_during_quiet_time, 0);
#endif

    s_current_frame = 0;
    s_is_animating = true;
    #if ENABLE_DEBUG
        update_debug_text();
    #endif
    s_timer = app_timer_register(FRAME_DURATION_MS, timer_callback, NULL);
}

// ================== ACCELEROMETER CALLBACK ==================
static void accel_tap_callback(AccelAxisType axis, int32_t direction) {
    if (s_bt_animating) return;
    if (s_settings.animate_on_flick && !s_is_animating) {
        int bg = get_next_background();
        if (!load_background(bg)) {
            load_background(s_current_bg_index);
        } else {
            s_current_bg_index = bg;
        }
        s_current_frame = 0;
        if (load_foreground_frame(0)) {
            s_is_animating = true;
#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
            play_feedback(s_settings.vibrate_on_anim_type,
                          s_settings.play_sound_on_anim,
                          s_settings.vibrate_during_quiet_time,
                          RESOURCE_ID_ANIME_SOUND);
#else
            play_feedback(s_settings.vibrate_on_anim_type,
                          s_settings.play_sound_on_anim,
                          s_settings.vibrate_during_quiet_time,
                          0);
#endif
            #if ENABLE_DEBUG
                update_debug_text();
            #endif
            s_timer = app_timer_register(FRAME_DURATION_MS, timer_callback, NULL);
        }
    }
}

// ================== TIME HANDLER ==================
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    if (units_changed & MINUTE_UNIT) {
        layer_mark_dirty(s_clock_layer);
    }

    if (units_changed & SECOND_UNIT) {
        bool show_seconds = false;
#if defined(PBL_PLATFORM_EMERY)
        if (s_settings.hud_content_left == 4 || s_settings.hud_content_right == 4) {
            show_seconds = true;
        }
#else
        if (s_settings.hud_content == 4) {
            show_seconds = true;
        }
#endif
        if (show_seconds) {
            layer_mark_dirty(s_clock_layer);
        }
    }

    #if ENABLE_DEBUG
        update_debug_text();
    #endif
    
    bool hour_feedback_played = false;
    
    if (units_changed & HOUR_UNIT) {
        int current_hour = tick_time->tm_hour;
        if (s_last_hour != current_hour) {
            s_last_hour = current_hour;
            bool hour_enabled = (s_settings.vibrate_every_hour_type != 0) || s_settings.play_sound_every_hour;
            if (hour_enabled) {
#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
                play_feedback(s_settings.vibrate_every_hour_type,
                              s_settings.play_sound_every_hour,
                              s_settings.vibrate_during_quiet_time,
                              RESOURCE_ID_ANIME_SOUND);
#else
                play_feedback(s_settings.vibrate_every_hour_type,
                              s_settings.play_sound_every_hour,
                              s_settings.vibrate_during_quiet_time,
                              0);
#endif
                hour_feedback_played = true;
            }
        }
    }
    
    if (s_bt_animating) return;
    
    if (s_settings.frequency == 1 && (units_changed & MINUTE_UNIT)) {
        if (hour_feedback_played) {
            start_animation_silent();
        } else {
            start_animation();
        }
    } else if (s_settings.frequency == 0 && (units_changed & HOUR_UNIT)) {
        if (hour_feedback_played) {
            start_animation_silent();
        } else {
            start_animation();
        }
    }
}

// ================== APPMESSAGE ==================
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    bool changed = false;
    
    // --- Animation ---
    Tuple *freq = dict_find(iterator, MESSAGE_KEY_Frequency);
    if (freq) {
        int new_freq = atoi(freq->value->cstring);
        if (new_freq >= 0 && new_freq <= 3) { s_settings.frequency = new_freq; changed = true; }
    }
    
    Tuple *flick = dict_find(iterator, MESSAGE_KEY_AnimateOnFlick);
    if (flick) {
        s_settings.animate_on_flick = flick->value->int32 == 1;
        changed = true;
    }
    
    Tuple *bt_anim = dict_find(iterator, MESSAGE_KEY_BluetoothDisconnectAnimation);
    if (bt_anim) {
        s_settings.bt_animation = bt_anim->value->int32 == 1;
        changed = true;
    }
    
    // --- Display ---
    Tuple *hud = dict_find(iterator, MESSAGE_KEY_HudContent);
    if (hud) {
        int val = atoi(hud->value->cstring);
        if (val >= 0 && val <= 4) { s_settings.hud_content = val; changed = true; }
    }
    
    Tuple *hud_left = dict_find(iterator, MESSAGE_KEY_HudContentLeft);
    if (hud_left) {
        int val = atoi(hud_left->value->cstring);
        if (val >= 0 && val <= 4) { s_settings.hud_content_left = val; changed = true; }
    }
    
    Tuple *hud_right = dict_find(iterator, MESSAGE_KEY_HudContentRight);
    if (hud_right) {
        int val = atoi(hud_right->value->cstring);
        if (val >= 0 && val <= 4) { s_settings.hud_content_right = val; changed = true; }
    }
    
    Tuple *current_right = dict_find(iterator, MESSAGE_KEY_CurrentRight);
    if (current_right) {
        s_settings.current_right = current_right->value->int32 == 1;
        changed = true;
    }
    
    Tuple *flash = dict_find(iterator, MESSAGE_KEY_FlashCurrentWhileCharging);
    if (flash) {
        s_settings.flash_while_charging = flash->value->int32 == 1;
        changed = true;
        BatteryChargeState state = battery_state_service_peek();
        if (s_settings.flash_while_charging && state.is_charging) {
            start_flash_timer();
        } else {
            stop_flash_timer();
        }
    }
    
    Tuple *flash_side_t = dict_find(iterator, MESSAGE_KEY_FlashSide);
    if (flash_side_t) {
        int val = atoi(flash_side_t->value->cstring);
        if (val >= 0 && val <= 2) { s_settings.flash_side = val; changed = true; }
    }
    
    Tuple *show_bat = dict_find(iterator, MESSAGE_KEY_ShowBattery);
    if (show_bat) {
        s_settings.show_battery = show_bat->value->int32 == 1;
        changed = true;
    }
    
    // --- Player Mode ---
    Tuple *player_mode = dict_find(iterator, MESSAGE_KEY_PlayerMode);
    if (player_mode) {
        s_settings.player_mode = player_mode->value->int32 == 1;
        changed = true;
    }
    
    // --- Haptics ---
    Tuple *vib_anim_type = dict_find(iterator, MESSAGE_KEY_VibrateOnAnimationType);
    if (vib_anim_type) {
        int val = atoi(vib_anim_type->value->cstring);
        if (val >= 0 && val <= 4) { s_settings.vibrate_on_anim_type = val; changed = true; }
    }
    
    Tuple *vib_bt_type = dict_find(iterator, MESSAGE_KEY_VibrateOnBTDisconnectType);
    if (vib_bt_type) {
        int val = atoi(vib_bt_type->value->cstring);
        if (val >= 0 && val <= 4) { s_settings.vibrate_on_bt_type = val; changed = true; }
    }
    
    Tuple *vib_hour_type = dict_find(iterator, MESSAGE_KEY_VibrateEveryHourType);
    if (vib_hour_type) {
        int val = atoi(vib_hour_type->value->cstring);
        if (val >= 0 && val <= 4) { s_settings.vibrate_every_hour_type = val; changed = true; }
    }
    
    Tuple *vib_quiet = dict_find(iterator, MESSAGE_KEY_VibrateDuringQuietTime);
    if (vib_quiet) {
        s_settings.vibrate_during_quiet_time = vib_quiet->value->int32 == 1;
        changed = true;
    }
    
    Tuple *suppress_vibes = dict_find(iterator, MESSAGE_KEY_SuppressStartupVibes);
    if (suppress_vibes) {
        s_settings.suppress_startup_vibes = suppress_vibes->value->int32 == 1;
        changed = true;
    }
    
    // --- Sound ---
    Tuple *sound_anim = dict_find(iterator, MESSAGE_KEY_PlaySoundOnAnimation);
    if (sound_anim) {
        s_settings.play_sound_on_anim = sound_anim->value->int32 == 1;
        changed = true;
    }
    
    Tuple *sound_bt = dict_find(iterator, MESSAGE_KEY_PlaySoundOnBTDisconnect);
    if (sound_bt) {
        s_settings.play_sound_on_bt = sound_bt->value->int32 == 1;
        changed = true;
    }
    
    Tuple *sound_hour = dict_find(iterator, MESSAGE_KEY_PlaySoundEveryHour);
    if (sound_hour) {
        s_settings.play_sound_every_hour = sound_hour->value->int32 == 1;
        changed = true;
    }
    
    Tuple *sound_quiet = dict_find(iterator, MESSAGE_KEY_SoundDuringQuietTime);
    if (sound_quiet) {
        s_settings.sound_during_quiet_time = sound_quiet->value->int32 == 1;
        changed = true;
    }
    
    Tuple *suppress_sounds = dict_find(iterator, MESSAGE_KEY_SuppressStartupSounds);
    if (suppress_sounds) {
        s_settings.suppress_startup_sounds = suppress_sounds->value->int32 == 1;
        changed = true;
    }
    
    // --- Clock ---
    Tuple *clock = dict_find(iterator, MESSAGE_KEY_ClockMode);
    if (clock) {
        int new_mode = atoi(clock->value->cstring);
        if (new_mode >= 0 && new_mode <= 2) { s_settings.clock_mode = new_mode; changed = true; }
    }
    
    Tuple *zeros = dict_find(iterator, MESSAGE_KEY_LeadingZeros);
    if (zeros) {
        s_settings.leading_zeros = zeros->value->int32 == 1;
        changed = true;
    }
    
    Tuple *ampm = dict_find(iterator, MESSAGE_KEY_AMPM_Position);
    if (ampm) {
        int new_pos = atoi(ampm->value->cstring);
        if (new_pos >= 0 && new_pos <= 2) { s_settings.ampm_position = new_pos; changed = true; }
    }
    
    // --- Date ---
    Tuple *date_fmt = dict_find(iterator, MESSAGE_KEY_DateFormat);
    if (date_fmt) {
        int val = atoi(date_fmt->value->cstring);
        if (val >= 0 && val <= 1) { s_settings.date_format = val; changed = true; }
    }
    
    Tuple *date_sep = dict_find(iterator, MESSAGE_KEY_DateSeparator);
    if (date_sep) {
        int val = atoi(date_sep->value->cstring);
        if (val >= 0 && val <= 3) { s_settings.date_separator = val; changed = true; }
    }
    
    Tuple *month_fmt = dict_find(iterator, MESSAGE_KEY_MonthFormat);
    if (month_fmt) {
        int val = atoi(month_fmt->value->cstring);
        if (val >= 0 && val <= 1) { s_settings.month_format = val; changed = true; }
    }
    
    Tuple *date_zeros = dict_find(iterator, MESSAGE_KEY_DateLeadingZeros);
    if (date_zeros) {
        s_settings.date_leading_zeros = date_zeros->value->int32 == 1;
        changed = true;
    }
    
    // --- Phone battery ---
    Tuple *phone_batt = dict_find(iterator, MESSAGE_KEY_PhoneBattLevel);
    if (phone_batt) {
        s_phone_battery = phone_batt->value->int32;
        s_phone_battery_known = true;
        changed = true;
    }

    if (changed) {
        prv_save_settings();
        layer_mark_dirty(s_clock_layer);
    }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {}
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {}
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {}

// ================== CLEANUP ==================
static void cleanup_bitmaps() {
    if (s_bg_bitmap) {
        gbitmap_destroy(s_bg_bitmap);
        s_bg_bitmap = NULL;
    }
    if (s_fg_bitmap) {
        gbitmap_destroy(s_fg_bitmap);
        s_fg_bitmap = NULL;
    }
}

static void start_animation_wrapper(void *data) {
    bool is_startup = (data != NULL);
    start_animation_with_current_bg(is_startup);
}

static void startup_timer_callback(void *data) {
    s_startup_pending = false;

    // Record initial Bluetooth state (true if disconnected at startup)
    s_bt_disconnected = !bluetooth_connection_service_peek();

    if (s_bt_animating) return;

    if (!bluetooth_connection_service_peek() && s_settings.bt_animation) {
        bt_animation_start();
    } else {
        if (s_settings.frequency == 2) {
            load_last_frame();
        } else {
            load_foreground_frame(0);
            s_current_frame = 0;
            s_startup_animation_pending = true;
            app_timer_register(100, start_animation_wrapper, (void*)true);
        }
    }
}

// ================== MAIN WINDOW ==================
static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    window_set_background_color(window, GColorBlack);

    s_bg_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_compositing_mode(s_bg_layer, GCompOpSet);
    layer_add_child(window_layer, bitmap_layer_get_layer(s_bg_layer));

    s_fg_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_compositing_mode(s_fg_layer, GCompOpSet);
    layer_add_child(window_layer, bitmap_layer_get_layer(s_fg_layer));

    s_clock_layer = layer_create(bounds);
    layer_set_update_proc(s_clock_layer, clock_layer_update_callback);
    layer_add_child(window_layer, s_clock_layer);

    s_arcade_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ARCADE_12));
    if (!s_arcade_font) {
        APP_LOG(APP_LOG_LEVEL_WARNING, "ARCADE_12 font failed to load, using fallback");
    }

    TextLayer *temp_layer = text_layer_create(GRect(0, 0, 100, 100));
    text_layer_set_font(temp_layer, s_arcade_font ? s_arcade_font : fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text(temp_layer, "G");
    GSize char_size = text_layer_get_content_size(temp_layer);
    s_char_width = char_size.w;
    text_layer_destroy(temp_layer);
    if (s_char_width < 1) s_char_width = 12;

    battery_state_service_subscribe(battery_callback);
    bluetooth_connection_service_subscribe(bt_connection_handler);

#if ENABLE_DEBUG
    s_debug_layer = text_layer_create(GRect(5, 5, bounds.size.w - 10, 50));
    text_layer_set_text_color(s_debug_layer, GColorWhite);
    text_layer_set_background_color(s_debug_layer, GColorBlack);
    text_layer_set_font(s_debug_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
    text_layer_set_text_alignment(s_debug_layer, GTextAlignmentLeft);
    layer_add_child(window_layer, text_layer_get_layer(s_debug_layer));
    layer_set_hidden(text_layer_get_layer(s_debug_layer), !DEBUG_SHOW_INFO);
    if (DEBUG_SHOW_INFO) {
        update_debug_text();
    }

    if (DEBUG_FORCE_TIME) {
        s_fake_time = (struct tm){0};
        s_fake_time.tm_hour = DEBUG_TIME_HOUR;
        s_fake_time.tm_min  = DEBUG_TIME_MINUTE;
        s_fake_time.tm_sec  = 0;
        s_fake_time.tm_mday = 1;
        s_fake_time.tm_mon  = 0;
        s_fake_time.tm_year = 2020 - 1900;
        s_fake_time_toggle = false;
        s_debug_timer = app_timer_register(5000, debug_timer_callback, NULL);
    }
#endif

    int initial_bg = get_next_background();
    s_last_played = initial_bg;

    load_background(initial_bg);
    s_current_bg_index = initial_bg;
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    s_last_hour = t->tm_hour;

    app_timer_register(200, startup_timer_callback, NULL);

    BatteryChargeState init_state = battery_state_service_peek();
    if (s_settings.flash_while_charging && init_state.is_charging) {
        start_flash_timer();
    }
}

static void main_window_appear(Window *window) {
    if (s_startup_pending) return;

    // If startup is already scheduling an animation, don't interfere
    if (s_startup_animation_pending) {
        s_startup_animation_pending = false;
        return;
    }

    if (s_is_animating || s_bt_animating) return;

    if (s_settings.frequency == 2) {
        int bg = get_next_background();
        if (load_background(bg)) {
            s_current_bg_index = bg;
        } else {
            load_background(s_current_bg_index);
        }
        load_last_frame();
    } else if (s_settings.frequency == 3) {
        if (!s_is_animating) {
            int bg = get_next_background();
            if (load_background(bg)) {
                s_current_bg_index = bg;
            } else {
                load_background(s_current_bg_index);
            }
            start_animation_with_current_bg(true);
        }
    } else {
        if (!s_is_animating && s_timer == NULL) {
            start_animation();
        }
    }
}

static void main_window_unload(Window *window) {
    if (s_timer) { app_timer_cancel(s_timer); s_timer = NULL; }
    if (s_bt_timer) { app_timer_cancel(s_bt_timer); s_bt_timer = NULL; }
    if (s_delayed_vibe_timer) {
        app_timer_cancel(s_delayed_vibe_timer);
        s_delayed_vibe_timer = NULL;
    }
    stop_flash_timer();
    accel_tap_service_unsubscribe();
    battery_state_service_unsubscribe();
    bluetooth_connection_service_unsubscribe();

#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_FLINT)
    if (s_active_wav != NULL) {
        speaker_set_finish_callback(NULL, NULL);
        speaker_stop();
        free(s_active_wav);
        s_active_wav = NULL;
    }
#endif

    bitmap_layer_destroy(s_bg_layer);
    bitmap_layer_destroy(s_fg_layer);
    layer_destroy(s_clock_layer);
    if (s_arcade_font) {
        fonts_unload_custom_font(s_arcade_font);
    }
#if ENABLE_DEBUG
    if (s_debug_timer) {
        app_timer_cancel(s_debug_timer);
        s_debug_timer = NULL;
    }
    text_layer_destroy(s_debug_layer);
#endif
    cleanup_bitmaps();
}

// ================== INIT / DEINIT ==================
static void init() {
    prv_load_settings();
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers){
        .load = main_window_load,
        .appear = main_window_appear,
        .unload = main_window_unload
    });
    window_stack_push(s_main_window, true);

    srand(time(NULL));
    tick_timer_service_subscribe(MINUTE_UNIT | SECOND_UNIT | HOUR_UNIT, tick_handler);

    accel_tap_service_subscribe(accel_tap_callback);

    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);
    app_message_open(512, 512);
}

static void deinit() {
    tick_timer_service_unsubscribe();
    accel_tap_service_unsubscribe();
    battery_state_service_unsubscribe();
    bluetooth_connection_service_unsubscribe();
    app_message_deregister_callbacks();
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}