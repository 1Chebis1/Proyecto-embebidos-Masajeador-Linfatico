# DOCUMENTO PRELIMINAR - PROYECTO DE AULA SISTEMAS EMBEBIDOS: MASAJEADOR LINFÁTICO INTELIGENTE

## Introducción

El sistema linfático cumple un papel fundamental en el organismo, ya que participa en la eliminación de toxinas, la regulación de los líquidos corporales y el funcionamiento del sistema inmunológico. Cuando este sistema se ve afectado, pueden presentarse problemas como inflamación, acumulación de líquidos, fatiga muscular y dolor en diferentes partes del cuerpo. Estas alteraciones son comunes en enfermedades como el linfedema, la esclerosis múltiple o la fibromialgia, aunque también pueden aparecer en personas con problemas de circulación o retención de líquidos.
Una de las técnicas más utilizadas para aliviar estos síntomas es el drenaje linfático manual, un tipo de masaje terapéutico que estimula el flujo de la linfa y ayuda a reducir la inflamación. Esta técnica también ha ganado popularidad dentro de la macro-tendencia del wellness, debido a sus beneficios asociados con la reducción de la retención de líquidos, el manejo del estrés y la mejora del bienestar general, además de contribuir a una piel más saludable y a la relajación muscular.
Este proyecto propone el desarrollo de un masajeador de drenaje linfático basado en sistemas embebidos, que permita estimular la circulación linfática de forma accesible y guiada. El objetivo es diseñar un dispositivo capaz de asistir al usuario durante la aplicación del masaje, promoviendo una estimulación adecuada del flujo linfático para ayudar a reducir la inflamación, aliviar la fatiga muscular y contribuir tanto al tratamiento terapéutico como al cuidado preventivo y al bienestar general.

## Descripción del Problema

El sistema linfático es responsable de eliminar toxinas, regular los líquidos del cuerpo y apoyar el sistema inmunológico. Cuando su funcionamiento se ve afectado, puede producirse acumulación de líquidos, inflamación y dolor en diferentes partes del cuerpo.
Algunas enfermedades como la Esclerosis múltiple, el Linfedema o la Fibromalgia afectan el flujo adecuado de la linfa, generando fatiga muscular, hinchazón, dolor e incomodidad. Por esta razón se utiliza el drenaje linfático manual, un tratamiento que ayuda a mejorar la circulación y reducir la inflamación, pero que generalmente requiere la intervención de un especialista.
Por ello surge la necesidad de desarrollar un masajeador de drenaje linfático que permita estimular la circulación de manera accesible, ayudando a reducir la inflamación, aliviar la fatiga y mejorar el bienestar tanto en pacientes con estas enfermedades como en cualquier persona que experimente estrés muscular, mala circulación o retención de líquidos, ampliando así su impacto en la salud preventiva y el cuidado personal.



## Alcance del Proyecto
El dispositivo propuesto se enfoca en terapias para tronco y extremidades (piernas y brazos) no siendo óptimo para terapias faciales. 

El nivel de automatización del dispositivo se limita a retroalimentación inteligente para una terapia apropiada según la zona que se está tratando haciendo énfasis en que es el usuario quien tiene el control todo el tiempo sobre la presión que ejerce con el dispositivo y el nivel de vibración. 
Para el desarrollo se utilizará un microcontrolador tipo ESP32 con una interfaz virtual para dar la retroalimentación. 
Se propone un índice IP50 por lo que el dispositivo no podrá ser sumergible ni utilizable con aceites u otros productos que afecten el funcionamiento óptimo del producto sin embargo sí es completamente viable para el contacto humano directo. 


## Objetivos 

### Objetivo general 
Desarrollar un dispositivo electronico portatil para hacer masajes de drenaje linfático con diferentes estímulos fisicos que buscan simular un masaje hecho manualmente

### Objetivos especificos 

* Diseñar un dispositivo que sea facil de cargar y manipular para facilitar la realización de los masajes con poca o ninguna ayuda de otras personas
* Integrar, mediante componentes fisicos y control electronico diferentes estimulos que se adapten a la situación requerida para optimizar el masaje
* Desarrollar el dispositivo con materiales y funciones que sean seguras para la persona que esta recibiendo los masajes, para no afectar la piel o el sistema linfatico
## Repartición de Roles
**Technical Lead:** Sebastián Escobar Restrepo

**Firmware Engineer** Laura Gutiérrez Arias 

**Hardware Integration Engineer** Felipe Vásquez Martinez

**Verification & Testing Engineer** Manuela Arboleda Sánchez 

