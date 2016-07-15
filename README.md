[![Build Status](https://travis-ci.org/Xevib/agenda.svg?branch=master)](https://travis-ci.org/Xevib/agenda)



This program uses Enlightenment Foundation Libraries.
For use it, you must have installed these packages :
apt-get install libecore-evas1 libevas-dev libevas-loaders libevas1 libevas1-engines-x libecore-con1 libecore-dev libecore-evas1 libecore-fb1 libecore-file1 libecore-imf1 libecore-input1 libecore-ipc1 libecore-x1 libecore1 libedbus1 libedje-bin libedje1 libeet1 libeeze1 libefreet-bin libefreet1 libefreet1a  libeina1 libeio1 libevas1 libevas1-engine-software-x11 libpam0g libxcb-keysyms1 libxcb-shape0 libxcb1  
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
Added graphical interface using Enlightenment Foundation Libraries.
for build it :
type  "make grafic" into term.

For install EFL in 
- debian :
https://packages.debian.org/jessie/e17
- Ubuntu :
https://help.ubuntu.com/community/Enlightenment
- freeBSD :
https://phab.enlightenment.org/w/installation_freebsd/
- openBSD :
https://phab.enlightenment.org/w/openbsd/
- etc ...

