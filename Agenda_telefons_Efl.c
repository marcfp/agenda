#ifdef HAVE_ELEMENTARY_X
# include <Ecore_X.h>
#endif

#include <stdio.h>
#include <Eina.h>

#include <Elementary.h>
#include <Evas.h>

#include <Ecore.h>
#include <Ecore_Evas.h>
#include <unistd.h>

#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <math.h>

#include <libintl.h> //translatable?

#include <libpq-fe.h> //conexió base de dades

#include "funcions/funcions.h" //funcions base

#define WIDTH 1024
#define HEIGHT 760

#define __UNUSED__ __attribute__((__unused__))


int pagina=1;

static void _print_items(void *data, Evas_Object *obj, void *event_info);
static void carrega_registres(void *data, Evas_Object *obj, void *event_info);
static void _free(void *data, Evas_Object *obj, void *event_info);
static void _add_item(void *data, Evas_Object *obj, void *event_info);


/* here just to keep our example's window size and background image's
 * size in synchrony */
int
conecta()//void *data , Evas_Object *obj , void *event_info )
{
  FILE *debug_insert;
          PGconn *conexion;
          PGresult   *resultado, * resultat2;

          int columnas=0;
          int filas=0;
          int i =0;
          int j=0;
          int valor=0;
          int id;
          char buffer[1024];
	  		if(DEBUG==1)printf("\n\t\t\tINSERCIÓ DE CITES EN L'AGENDA");
				//lectura_dades_notes(data,lloc,motiu,caducitat);
 conexion = PQsetdbLogin("localhost", // nombre del servidor, NULL agrra por defaul a localhost
                                NULL, // Puerto, NULL toma el que esta por default
                                NULL,//Opciones adicionales
                                NULL, // Fichero o terminal de la salida
                                "agenda",//Nombre de la base de datos
                                "marc", // Nombre de usuario
                                "Datsxku1!"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
	}
	else{
		printf(" Connexió feta amb éxit!!! \n");
	}
        

 	debug_insert = fopen("instelefonsGRAFIC.log.txt","w");
	PQtrace(conexion, debug_insert); /* Aqui le asignamos al archivo la salida  */
	return conexion;	
}
void desconecta(PGconn *conexion)
{
 if(DEBUG==1)printf("Desconecta ");
 PQfinish(conexion);
}
static void
conecta_en_en(void *data, Evas_Object *obj, void *event)
{
	if (DEBUG==1) printf("\nconecta_en_en\n");
}
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
	if(DEBUG==1)printf("\nTorna a la finestra principal\n");
	evas_object_del(data);
}

static void
_mostra_principal(void *data, Evas_Object *obj, void *event_info)
{
	if(DEBUG==1)printf("\nMostra la finestra principal\n");
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
neteja_text(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	Evas_Object *en;
	if(DEBUG)printf("\n He de netejar text ?\n");
	en = (Evas_Object *) obj;
	elm_object_text_set(en, "");
}
static void
changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Evas_Object *en;
if(DEBUG==1) printf("\nchanged_cb\n");
   en = (Evas_Object*)data;

   elm_entry_editable_set(en, elm_check_state_get(obj));
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
esborra_hoversel_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
 	printf("\nEsborra hoversel\n");
}
static void
esborra_en_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
 	printf("\nEsborra en\n");
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
evas_object_show(bt_torna);

   evas_object_smart_callback_add(popup, "block,clicked", _block_clicked, NULL);

   evas_object_smart_callback_add(win, "block,clicked", _block_clicked, NULL);
   evas_object_resize(win, ALT, AMPLA);
   evas_object_show(win);
}


static void
_esborra(void *data, Evas_Object *obj, void *event_info)
{

if(DEBUG) printf("\nEsborra\n");
	
/* 
	EL TINC MAL PLANTEJAT ? NO POT SER QUE HAGI DE FER UN TEXT_BOX, ENTRAR EL CAMP I ESCOLLIR EL QUE ÉS D'UN LIST BOX ? I A PARTIR D'AQUÍ LLENSAR LA CERCA ? AMB AIXÒ _transitions[9] NO ES POT FER ?

https://blog.jooq.org/2014/12/30/the-awesome-postgresql-9-4-sql2003-filter-clause-for-aggregate-functions/
http://www.techonthenet.com/postgresql/like.php
https://www.postgresql.org/docs/9.4/static/sql-expressions.html
http://www.cybertec.at/2015/02/postgresql-9-4-aggregation-filters-they-do-pay-off/


*/
Evas_Object *win, *gd, *bg, *en, *tg, *lb, *sp, *check_nom, *en_nomt, *en_nom, *check_cog1, *en_cog1t, *en_cog1, *check_cog2, *en_cog2t, *en_cog2, *check_mail, *en_mailt, *en_mail,  *check_casat, *en_tlf_casat, *en_tlf_casa, *check_mobil1t, *en_tlf_mobil1t, *en_tlf_mobil1, *check_tlf_altres, *en_tlf_altrest, *en_tlf_altres,  *check_altres, *en_altrest, *en_altres;
   Evas_Object *bt, *en2;
   Evas_Object *bt_torna;

Eina_Bool value;

   evas_object_hide(data);




   win = elm_win_util_standard_add("entry7", "Esborra");
//   elm_win_autodel_set(win, EINA_TRUE);//EINA_TRUE <- NO TANCA PRINCIPAL EINA_FALSE
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);//per tancar
   elm_win_title_set(win, "Esborra dades de l'agenda");
