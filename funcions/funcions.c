#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include "funcions.h"  /* Include the header (not strictly necessary here) */


#define INSERTT "INSERT INTO telefons (id_telefons, nom_telefons, cognom1_telefons, cognom2_telefons, correu_telefons, telefon_casa, telefon_mobil, telefon_2_mobil, altres_telefons) VALUES (nextval('id_seq_telefons'::regclass),'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') ;"

int foo(int x)    /* Function definition */
{
    return x + 5;
}





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


int comproba_mail(char *camp)
{
	int arroba;
	int punt;
	int pos=-1;
	int i=0;
	int existeix=-1;
	int allargada = strlen(camp);
	int trobat=0;
	int punt_trobat=0;
	int pos_arroba=0;
	int mail=-1;
	arroba=64;
	punt=46;

	for (i=0;i<allargada;i++)
		{
			printf("\ncamp[%d]=%c", i, camp);
			if(camp[i]==arroba && i >1) { 
							trobat=1;
							pos_arroba=i;
							mail=0;
							}
			if(camp[i]==punt && i>=pos_arroba+2 ) {
							printf("\n\npunt trobat a %d = %c", i, camp[i]);
							punt_trobat=i;
							mail=1;
							}
		}

	printf("\t\ttorbat = %d, pos_arroba=%d, punt_trobat=%d",trobat,pos_arroba,punt_trobat);

	if (trobat!=0)	{
//	if (trobat==1) 
		printf("\nConté arroba");
		if(punt_trobat!=0){
				printf("\nAQUEST CORREU CONTÉ ARROBA I PUNT BEN POSICIONAT");
				return(1);
				}
		else {
			printf(" no conté punt ben posicionat?");
			return(0);
			}
	}
	else printf("\nNo la conté!!!!!");
	printf("\ncamp =%s \n comproba mail",camp);
	return (0);
}
int comproba_ultim(char txt[])
{
	printf("\n\n\n txt ultim telefon = %s allargada =%d\n\n\n",txt, strlen(txt));
	int bo=0;
	int i,j, lletra=0;
	char *str2 = "Altres telèfons";
	j = strncmp(txt, str2, 15);
	printf("\ntercer telefon bo = %d\n i = %d",bo, i);
	for (i=0; i<strlen(txt);i++){
		if(txt[i]<'0' || txt[i]>'9'){
			bo++;
			lletra=1;
		}
	}

	if ((j!=0 || bo!=strlen(txt)) && lletra==0) {
		printf("alguna cosa diferent hi ha");
		return(0);
	}
	else{
		printf("mostrar missatge que revisi telefons");
		return (1);
	}
}

int comproba_telefons(char txt[])
{
	int num_error=0;
	int i;
	int maxim=strlen(txt);
	int valor=0;
	if (maxim <9 || maxim> 9) valor=1;
	else{
		for(i=0;i<maxim;i++){
			if(txt[i]<'0' || txt[i]>'9') valor=1;
		}
	}
	if(DEBUG==1)printf("\nvalor = %d", valor);
	return valor;

}

