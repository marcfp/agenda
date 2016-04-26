
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

EAPI_MAIN int
elm_main(int argc, char **argv)
{
	Evas_Object *finestra, *gd, *bg, *Caixa, *bt_quit, *bt_drop, *bt_search, *bt_add, *datetime, *ck;

	finestra =elm_win_util_standard_add("principal", "Agenda");

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);//per tancar

	elm_win_title_set(finestra, "Pantalla principal de l'agenda");
	elm_win_autodel_set(finestra, EINA_TRUE);
	elm_win_focus_highlight_enabled_set(finestra, EINA_TRUE);
	evas_object_resize(finestra, WIDTH, HEIGHT);
	
	evas_object_size_hint_align_set(finestra, EVAS_HINT_FILL, EVAS_HINT_FILL);

gd = elm_grid_add(finestra);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(finestra, gd);
   evas_object_show(gd);


	bg = elm_bg_add(finestra);


//RELLOTGE
ck = elm_clock_add(finestra);
elm_clock_show_seconds_set(ck, EINA_TRUE);
elm_clock_show_am_pm_set(ck, EINA_FALSE);
elm_box_pack_end(finestra, ck);
evas_object_resize(ck, 120,120);
//evas_object_move(ck, 3*(WIDTH/4)-60, 5*(HEIGHT/8));
 //how a container object should resize a given child within its area
    evas_object_size_hint_weight_set(ck, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    //how to align an object
    evas_object_size_hint_align_set(ck, EVAS_HINT_FILL, 0.5);
elm_grid_pack(gd, ck, 60, 60, 25, 10);
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
	evas_object_show(bg);
	evas_object_show(finestra);

   elm_run();
   return 0;
}
ELM_MAIN()
