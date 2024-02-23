><img src="https://upload.wikimedia.org/wikipedia/commons/4/4a/Usac_logo.png" alt="drawing" width="75">
>
>Universidad San Carlos de Guatemala
>
>Facultad de Ingeniería 
>
>Escuela de Ciencias y Sistemas 
>
>Primer Semestre, 2024
>
>Laboratorio de Arquitectura de Computadores y Ensambladores 2

### Grupo No. 2

Integrantes:

| Nombre                               | Carnet    | 
| ------------------------------------ | --------- | 
| <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRH6Uyi30Ty2WkMb0ZjuFLoXmkRwrrMObm-X2zztWtGbOgyA-i7mFzuiSKltN14HLAJDVM&usqp=CAU" alt="drawing" width="20"> &nbsp;Uzzi Libni Aarón Pineda Solórzano          | 201403541 | 
| <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRH6Uyi30Ty2WkMb0ZjuFLoXmkRwrrMObm-X2zztWtGbOgyA-i7mFzuiSKltN14HLAJDVM&usqp=CAU" alt="drawing" width="20"> &nbsp; Juan Orlando Cabrera          | 200112856 |
| <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRH6Uyi30Ty2WkMb0ZjuFLoXmkRwrrMObm-X2zztWtGbOgyA-i7mFzuiSKltN14HLAJDVM&usqp=CAU" alt="drawing" width="20"> &nbsp; José Fernando Recinos Acuté           | 201114236 | 
|  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQvke8Pr8T6xz52yM8v0ieg0oQy9L9SwfkO4hy4IKoRpxyQBKSGUWto7sWmzj9YYgm1VzU&usqp=CAU" alt="drawing" width="20"> &nbsp; Melyza Alejandra Rodríguez Contreras | 201314821 | 


## CONTROL DEL AMBIENTE, EN DORMITORIOS INTELIGENTES IMPLEMENTANDO ANALISIS METEOROLÓGICO IoT

Una estación meteorológica de IoT es un sistema avazado de monitoreo que integra diversos sensores para recopilar datos climáticos en tiempo real. Esta estación está diseñada para recopilar información sobre cuatro variables principales:

    1. Temperatura.
    2. Iluminación.
    3. Humedad.
    4. Concentración de Co2 en el aire.

Este sistema de estación meteorológica IoT recopila los datos de estos sensores y los envía a una plataforma centralizada implementando un sistema de cola de mensajes (**MQTT**). Los datos se almacenan en una **base de datos** y pueden visualizarse, implementando una **aplicación web**. Esto permite que los usuarios monitoreen y accionen sobre los diversos componentes que administra la estación.

## FUNCIONES

    - MEDICIÓN DE TEMPERATURA        
    - CANTIDAD DE LUZ AMBIENTAL
    - MEDICIÓN DE CALIDA DE AIRE
    - MEDICIÓN DE PROXIMIDAD

## OBJETIVO DEL DISPOSITIVO
El propósito del sistema es gestionar un ambiente inteligente en una habitación, con la capacidad de asegurar condiciones saludables para el ocupante y controlar los gastos energéticos analizando la información obtenida.

## DESCRIPCIÓN DE CAPAS
    - Hardware
        - Uso de Microcontrolador Arduino.
        - Implementación de Sensores.
            - Sensor de temperatura y humedad.
                - Dht11
                - Dht22
            - Sensor de movimiento
                - Sensor Ultrasonido HC-SR04
                - Implementación de led infrarrojo
            - Sensor de iluminación
                - Fotocelda
                - Modulo TCS3200
            - Sensor de Co2
                - MQ135
            - Pantalla LCD
    - Software
        - Arduino IDE
        - Python (Opcional)
    - Plataforma
        - Pantalla LCD

## FLUJO DEL SISTEMA

- Los sensores deescritos realizarán lecturas del ambiente, para trasladar estos datos al microcontrolador Arduino.

- Luego de contar con los datos en el sistema, dicha información, será presentada implementando una pantalla LCD. Con el objetivo de mostrar información al usuario, se necesita implementar, botones que permitan interactuar con el microcontrolador.

- Los botones tendrán la capacidad de mostrar información en tiempo real.

- Para mostrrar la información histórica es necesario el uso de memoria EEPROM ya que se necesita guardar la información resultante de la diversidad de sensores, implementando un botón exclusivo, que permita generar el guardado de información.

![Diagrama](./img/diagrama_IoT.PNG)

## DIAGRAMA DE FLUJO



TABLA No.1: Nombre y Tipo de Variables
| NOMBRE VARIABLE | TIPO | DESCRIPCIÓN | PIN ARDUINO |
| ------ | ------ | ------ | ------ |
| pinButtonFoto | int | Pin vinculado al sensor de la fotoresistencia, en la cual se envia informacion al arduino. | 6 |
| buttonValFoto | int | Valor obtenido del pin de la fotoresistencia. |  |
| pinButtonCO2 | int | Pin vinculado al sensor del CO2, dicho sensor envia la informacion al arduino. | 8 |
| buttonValCO2 | int | Valor obtenido del pin del sensor CO2. |  |
| pinButtonTem | int | Pin vinculado al sensor de temperatura, este envia la informacion al arduino. | 9 |
| buttonValTem | int | Valor obtenido del pin de temperatura |  |
| pinButtonHum | int | Pin vinculado al sensor de humedad, este enviara la informacion al arduino. | 10 |
| buttonValHum | int | Valor obtenido del pin de humedad. |  |
| pinButtonSave | int | Pin vinculado al boton que guardara la informacion de los sensores en ese instante. | 11 |
| buttonValSave | int | Valor obtenido del pin Guardar. |  |
| pinButtonHis | int | Pin vinculado al boton de historial, mostrara la informacion guardada. | 12 |
| buttonValHis | int | Valor del pin Historial. |  |
| pintButtonAll | int | Pin vinculado al boton para mostrar todos los datos. | 13 |
| buttonValAll | int | Valor obtenido del pin ALL. |  |
| pinButtonUltrasonico | int | Pin vinculado al boton de fotoresistencia para envia de informacion al arduino. | 22 |
| buttonValUltrasonico | int | Valor obtenido del pin de la fotoresistencia. |  |
| dl | int | Valor del delay siendo este de 400. |  |
| sensorIluValue | int | Valor de iluminacion. |  |
| sensorCO2Value | int | Valor de CO2. |  |
| h | int | Variable para humedad. |  |
| t | int | Variable para temperatura. |  |
| duracion | long | Variable para guardar informacion de duracion. |  |
| distancia | long | Variable para guardar informacion de distancia. |  |
| buffer1[16] | char | Cadena para mostrar en la pantalla. |  |
| mostrarLCD | int | Opciones a mostrar en la pantalla siendo estos: 0-Todos, 1-CO2, 2-Temperatura y 3-Humedad. |  |

