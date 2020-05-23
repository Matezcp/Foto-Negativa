#include <stdio.h>
#include <stdlib.h>

//Neastrest neighbor para n�o alterar a palheta de cores

/* Struct que descreve o cabecalho do arquivo BMP */
typedef struct header
{
   char inicial[3];
   int size_bmpfile;
   int reservado;
   int BfOffSetBits;
   int tamOutroCacabecalho;
   int altura;
   int largura;
   int numPlanos;
   int bytesPorPixel;
   int compreensao;
   int BiSizeImag;
   int resoHorizontalPixelPorMetro;
   int resoVerticalPixelPorMetro;
   int numCoresUsadas;
   int allImportants;
} Header;

typedef struct palet{
	int blue;
	int green;
	int red;
	int reservado;
}palheta;

Header *readHeader(FILE *arquivo){
	Header *cabecalho = (Header *) calloc(sizeof(Header),1);
	fread(&cabecalho->inicial, sizeof(char), 2, arquivo);
	cabecalho->inicial[2] = '\0';
	fread(&cabecalho->size_bmpfile, sizeof(int), 1, arquivo);
	fread(&cabecalho->reservado, sizeof(int), 1, arquivo);
	fread(&cabecalho->BfOffSetBits, sizeof(int), 1, arquivo);
	fread(&cabecalho->tamOutroCacabecalho, sizeof(int), 1, arquivo);
	fread(&cabecalho->largura, sizeof(int), 1, arquivo);
	fread(&cabecalho->altura, sizeof(int), 1, arquivo);
	fread(&cabecalho->numPlanos,2, 1, arquivo);
	fread(&cabecalho->bytesPorPixel,2, 1, arquivo);
	fread(&cabecalho->compreensao,sizeof(int), 1, arquivo);
	fread(&cabecalho->BiSizeImag,sizeof(int), 1, arquivo);
	fread(&cabecalho->resoHorizontalPixelPorMetro,sizeof(int), 1, arquivo);
	fread(&cabecalho->resoVerticalPixelPorMetro,sizeof(int), 1, arquivo);
	fread(&cabecalho->numCoresUsadas,sizeof(int), 1, arquivo);
	fread(&cabecalho->allImportants,sizeof(int), 1, arquivo);
	
	return cabecalho;
}

void writeHeader(FILE *arquivo,Header *cabecalho){
	fwrite(&cabecalho->inicial, sizeof(char), 2, arquivo);
	fwrite(&cabecalho->size_bmpfile, sizeof(int), 1, arquivo);
	fwrite(&cabecalho->reservado, sizeof(int), 1, arquivo);
	fwrite(&cabecalho->BfOffSetBits, sizeof(int), 1, arquivo);
	fwrite(&cabecalho->tamOutroCacabecalho, sizeof(int), 1, arquivo);
	fwrite(&cabecalho->largura, sizeof(int), 1, arquivo);
	fwrite(&cabecalho->altura, sizeof(int), 1, arquivo);
	fwrite(&cabecalho->numPlanos,2, 1, arquivo);
	fwrite(&cabecalho->bytesPorPixel,2, 1, arquivo);
	fwrite(&cabecalho->compreensao,sizeof(int), 1, arquivo);
	fwrite(&cabecalho->BiSizeImag,sizeof(int), 1, arquivo);
	fwrite(&cabecalho->resoHorizontalPixelPorMetro,sizeof(int), 1, arquivo);
	fwrite(&cabecalho->resoVerticalPixelPorMetro,sizeof(int), 1, arquivo);
	fwrite(&cabecalho->numCoresUsadas,sizeof(int), 1, arquivo);
	fwrite(&cabecalho->allImportants,sizeof(int), 1, arquivo);
}

void readPalheta(palheta cores[255],FILE *arquivo){
	int i;
	for(i=0;i<256;i++){
		fread(&cores[i].blue,1,1,arquivo);
		fread(&cores[i].green,1,1,arquivo);
		fread(&cores[i].red,1,1,arquivo);
		fread(&cores[i].reservado,1,1,arquivo);
	}
}

void initializeVector255bytes(palheta *vector){
	int i;
	for(i=0;i<256;i++){
	vector[i].blue = 0;
	vector[i].green = 0;
	vector[i].red = 0;
	vector[i].reservado = 0;

	}	
}

int **allocateMatrix(int altura,int largura){
	int **matrix;
	int i;
	
	matrix = (int **) calloc(altura,sizeof(int*));
	for(i=0;i<altura;i++){
		matrix[i] = (int *) calloc(largura,sizeof(int));
	}
	
	return matrix;
}

