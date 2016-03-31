//aquest software està sota llicència CC BY SA 2.0
#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <math.h>


#define DEBUG 1 //0 no debug, 1 debug

#define MAXD  12  	//maxim buffer data
#define MAXLL  256 	//maxim buffer lloc
#define MAXM 1024 	//maxim buffer motiu de cites i altres de telefons
#define MAXC 5 		//maxim buffer caducitat
#define MAXTELEFON 11//telefon de 9 digits + intro
#define MAXLLETRES 60	//noms maxims de 60 lletres
#define INSERTN "INSERT INTO notes VALUES( "
//DELETE FROM films USING producers
//  WHERE producer_id = producers.id AND producers.name = 'foo';
#define DELETEN "DELETE FROM notes WHERE id="//"DELETE FROM notes where id="
#define SELECTN "SELECT * FROM notes"
#define SELECTT "SELECT * FROM telefons"
#define INSERTT "INSERT INTO telefons VALUES ( "
#define DELETET "DELETE FROM telefons WHERE id_telefons="
void mostra_telefon(wchar_t *cadena)
{
	int longitud,i;
	if(DEBUG)printf("entro a mostra_telefon");
	longitud=wcslen(cadena);
	for (i=0;i<longitud;i++) printf("%c",cadena[i]);
}
void treu_intro (wchar_t *cadena)
{
		int longitud;
	
		longitud=wcslen(cadena);
		if (cadena[longitud-1]='\n') cadena[longitud-1]='\0';
}
void escapa_cometes (wchar_t *cadena)
{
		int longitud;
		int i;
		longitud=wcslen(cadena);
		for (i=0;i<longitud;i++){
			 if(DEBUG)printf("posicio %d -> lletra %c \n",i,cadena[i]);
			 if(cadena[i]=='\'') cadena[i]='^' ;
		}

}
int valida_caducitat(wchar_t *caducitat)
{
	int longitud;
	int i;
	longitud=wcslen(caducitat);
	if(DEBUG)printf("\n caducitat = %ls\n, longitud=%d\n",caducitat,longitud);
	for (i=0;i<longitud;i++){
		if(caducitat[i]=='s'){
			if(DEBUG) printf("\ns minuscula\n");
			return 0;
		}
			else if(caducitat[i]=='S'){
			if(DEBUG) printf("\nS majúscula\n");
			return 0;
			}
				else if(caducitat[i]=='n'){
					if(DEBUG) printf("\nn minúscula\n");
					return 0;
				}
					else if(caducitat[i]=='N'){
						if(DEBUG) printf("\nN majúscula\n");
						return 0;
					}
						else {
							if(DEBUG) printf("\n valor erroni?\n");
							return 1;
						}
	}
	if(DEBUG)printf("valida caducitat final");
	return 0;
}
int llegir_data(wchar_t *data) 				//hauria de forçar a que quan trobes un error, no mires més ... com ho faig ???
{
int longitud;
int i;
int valid=99;
int bucle;
int valor=0;
while(valor==0 ){
	printf("\n Entra la data en format AAAA-MM-DD\t:");
	fgetws(data,MAXD,stdin); //, encoding='UTF8');
	treu_intro(data);
	valor=valida_data(data);
	}
}
int char_int_ascii(char numero)
{
	switch(numero)
	{
		case 48: return 0; break;
		case 49: return 1; break;
		case 50: return 2; break;
		case 51: return 3; break;
		case 52: return 4; break;
		case 53: return 5; break;
		case 54: return 6; break;
		case 55: return 7; break; 
		case 56: return 8; break;
		case 57: return 9; break;
	}
}
int valida_data(wchar_t *data)
{
	int longitud=wcslen(data);
	int i;
	int bolea=1;
		//int deb=1; //deb = 0 no debug, deb = 1 debug
	if(DEBUG)printf(" data = %ls \n, longitud = %d\n", data, longitud);	
	int mileni = char_int_ascii(data[0])*1000;
//9 = 57 ascii
//0 = 48 ascii
	int centena = char_int_ascii(data[1])*100;
	int decena = char_int_ascii(data[2])*10;
	int unitat = char_int_ascii(data[3])*1;
//switch valor=char_int_ascii(valor);

	int any = unitat+decena+centena+mileni;
	int mes = char_int_ascii(data[5])*10 + char_int_ascii(data[6]);
	int dia =char_int_ascii(data[8])*10+ char_int_ascii(data[9]);
	int traspas=0; //0 no traspas, 1, traspas

	if(DEBUG)printf("\n\nany llegit = %d, mes llegit = %d, dia llegit = %d", any, mes, dia);
		
	if((any %4 ==0) && (any % 100 != 0) || (any % 400 == 0)) {
				traspas=1;
				if(DEBUG)printf("es de traspas");
	}
	else {
		traspas=0;
		if(DEBUG)printf("NO HO ÉS");
	}
		
	//És de traspàs cada any múltiple de 4, excepte el múltiples de 100, que no ho són, i excepte els múltiples de 400, que sí que ho són.
	printf("mes = %d", mes);
	if(mes==2 && traspas==1 && (dia<=29 && dia >=1)){
			if(DEBUG)printf("any de traspas correcte, el febrer té 29 dies.. ho deixo passar");
			bolea=1;
		}
	else{
		if((mes==1 || mes ==3 || mes==5 || mes == 7 || mes ==8 || mes==10 || mes ==12)&& (dia<1 || dia>31)){
				if(DEBUG)printf("\n\n\n31 dies introduits erronis\n\n\n");
				bolea=0;
			}
			else if((mes==4 || mes==6 || mes ==9 || mes == 11)&&(dia<1 || dia >30)){
				if(DEBUG)printf("\n\n\n30 dies introduits erronis\n\n\n");
				bolea=0;
				}
				else if((mes==2) && (dia<1 || dia>28)){
					printf("\n\n\n mes de febrer mal introduit\n\n\n");
					bolea =0;
					}
					else if(traspas==1 && longitud==10){
						if(DEBUG)printf("\n\n\nany correcte, no modul i tot ok\n\n\n");
						bolea=1;
						}
						else{
							printf("\n\n\nREPASSA LA DATA, hi ha coses INCORRECTES!!!HA DE SER AIXI : AAAA-MM-DD\n\n\n");
							bolea=0;
						}
	}

	return bolea;
	
}

