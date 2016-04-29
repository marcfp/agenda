//https://www.enlightenment.org/program_guide/event_effect_pg
#ifdef HAVE_ELEMENTARY_X
# include <Ecore_X.h>
#endif

#include <Elementary.h>
#include <Evas.h>

#include <Ecore.h>
#include <Ecore_Evas.h>
#include <unistd.h>

#define DEBUG 1
#define WIDTH 640
#define HEIGHT 480


/* here just to keep our example's window size and background image's
 * size in synchrony */

static void
_yes_quit_cb(void *data, Evas_Object *obj, void *event)
{
   elm_exit();
}
static void
_no_quit_cb(void *data, Evas_Object *obj, void *event)
{
   evas_object_del(data);
}
static void
_torna(void *data, Evas_Object *obj, void *event_info)
{
	if(DEBUG)printf("\nTorna a la finestra principal\n");
	evas_object_del(data);
}

static void
_mostra_principal(void *data, Evas_Object *obj, void *event_info)
{
	if(DEBUG)printf("\nMostra la finestra principal\n");
	evas_object_show(data);
}

static void
_main_win_del_cb(void *data, Evas_Object *obj, void *event)
{
   Evas_Object *msg, *box, *box2, *btn, *lbl, *sep;


   if(DEBUG)printf("\nfunció main_win_del_cb\n");

   msg = elm_notify_add(obj);
   elm_notify_align_set(msg, 0.5, 0.5);
   elm_notify_allow_events_set(msg, EINA_FALSE);
   evas_object_show(msg);
   box = elm_box_add(obj);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(msg, box);
   evas_object_show(box);
   lbl = elm_label_add(obj);
   elm_object_text_set(lbl, "Estàs segur de sortir?");
   evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, lbl);
   evas_object_show(lbl);
   sep = elm_separator_add(obj);
   elm_separator_horizontal_set(sep, EINA_TRUE);
   elm_box_pack_end(box, sep);
   evas_object_show(sep);
   box2 = elm_box_add(obj);
   elm_box_horizontal_set(box2, EINA_TRUE);
   evas_object_size_hint_weight_set(box2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, box2);
   evas_object_show(box2);
   btn = elm_button_add(obj);
   elm_object_text_set(btn, "Yes");
   elm_box_pack_end(box2, btn);
   evas_object_show(btn);
   evas_object_smart_callback_add(btn, "clicked", _yes_quit_cb, NULL);
   btn = elm_button_add(obj);
   elm_object_text_set(btn, "No");
   elm_box_pack_end(box2, btn);
   evas_object_show(btn);
   evas_object_smart_callback_add(btn, "clicked", _no_quit_cb, msg);
}
static Eina_Bool
_stdin_cb_eina(void *data EINA_UNUSED, Ecore_Fd_Handler *handler EINA_UNUSED)
{
   Eina_List *l;
   Ecore_Evas *ee;
   char c;
   int ret = scanf("%c", &c);
   if (ret < 1) return ECORE_CALLBACK_RENEW;
   if (c == 'h')
     EINA_LIST_FOREACH(ecore_evas_ecore_evas_list_get(), l, ee)
       ecore_evas_hide(ee);
   else if (c == 's')
     EINA_LIST_FOREACH(ecore_evas_ecore_evas_list_get(), l, ee)
       ecore_evas_show(ee);
   return ECORE_CALLBACK_RENEW;
}
static void
_on_delete_eina(Ecore_Evas *ee)
{
   free(ecore_evas_data_get(ee, "key"));
   ecore_main_loop_quit();
}


static void
changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Evas_Object *en;
   en = (Evas_Object*)data;

   elm_entry_editable_set(en, elm_check_state_get(obj));
}

static void
en_changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Evas_Object *sp, *en;
   sp = (Evas_Object *)data;
   en = (Evas_Object *)obj;
   elm_spinner_min_max_set(sp, 0, strlen(elm_object_text_get(en)));
}

