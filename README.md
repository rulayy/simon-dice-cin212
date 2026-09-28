# Simón Dice — CIN-212

Proyecto individual de Hardware Digital.
Estudiante: Maximiliano Bernal.
Universidad de Valparaíso.

## Descripción
Juego de memoria con Arduino UNO, tres LED, tres pulsadores
y un buzzer pasivo. Cada nivel agrega un paso a la secuencia.
Al equivocarse, el juego vuelve al nivel 1.

## Simulación
https://wokwi.com/projects/476374630777247745

Abre el enlace y pulsa ▶. Espera a que termine la secuencia
y repítela usando los botones de colores. Suelta cada botón
antes de pulsar el siguiente.

## Conexiones
| Componente | Pin |
|---|---|
| Botones rojo, verde y azul | D2, D3 y D4 |
| LED rojo, verde y azul | D8, D9 y D10 |
| Buzzer pasivo | D11 |

Cada LED lleva una resistencia de 220 Ω en serie hacia GND.
Los botones conectan su entrada a GND y usan INPUT_PULLUP.
A0 se deja sin conectar para inicializar la semilla.

## Código
El archivo Simon_Dice.ino contiene el programa.
El antirrebote usa millis() con 30 ms de estabilidad.
Se exige soltar los botones entre entradas.
La secuencia admite hasta 100 pasos.

Para Arduino IDE, coloca Simon_Dice.ino dentro de una carpeta
llamada Simon_Dice y selecciona la placa Arduino UNO.

## Resultados
En Wokwi se alcanzó el nivel 5.
Después de una respuesta incorrecta, el juego volvió al nivel 1.
La implementación se probó en simulación, sin montaje físico.

## Archivos
- Simon_Dice.ino: código del juego.
- diagram.json: circuito para Wokwi.
- LEEME.md: guía ampliada.
- Informe_Simon_Dice_Maximiliano_Bernal.pdf: informe técnico.
