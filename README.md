# 🌦️ Estación Climática IoT con Wemos D1 Mini

Proyecto de estación meteorológica local que lee datos ambientales mediante sensores I2C y los envía a un servidor backend en Flask (Python) para visualizarlos en un dashboard web interactivo con gráficos SVG.

## 🛠️ Hardware Utilizado
* **Microcontrolador:** Wemos D1 Mini (ESP8266)
* **Sensor de Presión y Temperatura:** BMP280 (I2C)
* **Sensor de Humedad:** HTU2X / HTU21D (I2C)

### Conexiones BMP280 - HTU2X / HTU21D (Bus I2C compartido)
* **VCC** ➔ 3.3V
* **GND** ➔ GND
* **SCL / CL** ➔ Pin D1 (GPIO5)
* **SDA / DA** ➔ Pin D2 (GPIO4)

## 🖥️ Arquitectura del Software
1. **Wemos D1 Mini (`/wemos_codigo`):** Código en C++ que inicializa los sensores por I2C, se conecta a la red WiFi local y envía un objeto JSON vía HTTP POST cada 10 segundos.
2. **Servidor Backend (`app.py`):** API REST desarrollada en Flask que procesa los datos entrantes del microcontrolador y los almacena temporalmente en memoria (con un historial de las últimas lecturas).
3. **Dashboard Web (`clima_dashboard.html`):** Interfaz frontend moderna y responsive. Consume las APIs de Flask de forma asíncrona (`fetch`) y genera gráficas de tendencia usando SVG nativo.

## 🚀 Cómo Ejecutar el Proyecto

1. **Configurar el Wemos:**
   * Abre `wemos_clima.ino` en Arduino IDE.
   * Instala las librerías `Adafruit BMP280`, `Adafruit HTU21DF` y `ArduinoJson`.
   * Cambia el `SSID`, `PASSWORD` y la dirección IP de tu `SERVER_URL`.
   * Sube el código a tu Wemos D1 Mini.

2. **Arrancar el Servidor:**
   * Asegúrate de tener Python instalado.
   * Instala Flask: `pip install flask`
   * Ejecuta el backend: `python app.py`
   * Abre en tu navegador: `http://localhost:5000`


## 🤝 ¡El código es para Todos! (Contribuciones y Uso)

Este es un proyecto totalmente abierto. Si tienes ideas para mejorarlo, quieres agregarle nuevas funciones (¡como más sensores o gráficos avanzados!), optimizar el código del Wemos o simplemente adaptarlo para tu propia casa:

* **¡Eres libre de tomarlo, modificarlo y compartirlo!** 🚀
* Si encuentras un fallo o quieres proponer una mejora en el diseño web, siéntete libre de abrir un *Issue* o enviar un *Pull Request*. 

¡Aprender comunidad es mucho más divertido! Si te sirvió el proyecto, no olvides dejarle una ⭐ en GitHub.