static void
sp_changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Evas_Object *en, *sp;
   en = (Evas_Object *)data;
   sp = (Evas_Object *)obj;
   elm_entry_cursor_pos_set(en, elm_spinner_value_get(sp));
   elm_object_focus_set(en, EINA_TRUE);
}


static void
_block_clicked(void *data, Evas_Object *obj,
               void *event_info)
{
   evas_object_hide(obj);
}
static void
_timeout(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_hide(obj);
}

static void
esborra_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
Evas_Object *win, *popup, *content;
Evas_Object *bt_torna;
int ALT=200;
int AMPLA=200;
   win = elm_win_util_standard_add("popup", "Popup");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   content = elm_label_add(win);

   elm_object_text_set(content, "<align=center>Escull el registre a borrar</align>");
   popup = elm_popup_add(win);
   elm_popup_timeout_set(popup, 3.0);
   evas_object_smart_callback_add(popup, "timeout", _timeout, NULL);
 
   elm_object_content_set(popup, content);
   
   elm_object_part_text_set(popup, "title,text", "Registre a borrar");
   evas_object_show(popup);

   bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");
evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);

evas_object_resize(bt_torna, 100, 60);
//evas_object_move(bt_torna, (ALT/2)-50, (7*AMPLA/8)-40);
// elm_grid_pack(gd, bt_torna, 70,80,15,15);
evas_object_show(bt_torna);

   evas_object_smart_callback_add(popup, "block,clicked", _block_clicked, NULL);

   evas_object_smart_callback_add(win, "block,clicked", _block_clicked, NULL);
   evas_object_resize(win, ALT, AMPLA);
   evas_object_show(win);
}


static void
_esborra(void *data, Evas_Object *obj, void *event_info)
{
	if(DEBUG) printf("\nEsborrar \n");

Evas_Object *win, *gd, *bg, *en, *tg, *lb, *sp;
   Evas_Object *bt, *en2;
   Evas_Object *bt_torna;

   evas_object_hide(data);



   win = elm_win_util_standard_add("entry7", "Cerca");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);//per tancar

   elm_win_title_set(win, "Cercar dades ");
   //elm_win_autodel_set(finestra, EINA_TRUE);
   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_resize(win, WIDTH, HEIGHT);


   gd = elm_grid_add(win);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);

   /* upper entry group */
   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 255, 0, 0);
   elm_grid_pack(gd, bg, 10, 10, 60, 60);
   evas_object_show(bg);

   en = elm_entry_add(win);
   elm_entry_scrollable_set(en, EINA_TRUE);
   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en, "Escriu el camp que vulguis borrar");
   evas_object_show(en);
   elm_grid_pack(gd, en, 10, 10, 60, 60);
  


   bt = elm_button_add(win);
   elm_object_text_set(bt, "Esborra");
   elm_grid_pack(gd, bt, 10,70,15,5);
   evas_object_smart_callback_add(bt, "clicked", esborra_bt_clicked, en);
   evas_object_show(bt);

	evas_object_hide(data);



bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");
evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);
evas_object_smart_callback_add(bt_torna, "clicked",_mostra_principal, data);
evas_object_smart_callback_add(win, "delete,request", _main_win_del_cb,NULL);
evas_object_resize(bt_torna, 100, 60);
//evas_object_move(bt_torna, (WIDTH/2)-50, (7*HEIGHT/8)-40);
 elm_grid_pack(gd, bt_torna, 70,80,15,15);
evas_object_show(bt_torna);


		evas_object_show(win);

}

static void
_on_click(void *data, Evas_Object *obj, void *event_info)
{
 	if(DEBUG) printf("\nSurt\n");
//  elm_shutdown();//borra la finestra

  evas_object_del(data);
}

