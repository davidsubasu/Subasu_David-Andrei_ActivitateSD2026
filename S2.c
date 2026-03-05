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

void afisareMasina(Masina masina) {
	printf("ID: %d\nNumar usi: %d\nPret: %f\nModel: %-10s\nNume sofer: %-10s\nSerie: %c\n\n", masina.id,masina.nrUsi, masina.pret, masina.model, masina.numeSofer,masina.serie);

}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	if (masini == NULL || nrMasini == 0) {
		printf("Vectorul de masini este gol");
		return;
	}
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	//if (nrMasini == NULL) return;
	Masina* temp=(Masina*)malloc(sizeof(Masina)*((*nrMasini) + 1));
	for (int i = 0; i < *nrMasini; i++) {
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini]=masinaNoua;
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	char linie[256];
	fgets(linie,255,file);
	char delimitator[3] = ",\n";
	Masina masina;
	masina.id = atoi(strtok(linie,delimitator));
	masina.nrUsi = atoi(strtok(NULL, delimitator));
	masina.pret = atof(strtok(NULL, delimitator));
	char* aux = (strtok(NULL, delimitator));
	masina.model = (char*)malloc((strlen(aux) +1)* sizeof(char));
	strcpy(masina.model,aux);
	aux = (strtok(NULL, delimitator));
	masina.numeSofer = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.numeSofer, aux);
	masina.serie =  strtok(NULL, delimitator)[0];
	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("Eroare la deschidere\n");
		return;
	}
	else {
		Masina* vectorMasini = NULL;
				while (!feof(file)) {
			
			Masina masina = citireMasinaFisier(file);
			adaugaMasinaInVector(&vectorMasini,nrMasiniCitite,masina);
		}
				return vectorMasini;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	if (vector != NULL && *vector != NULL) {
		for (int i = 0;i < *nrMasini;i++) {
			if ((*vector)[i].model != NULL) {
				free((*vector)[i].model);
				(*vector)[i].model = NULL;
			}
			if ((*vector)[i].numeSofer != NULL){
				free((*vector)[i].numeSofer);
				(*vector)[i].numeSofer = NULL;
			}
		}
		free(*vector);
		*vector = NULL;
		*nrMasini = 0;
	}

}

int main() {
	int nrMasini = 0;

	Masina* masini=citireVectorMasiniFisier("masini.txt",&nrMasini);
	afisareVectorMasini(masini,nrMasini);

	printf("Dezalocare:\n\n");
	dezalocareVectorMasini(&masini,&nrMasini);
	//Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	//afisareVectorMasini(masini,nrMasini);
	printf("Masini dupa dezalocare");
	
	return 0;
}
