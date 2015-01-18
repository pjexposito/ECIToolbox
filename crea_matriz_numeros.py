#!/usr/bin/python
# -*- coding: utf-8 -*-

#Código sucio y tal, pero es mi primer programa en Python :D

import sqlite3 as lite
import sys
import calendar
import urllib
import os
from os.path import expanduser


print("Iniciando script...")
if os.path.isfile("ShiftCal.db"):
    print("Eliminando versiones anteriores")
    os.remove("ShiftCal.db")
print ("Descargando base de datos desde Dropbox...")
basededatos = urllib.URLopener()
basededatos.retrieve("https://dl.dropboxusercontent.com/u/119376/ShiftCal.db", "ShiftCal.db")
print ("Base de datos descargada. Creando archivo...")
con = lite.connect('ShiftCal.db')
cadena = ""
calendarios = 0

with con:
    
    cur = con.cursor()
    # Se seleccionan dos tablas (calendar y shifts), se unen y se ordenan por fecha
    cur.execute("SELECT * FROM calendar,shifts WHERE calendar.cal_shift_id1 = shifts.shift_rowid ORDER BY calendar.cal_date")
    #cur.execute("SELECT * FROM calendar")
    
    rows = cur.fetchall()
    ano = ""
    mes = ""
    # Se recorre toda la tabla
    for row in rows:
        este_mes = row[1][4:6]
        # Se guarda el año, el día y el turno también en una variable
        este_ano = row[1][:4]
        este_dia = row[1][6:8]
        este_turno = row[10]
		# Ojo, el valor anterior era 19, no 10. De esta forma se conseguía el turno con una o dos letras y no números
        # Se discrimina el año, para saber si ya se ha usado o no
        if ano == este_ano:
            # De nuevo, se discrimina por meses
            if mes == este_mes:
                cadena = cadena + str(este_turno)
                # Se suma uno a la variable salto_ahora. Si es 7, se mete un salto de carro, para que se lea mejor la array
            else:
                mes = este_mes
                # Con esto, se termina el turno. Por eso, se añade 1 a la variable calendario. Se cierra ese turno y se abre otro
                calendarios = calendarios+1
                #[:len(cadena)-2]
                cadena = cadena + "\", \n" + "\"data" + str(calendarios-1) + "\": \"" + str(calendar.monthrange(int(este_ano),int(este_mes))[1]) +ano + mes + str(este_turno)
        else:
            ano = este_ano
            mes = este_mes
            # De nuevo, como en el punto anterior, se cierra el turno y se añade 1 a calendario
            calendarios = calendarios+1
            #[:len(cadena)-2]
            cadena = cadena + "\", \n" + "\"data" + str(calendarios-1) + "\": \"" + str(calendar.monthrange(int(este_ano),int(este_mes))[1]) + ano + mes + str(este_turno)


# Se sale del bucle general. Empezamos a cerrar cosas
# Se eliminan dos carácteres de la cadena, que se han metido en el punto anterior (como dije, es código sucio).
# Luego, se añade un salto de carro para dejarlo más legible

cadena = cadena[3:len(cadena)] + "\" \n}\n}"
# Se mete la definición de la variable y esas cosas
cadena = "{\"main\": {" + cadena

# Y aquí ya se imprime todo. Primero el define y el se cuentan todos los calendarios que suman el array
#print ("#define MESES_TURNOS "+ str(calendarios) + "\n")

# Y para terminar, se pinta el array
#print (cadena)

# Se crea el archivo y se pinta todo
home = expanduser("~")
if os.path.isfile(home + "/Dropbox/Public/data.dat"):
    print("Eliminando versiones anteriores")
    os.remove(home + "/Dropbox/Public/data.dat")
file = open(home + "/Dropbox/Public/data.dat", "w")
file.write(cadena+"\n")
print ("Archivo \"" +home + "/Dropbox/Public/data.dat creado.")



# Todo lo siguiente es código de depuración que no sirve para nada. Por eso se queda así.
#print row[1][4:6], row[1][6:8], row[18], row[19]
#print(calendar.month(2015,01))
#print(calendar.monthrange(2015,1)[1])