void posa_cometa(char *text)
{
		int longitud;
		int i;
		longitud=strlen(text);
		for (i=0;i<longitud;i++){
			if (text[i]=='^') text[i]='\'';
		}
}

void menunotes(char *c)
	{
		printf("\n\tPROGRAMA AGENDA\n");
		printf("\n1 	<- entra %s",c);
	        printf("\n2 	<- consulta %s",c);
	        printf("\n3 	<- elimina %s",c);
       		printf("\n4       <- TORNA al menú principal\n");

	}

void paginacio(int i)
{
	char pausa[1024];
		if (i % 3 ==0 && i>0) {
			printf("Pitja ENTER per continuar mostrant les dades de l'agenda");
			fgets(pausa,1024,stdin);
		}

}
int valida_correu(wchar_t *camp)
{
 	char *arroba;
	int pos=-1;
	int i=0;
	int existeix=-1;
	const wchar_t *s;
	arroba="@";
	for (s =camp ; *s; ++s);//copia variable
	if(s-camp<9){
			if (DEBUG){
					printf("\nfalten lletres?");
					printf("allargada = %d",s-camp);
					printf("\nerror d'allargada");
			}
			return 2;
	}
	else {
			if(DEBUG){
					printf("Allargada bona = %d",s-camp);
					
			}
			//pos=strncmp(camp,arroba,0);
			//printf("\n\n strncmp = %d \n\n",strncmp(camp,arroba,0));
			//recorrer tot l'array buscant l'arroba
			for (i=0; i<(s-camp-1);i++){
				if(DEBUG)printf("\ncamp[i]=%c i = %d", camp[i],i);
				if(camp[i]=='@') {
					existeix=1; 
					if(DEBUG)printf("existeix = %d", existeix);
					i=(s-camp-1);
				}
				else{
					existeix=-2;
					if(DEBUG)printf("NO EXISTEIX ?");
				}					
			}
			if(existeix){
				if(DEBUG)printf("conté l'arroba! return 0");
				return existeix;	
			}
			else{
				if(DEBUG)printf("NO té arroba, RETURN 2");
				return existeix;
			}
	}
	/*
	//conta caracters
	const char *s;
	 for (s = str; *s; ++s);
	 return(s - str);
*/
}
//	}
int valida_text(wchar_t *camp)
{
	int i=0;
	while(camp[i]!='\n' && i<=MAXLLETRES) {
	if((camp[i]<'a' || camp[i]>'ú') && (camp[i]<'A' || camp[i]>'Ú') && camp[i]!=' ' )
 {  //EL PROBLEMA VE D'AQUÍ, HE DE MIRAR COM VALIDAR LES LLETRES AMB ACCENTS
			if(DEBUG)printf("Només s'accepten lletres!");
			return 2;
		}
	else {
		if(DEBUG)printf(" ho inserto"); //%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",camp[i]!=130, camp[i]!=133, camp[i]!=138, camp[i]!=141, camp[i]!=144, camp[i]!=149, camp[i]!=151, camp[i]!=161, camp[i]!=162, camp[i]!=164, camp[i]!=165, camp[i]!=181, camp[i]!=183, camp[i]!=212, camp[i]!=224, camp[i]!=227, camp[i]!=233, camp[i]!=235);
		i=i+1;
		}
	if (camp[i]=='\n') return 0;
	}
}
int valida_telefon(wchar_t *telefon)
{
	int i=0;
	while(telefon[i]!='\n' && i<=MAXTELEFON){
		if((telefon[i]<'0' || telefon[i]>'9' )) {			
				printf("\n\nAixò no és un telefon vàlid!!\n\n");	
				return 2;
		}
		else{	
			i=i+1;
		}
		if (telefon[i]=='\n' && i==9 ){
				printf("\n\nTelefon correcte\n\n");
				return 0;
				}
		
	}
	
}