static void
cerca_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
Evas_Object *win, *popup, *content;
Evas_Object *bt_torna;
int ALT=200;
int AMPLA=200;
   win = elm_win_util_standard_add("popup", "Popup");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   content = elm_label_add(win);

   elm_object_text_set(content, "<align=center>escull camp a cercar ?</align>");
   popup = elm_popup_add(win);
   elm_popup_timeout_set(popup, 3.0);
   evas_object_smart_callback_add(popup, "timeout", _timeout, NULL);
   
   elm_object_content_set(popup, content);
   
   elm_object_part_text_set(popup, "title,text", "Has cercat ?");
   evas_object_show(popup);

   bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");
evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);

evas_object_resize(bt_torna, 100, 60);
evas_object_move(bt_torna, (ALT/2)-50, (7*AMPLA/8)-40);
// elm_grid_pack(gd, bt_torna, 70,80,15,15);
evas_object_show(bt_torna);

   evas_object_smart_callback_add(popup, "block,clicked", _block_clicked, NULL);

   evas_object_smart_callback_add(win, "block,clicked", _block_clicked, NULL);
   evas_object_resize(win, ALT, AMPLA);
   evas_object_show(win);
}


static void
_cerca(void *data, Evas_Object *obj, void *event_info)
{
	if(DEBUG) printf("\nCerca\n");
	

Evas_Object *win, *gd, *bg, *en, *tg, *lb, *sp;
   Evas_Object *bt, *en2;
   Evas_Object *bt_torna;

   evas_object_hide(data);



   win = elm_win_util_standard_add("entry7", "Cerca");
//   elm_win_autodel_set(win, EINA_TRUE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);//per tancar

   elm_win_title_set(win, "Cercar dades de l'agenda");
   //elm_win_autodel_set(finestra, EINA_TRUE);
   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_resize(win, WIDTH, HEIGHT);


   gd = elm_grid_add(win);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);

   /* upper entry group */
   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 255, 0, 0);
   elm_grid_pack(gd, bg, 10, 10, 60, 60);
   evas_object_show(bg);

   en = elm_entry_add(win);
   elm_entry_scrollable_set(en, EINA_TRUE);
   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en, "Escriu el camp que vulguis cercar");
   evas_object_show(en);
   elm_grid_pack(gd, en, 10, 10, 60, 60);


   bt = elm_button_add(win);
   elm_object_text_set(bt, "Cerca");
   elm_grid_pack(gd, bt, 10,70,15,5);
   evas_object_smart_callback_add(bt, "clicked", cerca_bt_clicked, en);
   evas_object_show(bt);

evas_object_hide(data);

bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");
evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);
evas_object_smart_callback_add(bt_torna, "clicked",_mostra_principal, data);
evas_object_smart_callback_add(win, "delete,request", _main_win_del_cb,NULL);
evas_object_resize(bt_torna, 100, 60);
//evas_object_move(bt_torna, (WIDTH/2)-50, (7*HEIGHT/8)-40);
 elm_grid_pack(gd, bt_torna, 70,80,15,15);
evas_object_show(bt_torna);


		evas_object_show(win);

}

static void
insert_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
Evas_Object *win, *popup, *content;
Evas_Object *bt_torna;
int ALT=200;
int AMPLA=200;
   win = elm_win_util_standard_add("popup", "Popup");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   content = elm_label_add(win);

   elm_object_text_set(content, "<align=center>Si, està afegit</align>");
   popup = elm_popup_add(win);
   elm_popup_timeout_set(popup, 3.0);
   evas_object_smart_callback_add(popup, "timeout", _timeout, NULL);
   //Setting popup content
   elm_object_content_set(popup, content);
   //Seting popup title-text
   elm_object_part_text_set(popup, "title,text", "Has afegit ?");
   evas_object_show(popup);

   bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");
evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);

evas_object_resize(bt_torna, 100, 60);
evas_object_move(bt_torna, (ALT/2)-50, (7*AMPLA/8)-40);
// elm_grid_pack(gd, bt_torna, 70,80,15,15);
evas_object_show(bt_torna);

   evas_object_smart_callback_add(popup, "block,clicked", _block_clicked, NULL);

   evas_object_smart_callback_add(win, "block,clicked", _block_clicked, NULL);
   evas_object_resize(win, ALT, AMPLA);
   evas_object_show(win);
}

