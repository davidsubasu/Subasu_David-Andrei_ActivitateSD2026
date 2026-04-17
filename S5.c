#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	struct Nod* prev;
	struct Nod* next;
	Masina info;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniInceput(ListaDubla listaDubla) {
	Nod* aux;
	aux = listaDubla.first;
	while (aux) {
		afisareMasina(aux->info);
		aux = aux->next;
	}
}

void afisareListaMasiniSfarsit(ListaDubla listaDubla) {
	Nod* aux;
	aux = listaDubla.last;
	while (aux) {
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* listaDubla, Masina masinaNoua) {
	Nod* nou;
	nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = listaDubla->last;
	if (listaDubla->last) {
		listaDubla->last->next = nou;
	}
	else {
		listaDubla->first = nou;
	}
	listaDubla->last = nou;
}

void adaugaLaInceputInLista(ListaDubla* listaDubla, Masina masinaNoua) {
	Nod* nou;
	nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = listaDubla->first;
	if (listaDubla->first) {
		listaDubla->first->prev = nou;
	}
	else {
		listaDubla->last = nou;
	}
	listaDubla->first = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Masina masina;
	ListaDubla listaMasini;
	listaMasini.first = NULL;
	listaMasini.last = NULL;
	while (!feof(file)) {
		masina = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&listaMasini, masina);
	}
	fclose(file);
	return listaMasini;
}

void dezalocareLDMasini(ListaDubla* listaMasini) {
	Nod* aux;
	if (listaMasini->first) { //daca lista nu este null
		if (listaMasini->first->next) {//daca lista are minim 2 noduri
			aux = listaMasini->first->next;
			while (aux) {
				free((aux->prev->info).model);
				free((aux->prev->info).numeSofer);
				free(aux->prev);
				aux = aux->next;
			}
		}
		free(listaMasini->last->info.model);
		free(listaMasini->last->info.numeSofer);
		free(listaMasini->last);
		listaMasini->first = NULL;
		listaMasini->last = NULL;
	}
}

float calculeazaPretMediu(ListaDubla listaMasini) {
	Nod* masina = listaMasini.first;
	float suma = 0; int nr = 0;
	while (masina) {
		nr++;
		suma += masina->info.pret;
		masina = masina->next;
	}
	if (nr != 0) return suma / nr;
	return 0;
}

void stergeMasinaDupaID(ListaDubla* listaMasini, int id) {
	if (listaMasini == NULL || listaMasini->first == NULL) 
		return;
	Nod* aux = listaMasini->first;
	while (aux) {
		if (aux->info.id == id) {
			free((aux->info).model);
			free((aux->info).numeSofer);
			if (aux == listaMasini->first) {
				listaMasini->first = aux->next;
				if (listaMasini->first)
					listaMasini->first->prev = NULL;
				else 
					listaMasini->last = NULL;
			}
			else if (aux == listaMasini->last) {
				listaMasini->last == aux->prev;
				if (listaMasini->last)
					listaMasini->last->next == NULL;
			}
			else {
				aux->prev->next=aux->next;
				aux->next->prev=aux->prev;
			}
			free(aux);;
			return;
		}
		aux = aux->next;
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla listaMasini) {
	if (listaMasini.first == NULL)
		return NULL;
	Nod* aux = listaMasini.first;
	int maxPret = aux->info.pret;
	char* maxNumeSofer = aux->info.numeSofer;
	while (aux) {
		if (aux->info.pret > maxPret) {
			maxPret = aux->info.pret;
			maxNumeSofer = aux->info.numeSofer;
		}
		aux=aux->next;
	}
	//deep copy (dangling pointer prevention)
	char* copieNume = (char*)malloc((strlen(maxNumeSofer)+1) * sizeof(char));
	if(copieNume)
		strcpy_s(copieNume, strlen(maxNumeSofer) + 1,maxNumeSofer);
	return copieNume;
}

int main() {
	ListaDubla listaMasini = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniInceput(listaMasini);
	printf("Medie suma: %2f ", calculeazaPretMediu(listaMasini));
	printf("\n\n=======================================\n\n");
	stergeMasinaDupaID(&listaMasini,1);
	stergeMasinaDupaID(&listaMasini, 2);
	stergeMasinaDupaID(&listaMasini, 3);
	afisareListaMasiniInceput(listaMasini);
	afisareListaMasiniSfarsit(listaMasini);
	printf("\n\n===========================================\n\n");
	char* nume = getNumeSoferMasinaScumpa(listaMasini);
	printf("%s este sefu la bani\n", nume);

	return 0;
}
