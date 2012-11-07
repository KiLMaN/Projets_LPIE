CC=gcc

SRCDIR=src
HEADDIR=headers
LIBDIR=obj
BINDIR=bin
 
 
# Les diff�rentes options de compilations, soyons strictes !
CFLAGS= -I$(HEADDIR) -Wall
# Les diff�rents FrameWorks et biblioth�ques pour le linkage
GLLIBS = -lpthread -lm
 
# L'ex�cutable
BIN=MonExecutable
 
# O� trouver les diff�rents sources *.c qu'il faudra compiler pour cr�er les objets correspondants
SRC= $(wildcard $(SRCDIR)/*.c)
OBJ= $(SRC:$(SRCDIR)/%.c=$(LIBDIR)/%.o)
 
# O� trouver les diff�rents sources *.cpp qu'il faudra compiler pour cr�er les objets correspondants
SRCPP= $(wildcard $(SRCDIR)/*.cpp)
OBJCPP= $(SRCPP:$(SRCDIR)/%.cpp=$(LIBDIR)/%.o)
 
all: Sonde Router Coordinateur
 
#Cr�ation de l'ex�cutable
Sonde: $(LIBDIR)/main_sonde.o $(LIBDIR)/simSonde.o $(LIBDIR)/GestionReseau.o $(LIBDIR)/ListeChainee.o $(LIBDIR)/utils.o 
	@$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
	
#Cr�ation de l'ex�cutable
Router: $(LIBDIR)/main_router.o $(LIBDIR)/simSonde.o $(LIBDIR)/GestionReseau.o $(LIBDIR)/ListeChainee.o $(LIBDIR)/utils.o 
	@$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
	
#Cr�ation de l'ex�cutable
Coordinateur: $(LIBDIR)/main_coordinateur.o $(LIBDIR)/simSonde.o $(LIBDIR)/GestionReseau.o $(LIBDIR)/ListeChainee.o $(LIBDIR)/utils.o 
	@$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
 
# Cr�ation des diff�rents *.o � partir des *.c
$(LIBDIR)/%.o: $(SRCDIR)/%.c  
	$(CC) -o $@ -c $< $(CFLAGS)
 
# Cr�ation des diff�rents *.o � partir des *.cpp
$(LIBDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CPP) -o $@ -c $< $(CFLAGS)
 
# Nettoyage des objets => Tout sera recompiler !
clean:
	rm $(LIBDIR)/*.o
 
# Nettoyage complet => clean + effacement du l'ex�cutable
Clean: clean
	rm $(BINDIR)/*