int lectura_dades(char *variable, wchar_t *text, int maxim, int numero_funcio) //variable, necessito passar text, maxim buffer i funció de validació
{
	int i=1	;

	do{
	//flush(stdin);
	printf("%ls",text);
	fgets(variable, maxim, stdin);
		switch(numero_funcio)
		{
			case 1 : i=valida_text(text);
			treu_intro(text);
			break;
		case 2 : i=valida_text(text);
			treu_intro(text);
			break;
		case 3 : i=valida_text(text); 
			treu_intro(text);
			break;
	
		case 4 :
			if(DEBUG) printf("Entro a validar correu\n i = %d", i); 
				i=valida_correu(text);
			treu_intro(text);
			if(DEBUG)printf("Surto de validar correu\n i = %d", i);
			break;
		case 5 : i=valida_text(text);
			treu_intro(text);
			break;
		case 6 : i=valida_telefon(text);
			treu_intro(text);
			break;
		case 7 : i=valida_telefon(text);
			treu_intro(text);
			break;
		case 8 : i=valida_telefon(text);
			treu_intro(text);
			break;
		}	
	}while (i!=0);

}
void lectura_dades_telefons(wchar_t *nom, wchar_t *cognom1, wchar_t *cognom2, wchar_t *correu, wchar_t *direccio, wchar_t *telcasa, wchar_t *telmob, wchar_t *tel2, wchar_t *altres)
{
	int i=1;
	int j;
if (DEBUG) printf("lectura_dades_telefons llegeix les dades de la gent que es vol entrar al programet ...");
	do{
		//llegir nom
		fflush(stdin);
		printf("\n Entra el nom de la persona que desitjes entrar a l'agenda personal\t: ");
		fgetws(nom,MAXLLETRES,stdin);
		i=valida_text(nom);
//		printf(" bolea= %d ", i);
	}while(i!=0);	
	treu_intro(nom);
if (DEBUG) printf("Nom entrat = %ls", nom);
	i=1;
	do{
		printf("\n Entra el primer cognom de %ls\t: ",nom);
		fflush(stdin);
		fgetws(cognom1,MAXLLETRES,stdin);
		i=valida_text(cognom1);
//		printf("bolea  = %d",i);
	}while(i!=0);
	treu_intro(cognom1);
if  (DEBUG) printf("Cognom1 entrat = %ls", cognom1);

	i=1;
	do{
		printf("\n Entra el segon cognom de %ls %ls\t: ",nom,cognom1);
		fflush(stdin);
		fgetws(cognom2,MAXLLETRES,stdin);
		i=valida_text(cognom2);
//		printf("bolea  = %d",i);
	}while(i!=0);
	treu_intro(cognom2);
if (DEBUG) printf("cognom2 entrat = %ls",cognom2);
	i=1;
	do{
		printf("\n Entra la direcció de correu de %ls %ls %ls\t",nom,cognom1,cognom2);
		fgetws(correu,MAXLLETRES,stdin);
		i=valida_correu(correu);
		if(DEBUG)printf("valor; i direccio correu %d",i);
		if(i==1)
			{ 
			i=0;
			if(DEBUG)printf("trenco bucle?");
			}
	}while (i!=0);
	treu_intro(correu);
if (DEBUG) printf("correu entrat = %ls",correu);
	i=1;
	do{
		printf("\n Entra la direcció de %ls %ls %ls\t: ",nom, cognom1, cognom2);
		fflush(stdin);
		fgetws(direccio,MAXLLETRES,stdin);
		i=valida_text(direccio);
//		printf("bolea  = %d",i);
	}while(i!=0);
	treu_intro(direccio);
if (DEBUG) printf("direcció entrada = %ls",direccio);
	i=1;
	do {
		printf("\nEntra el telefon de casa de %ls %ls %ls\t: ",nom, cognom1, cognom2);
		fflush(stdin);
		fgetws(telcasa,MAXTELEFON,stdin);
		i=valida_telefon(telcasa);
	}while (i!=0);
	treu_intro(telcasa); //borra l'ultim nº entrat ? pk ?
if (DEBUG) printf("telefon casa entrat = %ls ",telcasa);
	i=1;
	do {
		printf("\nEntra el mòbil de %ls %ls %ls\t: ",nom, cognom1, cognom2);
		fgetws(telmob,MAXTELEFON,stdin);
		i=valida_telefon(telmob);
	}while (i!=0);
	treu_intro(telmob);
if (DEBUG) printf("mobil entrat  = %ls",telmob);
	i=1;
	do {
		printf("\nEntra el tercer Telefon de %ls %ls %ls\t: ",nom,cognom1, cognom2);
		fgetws(tel2,MAXTELEFON,stdin);
		i=valida_telefon(tel2);
	}while (i!=0);
	treu_intro(tel2);
if (DEBUG) printf("tercer telefon entrat = %ls",tel2);
	i=1;
//	do {
	printf("\nEntra notes curtes de %ls %ls %ls\t: ",nom,cognom1,cognom2);
	fgetws(altres,MAXTELEFON,stdin);
//		i=valida_telefon(telmob);
//	}while (i!=0);
	treu_intro(altres);

if (DEBUG) printf("notes curtes entrades = %ls",altres);	

}