//   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_resize(win, WIDTH, HEIGHT);


   gd = elm_grid_add(win);

   evas_object_size_hint_min_set(gd, 700 * elm_config_scale_get(), 550 * elm_config_scale_get()); //mida més petita
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);

   /* upper entry group */
 bg = elm_bg_add(win); //fons blau
   elm_bg_color_set(bg, 0, 0, 255);

   elm_grid_pack(gd, bg, 5, 5, 45, 45);
   evas_object_show(bg);

   en_nomt = elm_entry_add(win); //nom
   elm_entry_scrollable_set(en_nomt, EINA_FALSE);
   evas_object_size_hint_weight_set(en_nomt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_nomt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_nomt, "Escriu el nom"); 
   evas_object_show(en_nomt); 
   elm_grid_pack(gd, en_nomt, 5, 8, 8, 5);
   en_nom = elm_entry_add(win);
   elm_entry_scrollable_set(en_nom, EINA_TRUE);
   evas_object_size_hint_weight_set(en_nom, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_nom, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_nom, "Escriu el nom aquí");
   evas_object_show(en_nom);
   elm_grid_pack(gd, en_nom, 15, 8, 25, 5);

   en_cog1t = elm_entry_add(win); //COG1	
   elm_entry_scrollable_set(en_cog1t, EINA_FALSE);
   evas_object_size_hint_weight_set(en_cog1t, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog1t, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog1t, "Escriu el primer cognom"); 
   evas_object_show(en_cog1t); 
   elm_grid_pack(gd, en_cog1t, 5, 13, 8, 5);
   en_cog1 = elm_entry_add(win);
   elm_entry_scrollable_set(en_cog1, EINA_TRUE);
   evas_object_size_hint_weight_set(en_cog1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog1, "Escriu el primer cognom aquí");
   evas_object_show(en_cog1);
   elm_grid_pack(gd, en_cog1, 15, 13, 25, 5);

   en_cog2t = elm_entry_add(win); //COG2	
   elm_entry_scrollable_set(en_cog2t, EINA_FALSE);
   evas_object_size_hint_weight_set(en_cog2t, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog2t, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog2t, "Escriu el segon cognom"); 
   evas_object_show(en_cog2t); 
   elm_grid_pack(gd, en_cog2t, 5, 18, 8, 5);
   en_cog2 = elm_entry_add(win);
   elm_entry_scrollable_set(en_cog2, EINA_TRUE);
   evas_object_size_hint_weight_set(en_cog2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog2, "Escriu el segon cognom aquí");
   evas_object_show(en_cog2);
   elm_grid_pack(gd, en_cog2, 15, 18, 25, 5);

   en_mailt = elm_entry_add(win); //correu	
   elm_entry_scrollable_set(en_mailt, EINA_FALSE);
   evas_object_size_hint_weight_set(en_mailt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_mailt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_mailt, "Entra el correu"); 
   evas_object_show(en_mailt); 
   elm_grid_pack(gd, en_mailt, 5, 23, 8, 5); 
  en_mail = elm_entry_add(win);
   elm_entry_scrollable_set(en_mail, EINA_TRUE);
   evas_object_size_hint_weight_set(en_mail, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_mail, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_mail, "Escriu el correu aquí");
   evas_object_show(en_mail);
   elm_grid_pack(gd, en_mail, 15, 23, 25, 5);

   en_tlf_casat = elm_entry_add(win); //TELEFON1	
   evas_object_size_hint_weight_set(en_tlf_casat, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_casat, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_casat, "Escriu el telefon1"); 
   evas_object_show(en_tlf_casat); 
   elm_grid_pack(gd, en_tlf_casat, 5, 28, 8, 5);
   en_tlf_casa = elm_entry_add(win);
   elm_entry_scrollable_set(en_tlf_casa, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_casa, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_casa, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_casa, "Escriu el primer telefon aquí");
   evas_object_show(en_tlf_casa);
   elm_grid_pack(gd, en_tlf_casa, 15, 28, 25, 5);

   en_tlf_mobil1t = elm_entry_add(win); //MOVIL	
   elm_entry_scrollable_set(en_tlf_mobil1t, EINA_FALSE);
   evas_object_size_hint_weight_set(en_tlf_mobil1t, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_mobil1t, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_mobil1t, "Escriu el telefon2"); 
   evas_object_show(en_tlf_mobil1t); 
   elm_grid_pack(gd, en_tlf_mobil1t, 5, 33, 8, 5);
   en_tlf_mobil1 = elm_entry_add(win);
   elm_entry_scrollable_set(en_tlf_mobil1, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_mobil1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_mobil1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_mobil1, "Escriu el segon telefon aquí");
   evas_object_show(en_tlf_mobil1);
   elm_grid_pack(gd, en_tlf_mobil1, 15, 33, 25, 5);

   en_tlf_altrest = elm_entry_add(win); //MOVIL2	
   elm_entry_scrollable_set(en_tlf_altrest, EINA_FALSE);
   evas_object_size_hint_weight_set(en_tlf_altrest, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_altrest, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_altrest, "Escriu el telefon3"); 
   evas_object_show(en_tlf_altrest); 
   elm_grid_pack(gd, en_tlf_altrest, 5, 38, 12, 5);
   en_tlf_altres = elm_entry_add(win);
   elm_entry_scrollable_set(en_tlf_altres, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_altres, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_altres, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_altres, "Escriu el tercer telefon aquí");
   evas_object_show(en_tlf_altres);
   elm_grid_pack(gd, en_tlf_altres, 15, 38, 25, 5);

   en_altrest = elm_entry_add(win); //ALTRES
   elm_entry_scrollable_set(en_altrest, EINA_FALSE);
   evas_object_size_hint_weight_set(en_altrest, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_altrest, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_altrest, "Escriu 'altres'"); 
   evas_object_show(en_altrest); 
   elm_grid_pack(gd, en_altrest, 5, 43, 12, 5);
   
en_altres = elm_entry_add(win);
   elm_entry_scrollable_set(en_altres, EINA_TRUE);
   evas_object_size_hint_weight_set(en_altres, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_altres, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_altres, "Entra el que vulguis cercar");
   evas_object_show(en_altres);
   elm_grid_pack(gd, en_altres, 15, 43, 25, 5);



Evas_Object *rect1, *hoversel, *btn = NULL;
   rect1 = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_color_set(rect1, 2, 0, 0, 2);
   evas_object_show(rect1);
   hoversel = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hoversel, win);
   elm_hoversel_horizontal_set(hoversel, EINA_FALSE);
   elm_object_text_set(hoversel, "Mostra Registre");
evas_object_data_set(hoversel, "lb1", en_nom); //carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb2", en_cog1);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb3", en_cog2);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb4", en_mail);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb5", en_tlf_casa);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb6", en_tlf_mobil1);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb7", en_tlf_altres);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb8", en_altres);//carreguem les dades al butó ...
   evas_object_smart_callback_add(hoversel, "clicked", carrega_registres,en );
// evas_object_smart_callback_add(hoversel, "clicked", cerca_bt_clicked, NULL);
// evas_object_smart_callback_add(bt, "clicked", cerca_bt_clicked, en);
/*
https://www.enlightenment.org/program_guide/event_effect/ecore_events
https://www.enlightenment.org/program_guide/main_loop/threads?s[]=ecore
https://docs.enlightenment.org/api/ecore/doc/html/
*/



  evas_object_smart_callback_add(hoversel,"" , NULL , NULL );//"clicked", registre, entra); //és la línea bona
   elm_grid_pack(gd, hoversel, 62, 2, 38, 2);

   evas_object_show(hoversel);




    if(DEBUG==1) printf("\nelm_check_state_get\n(check_nom)=%s\n",elm_check_state_get(check_nom));
   bt = elm_button_add(win);
   elm_object_text_set(bt, "Cerca");
   elm_grid_pack(gd, bt, 10,70,15,5);
   evas_object_data_set(bt, "lb1", en_nom); //carreguem les dades al butó ...
   evas_object_data_set(bt, "lb2", en_cog1);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb3", en_cog2);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb4", en_mail);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb5", en_tlf_casa);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb6", en_tlf_mobil1);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb7", en_tlf_altres);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb8", en_altres);//carreguem les dades al butó ...
  /* evas_object_data_set(bt, "lb9", check_nom);//carreguem les dades al butó ...
 evas_object_data_set(bt, "lb10", check_cog1);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb11", check_cog2);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb13", check_casat);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb14", check_mobil1t);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb15", check_tlf_altres);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb16", check_altres);//carreguem les dades al butó ...
   */
 evas_object_smart_callback_add(bt, "clicked", esborra_hoversel_bt_clicked, hoversel);
   evas_object_smart_callback_add(bt, "clicked", esborra_en_bt_clicked, en);
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
  evas_object_del(data);
}

static void

_transit_check(Elm_Transit *trans)

{

   elm_transit_effect_flip_add(trans, ELM_TRANSIT_EFFECT_FLIP_AXIS_X, EINA_TRUE);

}



static void
cerca_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
Evas_Object *win, *popup, *content;
Evas_Object *bt_torna;
int ALT=200;
int AMPLA=200;
const char *nom, *cog1, *cog2, *mail, *tlf_casa, *tlf_mobil, *tlf_altres, *altres, cerca[16384], *hoversel_cerca;
 int chnom, chcog1, chcog2, chmail, chcasat, chmobil1t, chtlf_altres, chaltres; 
//*en_nom, *en_cog1, *en_cog2, *en_mail, *en_tlf_casa, *en_tlf_mobil1, *en_tlf_altres, *en_altres,

    	PGconn *conexion = conecta();
	PGresult        *res;
	int rec_count;
	int mail4,tlf_casa5, tlf_mobil6,tlf_altres7;
	int nFields, nRows;
	int fila, camp;
	char concat="";

	char *str = malloc(sizeof(char)*512 );
	
if(DEBUG==1)	printf("\naquí cercaré ...\n");



	nom = elm_object_text_get(evas_object_data_get(obj, "lb1")); //passo el camp de text, no el quadre de text

	cog1 = elm_object_text_get(evas_object_data_get(obj, "lb2"));

	cog2 = elm_object_text_get(evas_object_data_get(obj, "lb3"));

	mail = elm_object_text_get(evas_object_data_get(obj, "lb4"));

	tlf_casa = elm_object_text_get(evas_object_data_get(obj, "lb5"));//tlf 

	tlf_mobil = elm_object_text_get(evas_object_data_get(obj, "lb6"));//tlf

	tlf_altres = elm_object_text_get(evas_object_data_get(obj, "lb7"));//tlf

	altres = elm_object_text_get(evas_object_data_get(obj, "lb8"));
	

       	

	if(DEBUG==1)printf("\n nom=%s\n cog1=%s\n cog2=%s\n mail=%s\n tlf_casa=%s\n tlf_mobil=%s\n tlf_altres=%s\n altres=%s\n",nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);

