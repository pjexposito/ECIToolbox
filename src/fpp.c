#include <pebble.h>
#include "fpp.h"


// Valor por defecto 
#define NUM_DEFAULT 0
#define MESES_DEFAULT 12

  
// Claves donde se guardarán los datos del programa
#define KEY_FINANCIACION 97
#define KEY_PRECIO 98
#define KEY_MESES 99
  


// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static Layer *marcador;
static GFont s_res_gothic_24;
static GFont s_res_bitham_42_light;
static GFont s_res_gothic_14;
static TextLayer *lblImporte_layer;
static TextLayer *dig1_layer;
static TextLayer *dig2_layer;
static TextLayer *lblMeses_layer;
static TextLayer *lblIntereses_layer;
static TextLayer *dig3_layer;
static TextLayer *dig4_layer;
static TextLayer *digmeses_layer;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *txtIntereses_layer;
static TextLayer *txtCuota_layer;
static TextLayer *txtInfo_layer;

GBitmap *arriba_bitmap, *abajo_bitmap, *pulsar_bitmap, *play_bitmap, *buscar_bitmap;

static int numero1=0, numero2=0, numero3=0, numero4=0, meses, posicion=0, tipo_financiacion;
static float cuota, intereses;


void carga_datos_fpp(void)
{
  tipo_financiacion = persist_exists(KEY_FINANCIACION) ? persist_read_int(KEY_FINANCIACION) : NUM_DEFAULT;
  meses = persist_exists(KEY_MESES) ? persist_read_int(KEY_MESES) : NUM_DEFAULT;
  unsigned int dinero = persist_exists(KEY_PRECIO) ? persist_read_int(KEY_PRECIO) : NUM_DEFAULT;
  numero1 = dinero/1000;
  numero2 = (dinero % 1000) /100;
  numero3 = (dinero % 100) /10;
  numero4 = (dinero % 10);
}

void salva_datos_fpp(void)
{
  persist_write_int(KEY_PRECIO, (numero1*1000) + (numero2*100) + (numero3*10) + numero4);
  persist_write_int(KEY_FINANCIACION, tipo_financiacion);
  persist_write_int(KEY_MESES, meses);
}

float potencia_alt(float n,float p)
{
if (p==0) return(1);
if (p==1) return(n);
return(n*potencia_alt(n,p-1));
}

float potencia(float base,float exponente)
{
  float pot = base;
  for (int i = 1; i< exponente; i++) 
    {
      pot*=base;
    }
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "potencia de: %d al %d es %d", (int)base, (int)exponente, (int)pot );

   return pot;

}

void calcula(void)
{
  int precio=(numero1*1000)+(numero2*100)+(numero3*10)+numero4;
  float CalculoTemporal;
	float IndiceIntereses;
	float CalculoArriba;
	float CalculoAbajo;
  int TIN = 18;
  if ((tipo_financiacion == 0)  && (meses <= 12)) // Sin intereses
    {	
      intereses = 3;
	    if ((precio >= 0) && (precio <= 200)) intereses = 3;
	    if ((precio > 200) && (precio <= 400)) intereses = 5;
	    if ((precio > 400) && (precio <= 600)) intereses = 7;
	    if ((precio > 600) && (precio <= 1000)) intereses = 9;
	    if (precio > 1000) intereses = 12;
      cuota = (precio+intereses)/meses;
    }
  else
    {
      IndiceIntereses = ((float) TIN)/1200;
	    CalculoArriba = (float) (IndiceIntereses*(potencia_alt((IndiceIntereses+1),meses)));
	    CalculoAbajo = (float) (potencia_alt((IndiceIntereses+1),meses)-1);
	    CalculoTemporal = ((float) CalculoArriba / (float) CalculoAbajo)*(float)precio;
      cuota = CalculoTemporal;
      intereses = (cuota*meses)-precio;
    }
}  

void pinta_datos_fpp(void)
{
  static char buffer1[]="12",buffer2[]="12",buffer3[]="12",buffer4[]="12",buffer5[]="12",buffer6[]="9999.99",buffer7[]="9999.99";
  
  calcula();

  snprintf(buffer1, sizeof(buffer1), "%d", numero1);
	text_layer_set_text(dig1_layer, buffer1);
  snprintf(buffer2, sizeof(buffer2), "%d", numero2);
	text_layer_set_text(dig2_layer, buffer2);
  snprintf(buffer3, sizeof(buffer3), "%d", numero3);
	text_layer_set_text(dig3_layer, buffer3);
  snprintf(buffer4, sizeof(buffer4), "%d", numero4);
	text_layer_set_text(dig4_layer, buffer4);
  snprintf(buffer5, sizeof(buffer5), "%d", meses);
	text_layer_set_text(digmeses_layer, buffer5);  
  snprintf(buffer6, sizeof(buffer6), "%d", (int)(cuota));
	text_layer_set_text(txtCuota_layer, buffer6); 
  snprintf(buffer7, sizeof(buffer7), "%d", (int)(intereses));
	text_layer_set_text(txtIntereses_layer, buffer7); 
  if (tipo_financiacion==0)
    text_layer_set_text(txtInfo_layer, "Sin intereses");
  else
    text_layer_set_text(txtInfo_layer, "Con intereses");
}