void lectura_dades_notes(wchar_t data[MAXD], wchar_t lloc[MAXLL], wchar_t motiu[MAXM], wchar_t caducitat[MAXC])
{
	int i;
	llegir_data(data);
	printf("\nEntra el lloc \t:");
	fgetws(lloc,MAXLL,stdin);
	treu_intro(lloc);
	if(DEBUG)printf("Lloc entrat = %ls",lloc);
	escapa_cometes(lloc);
	if(DEBUG)printf("lloc cometes = %ls",lloc);
	printf("\nEntra el motiu de la cita \t:");
	fgetws(motiu,MAXM,stdin);
	treu_intro(motiu);
	if(DEBUG)printf("Motiu = %ls",motiu);
	escapa_cometes(motiu);
	if(DEBUG)printf("Motiu cometes = %ls\n",motiu);
	caducitat[0]='p';
	i=0;
	do{
		printf("Entra si té caducitat ('S','s','n','N'): ");
		fgetws(caducitat, MAXC, stdin);
		if(DEBUG)printf("\n\tcaducitat = %ls", caducitat);
		treu_intro(caducitat);
		//escapa_cometes(caducitat);
		if(DEBUG)printf("\t caducitat char = %c",caducitat);
		//fflush(stdin);
		i++;
	}while(caducitat[0]!='s' && caducitat[0]!='S' && caducitat[0]!='n' && caducitat[0]!='N');
	if(DEBUG)printf("\ncaducitat entrada = %c", caducitat);

}