//nom
	if(strcmp(nom,"Escriu el nom que vulguis cercar")==0) {
		snprintf(cerca,2048, "%s", SELECTT);
		if(DEBUG==1){
			printf("\ncerca (nom buit)= %s\n",cerca);
		}		
	}
	else{
		snprintf(cerca,2048, "%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom);		
		if(DEBUG==1)	printf("\ncerca (nom ple)= %s\n",cerca);
	}
//cog1
//	if( strstr(" WHERE ",cerca)==0) {
	if(strcmp(cog1,"Escriu el cognom1 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {

			printf("\nja hi havia alguna cosa cog1\ncerca = %s\n",cerca);
			//concatenar amb or
			if(strcmp(nom,"Escriu el cognom1 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1);
			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1);

			}
			printf("\ncerca cognom net = %s\n", cerca);
		} 
		else{
			printf("\nESTAVA NET cog1?\n");
			//posar ls select completa
			sprintf(cerca,"%s WHERE COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' ", SELECTT, cog1);
			printf("\ncerca cognom net = %s\n", cerca);		
		}
	}
//cog2 ...
	if(strcmp(cog2,"Escriu el cognom2 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			printf("\nja hi havia alguna cosa cog2\n");
			if(strcmp(nom,"Escriu el cognom2 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2);
			}
			printf("\ncerca cognom2  = %s\n", cerca);
		} 
		else{
			printf("\nESTAVA NET cog2?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' ", SELECTT, cog2);		
		}
	}
	if(strcmp(mail,"Escriu el mail que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa mail\n");
			if(strcmp(nom,"Escriu el mail que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail);
			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail);

			}
			printf("\ncerca mail = %s\n",cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET mail ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(correu_telefons, '' ) like '\%%%s\%%' ", SELECTT, mail);		
		}
	}
	if(strcmp(mail,"Escriu el telefon1 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon1\n");
			//concatenar amb or
			if(strcmp(nom,"Escriu el telefon1 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail, tlf_casa);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail, tlf_casa);
			}
			printf("\ncerca telefon1 %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon1 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(telefon_casa, '' ) like '\%%%s\%%' ", SELECTT, tlf_casa);		
		}
	}
	if(strcmp(mail,"Escriu el telefon2 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon2\n");
			//concatenar amb or
//			snprintf(cerca,1024, "%s or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", cerca, tlf_mobil);
			if(strcmp(nom,"Escriu el telefon2 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' and COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil);
			}
			printf("\ncerca telefon2 %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon2 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(telefon_mobil, '' ) like '\%%%s\%%' ", SELECTT, tlf_mobil);		
		}
	}
//Escriu el telefon3 que vulguis cercar
	if(strcmp(mail,"Escriu el telefon3 que vulguis cercar")!=0) { //telefon 3
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon3\n");
			//concatenar amb or
//			snprintf(cerca,1024, "%s or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", cerca, tlf_mobil);
			if(strcmp(nom,"Escriu el telefon3 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' and COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  and COALESCE(telefon_2_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' or COALESCE(telefon_mobil, '' ) like '\%%%s\%%' or COALESCE(telefon_2_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres);
			}
			printf("\ncerca telefon3 %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon2 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(telefon_mobil, '' ) like '\%%%s\%%' ", SELECTT, tlf_mobil);	
		}
	}
	if(strcmp(mail,"Entra el que vulguis cercar")!=0) { //últim camp
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon3\n");
			//concatenar amb or
			if(strcmp(nom,"Escriu el que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' and COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  and COALESCE(altres_telefons, '' ) like '\%%%s\%%' and COALESCE(altres_telefons, '' ) like  '\%%%s\%%'", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);
				
			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  or COALESCE(telefon_2_mobil, '' ) like '\%%%s\%%' or COALESCE(altres_telefons, '' ) like  '\%%%s\%%'", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);
			}
			printf("\ncerca \"altres\" %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon3 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(altres_telefons, '' ) like '\%%%s\%%' ", SELECTT, tlf_altres);		
		}
	}


								printf("\nRESULTAT CERCA : \'%s\'\n",cerca);
	
	res = PQexec(conexion,cerca);
	//PQerrorMessage(res);
	desconecta(conexion);


	nFields = PQnfields(res);
	nRows = PQntuples(res);


	if(nRows>0)for (fila=0;fila<=nRows-1;fila++){
			if(fila==0) printf("#####Primer camp######");//snprintf(str, 35, "%s","#####Primer camp######");
			else printf("#####Següentr camp######");//snprintf(str, 35, "%s","#####Següent camp######");
			for(camp=0;camp<=nFields-1;camp++)
				{
				if(DEBUG==1)printf("\nfila %d camp %d\n",fila,camp);
				if(camp==0) printf("\n ID : %s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " ID : %s",PQgetvalue(res,fila,camp)); //fila,camp);		
				else if(camp==1){ printf("\n NOM : %s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " NOM : %s",PQgetvalue(res,fila,camp));
//						printf("\n check_NOM : %s\n", elm_check_state_get(check_nom));
					}
				else if(camp==2){ printf("\n 1r COGNOM : %s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " 1r COGNOM : %s",PQgetvalue(res,fila,camp));
						//printf("\n check_cog1 : %s \n", check_cog1);
					}
				else if(camp==3){ printf("\n 2N COGNOM : %s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " 2N COGNOM : %s",PQgetvalue(res,fila,camp));
//						printf("\n check_cog2 : %s\n", check_cog2);
					}
				else if(camp==4){ printf("\n CORREU : %s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " CORREU : %s",PQgetvalue(res,fila,camp));
//						printf("\n check_mail : %s\n", check_mail);
					}
				else if(camp==5){ printf("\n TELEFON1 : %s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " TELEFON : %s",PQgetvalue(res,fila,camp));
//						printf("\n check_casat : %s\n", check_casat);
					}
				else if(camp==6){ printf("\n TELEFON2 : %s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " TELEFON : %s",PQgetvalue(res,fila,camp));
//						printf("\n check_mobil1t : %s\n", check_mobil1t);
					}
				else if(camp==7){ printf("\n TELEFON3 :%s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, " TELEFON :%s",PQgetvalue(res,fila,camp));
//						printf("\n check_tlf_altres : %s\n", check_tlf_altres);
					}
				else if(camp==8){ printf("\n Altres =%s\n",PQgetvalue(res,fila,camp));//snprintf(str, 90, "%s",PQgetvalue(res,fila,camp));
//						printf("\n check_altres : %s\n", check_altres);
					}
				if(DEBUG==1)printf(" valor llegit al carregar = %s en el camp %d", PQgetvalue(res,fila,camp),fila);
			}
	}
	printf("\nnúmero de camps = %d\n número de files %d\n",nFields, nRows);

}




static void
_cerca(void *data, Evas_Object *obj, void *event_info)
{
	if(DEBUG) printf("\nCerca\n");
	
/* 
	EL TINC MAL PLANTEJAT ? NO POT SER QUE HAGI DE FER UN TEXT_BOX, ENTRAR EL CAMP I ESCOLLIR EL QUE ÉS D'UN LIST BOX ? I A PARTIR D'AQUÍ LLENSAR LA CERCA ? AMB AIXÒ _transitions[9] NO ES POT FER ?

https://blog.jooq.org/2014/12/30/the-awesome-postgresql-9-4-sql2003-filter-clause-for-aggregate-functions/
http://www.techonthenet.com/postgresql/like.php
https://www.postgresql.org/docs/9.4/static/sql-expressions.html
http://www.cybertec.at/2015/02/postgresql-9-4-aggregation-filters-they-do-pay-off/


*/
Evas_Object *win, *gd, *bg, *en, *tg, *lb, *sp, *en_nomt, *en_nom, *en_cog1t, *en_cog1, *en_cog2t, *en_cog2, *en_mailt, *en_mail,   *en_tlf_casat, *en_tlf_casa,  *en_tlf_mobil1t, *en_tlf_mobil1,  *en_tlf_altrest, *en_tlf_altres,   *en_altrest, *en_altres;
   Evas_Object *bt, *en2;
   Evas_Object *bt_torna;

Eina_Bool value;

   evas_object_hide(data);




   win = elm_win_util_standard_add("entry7", "Cerca");
//   elm_win_autodel_set(win, EINA_TRUE);//EINA_TRUE <- NO TANCA PRINCIPAL EINA_FALSE
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);//per tancar
   elm_win_title_set(win, "Cercar dades de l'agenda");
//   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_resize(win, WIDTH, HEIGHT);


   gd = elm_grid_add(win);

   evas_object_size_hint_min_set(gd, 700 * elm_config_scale_get(), 550 * elm_config_scale_get()); //mida més petita
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);

   /* upper entry group */
 bg = elm_bg_add(win); //fons blau
   elm_bg_color_set(bg, 0, 0, 255);

   elm_grid_pack(gd, bg, 5, 5, 45, 45);
   evas_object_show(bg);


   en_nomt = elm_entry_add(win); //nom
   elm_entry_scrollable_set(en_nomt, EINA_FALSE);
   evas_object_size_hint_weight_set(en_nomt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_nomt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_nomt, "Escriu el nom"); 
   evas_object_show(en_nomt); 
   elm_grid_pack(gd, en_nomt, 5, 8, 8, 5);
   en_nom = elm_entry_add(win);
   elm_entry_scrollable_set(en_nom, EINA_TRUE);
   evas_object_size_hint_weight_set(en_nom, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_nom, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_nom, "Escriu el nom aquí");
   evas_object_smart_callback_add(en_nom, "focused", neteja_text, en_nom);
   evas_object_show(en_nom);
   elm_grid_pack(gd, en_nom, 15, 8, 25, 5);


   en_cog1t = elm_entry_add(win); //COG1	
   elm_entry_scrollable_set(en_cog1t, EINA_FALSE);
   evas_object_size_hint_weight_set(en_cog1t, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog1t, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog1t, "Escriu el primer cognom"); 
   evas_object_show(en_cog1t); 
   elm_grid_pack(gd, en_cog1t, 5, 13, 8, 5);
   en_cog1 = elm_entry_add(win);
   elm_entry_scrollable_set(en_cog1, EINA_TRUE);
   evas_object_size_hint_weight_set(en_cog1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog1, "Escriu el primer cognom aquí");
   evas_object_smart_callback_add(en_cog1, "focused", neteja_text, en_cog1);
   evas_object_show(en_cog1);
   elm_grid_pack(gd, en_cog1, 15, 13, 25, 5);


   en_cog2t = elm_entry_add(win); //COG2	
   elm_entry_scrollable_set(en_cog2t, EINA_FALSE);
   evas_object_size_hint_weight_set(en_cog2t, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog2t, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog2t, "Escriu el segon cognom"); 
   evas_object_show(en_cog2t); 
   elm_grid_pack(gd, en_cog2t, 5, 18, 8, 5);
   en_cog2 = elm_entry_add(win);
   elm_entry_scrollable_set(en_cog2, EINA_TRUE);
   evas_object_size_hint_weight_set(en_cog2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog2, "Escriu el segon cognom aquí");
evas_object_smart_callback_add(en_cog2, "focused", neteja_text, en_cog2);
   evas_object_show(en_cog2);
   elm_grid_pack(gd, en_cog2, 15, 18, 25, 5);


   en_mailt = elm_entry_add(win); //correu	
   elm_entry_scrollable_set(en_mailt, EINA_FALSE);
   evas_object_size_hint_weight_set(en_mailt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_mailt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_mailt, "Entra el correu"); 
   evas_object_show(en_mailt); 
   elm_grid_pack(gd, en_mailt, 5, 23, 8, 5); 
  en_mail = elm_entry_add(win);
   elm_entry_scrollable_set(en_mail, EINA_TRUE);
   evas_object_size_hint_weight_set(en_mail, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_mail, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_mail, "Escriu el correu aquí");
evas_object_smart_callback_add(en_mail, "focused", neteja_text, en_mail);
   evas_object_show(en_mail);
   elm_grid_pack(gd, en_mail, 15, 23, 25, 5);


   en_tlf_casat = elm_entry_add(win); //TELEFON1	
   evas_object_size_hint_weight_set(en_tlf_casat, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_casat, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_casat, "Escriu el telefon1"); 
   evas_object_show(en_tlf_casat); 
   elm_grid_pack(gd, en_tlf_casat, 5, 28, 8, 5);
   en_tlf_casa = elm_entry_add(win);
   elm_entry_scrollable_set(en_tlf_casa, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_casa, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_casa, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_casa, "Escriu el primer telefon aquí");
evas_object_smart_callback_add(en_tlf_casa, "focused", neteja_text, en_tlf_casa);
   evas_object_show(en_tlf_casa);
   elm_grid_pack(gd, en_tlf_casa, 15, 28, 25, 5);


   en_tlf_mobil1t = elm_entry_add(win); //MOVIL	
   elm_entry_scrollable_set(en_tlf_mobil1t, EINA_FALSE);
   evas_object_size_hint_weight_set(en_tlf_mobil1t, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_mobil1t, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_mobil1t, "Escriu el telefon2"); 
   evas_object_show(en_tlf_mobil1t); 
   elm_grid_pack(gd, en_tlf_mobil1t, 5, 33, 8, 5);
   en_tlf_mobil1 = elm_entry_add(win);
   elm_entry_scrollable_set(en_tlf_mobil1, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_mobil1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_mobil1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_mobil1, "Escriu el segon telefon aquí");
evas_object_smart_callback_add(en_tlf_mobil1, "focused", neteja_text, en_tlf_mobil1);
   evas_object_show(en_tlf_mobil1);
   elm_grid_pack(gd, en_tlf_mobil1, 15, 33, 25, 5);


   en_tlf_altrest = elm_entry_add(win); //MOVIL2	
   elm_entry_scrollable_set(en_tlf_altrest, EINA_FALSE);
   evas_object_size_hint_weight_set(en_tlf_altrest, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_altrest, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_altrest, "Escriu el telefon3"); 
   evas_object_show(en_tlf_altrest); 
   elm_grid_pack(gd, en_tlf_altrest, 5, 38, 12, 5);
   en_tlf_altres = elm_entry_add(win);
   elm_entry_scrollable_set(en_tlf_altres, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_altres, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_altres, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_altres, "Escriu el tercer telefon aquí");
evas_object_smart_callback_add(en_tlf_altres, "focused", neteja_text, en_tlf_altres);
   evas_object_show(en_tlf_altres);
   elm_grid_pack(gd, en_tlf_altres, 15, 38, 25, 5);


   en_altrest = elm_entry_add(win); //ALTRES
   elm_entry_scrollable_set(en_altrest, EINA_FALSE);
   evas_object_size_hint_weight_set(en_altrest, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_altrest, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_altrest, "Escriu 'altres'"); 
   evas_object_show(en_altrest); 
   elm_grid_pack(gd, en_altrest, 5, 43, 12, 5);
   
en_altres = elm_entry_add(win);
   elm_entry_scrollable_set(en_altres, EINA_TRUE);
   evas_object_size_hint_weight_set(en_altres, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_altres, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_altres, "Entra el que vulguis cercar");
evas_object_smart_callback_add(en_altres, "focused", neteja_text, en_altres);
   evas_object_show(en_altres);
   elm_grid_pack(gd, en_altres, 15, 43, 25, 5);



Evas_Object *rect1, *hoversel, *btn = NULL;
   rect1 = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_color_set(rect1, 2, 0, 0, 2);
   evas_object_show(rect1);
   hoversel = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hoversel, win);
   elm_hoversel_horizontal_set(hoversel, EINA_FALSE);
   elm_object_text_set(hoversel, "Mostra Registre");
   evas_object_data_set(hoversel, "lb1", en_nom); //carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb2", en_cog1);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb3", en_cog2);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb4", en_mail);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb5", en_tlf_casa);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb6", en_tlf_mobil1);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb7", en_tlf_altres);//carreguem les dades al butó ...
   evas_object_data_set(hoversel, "lb8", en_altres);//carreguem les dades al butó ...
   evas_object_smart_callback_add(hoversel, "clicked", carrega_registres,en );
// evas_object_smart_callback_add(hoversel, "clicked", cerca_bt_clicked, NULL);
// evas_object_smart_callback_add(bt, "clicked", cerca_bt_clicked, en);

   evas_object_smart_callback_add(hoversel,"" , NULL , NULL );//"clicked", registre, entra); //és la línea bona
   elm_grid_pack(gd, hoversel, 62, 2, 38, 2);
   evas_object_show(hoversel);


  // printf("\nelm_check_state_get\n(check_nom)=%s\n",elm_check_state_get(check_nom));
   bt = elm_button_add(win);
   elm_object_text_set(bt, "Cerca");
   elm_grid_pack(gd, bt, 10,70,15,5);
   evas_object_data_set(bt, "lb1", en_nom); //carreguem les dades al butó ...
   evas_object_data_set(bt, "lb2", en_cog1);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb3", en_cog2);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb4", en_mail);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb5", en_tlf_casa);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb6", en_tlf_mobil1);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb7", en_tlf_altres);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb8", en_altres);//carreguem les dades al butó ...

   evas_object_data_set(bt, "lb8", hoversel);//carrego dades de l'hoverselal butó ?
   evas_object_smart_callback_add(bt, "clicked", cerca_bt_clicked, hoversel);//carrega_registres, hoversel);
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
//char
insert_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
	const char *nom, *cog1, *cog2, *mail, *tlf_casa, *tlf_mobil, *tlf_altres, *altres, insert[4096]; 
//*en_nom, *en_cog1, *en_cog2, *en_mail, *en_tlf_casa, *en_tlf_mobil1, *en_tlf_altres, *en_altres,

    	PGconn *conexion = conecta();
	PGresult        *res;
	int rec_count;
	int mail4,tlf_casa5, tlf_mobil6,tlf_altres7;
if(DEBUG==1)	printf("\naquí insertaré quan vegi com passar tots els valors a la funció ... \n");

	 // simpler version
//    en_tot = evas_object_data_get(obj, "lb1");
//    txt1 = elm_entry_text_get(en_tot);

	nom = elm_object_text_get(evas_object_data_get(obj, "lb1"));
	cog1 = elm_object_text_get(evas_object_data_get(obj, "lb2"));
	cog2 = elm_object_text_get(evas_object_data_get(obj, "lb3"));
	mail = elm_object_text_get(evas_object_data_get(obj, "lb4"));
	tlf_casa = elm_object_text_get(evas_object_data_get(obj, "lb5"));//tlf 
	tlf_mobil = elm_object_text_get(evas_object_data_get(obj, "lb6"));//tlf
	tlf_altres = elm_object_text_get(evas_object_data_get(obj, "lb7"));//tlf
	altres = elm_object_text_get(evas_object_data_get(obj, "lb8"));
	if(DEBUG==1){
	if(DEBUG==1)printf("\nnom=%s\ncog1=%s\ncog2=%s\nmail=%s\ntlf_casa=%s\ntlf_mobil=%s\ntlf_altres=%s\naltres=%s\n", nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);
	mail4=comproba_mail(mail);
	tlf_casa5=comproba_telefons(tlf_casa);	
	tlf_mobil6=comproba_telefons(tlf_mobil);
	tlf_altres7=comproba_ultim(tlf_altres);
	if(DEBUG==1)printf("mail4 = %d, tlf_casa5 = %d, tlf_mobil6 = %d,tlf_altres7 = %d, tlf_casa = %s, tlf_mobil =%s, tlf_altres = %s, strlen(nom)!=0= %d",mail4, tlf_casa5, tlf_mobil6,tlf_altres7,tlf_casa, tlf_mobil,tlf_altres, strlen(nom)!=0);
	//	printf("INSERT INTO telefons(id_telefons, nom_telefons, cognom1_telefons, cognom2_telefons, correu_telefons, telefon_casa, telefon_mobil, telefon_2_mobil, altres_telefons) VALUES (nextval('id_seq_telefons'::regclass),'%s', '%s', '%s', '%s', %d, %d, %d, '%s')  RETURNING * ;", txt1,txt2,txt3,txt4,0,0,0,txt8);
	//			printf("\nLa sortida és : \t %s\n\n\n\n test5 = %d, test6= %d",insert,test5, test6);
		}
	if(tlf_altres=="000000000000000") tlf_altres="0";
	if(DEBUG==1) printf("\n\nINSERTT = %s\n\n", INSERTT);
	if(strlen(nom)!=0 && mail4==1 && tlf_casa5==0 && tlf_mobil6==0 && tlf_altres7==0 ){

		sprintf(insert, INSERTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);//"INSERT INTO telefons (id_telefons, nom_telefons, cognom1_telefons, cognom2_telefons, correu_telefons, telefon_casa, telefon_mobil, telefon_2_mobil, altres_telefons) VALUES (nextval('id_seq_telefons'::regclass),'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') ;",  nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);
//INSERTT,nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres); 
		//if(DEBUG==1)puts(insert);
		if(DEBUG==1)printf("\n\ninsert =%s\n\n", insert);
		res = PQexec(conexion,insert);


	}
	else{
		if(DEBUG==1)printf("\n\n no inserto pk no són números els telefons o me'n falten ...\n\n");//mostrar missatge 
		/*FALTA mostrar finestra d'error d'inserció'*/
Evas_Object *win,*gd, *bt_torna,*error;
  win = elm_win_util_standard_add("error", "Error!!!");
error = elm_entry_add(win); //nom
gd = elm_grid_add(win);
   elm_grid_size_set(gd, 50, 50);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_min_set(gd, 544 * elm_config_scale_get(), 374 * elm_config_scale_get());
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);
   elm_entry_scrollable_set(error, EINA_TRUE);
   evas_object_size_hint_weight_set(error, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(error, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(error, "HI HA UN ERROR, REVISA-HO");
 bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");
   evas_object_show(bt_torna);
evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);
//   evas_object_smart_callback_add(en_nom, "clicked", neteja_text, en_nom);
elm_grid_pack(gd, error, 20, 15, 10, 18);
elm_grid_pack(gd, bt_torna, 30, 15, 10, 16);

   evas_object_show(error);
   evas_object_show(win);
//   elm_grid_pack(gd, en_nom, 30, 10, 30, 6);
	}

	desconecta(conexion);


}

static void
_afegir(void *data, Evas_Object *obj, void *event_info)
{
	
	if(DEBUG==1) printf("\nAfegeix\n");
   Evas_Object *win, *gd, *bg,*lb_nom, *en_nom, *lb_cog1, *en_cog1, *lb_cog2, *en_cog2, *lb_mail, *en_mail, *lb_casa, *en_tlf_casa, *lb_mobil, *en_tlf_mobil1,*lb_taltres, *en_tlf_altres, *lb_altres, *en_altres,  *tg, *lb, *sp;
   Evas_Object *bt, *en2;
   Evas_Object *bt_torna;

   evas_object_hide(data);



   win = elm_win_util_standard_add("entry7", "Afegir");
//   elm_win_autodel_set(win, EINA_TRUE);

   gd = elm_grid_add(win);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_min_set(gd, 544 * elm_config_scale_get(), 374 * elm_config_scale_get());
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);

   /* upper entry group */
   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 0, 0, 0);
   elm_grid_pack(gd, bg, 10, 10, 60, 60);
   evas_object_show(bg);



   en_nom = elm_entry_add(win); //nom
   elm_entry_scrollable_set(en_nom, EINA_TRUE);
   evas_object_size_hint_weight_set(en_nom, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_nom, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_nom, "Entra el nom");
   evas_object_smart_callback_add(en_nom, "focused", neteja_text, en_nom);//"clicked", neteja_text, en_nom);
   evas_object_show(en_nom);
   elm_grid_pack(gd, en_nom, 30, 10, 30, 6);

   en_cog1 = elm_entry_add(win); //cognom1
   elm_entry_scrollable_set(en_cog1, EINA_TRUE);
   evas_object_size_hint_weight_set(en_cog1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog1, "Primer cognom");
   evas_object_smart_callback_add(en_cog1, "focused", neteja_text, en_cog1);
   evas_object_show(en_cog1);
   elm_grid_pack(gd, en_cog1, 30, 17, 30, 6);

   en_cog2 = elm_entry_add(win); //cognom2
   elm_entry_scrollable_set(en_cog2, EINA_TRUE);
   evas_object_size_hint_weight_set(en_cog2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_cog2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_cog2, "Segon cognom");
   evas_object_smart_callback_add(en_cog2, "focused", neteja_text, en_cog2);
   evas_object_show(en_cog2);
   elm_grid_pack(gd, en_cog2, 30, 24, 30, 6);

   en_mail = elm_entry_add(win); //Correu 
   elm_entry_scrollable_set(en_mail, EINA_TRUE);
   evas_object_size_hint_weight_set(en_mail, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_mail, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_mail, "Correu electrònic");
   evas_object_smart_callback_add(en_mail, "focused", neteja_text, en_mail);
   evas_object_show(en_mail);
   elm_grid_pack(gd, en_mail, 30, 31, 30, 6);
	
   en_tlf_casa = elm_entry_add(win); //telefon casa 
   elm_entry_scrollable_set(en_tlf_casa, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_casa, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_casa, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_casa, "Telefon casa");
   evas_object_smart_callback_add(en_tlf_casa, "focused", neteja_text, en_tlf_casa);
   evas_object_show(en_tlf_casa);
   elm_grid_pack(gd, en_tlf_casa, 30, 38, 30, 6);

   en_tlf_mobil1 = elm_entry_add(win); //mobil 
   elm_entry_scrollable_set(en_tlf_mobil1, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_mobil1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_mobil1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_mobil1, "Telefon mòbil");
   evas_object_smart_callback_add(en_tlf_mobil1, "focused", neteja_text, en_tlf_mobil1);
   evas_object_show(en_tlf_mobil1);
   elm_grid_pack(gd, en_tlf_mobil1, 30, 45, 30, 6);

   en_tlf_altres = elm_entry_add(win); //altres telefons 
   elm_entry_scrollable_set(en_tlf_altres, EINA_TRUE);
   evas_object_size_hint_weight_set(en_tlf_altres, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_tlf_altres, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_tlf_altres, "Tercer telefon");
   evas_object_smart_callback_add(en_tlf_altres, "focused", neteja_text, en_tlf_altres);
   evas_object_show(en_tlf_altres);
   elm_grid_pack(gd, en_tlf_altres, 30, 52, 30, 6);

   en_altres = elm_entry_add(win); //altres dades 
   elm_entry_scrollable_set(en_altres, EINA_TRUE);
   evas_object_size_hint_weight_set(en_altres, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en_altres, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(en_altres, "Altres dades");
   evas_object_smart_callback_add(en_altres, "focused", neteja_text, en_altres);
   evas_object_show(en_altres);
   elm_grid_pack(gd, en_altres, 30, 59, 30, 6);

   bt = elm_button_add(win);
   
   elm_object_text_set(bt, "Afegir dades");
   elm_grid_pack(gd, bt, 10,70,15,5);

   evas_object_data_set(bt, "lb1", en_nom); //carreguem les dades al butó ...
   evas_object_data_set(bt, "lb2", en_cog1);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb3", en_cog2);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb4", en_mail);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb5", en_tlf_casa);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb6", en_tlf_mobil1);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb7", en_tlf_altres);//carreguem les dades al butó ...
   evas_object_data_set(bt, "lb8", en_altres);//carreguem les dades al butó ...
   evas_object_smart_callback_add(bt, "clicked", insert_bt_clicked, NULL);
  

     evas_object_show(bt);

   bt_torna = elm_button_add(win);