void marcador_update_callback(Layer *me, GContext* ctx) 
{

  // Color del fondo y color del trazo
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);

	switch(posicion) 
    {
		case 0:
      graphics_fill_rect(ctx, GRect(71, 28, 11, 1), 0, GCornerNone);  
      break;
		case 1:
      graphics_fill_rect(ctx, GRect(82, 28, 11, 1), 0, GCornerNone);    
			break;
		case 2:
      graphics_fill_rect(ctx, GRect(93, 28, 11, 1), 0, GCornerNone);  
			break;    
		case 3:
      graphics_fill_rect(ctx, GRect(104, 28, 11, 1), 0, GCornerNone);  
			break;    
    case 4:
      graphics_fill_rect(ctx, GRect(59, 56, 20, 1), 0, GCornerNone);  
			break;  
    }
} 

void up_click_handler_fpp(ClickRecognizerRef recognizer, void *context) 
{
   switch(posicion) 
    {
		case 0:
      numero1==9 ? numero1=0 : numero1++;
      break;
		case 1:
      numero2==9 ? numero2=0 : numero2++;
      break;
		case 2:
      numero3==9 ? numero3=0 : numero3++;
      break;
	  case 3:      
      numero4==9 ? numero4=0 : numero4++;
      break;   
	  case 4:      
      meses==36 ? meses=1 : meses++;
      break;      
    }
  pinta_datos_fpp();
}

void down_click_handler_fpp(ClickRecognizerRef recognizer, void *context) 
{
  switch(posicion) 
    {
		case 0:
      numero1==0 ? numero1=9 : numero1--;
      break;
		case 1:
      numero2==0 ? numero2=9 : numero2--;
      break;
		case 2:
      numero3==0 ? numero3=9 : numero3--;
      break;
	  case 3:
      numero4==0 ? numero4=9 : numero4--;
      break;   
    case 4:
      meses==1 ? meses=36 : meses--;
      break; 
    }
  pinta_datos_fpp();
}

void select_click_handler_fpp(ClickRecognizerRef recognizer, void *context)
{
  switch(posicion) 
    {
		case 0:
      posicion=1;
      break;
		case 1:
      posicion=2;
			break;
		case 2:
      posicion=3;
      break;    
		case 3:
      posicion=4;
      break;  
    case 4:
      posicion=0;
      break;  
    }
  layer_mark_dirty(marcador);
  pinta_datos_fpp();
}

void down_long_click_handler_fpp(ClickRecognizerRef recognizer, void *context)
{
    tipo_financiacion==0 ? tipo_financiacion++ : tipo_financiacion--;
    pinta_datos_fpp();
}

void select_long_click_handler_fpp(ClickRecognizerRef recognizer, void *context)
{
    int precio=(numero1*1000)+(numero2*100)+(numero3*10)+numero4;
    precio = precio/1.21;
    numero1 = precio/1000;
    numero2 = (precio % 1000) /100;
    numero3 = (precio % 100) /10;
    numero4 = (precio % 10);
    pinta_datos_fpp();
}

void click_config_provider_fpp(void *context) 
{
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler_fpp);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler_fpp);
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler_fpp);
  window_long_click_subscribe(BUTTON_ID_DOWN, 2000, down_long_click_handler_fpp, NULL);
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler_fpp, NULL);

}

