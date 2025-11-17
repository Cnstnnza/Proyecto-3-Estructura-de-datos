# Proyecto-3-Estructura-de-datos
Este repositorio esta hecho para el desarrollo del proyecto numero tres del ramo Estructura de datos, el cual consiste en la implementacion de un sistema de búsqueda de patrones utilizando árboles tries.  

## Compilacion
Para compilar el programa, se debe ejecutar el siguiente comando:  
make  
Y para ejecutar el programa:  
./bio  
En caso de algun error utilizar make clean para limpiar el codigo generado.  
Para saber los comandos disponibles se puede utilizar la opcion help

## Documentacion
La documentacion del codigo fue generada con Doxygen. Para generarla se debe ejecutar el siguiente comando:  
doxygen -g  
Para crear la carpeta html donde se guardara la documentacion se debe ejecutar:  
doxygen Doxyfile  
Y para visualizarla se debe ejecutar:  
xdg-open html/index.html
Tambien esta disponible una carpeta llamada Latex, la cual corresponde a la documentacion creada por Doxygen. Se puede obtener el informe estando dentro de la carpeta Latex y utilizando make pdf.

## Informe
El informe  del proyecto fue generado con LaTeX. La carpeta esta disponible en el repositorio. Para generar el informe se debe ejecutar el siguiente comando, estando en la carpeta [Informe]:  
pdflatex main.tex  
Importante tambien compilar para obtener las referencias utilizando:  
biber main  
El informe esta disponible con el nombre informe.pdf