elm_object_text_set(bt_torna, "Torna");

evas_object_smart_callback_add(bt_torna, "clicked",_torna, win);
evas_object_smart_callback_add(bt_torna, "clicked",_mostra_principal, data);
evas_object_smart_callback_add(win, "delete,request", _main_win_del_cb,NULL);
evas_object_resize(bt_torna, 100, 60);
 elm_grid_pack(gd, bt_torna, 70,80,15,15);
evas_object_show(bt_torna);

lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra el <b>nom</b> :");
elm_grid_pack(gd,lb_nom, 13,10,20,6);
evas_object_show(lb_nom);
lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra el <b>PRIMER cognom</b> :");
elm_grid_pack(gd,lb_nom, 10,17,20,6);
evas_object_show(lb_nom);
lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra el <b>SEGON cognom</b> :");
elm_grid_pack(gd,lb_nom, 10,24,20,6);
evas_object_show(lb_nom);
lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra el <b>correu</b> :");
elm_grid_pack(gd,lb_nom, 12,31,20,6);
evas_object_show(lb_nom);

lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra el <b>telèfon</b> :");
elm_grid_pack(gd,lb_nom, 10,38,20,6);
evas_object_show(lb_nom);

lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra el <b>mòbil</b> :");
elm_grid_pack(gd,lb_nom, 10,45,20,6);
evas_object_show(lb_nom);

lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra un <b>altre mòbil</b> :");
elm_grid_pack(gd,lb_nom, 10,52,20,6);
evas_object_show(lb_nom);

lb_nom = elm_label_add(win);
elm_object_text_set(lb_nom, "Entra <b>altres dades</b> :");
elm_grid_pack(gd,lb_nom, 12,59,20,6);
evas_object_show(lb_nom);


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

static void
_clear_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
   if (!data) return;
   elm_hoversel_clear((Evas_Object *)data);
}
static void
_reverse_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
   if (!data) return;
   elm_object_mirrored_set(data, !elm_object_mirrored_get (data));
   printf("\nMirrored mode is now %s", elm_object_mirrored_get(data) ? "ON" : "OFF" );
   fflush(stdout);
}
Eina_Bool is_eng = EINA_TRUE;
static void
_arabic_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
   if (!data) return;
   is_eng = !is_eng;
   elm_object_text_set(data, is_eng ?
                       "Enable Arabic for new items" :
                       "Enable English for new items");
}
static int entra=0;
static void
neteja_hoversel(void *data, Evas_Object *obj, void *event_info)
{
elm_hoversel_clear(data);
}
static void
carrega_registres(void *data, Evas_Object *obj, void *event_info)
{
 elm_hoversel_clear(data);
const char *nom, *cog1, *cog2, *mail, *tlf_casa, *tlf_mobil, *tlf_altres, *altres, cerca[16384];
nom = elm_object_text_get(evas_object_data_get(obj, "lb1")); //passo el camp de text, no el quadre de text

	cog1 = elm_object_text_get(evas_object_data_get(obj, "lb2"));

	cog2 = elm_object_text_get(evas_object_data_get(obj, "lb3"));

	mail = elm_object_text_get(evas_object_data_get(obj, "lb4"));

	tlf_casa = elm_object_text_get(evas_object_data_get(obj, "lb5"));//tlf 

	tlf_mobil = elm_object_text_get(evas_object_data_get(obj, "lb6"));//tlf

	tlf_altres = elm_object_text_get(evas_object_data_get(obj, "lb7"));//tlf

	altres = elm_object_text_get(evas_object_data_get(obj, "lb8"));
if(DEBUG==1)printf("\n nom(carrega_registres)=%s\n cog1(carrega_registres)=%s\n cog2(carrega_registres)=%s\n mail(carrega_registres)=%s\n tlf_casa(carrega_registres)=%s\n tlf_mobil(carrega_registres)=%s\n tlf_altres(carrega_registres)=%s\n altres(carrega_registres)=%s\n",nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);

 if(DEBUG==1) printf("\n hauria de carregar els registres?\n");	
	PGconn *conexion = conecta();
	PGresult        *res;
	int rec_count, fila, camp;
	int nFields, nRows;

	static int num = 0;
   	char *str = malloc(sizeof(char)*250 );//* 11);
	Elm_Object_Item *hoversel_it;
	printf("\nSELECTT=%s\n\n",SELECTT);

if(DEBUG==1)printf("\n nom=%s\n cog1=%s\n cog2=%s\n mail=%s\n tlf_casa=%s\n tlf_mobil=%s\n tlf_altres=%s\n altres=%s\n",nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);

//nom
	if(strcmp(nom,"Escriu el nom que vulguis cercar")==0) {
		snprintf(cerca,2048, "%s", SELECTT);
		if(DEBUG==1){
			printf("\ncerca (nom buit)= %s\n",cerca);
		}		
	}
	else{
		snprintf(cerca,2048, "%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom);		
		if(DEBUG==1)	printf("\ncerca (nom ple)= %s\n",cerca);
	}
//cog1
//	if( strstr(" WHERE ",cerca)==0) {
	if(strcmp(cog1,"Escriu el cognom1 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {

			printf("\nja hi havia alguna cosa cog1\ncerca = %s\n",cerca);
			//concatenar amb or
			if(strcmp(nom,"Escriu el cognom1 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1);
			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1);

			}
			printf("\ncerca cognom net = %s\n", cerca);
		} 
		else{
			printf("\nESTAVA NET cog1?\n");
			//posar ls select completa
			sprintf(cerca,"%s WHERE COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' ", SELECTT, cog1);
			printf("\ncerca cognom net = %s\n", cerca);		
		}
	}
//cog2 ...
	if(strcmp(cog2,"Escriu el cognom2 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			printf("\nja hi havia alguna cosa cog2\n");
			if(strcmp(nom,"Escriu el cognom2 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2);
			}
			printf("\ncerca cognom2  = %s\n", cerca);
		} 
		else{
			printf("\nESTAVA NET cog2?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' ", SELECTT, cog2);		
		}
	}
	if(strcmp(mail,"Escriu el mail que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa mail\n");
			if(strcmp(nom,"Escriu el mail que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail);
			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail);

			}
			printf("\ncerca mail = %s\n",cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET mail ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(correu_telefons, '' ) like '\%%%s\%%' ", SELECTT, mail);		
		}
	}
	if(strcmp(mail,"Escriu el telefon1 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon1\n");
			//concatenar amb or
			if(strcmp(nom,"Escriu el telefon1 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail, tlf_casa);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' ", SELECTT, nom, cog1, cog2, mail, tlf_casa);
			}
			printf("\ncerca telefon1 %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon1 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(telefon_casa, '' ) like '\%%%s\%%' ", SELECTT, tlf_casa);		
		}
	}
	if(strcmp(mail,"Escriu el telefon2 que vulguis cercar")!=0) {
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon2\n");
			//concatenar amb or
//			snprintf(cerca,1024, "%s or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", cerca, tlf_mobil);
			if(strcmp(nom,"Escriu el telefon2 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' and COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil);
			}
			printf("\ncerca telefon2 %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon2 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(telefon_mobil, '' ) like '\%%%s\%%' ", SELECTT, tlf_mobil);		
		}
	}
//Escriu el telefon3 que vulguis cercar
	if(strcmp(mail,"Escriu el telefon3 que vulguis cercar")!=0) { //telefon 3
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon3\n");
			//concatenar amb or
//			snprintf(cerca,1024, "%s or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  ", cerca, tlf_mobil);
			if(strcmp(nom,"Escriu el telefon3 que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' and COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  and COALESCE(telefon_2_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres);

			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' or COALESCE(telefon_mobil, '' ) like '\%%%s\%%' or COALESCE(telefon_2_mobil, '' ) like '\%%%s\%%'  ", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres);
			}
			printf("\ncerca telefon3 %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon2 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(telefon_mobil, '' ) like '\%%%s\%%' ", SELECTT, tlf_mobil);	
		}
	}
	if(strcmp(mail,"Entra el que vulguis cercar")!=0) { //últim camp
		if( strstr(" WHERE ",cerca)==0) {
			if(DEBUG==1)printf("\nja hi havia alguna cosa telefon3\n");
			//concatenar amb or
			if(strcmp(nom,"Escriu el que vulguis cercar")==0) {
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom1_telefons, '' ) like '\%%%s\%%' and COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' and COALESCE(correu_telefons, '' ) like '\%%%s\%%' and  COALESCE(telefon_casa, '' ) like '\%%%s\%%' and COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  and COALESCE(altres_telefons, '' ) like '\%%%s\%%' and COALESCE(altres_telefons, '' ) like  '\%%%s\%%'", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);
				
			}
			else{
				sprintf(cerca,"%s WHERE COALESCE(nom_telefons, '' ) like '\%%%s\%%' or COALESCE(cognom1_telefons, '' ) like '\%%%s\%%'  or COALESCE(cognom2_telefons, '' ) like '\%%%s\%%' or COALESCE(correu_telefons, '' ) like '\%%%s\%%' or  COALESCE(telefon_casa, '' ) like '\%%%s\%%' or COALESCE(telefon_mobil, '' ) like '\%%%s\%%'  or COALESCE(telefon_2_mobil, '' ) like '\%%%s\%%' or COALESCE(altres_telefons, '' ) like  '\%%%s\%%'", SELECTT, nom, cog1, cog2, mail, tlf_casa, tlf_mobil, tlf_altres, altres);
			}
			printf("\ncerca \"altres\" %s\n", cerca);
		} 
		else{
			if(DEBUG==1)printf("\nESTAVA NET telefon3 ?\n");
			//posar ls select completa
			snprintf(cerca,1024, "%s WHERE COALESCE(altres_telefons, '' ) like '\%%%s\%%' ", SELECTT, tlf_altres);		
		}
	}


								printf("\nRESULTAT CERCA : \'%s\'\n",cerca);
	
	res = PQexec(conexion,cerca);
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		printf("\nHA FALLAT LA CONEXIÓ\n");
		PQerrorMessage(res);
	        fprintf(stderr, "SELECTT failed: %s", PQerrorMessage(conexion));
	        //exit_nicely(conn,res);
		PQclear(res);
		PQfinish(conexion);
	}
	PQerrorMessage(res);
	 desconecta(conexion);

	    // first, print out the table collumn attribute names
	    nFields = PQnfields(res);
	    nRows = PQntuples(res);
		if(DEBUG==1)printf("\nnFields = %d\n nRows = %d\n",nFields,nRows);

		for (fila=0;fila<=nRows-1;fila++){
			if(fila==0) snprintf(str, 35, "%s","#####Primer camp######");
			else snprintf(str, 35, "%s","#####Següent camp######");
			hoversel_it = elm_hoversel_item_add(obj, str, NULL, ELM_ICON_NONE, NULL,  str);
			for(camp=0;camp<=nFields-1;camp++)
				{
				if(DEBUG==1)printf("\nfila %d camp %d\n",fila,camp);
				if(camp==0) snprintf(str, 90, " ID : %s",PQgetvalue(res,fila,camp)); //fila,camp);		
				else if(camp==1) snprintf(str, 90, " NOM : %s",PQgetvalue(res,fila,camp));
				else if(camp==2) snprintf(str, 90, " 1r COGNOM : %s",PQgetvalue(res,fila,camp));
				else if(camp==3) snprintf(str, 90, " 2N COGNOM : %s",PQgetvalue(res,fila,camp));
				else if(camp==4) snprintf(str, 90, " CORREU : %s",PQgetvalue(res,fila,camp));
				else if(camp==5) snprintf(str, 90, " TELEFON : %s",PQgetvalue(res,fila,camp));
				else if(camp==6) snprintf(str, 90, " TELEFON : %s",PQgetvalue(res,fila,camp));
				else if(camp==7) snprintf(str, 90, " TELEFON :%s",PQgetvalue(res,fila,camp));
				else if(camp==8) snprintf(str, 90, "%s",PQgetvalue(res,fila,camp));
				if(DEBUG==1)printf(" valor llegit al carregar = %s en el camp %d", str,fila);
				hoversel_it = elm_hoversel_item_add(obj, str, NULL, ELM_ICON_NONE,NULL, 
						               str);
				elm_object_item_del_cb_set(hoversel_it, _free);
			//generar camps 
			}
	}
	if(DEBUG==1)printf("\nnúmero de camps = %d\n número de files %d\n",nFields, nRows);
}

