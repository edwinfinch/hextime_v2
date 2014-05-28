/*
Hextime 2.0
Based on the work of pokey9000's SDK 1.0 version: https://github.com/pokey9000/hextime
Ported by Edwin Finch

Under MIT license 
*/

#include <pebble.h>

Window *window;

TextLayer *time_layer;
GFont *main_font;
int hour, minute;
char time_text[] = "00:00..";
char buffer[6];
char nyb2hex[] = "0123456789ABCDEF";

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	minute = tick_time->tm_min;
	hour = tick_time->tm_hour;

	time_text[0]=nyb2hex[(((hour) & 0xF0) >> 4)];
	time_text[1]=nyb2hex[((hour) & 0x0F)];
	time_text[3]=nyb2hex[(((minute) & 0xF0) >> 4)];
	time_text[4]=nyb2hex[((minute) & 0x0F)];
	snprintf(buffer, sizeof(buffer), "%s", time_text);
	text_layer_set_text(time_layer, buffer);
}

void init() {

	window = window_create();
	window_set_background_color(window, GColorBlack);
	
	main_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TERMINUS_56));
	
	time_layer = text_layer_create(GRect(0, 45, 144, 168));
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_text_color(time_layer, GColorWhite);
	text_layer_set_font(time_layer, main_font);
	text_layer_set_text(time_layer, "boot");
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
	tick_timer_service_subscribe(MINUTE_UNIT, &tick_handler);
	window_stack_push(window, true);
	
	struct tm *t;
  	time_t temp;        
  	temp = time(NULL);        
  	t = localtime(&temp);
	
	tick_handler(t, MINUTE_UNIT);
}

void deinit(){
	text_layer_destroy(time_layer);
	window_destroy(window);
	fonts_unload_custom_font(main_font);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