static void initialise_ui(void) {
  s_window = window_create();
#ifdef PBL_SDK_2
  window_set_fullscreen(s_window, true);
#endif
  carga_datos_fpp();
  
  
    //Asignación de recursos gráficos
  arriba_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICONO_ARRIBA);
  abajo_bitmap =  gbitmap_create_with_resource(RESOURCE_ID_ICONO_ABAJO);
  pulsar_bitmap =  gbitmap_create_with_resource(RESOURCE_ID_ICON_PULSAR);
  play_bitmap =  gbitmap_create_with_resource(RESOURCE_ID_ICONO_PLAY);

  
  s_res_gothic_24 = fonts_get_system_font(FONT_KEY_GOTHIC_24);
  s_res_bitham_42_light = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  // lblImporte_layer
  lblImporte_layer = text_layer_create(GRect(8, 2, 63, 30));
  text_layer_set_text(lblImporte_layer, "Importe:");
  text_layer_set_font(lblImporte_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)lblImporte_layer);
  
  // dig1_layer
  dig1_layer = text_layer_create(GRect(72, 2, 10, 24));
  text_layer_set_text(dig1_layer, "0");
  text_layer_set_font(dig1_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)dig1_layer);
  
  // dig2_layer
  dig2_layer = text_layer_create(GRect(83, 2, 10, 24));
  text_layer_set_text(dig2_layer, "0");
  text_layer_set_font(dig2_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)dig2_layer);
  
  // lblMeses_layer
  lblMeses_layer = text_layer_create(GRect(8, 30, 54, 24));
  text_layer_set_text(lblMeses_layer, "Meses:");
  text_layer_set_font(lblMeses_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)lblMeses_layer);
  
  // lblIntereses_layer
  lblIntereses_layer = text_layer_create(GRect(7, 60, 73, 28));
  text_layer_set_text(lblIntereses_layer, "Intereses:");
  text_layer_set_font(lblIntereses_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)lblIntereses_layer);
  
  // dig3_layer
  dig3_layer = text_layer_create(GRect(94, 2, 10, 24));
  text_layer_set_text(dig3_layer, "0");
  text_layer_set_font(dig3_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)dig3_layer);
  
  // dig4_layer
  dig4_layer = text_layer_create(GRect(105, 2, 10, 24));
  text_layer_set_text(dig4_layer, "0");
  text_layer_set_font(dig4_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)dig4_layer);
  
  // digmeses_layer
  digmeses_layer = text_layer_create(GRect(59, 30, 30, 24));
  text_layer_set_text(digmeses_layer, "12");
  text_layer_set_font(digmeses_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)digmeses_layer);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  action_bar_layer_set_click_config_provider(s_actionbarlayer_1, click_config_provider_fpp);

  
  // txtIntereses_layer
  txtIntereses_layer = text_layer_create(GRect(78, 60, 41, 24));
  text_layer_set_text(txtIntereses_layer, "128");
  text_layer_set_font(txtIntereses_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)txtIntereses_layer);
  
  // txtCuota_layer
  txtCuota_layer = text_layer_create(GRect(0, 88, 123, 50));
  text_layer_set_text(txtCuota_layer, "111");
  text_layer_set_text_alignment(txtCuota_layer, GTextAlignmentCenter);
  text_layer_set_font(txtCuota_layer, s_res_bitham_42_light);
  layer_add_child(window_get_root_layer(s_window), (Layer *)txtCuota_layer);
  
  // txtInfo_layer
  txtInfo_layer = text_layer_create(GRect(0, 137, 121, 16));
  text_layer_set_background_color(txtInfo_layer, GColorBlack);
  text_layer_set_text_color(txtInfo_layer, GColorWhite);
  text_layer_set_text(txtInfo_layer, "Con intereses");
  text_layer_set_text_alignment(txtInfo_layer, GTextAlignmentCenter);
  text_layer_set_font(txtInfo_layer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)txtInfo_layer);
  
  
  
    //Asignación de iconos a la barra de opciones
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, arriba_bitmap );
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, abajo_bitmap);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, play_bitmap);
  
  marcador = layer_create(layer_get_bounds(window_get_root_layer(s_window)));
  layer_set_update_proc(marcador, marcador_update_callback); 
  layer_add_child(window_get_root_layer(s_window), marcador); 
  
  pinta_datos_fpp();
  
}

static void destroy_ui(void) {
  salva_datos_fpp();
  gbitmap_destroy(arriba_bitmap);
  gbitmap_destroy(abajo_bitmap);
  gbitmap_destroy(pulsar_bitmap);
  gbitmap_destroy(play_bitmap);
  gbitmap_destroy(buscar_bitmap);
  window_destroy(s_window);
  text_layer_destroy(lblImporte_layer);
  text_layer_destroy(dig1_layer);
  text_layer_destroy(dig2_layer);
  text_layer_destroy(lblMeses_layer);
  text_layer_destroy(lblIntereses_layer);
  text_layer_destroy(dig3_layer);
  text_layer_destroy(dig4_layer);
  text_layer_destroy(digmeses_layer);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(txtIntereses_layer);
  text_layer_destroy(txtCuota_layer);
  text_layer_destroy(txtInfo_layer);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void carga_fpp(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void descarga_fpp(void) {
  window_stack_remove(s_window, true);
}