void mostra_dades(int filas, PGresult *resultado, char tria)
{
	int i;
	if (tria=='n') for(i=0; i<filas; i++) {
	                printf("\ndata %d: %s",i, (char*)PQgetvalue(resultado,i,1));
			posa_cometa(PQgetvalue(resultado,i,2));
	                printf("\nlloc %d: %s",i, (char*)PQgetvalue(resultado,i,2));	//modificar pk no surtin l'escape de cometes
			posa_cometa(PQgetvalue(resultado,i,3));
	                printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,3));	//modificar pk no surtin l'escape de cometes
			printf("\ncaducitat %d: %s",i, (char*)PQgetvalue(resultado,i,4));
	                printf("\n**********************************\n");
			paginacio(i);
		        }
	else {
		if (tria=='t') for(i=0; i<filas; i++) {
	                printf("\nnom %d: %s",i, (char*)PQgetvalue(resultado,i,1));
			posa_cometa(PQgetvalue(resultado,i,2));
	                printf("\nPrimer cognom %d: %s",i, (char*)PQgetvalue(resultado,i,2));	//modificar pk no surtin l'escape de cometes
			posa_cometa(PQgetvalue(resultado,i,3));
	                printf("\nSegon cognom %d: %s",i, (char*)PQgetvalue(resultado,i,3));	//modificar pk no surtin l'escape de cometes
			printf("\nCorreu  %d: %s",i, (char*)PQgetvalue(resultado,i,4));
			printf("\ndireccio  %d: %s",i, (char*)PQgetvalue(resultado,i,5));
			printf("\ntelefon casa  %d: %s",i, (char*)PQgetvalue(resultado,i,6));
			printf("\nmobil  %d: %s",i, (char*)PQgetvalue(resultado,i,7));
			printf("\naltres telefons  %d: %s",i, (char*)PQgetvalue(resultado,i,8));
			printf("\naltres dades  %d: %s",i, (char*)PQgetvalue(resultado,i,9));

	                printf("\n**********************************\n");
			paginacio(i);
		        }

	}


}
void menui(char tria)//menu d'inserció 1
	{
        FILE *debug_insert;
        PGconn *conexion;
        PGresult   *resultado, * resultat2;

	//int deb=1; //deb = 0 no debug, deb = 1 debug
        int columnas=0;
        int filas=0;
        int i =0;
        int j=0;
        int valor=0;
	int id;
	char buffer[1024];
	
///inici ntnotes
	wchar_t data[MAXD];
	wchar_t lloc[MAXLL];
	wchar_t motiu[MAXM];
	wchar_t caducitat[MAXC];
	//wchar_t caduca;
///fi notes
///inici telefons
	wchar_t nom[MAXLLETRES];
	wchar_t cognom1[MAXLLETRES];
	wchar_t cognom2[MAXLLETRES];
	wchar_t correu[MAXLLETRES];
	wchar_t direccio[MAXLLETRES];
	wchar_t telcasa[MAXTELEFON];
	wchar_t telmob[MAXTELEFON];
	wchar_t tel2[MAXTELEFON];
	wchar_t altres[MAXM];
///fi telefons
	fflush(stdin);
	if (tria=='n')	{
				printf("\n\t\t\tINSERCIÓ DE CITES EN L'AGENDA");
				lectura_dades_notes(data,lloc,motiu,caducitat);
				//bolea_caduca=1;
			}
	else if(tria=='t')  {  
				printf("\n\t\t\tINSERCIÓ DE TELÈFONS A L'AGENDA\n\n");
				printf("\n1\t nom,\n2\t cognom1,\n3\t cognom2,\n4\t correu,\n5\t direccio,\n6\t telefon casa,\n7\t telefon mobil,\n8\t telefon2,\n9\t altres ");
				lectura_dades_telefons(nom, cognom1, cognom2, correu, direccio, telcasa, telmob,tel2, altres);
//				printf("\n\nfalta fer el codi d'insert, per que encara no  llegeixo res de variables ... per tant s'ha de fer això encara ... però ja ho faré, per què ara no tinc temps ....");
			}
        conexion = PQsetdbLogin("localhost", // nombre del servidor, NULL agrra por defaul a localhost
                                NULL, // Puerto, NULL toma el que esta por default
                                NULL,//Opciones adicionales
                                NULL, // Fichero o terminal de la salida
                                "agenda",//Nombre de la base de datos
                                "postgres", // Nombre de usuario
                                "Datsxku1!"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                if(DEBUG) printf("\n Error en la conexió a la base de dades de l'agenda ! ");
//                return 1;
        }

        if (DEBUG) printf(" Conectat!!! \n");
	if (tria=='n'){
			debug_insert = fopen("insnotes.log.txt","w");
		        PQtrace(conexion, debug_insert); /* Aqui le asignamos al archivo la salida  */
	}
	else if(tria=='t'){ 
//			printf("falta fer el codi");
			debug_insert = fopen("instelefons.log.txt","w");
			PQtrace(conexion, debug_insert);
	}
	
//if (deb==1)	printf("sentecia = %s nextval('id_seq'), '%s', '%s', '%s', '%c');",INSERTN, data, lloc, motiu, caducitat);
	if(tria=='n') {
			if(DEBUG)printf("sentenci = %s nextval('id_seq'), '%ls', '%ls', '%ls', '%ls');", INSERTN, data, lloc, motiu, caducitat);

			sprintf(buffer,"%s nextval('id_seq'),\'%ls\',\'%ls\',\'%ls\',\'%ls\');",INSERTN,data,lloc,motiu,caducitat); //construcció de l'insert
			if (DEBUG)	printf("Buffer = %s",buffer);
		      }
	else  if(tria=='t'){ 
//			printf("falta fer codi d'aquesta part d'insertar telefons");
			sprintf(buffer,"%s nextval('id_seq'),\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\');",INSERTT,nom, cognom1,cognom2,correu,direccio,telcasa,telmob,tel2,altres);
			if (DEBUG) printf("Buffer telefons = %s",buffer);
			}
	resultado = PQexec(conexion,buffer);
//codi mogut, anava després de la construcció de l'insert de telefons, i just a sobre del sprintf(buffer,"%s",selectn);
	if (!resultado || PQresultStatus(resultado) != PGRES_COMMAND_OK) {
					if(DEBUG)printf("el comando INSERT de notes ha fallat\n");
					PQclear(resultado);
			               	PQfinish(conexion);
	}
	else {
		if(DEBUG)printf("he insertat a la base de dades !!!");
		resultado = PQexec(conexion,"COMMIT");
		if (!resultado || PQresultStatus(resultado) != PGRES_COMMAND_OK) {
					        		        if(DEBUG)printf("el comando BEGIN ha fallat\n");
							                PQclear(resultado);
							                PQfinish(conexion);
								        }
		else if(DEBUG)printf("\ncommit fet a la DB\n");
	}
	if(tria=='n') sprintf(buffer,"%s",SELECTN);
	else if(tria=='t') sprintf(buffer,"%s",SELECTT);
        resultado = PQexec(conexion,buffer);
        columnas = PQnfields(resultado); // Obtenemos el numero de columnas
        filas = PQntuples(resultado); //Obtener el numero de filas
	mostra_dades(filas,resultado,tria);
        PQclear(resultado);
        PQfinish(conexion);

//fi codi mogut

       
}
int allargada_identificador(wchar_t *identificador)
{
//copia
	const wchar_t *s;
	for (s = identificador; *s; ++s);
	return(s - identificador);
//fi copia

}
/*int comprova_identificador(wchar_t *identificador, int id_min, int id_max)
{

	int i;
	int bolea=0;
	//int longitud=atoi(identificador);
	
	int longitud=allargada_identificador(identificador);

	int allargada;//=atoi(identificador);// = allargada_identificador(identificador);
	char llarg;
	//sprintf
	//memcpy(llarg,identificador,longitud);
	allargada=wcstol(identificador,NULL, allargada);
	printf("llarg char = %c",llarg);
	if (DEBUG==1){
			printf("allargada=atoi(identificador) = %d",allargada);//printf("longitud =%d",longitud);
			printf("longitud del número = %d", longitud);
			}
	//if (longitud>=id_min && longitud<=id_max) {
	if(allargada>id_min && allargada<=id_max){
					printf("\n\nvalor correcte\n");
					//return 1;
					//bolea=atoi(identificador);
	}
	else {
		printf("\n\nvalor fora de rang!!\n");
		//return 10;
		bolea=0;
	}
 return allargada;
}*/