static void
_print_items(void *data, Evas_Object *obj, void *event_info)
{
   const Eina_List *items = elm_hoversel_items_get(obj);
   const Eina_List *l;
   Elm_Object_Item *hoversel_it;
   EINA_LIST_FOREACH(items, l, hoversel_it)
     printf("%s\n", elm_object_item_text_get(hoversel_it));
}
static void
_add_item(void *data, Evas_Object *obj, void *event_info)
{
   static int num = 0;
   char *str = malloc(sizeof(char) * 11);
   Elm_Object_Item *hoversel_it;
   if(is_eng)
     snprintf(str, 11, "item %d", ++num);
   else
     snprintf(str, 11, "بند %d", ++num);
   hoversel_it = elm_hoversel_item_add(obj, str, NULL, ELM_ICON_NONE, NULL,
                                       str);
   elm_object_item_del_cb_set(hoversel_it, _free);
}
static void
_free(void *data, Evas_Object *obj, void *event_info)
{
  // free(data);
}
/*fi hoversel*/

EAPI_MAIN int
elm_main(int argc, char **argv)
{
	Evas_Object *finestra, *text, *text1, *layout, *tab, *rec, *naviframe, *gd, *gd1, *bg, *Caixa, *bt_quit, *bt_drop, *bt_search, *bt_add, *datetime, *ck, *pagina_nova;


Evas_Object *target;//animation object

/*translatable*/
	setlocale (LC_ALL, "");

	bindtextdomain ("agenda", "/usr/share/locale");

	textdomain ("agenda");
/*fi translatable*/



	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);//per tancar


	finestra =elm_win_util_standard_add("principal", "Agenda");
