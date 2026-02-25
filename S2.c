#include<stdio.h>
#include<malloc.h>

//ocupa spatiu de memorie contigua

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};
struct Telefon initializare(int id, int RAM, char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc((strlen(producator) + 1) * sizeof(char));
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}

struct Telefon copiazaTelefon(struct Telefon t) {
	struct Telefon telefon = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return telefon;
}

void afisare(struct Telefon t) {
	printf("Id: %d\nRAM: %d\nProducator: %s\nPret: %5.2f\nSerie: %c\n", t.id, t.RAM, t.producator, t.pret, t.serie);
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
		printf("\n");
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate > nrElemente) 
		nrElementeCopiate = nrElemente;

		struct Telefon* vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
		for (int i = 0;i < nrElementeCopiate;i++) {
			vectorNou[i] = copiazaTelefon(vector[i]);
		}
		return vectorNou;

}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente;i++) {
		free((*vector)[i].producator); //(*vector) pt a ajunge la continutul vectorului
		(*vector)[i].producator = NULL;
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pret_min, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pret_min) {
			(*dimensiune)++;
		}
	}
	*vectorNou = malloc((*dimensiune) * sizeof(struct Telefon));
	for (int i = 0, j = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pret_min) {
			(*vectorNou)[j] = copiazaTelefon(vector[i]);
			j += 1;
		}
	}
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon t;
	t.id = 1;

	return t;
}
	


int main() {
	struct Telefon t;
	t.id = 1;
	t = initializare(3,124,"Xiaomi", 10, 'O');
	afisare(t);
	int nrTelefoane = 3;
	int nrTelefoaneCopiate = 2;
	struct Telefon* telefoane=(struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane); //fara +1 deoarece este vector de elemente, nu caractere (pt \0)
	telefoane[0] = initializare(1, 8, "Samsung", 3000.5, 'A');
	telefoane[1] = initializare(2, 16, "iPhone", 5000.5, 'S');
	telefoane[2] = t;
	afisareVector(telefoane, nrTelefoane);
	struct Telefon* telefoaneCopiate = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrTelefoaneCopiate);
	printf("\nTelefoane copiate: \n");
	afisareVector(telefoaneCopiate, nrTelefoaneCopiate);
	dezalocare(&telefoaneCopiate,&nrTelefoaneCopiate);
	printf("\nTelefoane copiate dupa dezalocare: \n");
	afisareVector(telefoaneCopiate, nrTelefoaneCopiate);
	struct Telefon *vectorNou;
	int dimensiune;
	float pret_minim=1000.00;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, pret_minim,&vectorNou,&dimensiune);
	printf("\nTelefoane scumpe: \n");
	afisareVector(vectorNou,dimensiune);
	return 0;
}