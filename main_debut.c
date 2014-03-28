#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "von_neumann.h"
#include "aes.h"
#include "mersenne_twister.h"

#define ARRAY_MAX_SIZE 1000
#define OLDRAND_MAX 2147483647

static unsigned int next;

int rdtsc()
{
	// cette fonction suivante cree un warning : c'est normal.
	__asm__ __volatile__("rdtsc");
}

void oldinit_rand(unsigned int seed)
{
	next = seed;
}

unsigned int oldrand()
{
	next = next * 1103515245 + 12345;
	return (unsigned int)(next % OLDRAND_MAX);
}


//Return the most significant value of oldrand()
int randMostSignificantValue( )
{
	return (oldrand()>>27)&0x0F;
}

//Return the least significant value of oldrand()
int randLeastSignificantValue( )
{
	return oldrand()&0x0F;
}


int sobs(int n, int d)
{
	int r=0;
	while(d--)
	{
		r+=2*(n%2)-1;
		n>>1;
	}
	return r;
}


int main()
{
	word16 x=1111; // nombre entre 1000 et 9999 pour Von Neumann
	struct mt19937p mt; // Pour Mersenne-Twister
	int tmp = rand(), seed; // Pour Mersenne-Twister
	u32 Kx[NK], Kex[NB*NR], Px[NB]; // pour l'AES

	int j;
	unsigned int output_msv;
	unsigned int output_lsv;
	unsigned int output_rand; // sortie du rand du C	
	word32 output_AES; // sortie pour l'AES
	word16 output_VN; // sortie pour pour Von Neumann
	word32 output_MT; // sortie pour Mersenne-Twister

	int sobs_msv;
	int sobs_lsv;
	int sobs_VN;
	int sobs_MT;
	int sobs_AES;

	double p_msv;
	double p_lsv;
	double p_VN;
	double p_MT;
	double p_AES;

	FILE *fLeastSignificantValue;
	FILE *fMostSignificantValue;
	FILE *fVonNeumann;
	FILE *fMersenneTwister;
	FILE *fAES;

	// initialisation des graines des generateurs
	srand(rdtsc());  // rand du C 
	seed = rand();
	oldinit_rand(seed);
	sgenrand(time(NULL)+(tmp), &mt); // Mersenne-Twister
	// Initialisation de la clé et du plaintext pour l'AES 
	// 45 est un paramètre qui doit changer à chaque initialisation
	init_rand(Kx, Px, NK, NB, 45);
	KeyExpansion(Kex,Kx); // AES : sous-clefs


	// sorties des generateurs	
	output_rand = randMostSignificantValue( ); // rand du C
	output_VN = Von_Neumann(&x); // Von Neumann
	output_MT = genrand(&mt); // Mersenne-Twister
	output_AES = AES(Px, Kex); // AES

	// affichage
	printf("- Generation de nombres aleatoires -\n");
	printf("rand du C : %u \n",output_rand); 
	printf("Von Neumann : %u\n",output_VN);
	printf("Mersenne Twister : %u\n",output_MT);
	printf("AES : %u\n",output_AES);


	fLeastSignificantValue=fopen("LeastSignificant.txt","w+");
	fMostSignificantValue=fopen("MostSignificant.txt","w+");
	fVonNeumann=fopen("VonNeumann.txt","w+");
	fMersenneTwister=fopen("MersenneTwister.txt","w+");
	fAES=fopen("AES.txt","w+");


	sobs_msv=0;
	sobs_lsv=0;
	sobs_VN=0;
	sobs_MT=0;
	sobs_AES=0;

	for(j=0;j<1024;j++)
	{
		// sorties des generateurs
		output_msv = randMostSignificantValue(); // rand du C
		output_lsv = randLeastSignificantValue();
		output_VN = Von_Neumann(&x); // Von Neumann
		output_MT = genrand(&mt); // Mersenne-Twister
		output_AES = AES(Px, Kex); // AES

		sobs_msv+=sobs(output_msv,4);
		sobs_lsv+=sobs(output_lsv,4);
		sobs_VN+=sobs(output_VN,13);//13 bits car Von neumann est modulé par 10 000
		sobs_MT+=sobs(output_MT,32);
		sobs_AES+=sobs(output_AES,32);


		fprintf(fLeastSignificantValue,"%u \n",output_msv);
		fprintf(fMostSignificantValue,"%u \n",output_lsv);
		fprintf(fVonNeumann,"%u \n",output_VN);
		fprintf(fMersenneTwister,"%u \n",output_MT);
		fprintf(fAES,"%u \n",output_AES);
	}

	p_msv=erfc((double)sobs_msv/sqrt(1024*4));
	p_lsv=erfc((double)sobs_lsv/sqrt(1024*4));
	p_VN=erfc((double)sobs_VN/sqrt(1024*13));
	p_MT=erfc((double)sobs_MT/sqrt(1024*32));
	p_AES=erfc((double)sobs_AES/sqrt(1024*32));

	printf("P_valeur\n");
	printf("MSV: %d\n",p_msv);
	printf("LSV: %d\n",p_lsv);
	printf("VN: %d\n",p_VN);
	printf("MT: %d\n",p_MT);
	printf("AES: %d\n",p_AES);

	return 0;
}