//	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
	elm_win_autodel_set(finestra, EINA_TRUE);

	evas_object_smart_callback_add(finestra, "delete,request",_yes_quit_cb, NULL);//obligo a tancar

//	elm_win_title_set(finestra, "Pantalla principal de l'agenda");
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
   evas_object_size_hint_min_set(rec, 544 * elm_config_scale_get(), 374 * elm_config_scale_get());
   elm_table_pack(tab, rec, 0, 0, 1, 1);


gd = elm_grid_add(finestra);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(finestra, gd);
   evas_object_show(gd);
//
 Evas_Object *box, *taula;


   box = elm_box_add(finestra); //i si hi poses un grid o alguna altre cosa ?
   elm_box_horizontal_set(box, EINA_FALSE);
   elm_box_homogeneous_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(box);
 //  evas_object_resize(box, 400,200); 
//   evas_object_move(box, 500,400);
 elm_grid_pack(gd, box, 48,40,50,50);




Evas_Object *rect1, *hoversel, *btn = NULL;
   rect1 = evas_object_rectangle_add(evas_object_evas_get(finestra));
   evas_object_color_set(rect1, 255, 0, 0, 255);
   evas_object_show(rect1);
   hoversel = elm_hoversel_add(finestra);
   elm_hoversel_hover_parent_set(hoversel, finestra);
   elm_hoversel_horizontal_set(hoversel, EINA_FALSE);
   elm_object_text_set(hoversel, "Mostra Telefons");

