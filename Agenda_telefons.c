//aquest software està sota llicència CC BY SA 2.0

//https://www.enlightenment.org/tutorial/form_tutorial <- mirar aquí, per veure com puc generar el naviframe del select a la base de dades guardant posició i offset
//això n'és un altre exemple : 
//https://www.enlightenment.org/tutorial/naviframe_tutorial
//i això que no sé exactament què és : 
//https://docs.enlightenment.org/stable/elementary/entry_example.html
#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <math.h>
#include "headers/funcions.h"



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

int lectura_dades(char *variable, wchar_t *text, int maxim, int numero_funcio) //variable, necessito passar text, maxim buffer i funció de validació
{
	int i=1	;

	do{
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
	}while(i!=0);	
	treu_intro(nom);
if (DEBUG) printf("Nom entrat = %ls", nom);
	i=1;
	do{
		printf("\n Entra el primer cognom de %ls\t: ",nom);
		fflush(stdin);
		fgetws(cognom1,MAXLLETRES,stdin);
		i=valida_text(cognom1);
	}while(i!=0);
	treu_intro(cognom1);
if  (DEBUG) printf("Cognom1 entrat = %ls", cognom1);

	i=1;
	do{
		printf("\n Entra el segon cognom de %ls %ls\t: ",nom,cognom1);
		fflush(stdin);
		fgetws(cognom2,MAXLLETRES,stdin);
		i=valida_text(cognom2);
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
	/*i=1;
	do{
		printf("\n Entra la direcció de %ls %ls %ls\t: ",nom, cognom1, cognom2);
		fflush(stdin);
		fgetws(direccio,MAXLLETRES,stdin);
		i=valida_text(direccio);
	}while(i!=0);
	treu_intro(direccio);
if (DEBUG) printf("direcció entrada = %ls",direccio);*/
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
	printf("\nEntra notes curtes de %ls %ls %ls\t: ",nom,cognom1,cognom2);
	fgetws(altres,MAXTELEFON,stdin);
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
		if(DEBUG)printf("\t caducitat char = %c",caducitat);
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
			printf("\ntelefon casa  %d: %s",i, (char*)PQgetvalue(resultado,i,5));
			printf("\nmobil  %d: %s",i, (char*)PQgetvalue(resultado,i,6));
			printf("\naltres telefons  %d: %s",i, (char*)PQgetvalue(resultado,i,7));
			printf("\naltres dades  %d: %s",i, (char*)PQgetvalue(resultado,i,8));

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
			}
        conexion = PQsetdbLogin("localhost", // nombre del servidor, NULL agrra por defaul a localhost
                                NULL, // Puerto, NULL toma el que esta por default
                                NULL,//Opciones adicionales
                                NULL, // Fichero o terminal de la salida
                                "agenda",//Nombre de la base de datos
                                "marc", // Nombre de usuario
                                "Datsxku1!"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                if(DEBUG) printf("\n Error en la conexió a la base de dades de l'agenda ! ");
        }

        if (DEBUG) printf(" Conectat!!! \n");
	if (tria=='n'){
			debug_insert = fopen("insnotes.log.txt","w");
		        PQtrace(conexion, debug_insert); /* Aqui le asignamos al archivo la salida  */
	}
	else if(tria=='t'){ 
			debug_insert = fopen("instelefons.log.txt","w");
			PQtrace(conexion, debug_insert);
	}
	
	if(tria=='n') {
			if(DEBUG)printf("sentencia = %s nextval('id_seq'), '%ls', '%ls', '%ls', '%ls');", INSERTN,  lloc, motiu, caducitat, data);

			sprintf(buffer,"%s nextval('id_seq'),\'%ls\',\'%ls\',\'%ls\',\'%ls\');",INSERTN,lloc,motiu,caducitat,data); //construcció de l'insert
			if (DEBUG)	printf("\nBuffer = %s \n",buffer);
		      }
	else  if(tria=='t'){
			if(DEBUG) printf("\n\nsentencia = %s nextval('id_seq_telefons'),'%ls', '%ls', '%ls', '%ls', '%ls', '%ls', '%ls', '%ls','%ls');",INSERTT,nom, cognom1,cognom2,correu,telcasa,telmob,tel2,altres);

			sprintf(buffer,"%s nextval('id_seq_telefons'),\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\',\'%ls\');",INSERTT,nom, cognom1,cognom2,correu,telcasa,telmob,tel2,altres);
			if (DEBUG) printf("\n\nBuffer telefons = %s",buffer);
			}
	resultado = PQexec(conexion,buffer);
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


       
}
int allargada_identificador(wchar_t *identificador)
{
	const wchar_t *s;
	for (s = identificador; *s; ++s);
	return(s - identificador);
//fi copia

}

