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


struct NodArbore {
	Masina info;
	struct NodArbore* left;
	struct NodArbore* right;
};
typedef struct NodArbore Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(Nod** root, Masina masinaNoua) {
	if (*root) {
		if ((*root)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*root)->left,masinaNoua);
		}
		else
		{
			adaugaMasinaInArbore(&(*root)->right, masinaNoua);
		}
	}
	else {
		Nod *nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->left = NULL;
		nou->right = NULL;
		*root = nou;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* root = NULL;
	if (file) {
		while (!feof(file)) {
			Masina m = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&root, m);
		}
	}
	fclose(file);
	return root;
}

void afisareMasiniDinArbore(Nod* root) {
	//TREI moduri de parcurgere RSD SRD SDR
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}
}

void afisareInordine(Nod* root) {
	if (root) {
		afisareInordine(root->left);
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->right);
	}
}

void afisarePostordine(Nod* root) {
	if (root) {
		afisarePostordine(root->right);
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
	}
}

void dezalocareArboreDeMasini(Nod** root) {
	if (*root) {
		dezalocareArboreDeMasini((*root)->left);
		dezalocareArboreDeMasini((*root)->right);
		free((*root)->info.model);
		free((*root)-> info.numeSofer);
		free(*root);
		*root = NULL;
	}
}

Masina getMasinaByID(Nod* root, int id) {
	if (root == NULL) {
		Masina m;
		m.id = -1;
		return m;
	}
	else if (id < root->info.id) {
		return getMasinaByID(root->left, id);
	}
	else  if (id > root->info.id){
		return getMasinaByID(root->right, id);
	}
	else {
		Masina m = root->info;
		m.model = malloc(strlen(root->info.model)+1);
		strcpy(m.model,root->info.model);
		m.numeSofer = malloc(strlen(root->info.numeSofer) + 1);
		strcpy(m.numeSofer, root->info.numeSofer);
		return m;
	}
}

int determinaNumarNoduri(Nod* root) {
	if (root) 
		return 1 + determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right);
	else 
		return 0;
}

int calculeazaInaltimeArbore(Nod* root) {
	if (root) {
		return 1 + max(calculeazaInaltimeArbore(root->left),calculeazaInaltimeArbore(root->right)); //ce drq
	}
	else {
		return 0;
	}
}

float calculeazaPretTotal(Nod* root) {
	if (root) {
		return root->info.pret + calculeazaPretTotal(root->left) + calculeazaPretTotal(root->right); // nu mere
	}
	else {
		return 0;
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* root, const char* numeSofer) {
	if (root) {
		if (strcmp(root->info.numeSofer, numeSofer) == 0) {
			return root->info.pret + calculeazaPretulMasinilorUnuiSofer(root->left,numeSofer) + calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		}
		else {
			return 0;
		}
	}
}

int main() {
	Nod* root=citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInordine(root);
	afisareMasina(getMasinaByID(root,3));
	int n = determinaNumarNoduri(root);
	printf("Sunt %d noduri\n",n);
	int m = calculeazaInaltimeArbore;
	printf("Sunt %d etaje\n", m);
	int p = calculeazaPretTotal;
	printf("Pret total %d \n", m);
	printf("Pretul masinilor lui Ionescu: %5.2f \n",calculeazaPretulMasinilorUnuiSofer(root,"Ionescu"));
	return 0;
}