int array_length(char a[]){ 
    return sizeof(a)/sizeof(char); 
} 

int llegir_identificador(wchar_t *identificador, int id_min, int id_max, int id)
{
	//int i;
	int bolea=0;
	//int deb=1; //deb = 0 no debug, deb = 1 debug
	long int max=8;

	char buffer[4],allargada;
	int ret;
	int conv[5];

	while(bolea==0  ){
		//fflush(stdin);
		printf("\n\nentra BÉ l'id que vols borrar ");
		fgetws(identificador,32,stdin);
		treu_intro(identificador);	
		ret = wcstombs ( buffer, identificador, sizeof(buffer) );
		//max=strtol(max, identificador,32);
		printf("max = %ld",max);
		if (ret==2) buffer[3]='\0';
		if (ret){
			// printf ("multibyte string: %s \n allargada = %d",buffer,array_length(buffer));
			bolea=1;
		}
		else {
			//printf("això no funciona?");
			bolea=0;
			}
		if (DEBUG)printf("bolea == %d, buffer = %s",bolea,buffer);
		
	}
	int valor=atoi(buffer);
	printf("valor %d ¿?¿¿?¿?",valor);
	return valor;


}
int menub(char tria) //menu de borrat 3
	{
        FILE *debug_delete;
        PGconn *conexion;
        PGresult   *resultado, * resultat2;

	//int deb=1; //deb = 0 no debug, deb = 1 debug
        int columnas=0;
        int filas=0;
        int i =0;
        int j=0;
        int valor=0;
	int id;
	char buffer[1024];
	char data[MAXD];
	char lloc[MAXLL];
	char motiu[MAXM];
	char caducitat;
	wchar_t identificador[4];
	int bolea=0;
	int id_min;
	int id_max;

if (tria=='n')	printf("\n\t\t\tELIMINACIÓ DE CITES EN L'AGENDA");
	else  if(tria=='t')   printf("\n\t\t\tELIMINACIÓ DE TELEFÒNS I DADES PERSONALS A L'AGENDA\n");


        conexion = PQsetdbLogin("localhost", // nombre del servidor, NULL agrra por defaul a localhost
                                NULL, // Puerto, NULL toma el que esta por default
                                NULL,//Opciones adicionales
                                NULL, // Fichero o terminal de la salida
                                "agenda",//Nombre de la base de datos
                                "postgres", // Nombre de usuario
                                "Datsxku1!"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
//                return 1;
        }
//select per borrar
if (tria=='n'){
		printf("buffer = %s, selectn = %s", buffer, SELECTN); 
		sprintf(buffer,"%s",SELECTN);

	}
	else if(tria=='t') {
				printf("buffer = %s, select = %s", buffer, SELECTT);
				sprintf(buffer,"%s",SELECTT);
	}
	resultado = PQexec(conexion,buffer);
        columnas = PQnfields(resultado); // Obtenemos el numero de columnas
        filas = PQntuples(resultado); //Obtener el numero de filas
if (tria=='n') {
		printf("\n\nDADES QUE ES PODEN BORRAR\n\n");
		for(i=0; i<filas; i++) {
			printf("\nIdentificador del camp %d : %s",i,(int*)PQgetvalue(resultado,i,0));
			printf("\ndata %d: %s",i, (char*)PQgetvalue(resultado,i,1));
			posa_cometa(PQgetvalue(resultado,i,2));
      			printf("\nlloc %d: %s",i, (char*)PQgetvalue(resultado,i,2));	//modificar pk no surtin l'escape de cometes
			posa_cometa(PQgetvalue(resultado,i,3));
                	printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,3));	//modificar pk no surtin l'escape de cometes
			printf("\ncaducitat %d: %s",i, (char*)PQgetvalue(resultado,i,4));
			printf("\n**********************************\n");
			paginacio(i);
			if (i==0) {
					id_min=atoi(PQgetvalue(resultado,i,0));
					if (DEBUG)printf("id_min =%d i=0",id_min);
				}
			if (i==filas-1){
				 id_max=atoi(PQgetvalue(resultado,i,0));
				if (DEBUG)printf("id_max = %d i=filas-1",id_max);
			}
        	}
		PQclear(resultado);
	        PQfinish(conexion);
}
else if (tria=='t') {
	printf("\n\nDADES QUE ES PODEN BORRAR\n\n");
	for(i=0; i<filas; i++) {
		printf("\nIdentificador del camp %d : %s",i,(int*)PQgetvalue(resultado,i,0));
		printf("\ndata %d: %s",i, (char*)PQgetvalue(resultado,i,1));
		posa_cometa(PQgetvalue(resultado,i,2));
                printf("\nlloc %d: %s",i, (char*)PQgetvalue(resultado,i,2));	//modificar pk no surtin l'escape de cometes
		posa_cometa(PQgetvalue(resultado,i,3));
                printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,3));	//modificar pk no surtin l'escape de cometes
		printf("\ncaducitat %d: %s",i, (char*)PQgetvalue(resultado,i,4));
			posa_cometa(PQgetvalue(resultado,i,5));
                printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,5));
		posa_cometa(PQgetvalue(resultado,i,6));
                printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,6));
		posa_cometa(PQgetvalue(resultado,i,7));
                printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,7));
		posa_cometa(PQgetvalue(resultado,i,8));
                printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,8));
		posa_cometa(PQgetvalue(resultado,i,9));
                printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,9));
		//posa_cometa(PQgetvalue(resultado,i,10));
	        //        printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,10));
               	//posa_cometa(PQgetvalue(resultado,i,10));
                //printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,3));printf("\n**********************************\n");
		paginacio(i);
		if (i==0) {
				id_min=atoi(PQgetvalue(resultado,i,0));
				if (DEBUG)printf("id_min =%d",id_min);
			}
		if (i==filas-1){
				 id_max=atoi(PQgetvalue(resultado,i,0));
				if (DEBUG)printf("id_max = %d",id_max);
			}
        }
	PQclear(resultado);
