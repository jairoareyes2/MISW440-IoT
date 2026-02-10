# Proyecto IoT - MISW-4401

Este es el proyecto de la materia **4401 - IoT** de la Maestría en Ingeniería de Software de la Universidad de los Andes. El proyecto integra simulación de redes inalámbricas de sensores con desarrollo de firmware embebido.

## 📋 Descripción General

El proyecto consta de dos componentes principales:

1. **Simulación de Red (CupCarbon)**: Simulación de una red de sensores inalámbricos para monitoreo ambiental
2. **Firmware Embebido (NodeMCU)**: Código C++ para placas de desarrollo ESP8266 con sensores reales (DHT11 y fotoresistencia)

## 👥 Autores

- **Jairo Reyes**: ja.reyesr1@uniandes.edu.co
- **Manuel Sanchez**: mg.sanchezb1@uniandes.edu.co

## 📁 Estructura del Proyecto

```
MISW-4401/
├── codigo_cupcarbon/          # Simulación de red en CupCarbon
│   └── EntregableSem3/
│       ├── EntregableSem3.cup # Archivo principal de simulación
│       ├── config/            # Configuración de nodos y sensores
│       ├── scripts/           # Scripts de simulación (CSC)
│       ├── natevents/         # Eventos naturales de la simulación
│       ├── results/           # Resultados de la simulación
│       └── logs/              # Registros de la simulación
│
└── codigo_nodemcu/            # Firmware para placas NodeMCU/ESP8266
    ├── platformio.ini         # Configuración de PlatformIO
    ├── src/                   # Código fuente (main.cpp)
    ├── include/               # Headers y archivos de configuración
    ├── lib/                   # Librerías del proyecto
    └── imgs/                  # Imágenes y diagramas
```

## 🛠️ Componentes de Hardware

### Placa de Desarrollo
- **NodeMCU V1 (ESP 12-E)**: Microcontrolador con WiFi integrado

### Sensores
- **DHT11**: Sensor de temperatura y humedad
- **Fotoresistencia (GL5537)**: Sensor de luz ambiente

### Electrónica Auxiliar
- Resistencias de 10K ohms
- Cableado y componentes de prototipado

## 🚀 Primeros Pasos

### Para la Simulación (CupCarbon)

1. Instale [CupCarbon](http://cupcarbon.com/)
2. Abra el archivo `codigo_cupcarbon/EntregableSem3/EntregableSem3.cup`
3. Configure los parámetros en `config/simulationParams.cfg`
4. Ejecute la simulación

### Para el Firmware Embebido (PlatformIO)

Consulte [codigo_nodemcu/README.md](codigo_nodemcu/README.md) para:
- Instrucciones de instalación de PlatformIO
- Configuración de credenciales WiFi
- Compilación y carga del firmware
- Requisitos del entorno

## 📊 Configuración de la Red

La simulación incluye:
- **1 Estación Base** (basestation_100)
- **21 Nodos Sensores** (sensor_1 a sensor_21)
- **134 Nodos de Gas** (gas_1 a gas_134)
- Configuración de radios y parámetros de simulación

## 📝 Archivos Importantes

| Archivo | Descripción |
|---------|-------------|
| `EntregableSem3.cup` | Archivo de proyecto CupCarbon con la topología de red |
| `config/simulationParams.cfg` | Parámetros globales de la simulación |
| `scripts/ScriptNodoBase.csc` | Script para comportamiento de estación base |
| `scripts/ScriptNodoSensor.csc` | Script para comportamiento de nodos sensores |
| `results/wisen_simulation.csv` | Resultados en formato CSV |
| `logs/log.txt` | Registros de ejecución |
| `codigo_nodemcu/src/main.cpp` | Código principal del firmware |

## 🔧 Requisitos Generales

### Para Simulación
- CupCarbon instalado
- Java Runtime Environment (JRE)

### Para Firmware
- Visual Studio Code (recomendado)
- Python 3
- PlatformIO IDE o CLI
- Controlador USB para NodeMCU

## 📚 Documentación Adicional

- [Documentación de CupCarbon](http://cupcarbon.com/documentation.html)
- [Documentación de PlatformIO](https://docs.platformio.org/)
- [Hoja técnica ESP8266](https://www.espressif.com/en/products/socs/esp8266)

## 📄 Licencia

Este proyecto es parte de la Maestría en Ingeniería de Software de la Universidad de los Andes.

---

**Última actualización**: Febrero 2026
