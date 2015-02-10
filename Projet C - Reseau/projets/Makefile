CC=gcc

SRCDIR=src
HEADDIR=headers
LIBDIR=obj
BINDIR=bin
 
 
# Les différentes options de compilations, soyons strictes !
CFLAGS= -I$(HEADDIR) -Wall
# Les différents FrameWorks et bibliothèques pour le linkage
GLLIBS = -lpthread -lm
 
# L'exécutable
BIN=MonExecutable
 
# Où trouver les différents sources *.c qu'il faudra compiler pour créer les objets correspondants
SRC= $(wildcard $(SRCDIR)/*.c)
OBJ= $(SRC:$(SRCDIR)/%.c=$(LIBDIR)/%.o)
 
# Où trouver les différents sources *.cpp qu'il faudra compiler pour créer les objets correspondants
SRCPP= $(wildcard $(SRCDIR)/*.cpp)
OBJCPP= $(SRCPP:$(SRCDIR)/%.cpp=$(LIBDIR)/%.o)
 
all: Sonde Router Coordinateur cpConf
 
#Création de l'exécutable
Sonde: $(LIBDIR)/main_sonde.o $(LIBDIR)/parserConfig.o $(LIBDIR)/simSonde.o $(LIBDIR)/GestionReseau.o $(LIBDIR)/ListeChainee.o $(LIBDIR)/utils.o 
	@$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
	
#Création de l'exécutable
Router: $(LIBDIR)/main_router.o $(LIBDIR)/parserConfig.o $(LIBDIR)/simSonde.o $(LIBDIR)/GestionReseau.o $(LIBDIR)/ListeChainee.o $(LIBDIR)/utils.o 
	@$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
	
#Création de l'exécutable
Coordinateur: $(LIBDIR)/main_coordinateur.o $(LIBDIR)/parserConfig.o $(LIBDIR)/simSonde.o $(LIBDIR)/GestionReseau.o $(LIBDIR)/ListeChainee.o $(LIBDIR)/utils.o 
	@$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
 
# Création des différents *.o à partir des *.c
$(LIBDIR)/%.o: $(SRCDIR)/%.c  
	$(CC) -o $@ -c $< $(CFLAGS)
 
# Création des différents *.o à partir des *.cpp
$(LIBDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CPP) -o $@ -c $< $(CFLAGS)
 
# Nettoyage des objets => Tout sera recompiler !
clean:
	rm $(LIBDIR)/*.o
 
# Nettoyage complet => clean + effacement du l'exécutable
Clean: clean
	rm $(BINDIR)/*


cpConf:
	@for file in config_*; do \
		cp "$$file" $(BINDIR)/ -f; \
	done