//        PQfinish(conexion);
}
//else printf("falla al mostrar les dades per borrarles ?");

//		fflush(stdin);
	//codi borrat lectura dades
	//printf("\nEscriu l'identificador de camp que vulguis borrar :");
if (tria=='n'){ 
		while (bolea==0){
			printf("\nEscriu l'identificador de camp que vulguis borrar :");
			fflush(stdin);
			bolea=llegir_identificador(identificador, id_min, id_max,id ); //No guardo bé el que retorno ... mirar a veure per què falla ...
		}
		if(DEBUG)printf("bbbbolea = %ld",bolea);
	}
else if(tria=='t') {
		while (bolea==0){
			printf("Escriu l'id del telefon que vols borrar\"falta implementar tot el codi\"");
			fflush(stdin);
			bolea=llegir_identificador(identificador,id_min, id_max,id);//No guardo bé el que retorno ... mirar a veure per què falla ...
			}
		}
	//fi codi borrat lectura dades

	
//fi select per borrar
	if(DEBUG)printf("Identificador borrar abans debug de connectat = %d", bolea);
	id=bolea;//atoi((char*)identificador);
	fflush(stdin);
	if(DEBUG)printf("\n\n\tid = %d", id);
        if (DEBUG){
		 printf(" Conectat!!! \n");
		printf("\n Valor identificador = %d\n ",id);//identificador);
	}
	if (tria=='n')debug_delete = fopen("deleten.log.txt","w");
	else if(tria=='t')debug_delete= fopen("deletet.log.txt","w");
        PQtrace(conexion, debug_delete); /* Aqui le asignamos al archivo la salida  */
	
	//id=atoi(identificador);
	//printf("Id després de triar el fitxer de log = %d, identificador =", id,identificador);
	if (tria=='n'){
			if(DEBUG)printf("\nborrar = %s'%d'",DELETEN,id);
			sprintf(buffer,"%s%d",DELETEN,id); //construcció 
			if(DEBUG)printf("\nSentència de borrar = %s\n", buffer);
		}
	else if(tria=='t') 	{
	   			if(DEBUG)printf("\n borrar = %s'%d'", DELETET,id);
				sprintf(buffer,"%s%d",DELETET,id);
				if(DEBUG)printf("\nSentència de borrar %s\n",buffer);
				}
		
if (DEBUG)	printf("\n\n\n\t\tBuffer fora bucles = %s",buffer);
//if (tria=='n') 
PQexec(conexion,buffer);
printf("registre borrat ???? comprobar-ho!!");
//	e1±lse if(tria=='t') printf("eliminar això per l'execució del delete de telefons");

/*        if (!resultado || PQresultStatus(resultado) != PGRES_COMMAND_OK) {
               if (tria=='n')
			{ 
				if(DEBUG) printf("la comanda DELETE de notes ha fallat\n");
				//PQclear(resultado);
		                //PQfinish(conexion);
			}
			else if(tria=='t'){
					if(DEBUG)printf("la comanda DELETE de telefons ha fallat\n");
					//PQclear(resultado);
					//PQfinish(conexion);
				}
        }
	else{
		printf("he borrat de la base de dades !!!");
		/*char *kk;
		char a;
		fflush(stdin);
		scanf("%c", &a);
		fgets(kk,1024, stdin);
		fflush(stdin);*/
//	}

/*if (tria=='n' || tria=='t')	//s'ha de borrar un cop fet el codi de borrar i existeix la taula de telefons i demés ...
	{ 
	PQclear(resultado);
        PQfinish(conexion);
	}*/
}

