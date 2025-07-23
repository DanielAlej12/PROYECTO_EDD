# Árbol de Magos - Proyecto C++

Este proyecto simula un hechizo para manejar la línea de sucesión y dueños de un antiguo reino de magos.

# Estructura del proyecto

- `bin/`: Contiene el ejecutable y el archivo csv con datos de los magos.
- `src/`: Código fuente C++ del proyecto (`arbol_mago.cpp`).
- `README.md`: Este archivo con explicación.

# Funcionalidad

- Carga un árbol binario de magos desde `magos_data.csv`
- Muestra la línea de sucesión actual (solo magos vivos)
- Asigna automáticamente un nuevo dueño del hechizo con base en reglas complejas
- Permite modificar datos y mostrar hechizos de un mago

# Reglas para asignar nuevo dueño

1. Si el dueño tiene discípulos vivos con magia elemental o unique, es el primero que se asigna.
2. Si no tiene, se busca el primero con magia mixed o el primer hombre vivo.
3. Si el dueño murió y no tiene discípulos, se asigna al compañero discípulo compartiendo la magia.
4. Si esto no se cumple, se reintentan las reglas.
5. Si no se cumple ninguna condición, se asigna a la mujer más joven con discípulos, cuyo maestro fue dueño y tiene magia mixed.
6. De no hallarse, se asigna a la mujer viva más joven.
7. Si un mago tiene más de 70 años, el heredero será su discípulo con magia igual o el más viejo.

(Para más información detallada, revisar el codigo fuente)
