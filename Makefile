# Compilação do projeto. Use "make" no Linux (ou "mingw32-make" no Windows com MinGW).

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

# arquivos das árvores, usados tanto pelo programa quanto pelos testes
ARVORES  = src/avl.cpp src/rn.cpp src/imagem.cpp src/dados.cpp
HEADERS  = $(wildcard include/*.hpp)

ifeq ($(OS),Windows_NT)
  EXT     = .exe
  LDFLAGS = -static
else
  EXT     =
  LDFLAGS =
endif

PROGRAMA = bin/arvores$(EXT)
TESTES   = bin/testes$(EXT)
EXEMPLOS = bin/exemplos$(EXT)

.PHONY: all testes rodar-testes exemplos clean

all: $(PROGRAMA) $(TESTES)

$(PROGRAMA): src/main.cpp $(ARVORES) $(HEADERS)
	$(CXX) $(CXXFLAGS) src/main.cpp $(ARVORES) -o $@ $(LDFLAGS)

$(TESTES): tests/testes.cpp $(ARVORES) $(HEADERS)
	$(CXX) $(CXXFLAGS) tests/testes.cpp $(ARVORES) -o $@ $(LDFLAGS)

testes: $(TESTES)

# gera as imagens de docs/img (precisa do Graphviz)
exemplos: $(EXEMPLOS)
	./$(EXEMPLOS)

$(EXEMPLOS): tests/exemplos.cpp $(ARVORES) $(HEADERS)
	$(CXX) $(CXXFLAGS) tests/exemplos.cpp $(ARVORES) -o $@ $(LDFLAGS)

rodar-testes: $(TESTES)
	./$(TESTES)

clean:
	rm -f bin/arvores bin/testes bin/exemplos bin/arvores.exe bin/testes.exe bin/exemplos.exe
