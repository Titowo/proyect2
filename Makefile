# Nombre del compilador C++ a usar
CXX = g++

# Flags de compilación:
# -std=c++17: Usar el estándar C++17
# -Wall -Wextra: Habilitar todas las advertencias (muy recomendado para buen código)
# -g: Incluir información de depuración (útil para gdb)
# -O2: Nivel de optimización 2 (descomentar para versiones finales, no para depuración)
CXXFLAGS = -std=c++17 -Wall -Wextra -g
# CXXFLAGS += -O2

# Directorios del proyecto
SRCDIR = src
DATADIR = data
BUILDDIR = build

# Archivos fuente C++ (todos los .cpp en el directorio src)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Archivos objeto correspondientes (ej. src/main.cpp -> build/main.o)
# Se excluye LinkedList.tpp porque es un archivo de inclusión de template, no un .cpp a compilar
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter-out $(SRCDIR)/LinkedList.tpp,$(SOURCES)))

# Nombre del ejecutable final
TARGET = $(BUILDDIR)/search_engine_project1

# Reglas Phony (no corresponden a archivos, son solo nombres de tareas)
.PHONY: all clean run setup

# Regla predeterminada: Compila todo y crea el ejecutable
all: setup $(TARGET)

# Regla para crear el directorio de construcción
# Se usa mkdir -p para evitar errores si el directorio ya existe
setup:
	@mkdir -p $(BUILDDIR)
	@echo "Directorio '$(BUILDDIR)/' asegurado."

# Regla para construir el ejecutable final a partir de los archivos objeto
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@
	@echo "Construcción completada: $@"

# Regla genérica para compilar archivos .cpp en .o
# $< es el nombre del archivo fuente (.cpp)
# $@ es el nombre del archivo objetivo (.o)
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compilado: $@"

# Regla para ejecutar el programa
run: all
	@echo "Ejecutando el programa..."
	./$(TARGET)

# Regla para limpiar los archivos de construcción
clean:
	@echo "Limpiando el directorio de construccion..."
	rm -rf $(BUILDDIR)
	@echo "Limpieza completada."