void showHeader(Header *cabecalho){
	printf("------------------------------------------------------------------\n");
	printf("Iniciais:%s\n", cabecalho->inicial);
	printf("Size of the BMP file:%d\n", cabecalho->size_bmpfile);
	printf("DEVE SER 0: %d\n", cabecalho->reservado);
	printf("Especifica o deslocamento, em bytes, para o in�cio da �rea de dados: %d\n",cabecalho->BfOffSetBits);
	printf("Tamanho em bytes do segundo cabecalho: %d\n",cabecalho->tamOutroCacabecalho);
	printf("A imagem eh %d x %d\n",cabecalho->largura,cabecalho->altura);
	printf("DEVE SER 1: %d\n",cabecalho->numPlanos);
	printf("Bytes por pixel: %d\n",cabecalho->bytesPorPixel);
	printf("Compressao usada: %d\n",cabecalho->compreensao); 
	printf("Tamanho imagem: %d\n",cabecalho->BiSizeImag);
	printf("Resolucao horizontal: %d pixel por metro\n",cabecalho->resoHorizontalPixelPorMetro);
	printf("Resolucao Vertical: %d pixel por metro\n",cabecalho->resoVerticalPixelPorMetro);
	printf("Numero de cores usadas: %d\n",cabecalho->numCoresUsadas);
	printf("Numero de cores importantes: %d\n",cabecalho->allImportants);
	printf("------------------------------------------------------------------\n");
}

int calculaPadding(Header *cabecalho){
	if(cabecalho->largura % 4 != 0){
		return 4 - (cabecalho->largura % 4);
	}
	return 0;
}

int readData(FILE *arquivo,Header *cabecalho,palheta *novasCores,palheta *cores,int **matriz,int **novaMatriz,int numPadding){
	int i,j,k;
	int numCores = 0;
	
	char a;
	for(i=cabecalho->altura-1;i>=0;i--){
		for(j=0;j<cabecalho->largura;j++){
			int flag = -1;
			fread(&matriz[i][j],1,1,arquivo);
			palheta nova;
			nova.blue = 255 - cores[matriz[i][j]].blue;
			nova.green = 255 - cores[matriz[i][j]].green;
			nova.red = 255 - cores[matriz[i][j]].red;
			nova.reservado = 0;
			for(k=0;k<numCores;k++){
				if(novasCores[k].blue == nova.blue && novasCores[k].green == nova.green && novasCores[k].red == nova.red){
					flag = k;
					break;
				}
			}
			if(flag == -1){
				novasCores[numCores] = nova;
				novaMatriz[i][j] = numCores;
				numCores++;
			}
			else{
				novaMatriz[i][j] = flag;
			}
		}
		if(numPadding != 0){
			for(k=0;k<numPadding;k++){
				fread(&a,1,1,arquivo);
			}
		}
	}
	
	return numCores;
}

void writePalheta(palheta *novasCores,FILE *arquivo){
	int i;
	
	for(i=0;i<256;i++){
		fwrite(&novasCores[i].blue,1,1,arquivo);
		fwrite(&novasCores[i].green,1,1,arquivo);
		fwrite(&novasCores[i].red,1,1,arquivo);
		fwrite(&novasCores[i].reservado,1,1,arquivo);
	}
	
}

int main(){
    FILE *origem;
    FILE *destino;
    Header *cabecalho;
    palheta cores[256];
    palheta novasCores[256];
    char imagem[50];

    int **matriz;
    int **novaMatriz;
    int i,j,k,l;
    
    scanf("%s",imagem);
    
    origem = fopen(imagem,"rb");
    destino = fopen("final.bmp","wb");
    
    initializeVector255bytes(cores);
    initializeVector255bytes(novasCores);

	cabecalho = readHeader(origem);
	
	matriz = allocateMatrix(cabecalho->altura,cabecalho->largura);
	novaMatriz = allocateMatrix(cabecalho->altura,cabecalho->largura);
	
	readPalheta(cores,origem);
 	
 	showHeader(cabecalho);

	int numPadding = calculaPadding(cabecalho);

	int numCores = readData(origem,cabecalho,novasCores,cores,matriz,novaMatriz,numPadding);
	
	cabecalho->numCoresUsadas = 0;
	if(numCores == 256){
		cabecalho->allImportants = 0;
	}
	else{
		cabecalho->allImportants = numCores;
	}
	
	writeHeader(destino,cabecalho);
	
	writePalheta(novasCores,destino);
	
	showHeader(cabecalho);

	char lixo = '$';
	for(i=cabecalho->altura-1;i>=0;i--){
		for(j=0;j<cabecalho->largura;j++){
			fwrite(&novaMatriz[i][j],1,1,destino);
		}
		if(numPadding != 0){
			for(k=0;k<numPadding;k++){
				fwrite(&lixo,1,1,destino);
			}
		}
	}
	
	return 0;
}