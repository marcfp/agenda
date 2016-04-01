# agenda
writed using c and postgresql database version 1
########################################
For build :
########################################
make all
########################################
For remove:
########################################
make clean
########################################
Database Table :
########################################
notes with:
#######################################
id_notes, integer,NOT NULL,     pk
data, date, NOT NULL,
lloc, text,
motiu, text
caducitat, character(1)
#######################################
telefons with:
#######################################
id_telefons, integer, NOT NULL , pk
nom, character, varying(256),
cognom1, character, varying(256)
cognom2, character, varying(256)
correu, character, varying(512)
direccio, character, varying(512)
tlf_casa, numeric(9,0),
tlf_mobil, numeric(9,0)
tlf_mobil_2, numeric(9,0)
altres, text
#######################################
