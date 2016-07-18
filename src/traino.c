#include <pebble.h>

#include "events.h"

static const int BATTERY_LEVEL_WIDTH_PX = 144;
static const int BATTERY_LEVEL_HEIGHT_PX = 5;

static Window *s_main_window;
static TextLayer *s_time_layer;

static BitmapLayer *s_image_layer;
static GBitmap *s_image_bitmap;

static int s_battery_percent;
static Layer *s_battery_level_layer;

static int s_current_event_image;

static void update_time_display() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[8];
  strftime(
    s_buffer,
    sizeof(s_buffer),
    clock_is_24h_style() ? "%H:%M" : "%l:%M", // %I = pad left with 0, %l no 0 pad
    tick_time
  );

  text_layer_set_text(s_time_layer, s_buffer);
}

static int determine_event(int hour, int minute) {
  int event = -1;
  int last_event;

  // APP_LOG(APP_LOG_LEVEL_DEBUG, "determine_event: hour:minute=%i:%i", hour, minute);

  uint num_scheduled_events = (uint)(sizeof scheduled_events / sizeof *scheduled_events);

  for (uint i = 0; i < num_scheduled_events; i++) {
    int scheduled_hour = scheduled_events[i][SCHEDULED_EVENT_HOUR_INDEX];
    int scheduled_minute = scheduled_events[i][SCHEDULED_EVENT_MINUTE_INDEX];
    last_event = scheduled_events[i][SCHEDULED_EVENT_EVENT_INDEX];

    int minutes_since_midnight = hour * MINUTES_PER_HOUR + minute;
    int scheduled_minutes_since_midnight = scheduled_hour * MINUTES_PER_HOUR + scheduled_minute;

    // APP_LOG(APP_LOG_LEVEL_DEBUG, "determine_event: scheduled=%i:%i, minutes:scheduled=%i:%i",
    //   scheduled_hour, scheduled_minute, minutes_since_midnight, scheduled_minutes_since_midnight);

    // ASSUME scheduled_events are already sorted in chronological order
    if (scheduled_minutes_since_midnight <= minutes_since_midnight) {
      event = last_event;
    }
  }

  // APP_LOG(APP_LOG_LEVEL_DEBUG, "determine_event: event=%i, last_event=%i", event, last_event);

  if (event == -1) {
    return last_event;
  } else {
    return event;
  }
}

static int determine_event_image(int hour, int minute) {
  int event = determine_event(hour, minute);
  if (event > 0) {
    return ImagesByEvent[event];
  } else {
    return RESOURCE_ID_IMAGE_LEAF;
  }
}

static void set_image(Layer *window_layer, GRect bounds, int resource_id) {
  if (s_image_bitmap != NULL) {
    gbitmap_destroy(s_image_bitmap);
  }

  if (s_image_layer != NULL) {
    layer_remove_from_parent(bitmap_layer_get_layer(s_image_layer));
    bitmap_layer_destroy(s_image_layer);
  }

  s_image_bitmap = gbitmap_create_with_resource(resource_id);
  // push the image down a bit to allow for the text at the top
  s_image_layer = bitmap_layer_create(
    GRect(
      0,                         // x from left
      24,                        // y from top
      bounds.size.w,             // width
      bounds.size.h              // height
    )
  );
  bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_image_layer));
}

static void update_event_image() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  int desired_event_image = determine_event_image(
    tick_time->tm_hour, tick_time->tm_min);

  // APP_LOG(APP_LOG_LEVEL_DEBUG, "update_event_image: s_current_event_image=%i, desired_event_image=%i", s_current_event_image, desired_event_image);

  if (s_current_event_image != desired_event_image) {
    APP_LOG(APP_LOG_LEVEL_INFO, "update_event_image: changing image from %i to %i", s_current_event_image, desired_event_image);
    s_current_event_image = desired_event_image;
    vibes_double_pulse();

    Window *window = window_stack_get_top_window(); // !! TODO: check if this is right
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    set_image(window_layer, bounds, s_current_event_image);
  }
}

static void on_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_event_image();
  update_time_display();
}

static void set_up_time_text(Layer *window_layer, GRect bounds) {
  // screen resolution (classic) 144×168
  s_time_layer = text_layer_create(
    GRect(
      0,                         // x from left
      5,                         // y from top
      bounds.size.w,             // width
      30                         // height
    )
  );
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void draw_battery_level(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  int bar_width = (int)(float)(
    ((float)s_battery_percent / 100.0F) *
    (float)BATTERY_LEVEL_WIDTH_PX);

  // Background
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Bar
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(
    ctx,
    GRect(0, 0, bar_width, bounds.size.h),
    0,
    GCornerNone);
}

static void set_up_battery_layer(Layer *window_layer, GRect bounds) {
  s_battery_level_layer = layer_create(GRect(0, 0, BATTERY_LEVEL_WIDTH_PX, BATTERY_LEVEL_HEIGHT_PX));
  layer_set_update_proc(s_battery_level_layer, draw_battery_level);
  layer_add_child(window_layer, s_battery_level_layer);
}

static void on_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  set_image(window_layer, bounds, RESOURCE_ID_IMAGE_LEAF);
  set_up_time_text(window_layer, bounds);
  set_up_battery_layer(window_layer, bounds);
}

static void on_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(s_image_bitmap);
  bitmap_layer_destroy(s_image_layer);
  layer_destroy(s_battery_level_layer);
}

static void on_battery_state_change(BatteryChargeState state) {
  s_battery_percent = state.charge_percent;
}

static void init(void) {
  s_main_window = window_create();

  // window_set_background_color(s_main_window, GColorBlack);

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = on_window_load,
    .unload = on_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_main_window, animated);

  update_event_image();

  update_time_display();
  tick_timer_service_subscribe(MINUTE_UNIT, on_tick);

  on_battery_state_change(battery_state_service_peek());
  battery_state_service_subscribe(on_battery_state_change);
}

static void deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