static void
_afegir(void *data, Evas_Object *obj, void *event_info)
{
	
	if(DEBUG) printf("\nAfegeix\n");
   Evas_Object *win, *gd, *bg, *en, *tg, *lb, *sp;
   Evas_Object *bt, *en2;
   Evas_Object *bt_torna;

   evas_object_hide(data);



   win = elm_win_util_standard_add("entry7", "Afegir");
//   elm_win_autodel_set(win, EINA_TRUE);

   gd = elm_grid_add(win);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);

   /* upper entry group */
   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 255, 0, 0);
   elm_grid_pack(gd, bg, 10, 10, 60, 60);
   evas_object_show(bg);

   en = elm_entry_add(win);
   elm_entry_scrollable_set(en, EINA_TRUE);
   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en, "Escriu el que vulguis insertar");
   evas_object_show(en);
   elm_grid_pack(gd, en, 10, 10, 60, 60);

   tg = elm_check_add(win);
   evas_object_size_hint_weight_set(tg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(tg, "Editable");
   elm_check_state_set(tg, EINA_TRUE);
   evas_object_smart_callback_add(tg, "changed", changed_cb, en);
   elm_grid_pack(gd, tg, 70, 10, 22, 10);
   evas_object_show(tg);


   sp = elm_spinner_add(win);//contador de lletres
   elm_spinner_label_format_set(sp, "%1.0f");
   elm_spinner_step_set(sp, 1);
   elm_spinner_wrap_set(sp, ELM_WRAP_CHAR);
   elm_spinner_min_max_set(sp, 0, strlen(elm_object_text_get(en)));
   evas_object_size_hint_align_set(sp, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(sp, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_grid_pack(gd, sp, 70, 30, 25, 10);
   evas_object_smart_callback_add(en, "changed", en_changed_cb, sp);
   evas_object_smart_callback_add(sp, "delay,changed", sp_changed_cb, en);
   evas_object_show(sp);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Insertar ");
   elm_grid_pack(gd, bt, 10,70,15,5);
//   elm_grid_pack(gd, bt, 55, 80, 40, 10);
   evas_object_smart_callback_add(bt, "clicked", insert_bt_clicked, en);
   evas_object_show(bt);

   bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");

evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);
evas_object_smart_callback_add(bt_torna, "clicked",_mostra_principal, data);
evas_object_smart_callback_add(win, "delete,request", _main_win_del_cb,NULL);
evas_object_resize(bt_torna, 100, 60);
//evas_object_move(bt_torna, (WIDTH/2)-50, (7*HEIGHT/8)-40);
 elm_grid_pack(gd, bt_torna, 70,80,15,15);
evas_object_show(bt_torna);

   evas_object_resize(win, WIDTH, HEIGHT);
   evas_object_show(win);

}
static void
_hora(void *data, Evas_Object *obj, void *event_info)
{
	if(DEBUG)printf("\nhora?\n");
}

static void
_slide_end_cb(void *data, Evas_Object *obj, void *event_info)
{
   printf("\nSlide has reach the end.\n");
}

/*
static Eina_Bool
_do_rotate(void *data, double pos)
{
   // Get the animation target
   Evas_Object *obj = data;
   // Declaration of an `Evas_Map`
   Evas_Map *m;
   // Variables to store the target size and position
   int x, y, w, h;
 
   // Getting the size and position of the target
   evas_object_geometry_get(obj, &x, &y, &w, &h);
   // Creation of an `Evas_Map` of 4 points
   m = evas_map_new(4);
   // Populate source and destination map points to match exactly object.
   evas_map_util_points_populate_from_object(m, obj);
   // Create a rotation of 360° with x+(w/2) "x" center and y +(h/2) "y" center.
   evas_map_util_rotate(m, 360.0 * pos, x + (w / 2), y + (h / 2));
   // Setting the object to "animate" in the `Evas_Map`
   evas_object_map_set(obj, m);
   // Starting the Animation
   evas_object_map_enable_set(obj, EINA_TRUE);
   // Free used memory
 //  evas_map_free(m);
 
   return EINA_TRUE;
}
*/
static Eina_Bool
_do_3d(void *data, double pos)
{
   Evas_Object *obj = data;
   Evas_Map *m;
   int x, y, w, h;
 
   evas_object_geometry_get(obj, &x, &y, &w, &h);
   m = evas_map_new(4);
   evas_map_util_points_populate_from_object(m, obj);
   evas_map_util_3d_rotate(m, pos * 360, pos * 360, pos * 360, x + (w / 3), y + 60, 0);
   evas_object_map_set(obj, m);
   evas_object_map_enable_set(obj, EINA_TRUE);
   evas_map_free(m);
 
   return EINA_TRUE;
}

static void
_btn_rotate_cb(void *data, Evas_Object *btn, void *ev)
{
   Evas_Object *target = data;
   ecore_animator_timeline_add(1, _do_3d, target);
}


static void
_signal_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
   printf("Info received from layout : %s %s\n", emission, source);
}

