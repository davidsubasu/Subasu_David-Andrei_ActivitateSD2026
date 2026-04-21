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

typedef struct Nod Nod;
struct Nod {
	Masina masina;
	Nod* next;
};

void pushStack(Nod ** cap, Masina masina) {
	Nod* first = (Nod*)malloc(sizeof(Nod));
	first->masina = masina;
	first->next = (*cap);
	(*cap) = first;
	
}

Masina popStack(Nod** cap) {
	if ((*cap) == NULL) {
		Masina rezultat;
		rezultat.id = -1;
		return rezultat;
	}
	Masina rezultat = (*cap)->masina;
	Nod* temp = (*cap)->next;
	free(*cap);
	*cap = temp;
	return rezultat;
}

char isEmptyStack(Nod* cap) {
	return cap == NULL;
}

void* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE *file = fopen(numeFisier, "r");
	if (!file) {
		return NULL;
	}
	Nod* cap = NULL;
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		pushStack(&cap, masina);
	}
	fclose(file);
	return cap;
}

void dezalocareStivaDeMasini(Nod** cap) {
	while (*cap) {
		free((*cap)->masina.model);
		free((*cap)->masina.numeSofer);
		Nod* temp = (*cap)->next;
		free(*cap);
		*cap = temp;
	}
	
}

int size(Nod* cap) {
	int ct = 0;
	Nod* curent = cap;
	while (curent) {
		ct++;
		curent = curent->next;
	}
	return ct;
}

typedef struct NodDublu NodDublu;
struct NodDublu {
	Masina info;
	NodDublu* next;
	NodDublu* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	NodDublu* first;
	NodDublu* last;
};

void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nodNou = malloc(sizeof(NodDublu));
	nodNou->info = masina;
	nodNou->next = NULL;
	nodNou->prev = coada->last;
	if(coada->last){
		coada->last->next = nodNou;
	}
	else {
		coada->last = nodNou;
	}
	coada->last = nodNou;
}

Masina dequeue(ListaDubla* coada) {
	Masina rezultat;
	rezultat.id = -1;
	if (coada->first) {
		rezultat = coada->first->info;
		NodDublu* temp = coada->first;
		if (coada->first != NULL) {
			coada->first->prev = NULL;}
		else { coada->last = NULL};
		free(temp);
	}
	return rezultat;
}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	ListaDubla coada;
	coada.first = coada.last = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			enqueue(&coada, citireMasinaDinFisier(f));
		}
		fclose(f);
	}
	return coada;
}

void dezalocareCoadaDeMasini(ListaDubla* coada) {
	if (coada == NULL) return;
	NodDublu* curent = coada->first;

	while (curent) {
		free(curent->info.model);
		free(curent->info.numeSofer);
		NodDublu* temp = curent->next;
		free(curent);
		curent = temp;
	}
	coada->first = NULL;
	coada->last = NULL;
}

Masina getMasinaByID(Nod** stiva, int id) { //cu extragere
	Masina rezultat;
	if (*stiva == NULL) {
		Masina rezultat;
		rezultat.id = -1;
		return rezultat;
	}
	Nod* stivaNou = NULL;
	while ((*stiva)) {
		Masina masinaNoua = popStack(stiva);
		if (masinaNoua.id == id) {
			rezultat = masinaNoua;
			break;
		}
		else {
			pushStack(&stivaNou, masinaNoua);
		}
	}
	while (stivaNou) {
		pushStack(stiva, popStack(&stivaNou));
	}
	return rezultat;

}

float calculeazaPretTotalCoada(ListaDubla coada) {
	if (coada.first == NULL) return;
	float suma = 0.0f;
	NodDublu* curent = coada.first;
	while (curent) {
		suma += curent->info.pret;
		curent = curent->next;
	}
	return suma;
}

int main() {
	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	afisareMasina(popStack(&stiva));
	afisareMasina(popStack(&stiva));

	afisareMasina(getMasinaByID(&stiva, 7));

	printf("coada\n");
	ListaDubla coada = citireCoadaDeMasiniDinFisier("masini.txt");
	afisareMasina(dequeue(&coada));
	afisareMasina(dequeue(&coada));

	return 0;
}
