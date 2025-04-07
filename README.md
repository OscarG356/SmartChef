# 🍴 SmartChef - Tu Asistente de Cocina Inteligente

Este repositorio contiene el código fuente para el desarrollo de una herramienta multifuncional que actúa como asistente de cocina.

## 🚀 Motivación

La motivación para desarrollar este proyecto surge de la necesidad de optimizar y simplificar la preparación de alimentos mediante el uso de tecnología. Buscamos ofrecer una solución innovadora que facilite la cocina, reduciendo el esfuerzo manual y minimizando errores en las recetas. Además, queremos integrar automatización e inteligencia en un dispositivo accesible, permitiendo a los usuarios disfrutar de una experiencia culinaria más eficiente y precisa.

## 📋 Descripción del Proyecto

SmartChef es un asistente de cocina especializado en la preparación de masas blandas (panquecas, arepas, tortas, etc.) que funciona siguiendo un proceso secuencial: primero, el usuario selecciona una receta del recetario digital e indica el número de comensales mediante una interfaz gráfica desarrollada en LabVIEW; el sistema calcula automáticamente las cantidades precisas de cada ingrediente y solicita al usuario que los vierta uno a uno, pesándolos mediante un sensor de presión hasta alcanzar la medida exacta; posteriormente, la máquina mezcla los ingredientes utilizando un motor DC con accesorios especializados y, si la receta lo requiere, programa un tiempo de reposo para la masa, dejándola lista para su posterior cocción.

## 🌟 Características Principales

- **Base de datos** con recetas almacenadas.
- **Interfaz intuitiva** para selección de recetas.
- **Cálculo automático** de ingredientes según la cantidad deseada.
- **Pesaje en tiempo real** para garantizar precisión.
- **Mezcla automatizada** con control de tiempo y temperatura.
- **Notificación clara** al finalizar el proceso de preparación.

## ✅ Requisitos Funcionales

### 1. Procesamiento de Datos

- El sistema deberá recibir como entradas la receta seleccionada y el número de comensales para calcular automáticamente las cantidades requeridas de cada ingrediente.
- Consultar la información donde están las recetas predefinidas y sus proporciones estándar.
- Ajustar dinámicamente las cantidades de ingredientes.

### 2. Comunicación

- Comunicación bidireccional entre LabVIEW y el microcontrolador.
- Envío de comandos desde la interfaz y recepción de datos de sensores.
- Notificación al usuario en caso de pérdida de comunicación y detención segura.

### 3. Control de Actuadores y Sensores

- Activación del motor DC según la receta.
- Monitoreo del sensor de presión en tiempo real.
- Indicación visual del pesaje correcto.
- Temporizador de reposo automático si la receta lo requiere.

### 4. Gestión de Energía

- Operación en modo de bajo consumo durante estado de espera/reposo.

### 5. Interfaz de Usuario

- Menú de selección de recetas al iniciar.
- Ingreso del número de comensales.
- Estado del proceso en tiempo real.
- Indicador claro cuando la masa esté lista.

### 6. Seguridad y Manejo de Errores

- Detección de errores en sensores o motor, con notificación.
- Botón o comando de emergencia para detener el proceso.

### 7. Tiempos de Respuesta

- Lectura del sensor de presión en < 500 ms.
- Activación/parada del motor en < 2 segundos tras el comando.

### 8. Cumplimiento de Secuencia

- Ejecución secuencial obligatoria: receta → pesaje → mezcla → reposo.

## 🧠 Requisitos No Funcionales

1. **Rendimiento:** Cálculo de cantidades en menos de 10 s tras ingresar porciones.
2. **Disponibilidad:** 80% de disponibilidad durante horario de cocina.
3. **Fiabilidad:** Al menos 3 ciclos completos sin fallos críticos.
4. **Escalabilidad:** Permite agregar nuevas recetas sin modificar hardware.
5. **Interoperabilidad:** Comunicación con LabVIEW vía UART sin software extra.
6. **Usabilidad:** Interfaz amigable y guiada; preparación en menos de 5 minutos.
7. **Consumo de Energía:**  
   - Reposo: < 3 W  
   - Mezcla: < 15 W
8. **Tiempos de Respuesta:** Registro de peso en menos de 500 ms.

## 🔧 Equipos y Materiales Requeridos

- Laptop con LabVIEW instalado  
- Placa de desarrollo con microcontrolador (Raspberry Pi Pico)  
- Sensor de presión  
- Motor DC + accesorio mezclador (brazo impreso en 3D)  
- Fuente de alimentación (5V o 12V según diseño)  
- Ingredientes simulados: harina, agua  
- Estructura base y sellado antifluidos (madera, acrílico o 3D)

## 🧪 Validación del Sistema

| Etapa              | Prueba                                                  | Validación                                         |
|--------------------|---------------------------------------------------------|----------------------------------------------------|
| 1. Encendido        | Encender el sistema                                     | Inicio correcto, sin errores ni bloqueos          |
| 2. Selección receta | Seleccionar “Panquecas” y 2 comensales                 | Comunicación GUI con el microcontrolador          |
| 3. Pesado           | Verter harina hasta el peso indicado                   | Lectura del sensor en tiempo real                 |
| 4. Mezcla           | Mezclado automático                                    | Control del motor sin bloqueos                    |
| 5. Reposo           | Reposo según lo indicado por la receta                 | Temporización y control adecuado                  |
| 6. Finalización     | Indicación de masa lista                               | Comunicación final al usuario                     |

## 💸 Presupuesto Estimado

| Nº | Componente / Material                                | Cantidad | Valor unitario (COP) |
|----|-------------------------------------------------------|----------|------------------------|
| 1  | Microcontrolador (Arduino UNO / ESP32 / STM32)        | 1        | $40.000               |
| 2  | Sensor de presión y temperatura                       | 1        | $65.000               |
| 3  | Motor DC (12V)                                        | 1        | $30.000               |
| 4  | Driver para motor (L298N / puente H)                  | 1        | $10.000               |
| 5  | Fuente de alimentación (12V - 2A adaptador o batería) | 1        | $25.000               |
| 6  | Buzzer, pasajes de bus, producto para sellar          | 1        | $15.000               |
| 7  | Accesorios de mezcla (3D impreso o manual)            | 1        | $20.000               |
| 8  | Protoboard / PCB / cables jumper                      | 1        | $15.000               |
| 9  | Estructura base (acrílico/madera/plástico reciclado)  | 1        | $20.000               |
|10  | Ingredientes simulados (harina, agua, etc.)           | 1        | $5.000                |

**💰 Total general:** $245.000 COP  
**👥 Costo por persona (3 integrantes):** $81.666 COP

## 🛠 Instalación y Uso

_Próximamente..._