static Eina_Bool
_do_animation(void *data, double pos)
{
   //evas_object_move(data, pos, pos);
   // Do some more animating...
}

static Eina_Bool
_my_animation(void *data, double pos)
{
   Evas_Object *obj = data;                       // Get the target object
   int x, y, w, h;                                // Target object geometry
   evas_object_geometry_get(obj, &x, &y, &w, &h); // Get current object position and size attributes
   evas_object_move(obj, 500 * pos, y);           // Linear translation of the Evas object
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
	Evas_Object *finestra, *text, *text1, *layout, *tab, *rec, *gd, *bg, *Caixa, *bt_quit, *bt_drop, *bt_search, *bt_add, *datetime, *ck;

Evas_Object *target;//animation object
	finestra =elm_win_util_standard_add("principal", "Agenda");
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
elm_win_autodel_set(finestra, EINA_TRUE);


//	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);//per tancar

	elm_win_title_set(finestra, "Pantalla principal de l'agenda");
//	elm_win_autodel_set(finestra, EINA_TRUE);
	elm_win_focus_highlight_enabled_set(finestra, EINA_TRUE);
	evas_object_resize(finestra, WIDTH, HEIGHT);
	evas_object_size_hint_align_set(finestra, EVAS_HINT_FILL, EVAS_HINT_FILL);
	//layout = elm_layout_add(finestra);
	//elm_layout_file_set(layout, "edje_animation.edj", "my_layout");
	
	


//mínim de 320x240 
tab = elm_table_add(finestra);
   evas_object_size_hint_weight_set(tab, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(finestra, tab);
   evas_object_show(tab);
 
   rec = evas_object_rectangle_add(evas_object_evas_get(finestra));
   evas_object_size_hint_weight_set(rec, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(rec, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_min_set(rec, 320 * elm_config_scale_get(), 240 * elm_config_scale_get());
   elm_table_pack(tab, rec, 0, 0, 1, 1);

gd = elm_grid_add(finestra);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(finestra, gd);
   evas_object_show(gd);


	bg = elm_bg_add(finestra);


//https://www.enlightenment.org/start?do=search&id=animation  <- d'aquí penja tot
//https://www.enlightenment.org/tutorial/effects/edje_animation/start_up
//https://www.enlightenment.org/tutorial/effects/edje_animation/on_click
//https://www.enlightenment.org/program_guide/event_effect/evas_map_animations
//https://www.enlightenment.org/tutorial/effects_tutorial


/* pròximes entrades */
Evas_Object *table, *rect;
table = evas_object_table_add(finestra);
evas_object_table_homogeneous_set(table, EVAS_OBJECT_TABLE_HOMOGENEOUS_NONE);
evas_object_table_padding_set(table, 20, 20);
//evas_object_resize(table, 200, 600);
elm_grid_pack(gd,table,48,42,50,50);
evas_object_show(table);
rect = evas_object_rectangle_add(finestra);
evas_object_color_set(rect, 10, 10,10,10); //255, 255, 255);
evas_object_size_hint_min_set(rect, 300, 300);
elm_grid_pack(gd, rect, 48, 42, 50, 50);
//evas_object_resize(rect, 110,110);
evas_object_show(rect);
//how a container object should resize a given child within its area
evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//how to align an object
evas_object_size_hint_align_set(table, EVAS_HINT_FILL, 0.5);
evas_object_table_pack(table, rect, 1, 1, 1, 1);
//how a container object should resize a given child within its area
evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//how to align an object
evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, 0.5);
elm_grid_pack(gd, table,48,42,50,50);
/*fi pròximems entrades*/

//RELLOTGE
ck = elm_clock_add(finestra);
elm_clock_show_seconds_set(ck, EINA_TRUE);
elm_clock_show_am_pm_set(ck, EINA_FALSE);
elm_box_pack_end(finestra, ck);
evas_object_resize(ck, 110,110);
//evas_object_move(ck, 3*(WIDTH/4)-60, 5*(HEIGHT/8));
 //how a container object should resize a given child within its area
    evas_object_size_hint_weight_set(ck, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    //how to align an object
    evas_object_size_hint_align_set(ck, EVAS_HINT_FILL, 0.5);
elm_grid_pack(gd, ck, 60, 30, 25, 8);
evas_object_show(ck);

//AFEGIR
bt_add = elm_button_add(finestra);
elm_object_text_set(bt_add, "Afegir ");
evas_object_smart_callback_add(bt_add, "clicked", _afegir, finestra);
evas_object_resize(bt_add, 120, 80);
//evas_object_move(bt_add,(WIDTH/2)-60, (HEIGHT/8)-40);
 //how a container object should resize a given child within its area
    evas_object_size_hint_weight_set(bt_add, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    //how to align an object
    evas_object_size_hint_align_set(bt_add, EVAS_HINT_FILL, 0.5);
 
elm_grid_pack(gd, bt_add, 20, 10, 25, 10);
evas_object_show(bt_add);

//CONSULTAR
bt_search = elm_button_add(finestra);
elm_object_text_set(bt_search, "Cerca ");
evas_object_smart_callback_add(bt_search, "clicked", _cerca, finestra);
evas_object_resize(bt_search, 120,80);
//evas_object_move(bt_search, (WIDTH/2)-60, (3*HEIGHT/8)-40);
 //how a container object should resize a given child within its area
    evas_object_size_hint_weight_set(bt_search, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    //how to align an object
    evas_object_size_hint_align_set(bt_search, EVAS_HINT_FILL, 3.5);

elm_grid_pack(gd, bt_search, 20, 30, 25, 10);

evas_object_show(bt_search);

//BORRAR
bt_drop = elm_button_add(finestra);
elm_object_text_set(bt_drop, "Borra ");
evas_object_smart_callback_add(bt_drop, "clicked", _esborra, finestra);
evas_object_resize(bt_drop, 120,80);
//evas_object_move(bt_drop, (WIDTH/2)-60, (5*HEIGHT/8)-40);
 //how a container object should resize a given child within its area
    evas_object_size_hint_weight_set(bt_drop, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    //how to align an object
    evas_object_size_hint_align_set(bt_drop, EVAS_HINT_FILL, 0.5);

elm_grid_pack(gd, bt_drop, 20, 50, 25, 10);

evas_object_show(bt_drop);

//SORTIR
bt_quit = elm_button_add(finestra);
elm_object_text_set(bt_quit, "Surt");
evas_object_smart_callback_add(bt_quit, "clicked",_main_win_del_cb, finestra);
evas_object_resize(bt_quit, 100, 60);
//evas_object_move(bt_quit, (WIDTH/2)-50, (7*HEIGHT/8)-40);

 //how a container object should resize a given child within its area
 //   evas_object_size_hint_weight_set(bt_quit, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    //how to align an object
//    evas_object_size_hint_align_set(bt_quit, EVAS_HINT_FILL, 0.5);

elm_grid_pack(gd, bt_quit, 17, 70, 30, 20);
evas_object_show(bt_quit);

//elm_layout_file_set(layout, "edje_example.edj", "my_layout");
//elm_layout_signal_callback_add(layout, "*", "*", _signal_cb, NULL);

/*   evas_object_image_file_set(img, "./logo.png", NULL);
   evas_object_move(img, 0, 0);
   evas_object_resize(img, 200, 200);
   evas_object_show(img);
 */



   Evas *e = evas_object_evas_get(finestra);
 
   // Create an image object
   Evas_Object *img = evas_object_image_filled_add(e);



   // Create another image object
   Evas_Object *img2 = evas_object_image_filled_add(e);
   evas_object_image_file_set(img2, "./logo_blanc1.png", NULL);
 
   // Disable smooth scaling
   evas_object_image_smooth_scale_set(img2, EINA_TRUE);
   elm_grid_pack(gd,img2, 56,1, 30, 30);
   evas_object_move(img2, 400, 30);
   evas_object_resize(img2, 2, 2);
   evas_object_show(img2);




//ANIMACIÓ
Elm_Transit *transit = elm_transit_add();
text= elm_label_add(finestra);//evas_object_text_add(finestra);//elm_label_add(finestra);
elm_object_text_set(text, "Agenda programada amb c");
elm_label_slide_duration_set(text, 500);
elm_label_slide_mode_set(text, ELM_LABEL_SLIDE_MODE_ALWAYS);
elm_grid_pack(gd,text, 40,4, 60, 60);
elm_object_style_set(text, "marker");
evas_object_color_set(text, 0, 0, 255, 255);
elm_transit_object_add (transit, text);
elm_transit_effect_rotation_add(transit, 0, 360);
elm_transit_duration_set(transit, 5);
/*
elm_transit_effect_add(Elm_Transit *                    transit,
                       Elm_Transit_Effect_Transition_Cb transition_cb,
                       Elm_Transit_Effect *             effect,
                       Elm_Transit_Effect_End_Cb        end_cb
                      )
*/
elm_transit_auto_reverse_set(transit, EINA_TRUE);
elm_transit_effect_color_add(transit,0,0,0,0,225,225,0,225);
elm_transit_go(transit);
//elm_transit_effect_del(transit); 
//https://www.enlightenment.org/program_guide/event_effect/elementary_transitions

//https://www.enlightenment.org/program_guide/event_effect/ecore_animators#Create_an_Ecore_Animation 
//https://www.enlightenment.org/program_guide/event_effect/elementary_transitions
//https://www.enlightenment.org/program_guide/event_effect/elementary_transitions
//ecore_animator_timeline_add(12, _do_animation, text);
//elm_transit_paused_set(transit,                        Eina_Bool     paused  )
evas_object_show(text);

/*
text1= elm_label_add(finestra);//evas_object_text_add(finestra);//elm_label_add(finestra);
elm_object_text_set(text1, "Agenda programada amb c");
elm_grid_pack(gd,text1, 40,4, 60, 60);
evas_object_color_set(text1, 255,255,0, 255);
evas_object_show(text1);
*/

/*
Moure rellotge
*/	
   //d.smt = evas_smart_example_add(d.evas);
  // evas_object_move(d.smt, WIDTH / 4, HEIGHT / 4);
//evas_object_move(ck, 10,4);
//evas_object_resize(ck,240, 240);
//evas_object_show(ck);
  // evas_object_resize(d.smt, WIDTH / 2, HEIGHT / 2);
  // evas_object_show(d.smt);

/*
Fi Moure rellotge
*/
	evas_object_show(bg);
	evas_object_show(finestra);

 

   elm_run();
   return 0;
}
ELM_MAIN()
