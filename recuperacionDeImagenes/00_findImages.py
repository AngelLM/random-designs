import subprocess
import os

destinationFolder="00_Recuperadas/"
outputName="recuperada"
fileExt=[".jpg", ".jpeg", ".png"]
contador=0
carpCont=0
excluidas=["00_Recuperadas", "00_findImages.py"]

carpetas = os.listdir(".")
for carpeta in carpetas:
    if not "00_Recuperadas" in carpeta and not "00_findImages.py" in carpeta:
        print("Entrando a " + carpeta)
        files = os.listdir(carpeta)
        for file in files:
            for ext in fileExt:
                if ext in file:
                    nombre=outputName+str(contador)+ext
                    # filenameformated=file.replace(" ", "\\ ")
                    print("moving " + carpeta + "/" + file + " to " + destinationFolder + nombre)
                    # subprocess.call("mv "  + carpeta + "/" + filenameformated + " " + destinationFolder + nombre)
                    os.rename(carpeta + "/" + file, destinationFolder + nombre)
                    contador+=1
        print("Saliendo de " + carpeta)

print("------------------")
print("------------------")
print("------------------")
print("-------DONE-------")
print("------------------")
print("------------------")
print("------------------")
