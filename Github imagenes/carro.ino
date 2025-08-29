# 1. Ve a la carpeta de tu proyecto (cambia 'mi-proyecto' por el nombre real)
cd ~/Este-equipo/descargas/Github imagenes

# 2. Crea la carpeta Github imagenes (si no existe)
mkdir "Github imagenes"

# 3. Copia las imágenes a la carpeta Github imagenes
cp ~/Este equipo/descargas/carro desde arriba.jpg ~Este equipo/descargas/carro vista de lado.jpg "Github imagenes/"

# 4. Verifica que las imágenes estén en la carpeta
ls "Github imagenes"

# 5. Agrega la carpeta y sus imágenes al seguimiento de Git
git add "Github imagenes/"

# 6. Haz el commit de los cambios
git commit -m "Agregué imágenes a la carpeta Github imagenes"

# 7. Sube los cambios a tu repositorio en GitHub
git push
