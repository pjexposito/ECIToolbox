#include "pebble.h"
#include "calendario.h"
#include "fpp.h"
  
#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 2
#define NUM_SECOND_MENU_ITEMS 2
  
int valor = 1;  
char valor_str[6];
int loading = 0;

static Window *window;

static MenuLayer *menu_layer;

void process_tuple(Tuple *t)
{
    int key = t->key;
    char string_value[64];
    memset(string_value, 0, 64);
    strcpy(string_value, t->value->cstring);
    persist_write_string(key, string_value);
}


void in_received_handler(DictionaryIterator *iter, void *context)
{
    (void) context;
    Tuple *t = dict_read_first(iter);
    while(t != NULL)
    {
        process_tuple(t);
        t = dict_read_next(iter);
    }
    vibes_short_pulse();
    loading = 0;
    layer_mark_dirty(menu_layer_get_layer(menu_layer));
}

void send_int(int key, int cmd)
{
  loading = 1;
  layer_mark_dirty(menu_layer_get_layer(menu_layer));
	DictionaryIterator *iter;
 	app_message_outbox_begin(&iter);
 	Tuplet value = TupletInteger(key, cmd);
 	dict_write_tuplet(iter, &value);	
 	app_message_outbox_send();
}


static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}
  
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}
  
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "Calendario de turnos");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "FPP");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, "Calendarios", "Mostrar calendarios", NULL);
          break;
        case 1:
          if (loading==0) 
            menu_cell_basic_draw(ctx, cell_layer, "Actualizar", "Último: 11-11-11", NULL);
          else
            menu_cell_basic_draw(ctx, cell_layer, "Actualizando...", "Por favor, espera.", NULL);
          break;
      }
      break;
    case 1:
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, "FPP", "Formula personal de pago", NULL);
          break; 
        case 1:
          memset(valor_str, 0, 6);
          snprintf(valor_str, sizeof(valor_str), "%d", valor);

          menu_cell_basic_draw(ctx, cell_layer, valor_str, "Formula personal de pago", NULL);
          break; 
      }
  }
}
  
  
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
      case 0:
        carga_calendario();
        break;
      case 1:
        send_int(5,5);
        break;
      }
      break;
    
    case 1:
      switch (cell_index->row) {
      case 0:
         carga_fpp();
         break;
      case 1:
         valor++;
         layer_mark_dirty(menu_layer_get_layer(menu_layer));
         break;
      }
      break;    
  }
}
  
  



static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  
  menu_layer_set_click_config_onto_window(menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(menu_layer);

}

int main(void) {
  window = window_create();
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());		
  app_message_register_inbox_received(in_received_handler);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  app_event_loop();
  window_destroy(window);
}