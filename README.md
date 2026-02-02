**Proyecto**

Este repositorio contiene el proyecto de la materia *4401 - IoT* de la Maestría en Ingeniería de Software de la Universidad de los Andes.

**Autores**

- **Jairo Reyes**: ja.reyesr1@uniandes.edu.co
- **Manuel Sanchez**: mg.sanchezb1@uniandes.edu.co

**Estructura del repositorio**

- `platformio.ini`: configuración del proyecto PlatformIO
- `src/`: código fuente (por ejemplo `src/main.cpp`)
- `include/`, `lib/`, `test/`: carpetas auxiliares del proyecto

**Requisitos**

- Visual Studio Code (opcional, recomendado)
- Python 3 (para instalación por CLI)
- PlatformIO (extensión de VSCode o CLI `platformio`)

**Instalación de PlatformIO**

1) Instalación como extensión en Visual Studio Code

- Abra Visual Studio Code.
- Vaya a la vista de extensiones y busque "PlatformIO IDE" o "PlatformIO".
- Instale la extensión y reinicie VS Code si se solicita.

2) Instalación por consola (CLI)

Ejemplo con pip (requiere Python 3):

```bash
python3 -m pip install --upgrade pip
python3 -m pip install -U platformio
```

Compruebe la instalación:

```bash
pio --version
```

Si usa un gestor de paquetes del sistema (por ejemplo `apt`, `brew`) o instaladores oficiales, consulte la documentación de PlatformIO.

**Compilar y ejecutar el proyecto**

Desde Visual Studio Code (recomendado):

- Abra la carpeta del proyecto en VS Code.
- Use el icono de PlatformIO en la barra lateral para `Build`, `Upload` o `Monitor`.
- Verifique el archivo `platformio.ini` para seleccionar el entorno objetivo.

Desde la consola (línea de comandos) en la raíz del repositorio:

```bash
# Compilar
pio run

# Limpiar artefactos de compilación
pio run --target clean

# Cargar (flashear) al dispositivo
pio run --target upload

# Monitorear puerto serie
pio device monitor

# Si existen múltiples entornos, especificar uno:
pio run -e <env>
pio run -e <env> --target upload
```

**Ejecutar pruebas**

Si el proyecto incluye tests, use:

```bash
pio test
# o para un entorno específico
pio test -e <env>
```

**Notas importantes**

- Revise `platformio.ini` para ver la plataforma, marco y entornos configurados.
- El archivo principal del ejemplo está en `src/main.cpp`.

**Siguientes pasos sugeridos**

- ¿Desea que ejecute una compilación de prueba aquí y comparta la salida?
- ¿Desea que haga un commit con este `README.md` y un push al repositorio remoto?
