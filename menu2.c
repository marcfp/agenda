//aquest software està sota llicència CC BY SA 2.0
#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>

#define debug 1 //0 no debug, 1 debug

#define maxd  12  	//maxim buffer data
#define maxll  256 	//maxim buffer lloc
#define maxm 1024 	//maxim buffer motiu de cites i altres de telefons
#define maxc 1 		//maxim buffer caducitat
#define maxtelefon 11//telefon de 9 digits + intro
#define maxlletres 60	//noms maxims de 60 lletres
#define INSERTn "INSERT INTO notes VALUES( "
#define DELETEn "DELETE FROM notes where id="
#define SELECTn "SELECT * FROM notes"
#define SELECTt "SELECT * FROM telefons"
#define INSERTt "INSERT INTO telefons VALUES ( "
#define DELETEt "DELETE FROM telefons where id="
void mostra_telefon(char *cadena)
{
	int longitud,i;
	printf("entro a mostra_telefon");
	longitud=strlen(cadena);
	for (i=0;i<longitud;i++) printf("%c",cadena[i]);
}
void treu_intro (char *cadena)
{
		int longitud;
	
		longitud=strlen(cadena);
		if (cadena[longitud-1]='\n') cadena[longitud-1]='\0';
}
void escapa_cometes (char *cadena)
{
		int longitud;
		int i;
		longitud=strlen(cadena);
		for (i=0;i<longitud;i++){
			 printf("posicio %d -> lletra %c \n",i,cadena[i]);
			 if(cadena[i]=='\'') cadena[i]='^' ;
		}

}
int llegir_data(char *data) 				//hauria de forçar a que quan trobes un error, no mires més ... com ho faig ???
{
int longitud;
int i;
int valid=99;
int bucle;
int valor=0;
while(valor==0 ){
	printf("\n Entra la data en format AAAA-MM-DD\t:");
	fgets(data,maxd,stdin);
	treu_intro(data);
	valor=valida_data(data);
	}
}
int valida_data(char *data)
{
	int longitud=strlen(data);
	int i;
	int bolea=1;
	int deb=1; //deb = 0 no debug, deb = 1 debug

	for (i=0;i<longitud;i++){ 
	if (deb) printf("\nposicio %d = caracter %c",i,data[i]);
		if((data[i]<'0' || data[i]>'9' ) && bolea==1) {
				if(i!=4 && i!=7){ 
						printf("\nAl lloc %d s'ha entrat %c i no és vàlid!!.\n\t Torna a entrar la data, gràcies\n",i,data[i]);
						bolea=0;
				}
				else {
					if(data[i]!='-' ){
						 printf("\nLa separació posada no és CORRECTA, s'ha de posar \"-\".\nTorna a entrar la data, gràcies\n");
							bolea=0;
							}
				}
		}
		else if (i==9 && bolea==1){
			if (deb)printf("\tdata correcte \t\tuououououo!!");
//			bolea=1;
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
int valida_correu(char *camp)
{

	int i=0;
	const char *s;
	for (s =camp ; *s; ++s);
	if(s-camp<9){
			if (debug){
					printf("falten lletres?");
					printf("%d",s-camp);
					printf("error d'allargada");
			}
			return 2;
	}
	else {
			if(debug){
					printf("ben entrat?");
					printf("%d",s-camp);
			}
			return 0;	
	}
	/*
	const char *s;
	 for (s = str; *s; ++s);
	 return(s - str);
*/
}
//	}
int valida_text(char *camp)
{
	int i=0;
	while(camp[i]!='\n' && i<=maxlletres) {
	if((camp[i]<'a' || camp[i]>'z') && (camp[i]<'A' || camp[i]>'Z') && camp[i]!=' ') {
			printf("Només s'accepten lletres!");
			return 2;
		}
	else {
		i=i+1;
		}
	if (camp[i]=='\n') return 0;
	}
}
int valida_telefon(char *telefon)
{
	int i=0;
	while(telefon[i]!='\n' && i<=maxtelefon){
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

int lectura_dades(char *variable, char *text, int maxim, int numero_funcio) //variable, necessito passar text, maxim buffer i funció de validació
{
int i=1;

do{
fflush(stdin);
printf("%s",text);
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
	if(debug) printf("Entro a validar correu"); 
	i=valida_correu(text);
	treu_intro(text);
	if(debug)printf("Surto de validar correu");
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
void lectura_dades_telefons(char *nom, char *cognom1, char *cognom2, char *correu, char *direccio, char *telcasa, char *telmob, char *tel2, char *altres)
{
	int i=1;
	int j;
if (debug) printf("lectura_dades_telefons llegeix les dades de la gent que es vol entrar al programet ...");
	do{
		//llegir nom
		fflush(stdin);
		printf("\n Entra el nom de la persona que desitjes entrar a l'agenda personal\t: ");
		fgets(nom,maxlletres,stdin);
		i=valida_text(nom);
//		printf(" bolea= %d ", i);
	}while(i!=0);	
	treu_intro(nom);
if (debug) printf("Nom entrat = %s", nom);
	i=1;
	do{
		printf("\n Entra el primer cognom de %s\t: ",nom);
		fflush(stdin);
		fgets(cognom1,maxlletres,stdin);
		i=valida_text(cognom1);
//		printf("bolea  = %d",i);
	}while(i!=0);
	treu_intro(cognom1);
if (debug) printf("cognom entrat = %s",cognom1);
	i=1;
	do{
		printf("\n Entra el segon cognom de %s\t: ",nom);
		fflush(stdin);
		fgets(cognom2,maxlletres,stdin);
		i=valida_text(cognom2);
//		printf("bolea  = %d",i);
	}while(i!=0);
	treu_intro(cognom2);
if (debug) printf("cognom2 entrat = %s",cognom2);
	i=1;
	do{
		printf("\n Entra la direcció de correu de %s %s\t",nom,cognom1);
		fgets(correu,maxlletres,stdin);
		i=valida_correu(correu);
	}while (i!=0);
	treu_intro(correu);
if (debug) printf("correu entrat = %s",correu);
	i=1;
	do{
		printf("\n Entra la direcció de %s\t: ",nom);
		fflush(stdin);
		fgets(direccio,maxlletres,stdin);
		i=valida_text(direccio);
//		printf("bolea  = %d",i);
	}while(i!=0);
	treu_intro(direccio);
if (debug) printf("direcció entrada = %s",direccio);
	i=1;
	do {
		printf("\nEntra el telefon de casa de %s\t: ",nom);
		fflush(stdin);
		fgets(telcasa,maxtelefon,stdin);
		i=valida_telefon(telcasa);
	}while (i!=0);
	treu_intro(telcasa); //borra l'ultim nº entrat ? pk ?
if (debug) printf("telefon casa entrat = %s ",telcasa);
	i=1;
	do {
		printf("\nEntra el mòbil de %s\t: ",nom);
		fgets(telmob,maxtelefon,stdin);
		i=valida_telefon(telmob);
	}while (i!=0);
	treu_intro(telmob);
if (debug) printf("mobil entrat  = %s",telmob);
	i=1;
	do {
		printf("\nEntra el tercer Telefon de %s\t: ",nom);
		fgets(tel2,maxtelefon,stdin);
		i=valida_telefon(tel2);
	}while (i!=0);
	treu_intro(tel2);
if (debug) printf("tercer telefon entrat = %s",tel2);
	i=1;
//	do {
		printf("\nEntra notes curtes de %s %s\t: ",nom,cognom1);
		fgets(altres,maxtelefon,stdin);
//		i=valida_telefon(telmob);
//	}while (i!=0);
	treu_intro(altres);

if (debug) printf("notes curtes entrades = %s",altres);	

}

void lectura_dades_notes(char data[maxd], char lloc[maxll], char motiu[maxm])
{
	llegir_data(data);
	printf("\nEntra el lloc \t:");
	fgets(lloc,maxll,stdin);
	treu_intro(lloc);
	printf("Lloc entrat = %s",lloc);
	escapa_cometes(lloc);
	printf("lloc cometes = %s",lloc);
	printf("\nEntra el motiu de la cita \t:");
	fgets(motiu,maxm,stdin);
	treu_intro(motiu);
	printf("Motiu = %s",motiu);
	escapa_cometes(motiu);
	printf("Motiu cometes = %s",motiu);
	fflush(stdin);

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

	int deb=1; //deb = 0 no debug, deb = 1 debug
        int columnas=0;
        int filas=0;
        int i =0;
        int j=0;
        int valor=0;
	int id;
	char buffer[1024];
///inici notes
	char data[maxd];
	char lloc[maxll];
	char motiu[maxm];
	char caducitat;
	char *caduca;
///fi notes
///inici telefons
	char nom[maxlletres];
	char cognom1[maxlletres];
	char cognom2[maxlletres];
	char correu[maxlletres];
	char direccio[maxlletres];
	char telcasa[maxtelefon];
	char telmob[maxtelefon];
	char tel2[maxtelefon];
	char altres[maxm];
///fi telefons
	fflush(stdin);
	if (tria=='n')	{
				printf("\n\t\t\tINSERCIÓ DE CITES EN L'AGENDA");
				lectura_dades_notes(data,lloc,motiu);
				while (caducitat!='s' && caducitat!='S' && caducitat !='n' && caducitat !='N'){
				//while ((caduca!='s' || caduca!='S') && (caduca !='n' || caduca !='N')){
					printf("Entra la caducitat ('s'/'S'/'n'/'N') \t:");
					fflush(stdin);
					scanf("%c",&caducitat);
					//fgets(caduca,1,stdin);
					//treu_intro(caduca);
					printf("\nCaducitat entrada = %c\n",caducitat);
					//printf("\nCaducitat entrada = %c\n",caduca);
				}
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
                                "marc", // Nombre de usuario
                                "clases"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
//                return 1;
        }

        if (deb==1) printf(" Conectat!!! \n");
		if (tria=='n'){
				debug_insert = fopen("insnotes.log.txt","w");
			        PQtrace(conexion, debug_insert); /* Aqui le asignamos al archivo la salida  */
			}
		else if(tria=='t'){ 
//				printf("falta fer el codi");
				debug_insert = fopen("instelefons.log.txt","w");
				PQtrace(conexion, debug_insert);
			}
	
//if (deb==1)	printf("sentecia = %s nextval('id_seq'), '%s', '%s', '%s', '%c');",INSERTn, data, lloc, motiu, caducitat);
	if(tria=='n') {
			sprintf(buffer,"%s nextval('id_seq'),\'%s\',\'%s\',\'%s\',\'%c\');",INSERTn,data,lloc,motiu,caducitat); //construcció de l'insert
			if (deb==1)	printf("Buffer = %s",buffer);
		      }
	else  if(tria=='t'){ 
//			printf("falta fer codi d'aquesta part d'insertar telefons");
			sprintf(buffer,"%s nextval('id_seq'),\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\');",INSERTt,nom, cognom1,cognom2,correu,direccio,telcasa,telmob,tel2,altres);
			if (deb ==1) printf("Buffer telefons = %s",buffer);
			}
	 resultado = PQexec(conexion,buffer);
//codi mogut, anava després de la construcció de l'insert de telefons, i just a sobre del sprintf(buffer,"%s",selectn);
				if (!resultado || PQresultStatus(resultado) != PGRES_COMMAND_OK) {
			        		        printf("el comando INSERT de notes ha fallat\n");
					                PQclear(resultado);
			                		PQfinish(conexion);
			        }
				else {
						printf("he insertat a la base de dades !!!");
						 resultado = PQexec(conexion,"COMMIT");
					        if (!resultado || PQresultStatus(resultado) != PGRES_COMMAND_OK) {
					        		        printf("el comando BEGIN ha fallat\n");
							                PQclear(resultado);
							                PQfinish(conexion);
								        }
						else printf("\ncommit fet a la DB\n");
	
				}
	if(tria=='n') sprintf(buffer,"%s",SELECTn);
	else if(tria=='t') sprintf(buffer,"%s",SELECTt);
        resultado = PQexec(conexion,buffer);
        columnas = PQnfields(resultado); // Obtenemos el numero de columnas
        filas = PQntuples(resultado); //Obtener el numero de filas
	mostra_dades(filas,resultado,tria);
        PQclear(resultado);
        PQfinish(conexion);

//fi codi mogut

       
}

int comprova_identificador(char *identificador, int id_min, int id_max)
{

	int i;
	int bolea=0;
	int longitud=atoi(identificador);
	int deb=1; //deb =0 no debug, deb =1 debug

//if (deb==1)printf("longitud =%d",longitud);
	if (longitud>=id_min && longitud<=id_max) {
					printf("\n\nvalor correcte\n");
					return 1;
				}
	else {
		printf("\n\nvalor fora de rang!!\n");
		return 0;
	}
}
int llegir_identificador(char *identificador, int id_min, int id_max)
{
	int i;
	int bolea=0;
	int deb=1; //deb = 0 no debug, deb = 1 debug
	int max=8;	
	while(bolea==0){
	printf("entra l'id que vols borrar ");
	fgets(identificador,max,stdin);
	bolea=comprova_identificador(identificador,id_min, id_max);
	if (deb==1)printf("bolea == %d",bolea);
	}
	


}
int menub(char tria) //menu de borrat 3
	{
        FILE *debug_delete;
        PGconn *conexion;
        PGresult   *resultado, * resultat2;

	int deb=1; //deb = 0 no debug, deb = 1 debug
        int columnas=0;
        int filas=0;
        int i =0;
        int j=0;
        int valor=0;
	int id;
	char buffer[1024];
	char data[maxd];
	char lloc[maxll];
	char motiu[maxm];
	char caducitat;
	char identificador[8];
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
                                "clases"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
//                return 1;
        }
//select per borrar
if (tria=='n')	sprintf(buffer,"%s",SELECTn);
	else if(tria=='t') sprintf(buffer,"%s",SELECTt);
	resultado = PQexec(conexion,buffer);
        columnas = PQnfields(resultado); // Obtenemos el numero de columnas
        filas = PQntuples(resultado); //Obtener el numero de filas
if (tria=='n') for(i=0; i<filas; i++) {
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
				if (deb==0)printf("id_min =%d",id_min);
			}
		if (i==filas-1){
				 id_max=atoi(PQgetvalue(resultado,i,0));
				if (deb==0)printf("id_max = %d",id_max);
			}
        }
else if (tria=='t') {
	printf("mostrar dades de la taula telefons");
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
				if (deb==0)printf("id_min =%d",id_min);
			}
		if (i==filas-1){
				 id_max=atoi(PQgetvalue(resultado,i,0));
				if (deb==0)printf("id_max = %d",id_max);
			}
        }

}
else printf("falla al mostrar les dades per borrarles ?");

//		fflush(stdin);
	//codi borrat lectura dades
	printf("Escriu l'identificador de camp que vulguis borrar :");
if (tria=='n')while (bolea==0){
	bolea=llegir_identificador(identificador, id_min, id_max);
	}
else if(tria=='t') {
			 printf("Escriu l'id del telefon que vols borrar\"falta implementar tot el codi\"");
			while (bolea==0){
			 bolea=llegir_identificador(identificador,id_min, id_max);
			}
		}
	//fi codi borrat lectura dades

	
//fi select per borrar
        if (deb){ printf(" Conectat!!! \n");
		if (tria=='n')debug_delete = fopen("deleten.log.txt","w");
		else if(tria=='t')debug_delete= fopen("deletet.log.txt","w");
        PQtrace(conexion, debug_delete); /* Aqui le asignamos al archivo la salida  */
	}
	id=atoi(identificador);
	if (tria=='n')sprintf(buffer,"%s'%d'",DELETEn,id); //construcció 
		else if(tria=='t') 	{
					  sprintf(buffer,"%s'%d'",DELETEt,id);
					  printf("construcció de la sentencia %s que no esta feta",buffer);

					}
if (deb==1)	printf("Buffer = %s",buffer);
//if (tria=='n') 
resultado = PQexec(conexion,buffer);
//	else if(tria=='t') printf("eliminar això per l'execució del delete de telefons");

        if (!resultado || PQresultStatus(resultado) != PGRES_COMMAND_OK) {
               if (tria=='n')
			{ 
				 printf("la comanda DELETE de notes ha fallat\n");
				PQclear(resultado);
		                PQfinish(conexion);
			}
			else if(tria=='t')printf("la comanda DELETE de telefons no esta construida\n");
                        }
	else{
		printf("he borrat de la base de dades !!!");
	}

if (tria=='n')	//s'ha de borrar un cop fet el codi de borrar i existeix la taula de telefons i demés ...
	{ 
	PQclear(resultado);
        PQfinish(conexion);
	}
}

int menuc(char tria) //menu de consulta 2
	{
        FILE *debugselect;
        PGconn *conexion;
        PGresult   *resultado ;

	int deb=1; //deb = 0 no debug, deb = 1 debug
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
                                "clases"); // PASSWORD del usuario
        if (PQstatus(conexion) == CONNECTION_BAD) {
                printf("\n Error en la conexió a la base de dades de l'agenda ! ");
        }

        if (deb==1){ 	
			printf("\n Conectat!!! \n");
		        if (tria=='n')debugselect = fopen("selectcites.log.txt","w");
			else  {
				if (tria=='t') 	debugselect = fopen("selecttelefons.log.txt","w");
				else printf("això falla !!");
			}
		        PQtrace(conexion, debugselect); /* asigna El fitxer de sortida de la select */
		}

	if (tria=='n')	sprintf(buffer,"%s",SELECTn);
	else if(tria=='t') sprintf(buffer,"%s",SELECTt);
	if (deb==1) printf("select = %s",buffer);
	resultado = PQexec(conexion,buffer);
        columnas = PQnfields(resultado); // Obtenemos el numero de columnas
        filas = PQntuples(resultado); //Obtener el numero de filas
	mostra_dades(filas,resultado, tria);
	if (deb) printf("\nselect fet a la DB\n");
        PQclear(resultado);
        PQfinish(conexion);
}

int dades(char tria)
{
		char cadena [100];
		int opcio;
		cadena[0]='a';
	
			while (cadena[0]!='4')// && cadena[0]!='q')
				{
				if (tria=='n')  menunotes("notes");
				else if(tria=='t') menunotes("telefons");
					  printf ("Introdueix una opció: ");
					  fflush(stdin);
					  fgets (cadena, 100, stdin);
					  treu_intro(cadena);
					  if (debug){
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
					if (debug)	printf("opcio = %d",opcio);
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
		char cadena [100];
		int opcio;
		cadena[0]='a';
	
			while (cadena[0]!='3')// && cadena[0]!='q')
				{
					  menup();
					  printf ("Introdueix una opció: ");
					  fflush(stdin);
					  fgets (cadena, 100, stdin);
					  treu_intro(cadena);
					  if (debug){
						  printf ("L'opció escollida és: '%s'\n", cadena);
						  opcio=(int)cadena[0];
						  printf("\nHas escollit l'opcio %d \n",opcio);
						}
		
				  	  switch(opcio)
							{
							case 49: printf("\nnotes de l'agenda\n");dades('n');break;
							case 50: printf("\nTelèfons de l'agenda\n");dades('t');break;
							case 51: printf("\nSortir d'agenda\n");break;
							default : printf("\nno entra als casos, va a default!!!\n");break;
							}
			//		if (debug)	printf("opcio = %d",opcio);
			}
  return 0;
}