int menuc(char tria) //menu de consulta 2
	{
        FILE *debugselect;
        PGconn *conexion;
        PGresult   *resultado ;

	//int deb=1; //deb = 0 no debug, deb = 1 debug
        int columnas=0;
        int filas=0;
        int i =0;
        int j=0;
        int valor=0;
	int id;
	char buffer[1024];

	if (tria =='n')	printf("\n\t\t\tCONSULTES DE CITES EN L'AGENDA\n");
	else  if(tria=='t')   printf("\n\t\t\tCONSULTES DE TELEFÒNS I DADES PERSONALS A L'AGENDA\n");
        conexion = PQsetdbLogin("localhost", // nombre del servidor, NULL agrra por defaul a localhost
                                NULL, // Puerto, NULL toma el que esta por default
                                NULL,//Opciones adicionales
                                NULL, // Fichero o terminal de la salida
                                "agenda",//Nombre de la base de datos
                                "postgres", // Nombre de usuario
                                "Datsxku1!"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
        }

        if (DEBUG){ 	
			printf("\n Conectat!!! \n");
		        if (tria=='n')debugselect = fopen("selectcites.log.txt","w");
			else  {
				if (tria=='t') 	debugselect = fopen("selecttelefons.log.txt","w");
				else printf("això falla !!");
			}
		        PQtrace(conexion, debugselect); /* asigna El fitxer de sortida de la select */
		}

	if (tria=='n')	sprintf(buffer,"%s",SELECTN);
	else if(tria=='t') sprintf(buffer,"%s",SELECTT);
	if (DEBUG) printf("select = %s",buffer);
	resultado = PQexec(conexion,buffer);
        columnas = PQnfields(resultado); // Obtenemos el numero de columnas
        filas = PQntuples(resultado); //Obtener el numero de filas
	mostra_dades(filas,resultado, tria);
	if (DEBUG) printf("\nselect fet a la DB\n");
        PQclear(resultado);
        PQfinish(conexion);
}

int dades(char tria)
{
		wchar_t cadena [100];
		int opcio;
		cadena[0]='a';
	
			while (cadena[0]!='4')// && cadena[0]!='q')
				{
				if (tria=='n')  menunotes("notes");
				else if(tria=='t') menunotes("telefons");
					cadena[0]=' ';
					  printf ("Introdueix una opció: ");
					  fflush(stdin);
					  fgetws (cadena, 100, stdin);
					  treu_intro(cadena);
					  if (DEBUG){
						  printf ("L'opció escollida és: '%s'\n", cadena);
						  opcio=(int)cadena[0];
						  printf("\nHas escollit l'opcio %d \n",opcio);
						}
		
				  	  switch(opcio)
							{
							case 49: if (tria=='n') { 
										printf("\nInserció   de NOTES\n");
										menui('n');
										break;
								} else if(tria=='t'){
										printf("\n Inserció de TELÈFONS\n");
										menui('t');
										break;
									}
							case 50: if (tria=='n') { 
										printf("\nConsulta  de NOTES\n");
										menuc('n');
										break;
								}else if(tria=='t'){
										printf("\nConsulta de TELEFONS\n");
										menuc('t');
										break;
									}
							case 51:  if (tria=='n') { 
										printf("\nEliminació de NOTES\n");
										menub('n');
										break;
								}else if(tria=='t'){
										printf("\nEliminació de TELÈFONS\n");
										menub('t');
										break;
									}
							case 52:  if (tria=='n') {
//										printf("\nTornar al menú principal\n");
										break;
								}else if(tria=='t'){
//										printf("\nTornar al menú principal\n");
										break;
										}
							default : printf("\nno entra als casos, va a default!!!\n");break;
							}
					if (DEBUG)	printf("opcio = %d",opcio);
			}
  return 0;
}





void menup()
{
printf("\n1 <- notes a l'agenda\n");
printf("\n2 <- Telefons a l'agenda\n");
printf("\n3 <- Sortir de l'agenda\n");
}

int main ()
{
		wchar_t cadena [100];
		int opcio;
		cadena[0]='a';

			setlocale(LC_ALL, "");
	
			while (cadena[0]!='3')// && cadena[0]!='q')
				{
					  menup();
					  printf ("Introdueix una opció: ");
					  fflush(stdin);
					  fgetws (cadena, 100, stdin);
					  treu_intro(cadena);
					  if (DEBUG){
						  printf ("L'opció escollida és: '%s'\n", cadena);
						  opcio=(int)cadena[0];
						  printf("\nHas escollit l'opcio %d \n",opcio);
						}
		
				  	  switch(opcio)
							{
							case 49: printf("\nnotes de l'agenda\n");dades('n');cadena[0]='a';break;
							case 50: printf("\nTelèfons de l'agenda\n");dades('t');cadena[0]='a';break;
							case 51: printf("\nSortir d'agenda\n");break;
							default : printf("\nno entra als casos, va a default!!!\n");break;
							}
			//		if (DEBUG)	printf("opcio = %d",opcio);
			}
  return 0;
}
