# Ogre3D

Biblioteca para renderizado 3d

## Requerimientos

La instalación de **Ogre3D** mediante paquetes *.deb* presenta ciertos bugs (no ubica las bibliotecas en los lugares).
Instalamos dependencias necesarias para compilar:

```
sudo apt-get install libxaw7-dev libgles2-mesa-dev libxt-dev libsdl2-dev libpugixml-dev libzzip-dev libfreetype-dev
```

Si se va a trabajar con **Blender** es recomendable instalar

```
sudo apt install ogre-1.12-tools
```

Descargamos las fuentes desde el repositorio oficial: `https://github.com/OGRECave/ogre/`, podemos clonarlo o bajar uno de los tags. Este tutorial fue escrito con la versión Ogre 1.12.9


## Compilación

Clonamos (o descomprimimos las fuentes) en una carpeta `ogre`

```
cd ogre
mkdir build
cd build
cmake -DOGRE_BUILD_DEPENDENCIES=OFF -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
```

Al ejecutar `cmake` se descargarán algunas dependencias y se creará el makefile de configuración.

La opción `-DOGRE_BUILD_DEPENDENCIES=OFF` es para evitar que Ogre compile sus propias dependencias, forzando a que use las del sistema. En caso de no usarlo, las bibliotecas compiladas quedan linkeadas contra `build/Dependencies`, pero podemos configurar su instalacion con la variable `OGRE_DEPENDENCIES_DIR`.

La opción `-DCMAKE_INSTALL_PREFIX=$HOME/opt` es para indicar dónde queremos instalar ogre3d. La ubicación default es `/usr/local`, pero si no queremos contaminar el sistema con archivos compilados por nosotros mismos, podemos especificar otra ubicación (en el ejemplo, una carpeta dentro de mi home para no necesitar uso de root).

Una vez que termina de configurarse, la salida debería listar qué paquetes externos fueron detectados y cuáles no.

En mi caso se imprime la siguiente lista:

```
-- The following external packages were located on your system.
-- This installation will have the extra features provided by these packages.
+ zlib
+ zziplib
+ freetype
+ X11
+ OpenGL
+ OpenGL ES 2.x
+ Python
+ pugixml
+ SDL2
+ Qt
+ Xaw
```

Ejecutamos `make` y luego instalamos con `make install` (usar sudo si se instala en un directorio del sistema)

# Compilando con Ogre

Una vez instalada nuestra biblioteca, armamos un proyecto utilizando CMake para que nos ayude a configurar los includes y links

Para comprobar que hayamos instalado correctamente nuestra biblioteca, compilamos uno de los proyectos demo. Indicamos a CMake dónde está Ogre de dos formas:

* Mediante variables de entorno:

Por ejemplo, suponiendo que instalamos ogre en nuestro home:

```
cmake -D OGRE_DIR=$HOME/opt/ogre/lib/OGRE/cmake
```

Asegurarse que esa carpeta contenga el archivo OGREConfig.cmake


* Agregando el path a `CMakelists.txt`

```
set(OGRE_DIR "$ENV{HOME}/opt/ogre/lib/OGRE/cmake")
```

## Configuración

Los siguientes archivos se deben ubicar en el working dir

`TODO`: ver cómo configurar programáticamente

* `plugins.cfg`: Elige la ruta donde se encuentran las bibliotecas dinámicas para renderizar. También elige el renderizador default

* `resources.cfg`: Nos permite listar recursos para nuestra aplicación, en el ejemplo agregamos un directorio con meshes

Para esta prueba verificar la ruta de plugins.cfg corresponda a la instalación realizada.


## Creación de meshes a partir de archivos de Blender

Descargar [blender2ogre](https://github.com/OGRECave/blender2ogre)

En el README del proyecto se encuentran las instrucciones para instalarlo en el sistema.

Para generar los meshes de este tutorial se optó por la ejecución por consola. Luego de clonar la utilidad en el directorio actual, ejecutamos

```
blender -b hello.blend --python blender2ogre/io_ogre/console.py -- meshes 'scene hello'
```

Esto nos genera una carpeta "meshes" con los archivos `.material`, `.mesh` y `.scene` modelados en nuestro archivo Blender.

### Agregar los recursos a nuestra aplicación

Una vez generada nuestra carpeta `meshes`, existen 2 formas posibles de hacerlo disponible a nuestra aplicación:

* Archivo `resources.cfg`: agregamos las siguientes linea a nuestro archivo `resources.cfg`

```
[General]
FileSystem=meshes
```

* De forma programática en nuestro código fuente:

```{c++}
ResourceGroupManager::getSingleton().addResourceLocation( "meshes", "FileSystem" ); 
```

## Demo

Una vez convertidos los meshes de Blender a Ogre, podemos compilar y ejecutar la aplicación. Para esto, correr los siguientes comandos:

```{bash}
mkdir build
cd build
cmake ..
cp -r meshes .
cp ../resources.cfg .
./0_Bootstrap
```

## Troubleshooting

* `undefined reference to zzip_...`

Las instalaciones de libzzip no agregan el link necesario para detectar la biblioteca libzzip.so
