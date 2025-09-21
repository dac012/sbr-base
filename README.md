# Sistema Basado en Reglas con Factores de Certeza

Este repositorio implementa y prueba un **sistema basado en reglas** que permite realizar inferencias bajo incertidumbre utilizando **factores de certeza (FC)**. El proyecto contiene ejemplos y casos de uso donde se modelan situaciones reales (selección de candidatos, predicción de resultados deportivos, análisis de accidentes, etc.) mediante reglas del tipo "Si... Entonces..." con un grado de certeza asociado.

## Características principales

- **Definición de reglas** con condiciones y conclusiones, cada una asociada a un factor de certeza.
- **Inferencia** sobre distintos conjuntos de hechos y reglas, calculando el resultado de las hipótesis objetivo.
- **Manejo de incertidumbre** gracias al uso de factores de certeza que cuantifican la confianza en cada regla.
- **Casos de prueba variados** (selección de personal, predicciones deportivas, análisis de accidentes, etc.).

## Estructura del repositorio

- `Prueba-1`, `Prueba-2`, `Prueba-3`, `Prueba-A`: Carpetas con ejemplos de reglas y hechos para diferentes dominios y pruebas.
- `Entorno de ejecución/`: Recursos relacionados con el entorno de ejecución del sistema.

## Ejemplo de regla

```text
R1: Si muchaExp Entonces competente, FC=0.4
R2: Si pocaExp Entonces competente, FC=-0.6
```

## ¿Para qué usar este proyecto?

- Como base para asignaturas o prácticas de sistemas inteligentes, inteligencia artificial, o sistemas expertos.
- Para experimentar con inferencia basada en reglas y manejo de incertidumbre.
- Como ejemplo de modelado de problemas con lógica difusa/factores de certeza.
