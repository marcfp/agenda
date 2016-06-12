#ifndef FUNCIONS_H_   /* Include guard */
#define FUNCIONS_H_


#define DEBUG (1) //0 no debug, 1 debug
#define MAXD  (12)  	//maxim buffer data
#define MAXLL  (256) 	//maxim buffer lloc
#define MAXM (1024) 	//maxim buffer motiu de cites i altres de telefons
#define MAXC (5) 	//maxim buffer caducitat
#define MAXTELEFON (11)	//telefon de 9 digits + intro
#define MAXLLETRES (60)	//noms maxims de 60 lletres
#define INSERTN "INSERT INTO notes VALUES( "
//DELETE FROM films USING producers
//  WHERE producer_id = producers.id AND producers.name = 'foo';
#define DELETEN "DELETE FROM notes WHERE id_cites=" //"DELETE FROM notes where id="
#define SELECTN "SELECT * FROM notes"
#define SELECTT "SELECT * FROM Telefons"
#define INSERTT "INSERT INTO telefons (id_telefons, nom_telefons, cognom1_telefons, cognom2_telefons, correu_telefons, telefon_casa, telefon_mobil, telefon_2_mobil, altres_telefons) VALUES (nextval('id_seq_telefons'::regclass),"
#define DELETET "DELETE FROM Telefons WHERE id_telefons="


int foo(int x);  /* An example function declaration */
void mostra_telefon(wchar_t *cadena);
void treu_intro (wchar_t *cadena);
void escapa_cometes (wchar_t *cadena);
int valida_caducitat(wchar_t *caducitat);
int llegir_data(wchar_t *data);
int char_int_ascii(char numero);
int valida_data(wchar_t *data);
void posa_cometa(char *text);
int valida_correu(wchar_t *camp);
int valida_text(wchar_t *camp);
int valida_telefon(wchar_t *telefon);

/* grafic*/
int comproba_telefons(char txt[]);
int comproba_ultim(char txt[]);
int comproba_mail(char *camp);

#endif // FOO_H_
