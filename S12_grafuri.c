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

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina m;
	struct NodP* next;
	struct NodS* vecini;
};

struct NodS {
	struct NodS* next;
	struct NodP* info;
};

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

void inserareListaP(NodP** graf, Masina m) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->m = m;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*graf) {
		NodP* aux = *graf;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareListaSecundara(NodS** cap, NodP* info) { // la inceputul listei
	NodS *nou = (NodS*)malloc(sizeof(NodS));
	nou->info = info;
	nou->next = *cap;
	*cap = nou;
}

/*void inserareListaS(NodS** lista, NodP* vecin) { //inserare la sfarsitul listei
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;
	if (*lista) {
		NodS* aux = *lista;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*lista = nou;
	}
}*/

NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	NodP* temp = listaPrincipala;
	while (temp) {
		if (temp->m.id == id) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaID(listaPrincipala, idStop);
	if (listaPrincipala) {
		if (start && stop) {
			inserareListaS(&start->vecini,stop);
			inserareListaS(&stop->vecini,start);
		}
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodP* graf = NULL;
	while (!feof(f)) {
		inserareListaP(&graf, citireMasinaDinFisier(f));
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		int idStop = 0;
		int idStart = 0;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(graf,idStart, idStop);
	}
	fclose(f);
}

void afisareListaS(NodS* cap) {
	while (cap) {
		afisareMasina(cap->info->m);
		cap = cap->next;
	}
}

void afisareListaVecini(NodP* graf, int id) {
	NodP* temp = cautaNodDupaID(graf,id);
	NodS* cap = temp->vecini;
	while (cap) {
		afisareMasina(cap->info->m);
		cap = cap->next;
	}
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
}

int main() {
	NodP* graf;
	graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);
	afisareListaVecini(graf,5);
	return 0;
}