elm_grid_pack(gd, hoversel, 48, 48, 40, 40);
   evas_object_show(hoversel);

  printf("\n focus hoversel = %d\n",elm_object_focus_get(hoversel));// focus hoversel = 0
   elm_object_focus_set(hoversel,EINA_TRUE);
   printf("\n focus hoversel = %d\n",elm_object_focus_get(hoversel));// focus hoversel = 0



//LOGO ENLIGHTENMENT BLANC
Evas *e = evas_object_evas_get(finestra);

// Create an image object
Evas_Object *img = evas_object_image_filled_add(e);

// Create another image object
Evas_Object *img2 = evas_object_image_filled_add(e);
evas_object_image_file_set(img2, "./logo_blanc1.png", NULL);
 
// Disable smooth scaling
evas_object_image_smooth_scale_set(img2, EINA_TRUE);
elm_grid_pack(gd,img2, 56,1, 30, 30);
evas_object_move(img2, 490, 30);
evas_object_resize(img2, 2, 2);
evas_object_show(img2);


//RELLOTGE
ck = elm_clock_add(finestra);
elm_clock_show_seconds_set(ck, EINA_TRUE);
elm_clock_show_am_pm_set(ck, EINA_FALSE);
elm_box_pack_end(finestra, ck);
evas_object_resize(ck, 110,110);
evas_object_size_hint_weight_set(ck, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(ck, EVAS_HINT_FILL, 0.5);
elm_grid_pack(gd, ck, 60, 16, 25, 8);
evas_object_show(ck);

//AFEGIR
bt_add = elm_button_add(finestra);
elm_object_text_set(bt_add, "Afegir ");
evas_object_smart_callback_add(bt_add, "clicked", _afegir, finestra);
evas_object_resize(bt_add, 120, 80);
evas_object_size_hint_weight_set(bt_add, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(bt_add, EVAS_HINT_FILL, 0.5);
elm_grid_pack(gd, bt_add, 10, 10, 25, 10);
evas_object_show(bt_add);

//CONSULTAR
bt_search = elm_button_add(finestra);
elm_object_text_set(bt_search, "Cerca ");
evas_object_smart_callback_add(bt_search, "clicked", _cerca, finestra);
evas_object_resize(bt_search, 120,80);
evas_object_size_hint_weight_set(bt_search, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//how to align an object
evas_object_size_hint_align_set(bt_search, EVAS_HINT_FILL, 3.5);
elm_grid_pack(gd, bt_search, 10, 30, 25, 10);
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

elm_grid_pack(gd, bt_drop, 10, 50, 25, 10);

evas_object_show(bt_drop);

//SORTIR
bt_quit = elm_button_add(finestra);
elm_object_text_set(bt_quit, "Surt");
evas_object_smart_callback_add(bt_quit, "clicked",_main_win_del_cb, finestra);
evas_object_resize(bt_quit, 100, 60);
elm_grid_pack(gd, bt_quit, 7, 70, 30, 20);
evas_object_show(bt_quit);



//ANIMACIÓ
Elm_Transit *transit = elm_transit_add();
text= elm_label_add(finestra);//evas_object_text_add(finestra);//elm_label_add(finestra);
elm_object_text_set(text, "Agenda programada amb c i llibreries gràfiques d' Enlightenment (EFL)");
elm_label_slide_duration_set(text, 500);
elm_label_slide_mode_set(text, ELM_LABEL_SLIDE_MODE_ALWAYS);
elm_grid_pack(gd,text, 40,4, 60, 60);
elm_object_style_set(text, "marker");
evas_object_color_set(text, 0, 0, 255, 255);
elm_transit_object_add (transit, text);
elm_transit_effect_rotation_add(transit, 0, 360);
elm_transit_duration_set(transit, 5);
elm_transit_auto_reverse_set(transit, EINA_TRUE);
elm_transit_effect_color_add(transit,1,0,1,0,225,225,0,225);
elm_transit_go(transit);

evas_object_show(text);

evas_object_show(finestra);

 

   elm_run();
   return 0;
}
ELM_MAIN()