int array_length(char a[]){ 
    return sizeof(a)/sizeof(char); 
} 

int llegir_identificador(wchar_t *identificador, int id_min, int id_max, int id)
{
	int bolea=0;
	long int max=8;

	char buffer[4],allargada;
	int ret;
	int conv[5];

	while(bolea==0  ){
		printf("\n\nentra BÉ l'id que vols borrar ");
		fgetws(identificador,32,stdin);
		treu_intro(identificador);	
		ret = wcstombs ( buffer, identificador, sizeof(buffer) );
		printf("max = %ld",max);
		if (ret==2) buffer[3]='\0';
		if (ret){
			bolea=1;
		}
		else {
			if(DEBUG)printf("això no funciona?");
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
        PGresult   *resultado, *resultat2;

        int columnas=0;
        int filas=0;
        int i =0;
        int j=0;
        int valor=0;
	int id;
	char buffer[254];
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
                                "marc", // Nombre de usuario
                                "Datsxku1!"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
//                return 1;
        }
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
//		posa_cometa(PQgetvalue(resultado,i,9));
  //              printf("\nmotiu %d: %s",i, (char*)PQgetvalue(resultado,i,9));
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
}
if (tria=='n'){ 
		while (bolea==0){
			//printf("\nEscriu l'identificador de camp que vulguis borrar :");
			fflush(stdin);
			bolea=llegir_identificador(identificador, id_min, id_max,id ); //No guardo bé el que retorno ... mirar a veure per què falla ...
		}
		if(DEBUG)printf("\nbolea = %ld",bolea);
	}
else if(tria=='t') {
		while (bolea==0){
			fflush(stdin);
			bolea=llegir_identificador(identificador,id_min, id_max,id);//No guardo bé el que retorno ... mirar a veure per què falla ...
			}
		}
	if(DEBUG)printf("\n\nIdentificador borrar abans debug de connectat = %d", bolea);
	id=bolea;//atoi((char*)identificador);
	fflush(stdin);
	if(DEBUG)printf("\n\n\tid = %d", id);
        if (DEBUG){
		 printf("\n Conectat!!! \n");
		printf("\n Valor identificador = %d\n ",id);//identificador);
	}
	if (tria=='n'){	
			debug_delete = fopen("deleten.log.txt","w");
			}
	else if(tria=='t') {
			debug_delete= fopen("deletet.log.txt","w");
			}
        PQtrace(conexion, debug_delete); /* Aqui le asignamos al archivo la salida  */
	
	if (tria=='n'){
			if(DEBUG)printf("arribo notes");
			if(DEBUG)printf("\nborrar = %s%d;",DELETEN,id);
			sprintf(buffer,"%s%d;",DELETEN,id); //reventa aquí, fa un bucle molt gran ... com delimitar això ?
			if(DEBUG)printf("\nSentència de borrar = %s\n", buffer);
		}
	else if(tria=='t') 	{
				if(DEBUG)printf("arribo telefons");
	   			if(DEBUG)printf("\n borrar = %s\'%d\';", DELETET,id);
				sprintf(buffer,"%s\'%d\';",DELETET,id);//reventa aquí, fa un bucle molt gran ..., com delimitar això ?
				if(DEBUG)printf("\nSentència de borrar %s\n",buffer);
				}
		
if (DEBUG)	printf("\n\n\n\t\tBuffer fora bucles = %s",buffer);
//if (tria=='n') 
//copia
        conexion = PQsetdbLogin("localhost", // nombre del servidor, NULL agrra por defaul a localhost
                                NULL, // Puerto, NULL toma el que esta por default
                                NULL,//Opciones adicionales
                                NULL, // Fichero o terminal de la salida
                                "agenda",//Nombre de la base de datos
                                "marc", // Nombre de usuario
                                "Datsxku1!"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
//                return 1;
        }

//fi copia
	PQexec(conexion,buffer);
	if(DEBUG){
		printf("registre borrat ???? comprobar-ho!!");
		printf("\n\nbuffer = %s",buffer);
	}
        PQfinish(conexion);
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
                                "marc", // Nombre de usuario
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
										break;
								}else if(tria=='t'){
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
			}
  return 0;
}
