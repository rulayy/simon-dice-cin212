# Simón Dice — CIN-212

Proyecto de apoyo para estudiar, simular y adaptar. Revisa y comprende cada función antes de defenderlo. Incluye código y circuito Wokwi; todavía no está publicado en Wokwi ni GitHub. No constituye un informe final ni evidencia de pruebas físicas.

## Empezar sin Arduino

1. Entra a https://wokwi.com/projects/new/arduino-uno
2. En la pestaña sketch.ino, reemplaza el contenido por el de Simon_Dice/Simon_Dice.ino.
3. En la pestaña diagram.json, reemplaza el contenido por el del archivo diagram.json incluido.
4. Inicia la simulación con el botón de reproducción. Observa la secuencia y repítela con los botones de colores. También están asignadas las teclas 1 (rojo), 2 (verde) y 3 (azul), con el diagrama enfocado.
5. Suelta el botón entre pulsaciones. Al acertar se agrega un paso; al fallar empieza una nueva partida.
6. Guarda el proyecto en tu cuenta y conserva capturas y resultados para el informe.

El rebote de botones se mantiene activado en el circuito. El monitor serial usa 9600 baudios y muestra nivel y tiempos entre respuestas. En un simulador la lectura de A0 puede repetirse, por lo que no se garantiza una secuencia diferente al reiniciar. En el circuito físico A0 debe quedar sin conectar, tal como solicita la tarea; tampoco constituye una fuente criptográfica de azar.

## Conexiones

| Elemento | Pin Arduino | Conexión restante |
|---|---|---|
| Botón rojo | D2 | Otro contacto a GND |
| Botón verde | D3 | Otro contacto a GND |
| Botón azul | D4 | Otro contacto a GND |
| LED rojo, ánodo | D8 | Cátodo a resistencia de 220 Ω y luego GND |
| LED verde, ánodo | D9 | Cátodo a resistencia de 220 Ω y luego GND |
| LED azul, ánodo | D10 | Cátodo a resistencia de 220 Ω y luego GND |
| Buzzer piezoeléctrico pasivo, positivo | D11 | Negativo a GND |
| A0 | Sin conectar | Semilla pseudoaleatoria |

Todos comparten GND. En pulsadores de cuatro patas, identifica los dos contactos diferentes: las patas del mismo contacto ya están unidas internamente. INPUT_PULLUP permite leer HIGH en reposo y LOW al pulsar; no conectes el botón a 5 V.

Para Arduino IDE abre Simon_Dice/Simon_Dice.ino, selecciona Arduino UNO y el puerto de la placa y compila/sube. La carpeta y el archivo tienen el mismo nombre. El buzzer físico debe ser un piezo pasivo de bajo consumo apto para accionamiento GPIO; verifica la ficha del componente concreto.

## Cómo funciona el código

- setup configura entradas/salidas y la semilla.
- iniciarNivel conserva la secuencia previa y agrega un número 0..2.
- reproducirSecuencia enciende cada LED 500 ms con su nota y deja 200 ms entre pasos.
- actualizarBotones filtra cambios con millis: 30 ms estables, sin delay en el antirrebote.
- leerEntrada exige liberar todos los botones entre respuestas. Una pulsación múltiple estable se trata como error.
- procesarRespuesta compara inmediatamente el botón aceptado con el paso esperado.
- exito reproduce tres notas. error hace parpadear todos los LED, suena y limpia la partida.
- El límite explícito es 100 pasos; al completarlos comienza otra partida para proteger el arreglo.

Se usan byte para colores e índices (0..100), int para entradas con códigos negativos, unsigned int para frecuencias y unsigned long para tiempos. La resta de tiempos sin signo permite manejar el desbordamiento de millis. Los delay se limitan a presentación y señales, cuando no se aceptan respuestas; el filtrado de entradas usa millis.

## Pruebas que debes realizar y registrar

| Prueba | Resultado esperado | Resultado observado / captura |
|---|---|---|
| Inicio | Secuencia de un paso | Pendiente |
| Aciertos en niveles 1 a 5 | Conserva prefijo y agrega un paso | Pendiente |
| Error en primer paso | Sonido, luces y retorno a nivel 1 | Pendiente |
| Error en paso intermedio | Reinicio inmediato tras detectar el error | Pendiente |
| Mantener botón | Solo una entrada hasta soltar | Pendiente |
| Dos pasos seguidos del mismo color | Requiere pulsar, soltar y volver a pulsar | Pendiente |
| Rebote activado | Sin pulsaciones duplicadas | Pendiente |
| Pulsar durante reproducción | No cuenta como respuesta | Pendiente |
| Reiniciar simulación / placa | Registrar secuencias y comentar limitaciones | Pendiente |

Verificación realizada durante preparación: compilación C++ con funciones Arduino sustituidas por un entorno de pruebas y comprobaciones de rebote, progresión, pulsación sostenida, error, pulsación múltiple y límite del arreglo. Esto verifica lógica, pero NO sustituye compilación para AVR, simulación Wokwi ni prueba física. Estas tres comprobaciones siguen pendientes.

## GitHub y trabajo incremental

Crea un repositorio y registra el punto de partida que realmente uses. A partir de ahí haz commits al realizar avances reales: ajustes de conexiones, pruebas y correcciones, diagramas, resultados e informe. No inventes un historial de trabajo previo. La rúbrica excelente pide al menos seis commits significativos y participación real de ambos integrantes. Incluye este README adaptado, el código, el circuito y la documentación. No se ha creado ni publicado un repositorio por ti.

## Informe pendiente: 4 a 6 páginas

1. Portada: asignatura, título, integrantes, profesor y fecha.
2. Introducción/objetivos: memorizar secuencias, manejar GPIO, filtrar rebote y generar audio.
3. Hardware: incluir esquema eléctrico, conexiones y cálculo R=(5−Vf)/I. Como ejemplo, con Vf=2 V y R=220 Ω, I=13,6 mA y P=0,041 W; una resistencia de 1/4 W tiene margen. Usar el Vf real del LED y verificar límites de corriente del UNO y del microcontrolador, incluidos los agregados.
4. Software: incluir diagrama de bloques y flujo o seudocódigo concordante con el programa; explicar funciones, tipos, temporización y límite de memoria.
5. Resultados: completar la tabla con pruebas propias, nivel alcanzado y tiempos observados. Identificar si son resultados de simulación o de hardware.
6. Conclusiones: describir dificultades que realmente aparecieron y sus soluciones; referencias IEEE y URL real del repositorio.

Fuente 11–12 pt, interlineado 1,15, márgenes de 2,5 cm, páginas numeradas. Esta pauta no es el informe final. No entregar casillas pendientes como resultados.

## Fuentes técnicas

- Enunciado CIN_212_Tarea1_Entradas_y_Salidas_Digitales.pdf suministrado por el estudiante.
- Arduino, Debounce: https://docs.arduino.cc/built-in-examples/digital/Debounce/
- Wokwi, formato de circuito: https://docs.wokwi.com/diagram-format
- Wokwi, pulsador: https://docs.wokwi.com/parts/wokwi-pushbutton
- Wokwi, buzzer: https://docs.wokwi.com/parts/wokwi-buzzer

La entrega con solo simulación tiene el descuento indicado en el enunciado y requiere defensa presencial. Coordina la fecha con el docente.
