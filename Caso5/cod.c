#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void invertPalheta(palheta *cores,palheta *novasCores){
	int i;
	for(i=0;i<256;i++){
		novasCores[i].blue = 255 - cores[i].blue;
		novasCores[i].red = 255 - cores[i].red;
		novasCores[i].green = 255 - cores[i].green;
	}
}

void cinzaPalheta(palheta *cores,palheta *novasCores){
	int i,media;
	for(i=0;i<256;i++){
		media = (cores[i].green+cores[i].blue+cores[i].red)/3;
        novasCores[i].blue = novasCores[i].red = novasCores[i].green = media;
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
	printf("CABECALHO:\n");
	printf("Iniciais: %s\n", cabecalho->inicial);
	printf("Tamanho do arquivo: %d\n", cabecalho->size_bmpfile);
	printf("Reservado: %d\n", cabecalho->reservado);
	printf("Deslocamento, em bytes, para o inicio da area de dados: %d\n",cabecalho->BfOffSetBits);
	printf("Tamanho em bytes do segundo cabecalho: %d\n",cabecalho->tamOutroCacabecalho);
	printf("Resolucao: %d x %d\n",cabecalho->largura,cabecalho->altura);
	printf("Numero  de planos: %d\n",cabecalho->numPlanos);
	printf("Bits por pixel: %d\n",cabecalho->bytesPorPixel);
	printf("Compressao usada: %d\n",cabecalho->compreensao); 
	printf("Tamanho imagem: %d\n",cabecalho->BiSizeImag);
	printf("Resolucao horizontal: %d pixel por metro\n",cabecalho->resoHorizontalPixelPorMetro);
	printf("Resolucao Vertical: %d pixel por metro\n",cabecalho->resoVerticalPixelPorMetro);
	printf("Numero de cores usadas: %d\n",cabecalho->numCoresUsadas);
	printf("Numero de cores importantes: %d\n",cabecalho->allImportants);
}

void imprimePalheta(palheta *cores){
	int i;
	for(i=0;i<256;i++){
		printf("Paleta[%d]: R:%d G:%d B:%d\n",i,cores[i].red,cores[i].green,cores[i].blue);
	}

}

int calculaPadding(Header *cabecalho){
	if(cabecalho->largura % 4 != 0){
		return 4 - (cabecalho->largura % 4);
	}
	return 0;
}

void readData(FILE *arquivo,Header *cabecalho,int **matriz,int numPadding){
	int i,j,k;
	
	char a;
	for(i=cabecalho->altura-1;i>=0;i--){
		for(j=0;j<cabecalho->largura;j++){
			fread(&matriz[i][j],1,1,arquivo);
		}
		if(numPadding != 0){
			for(k=0;k<numPadding;k++){
				fread(&a,1,1,arquivo);
			}
		}
	}
	
}

void imprimeMatriz(int **matriz,Header *cabecalho,int numPadding){
	int i,j,k;
	
	for(i=0;i<cabecalho->altura;i++){
		for(j =0;j<cabecalho->largura;j++){
			printf("%d ",matriz[i][j]);
			if(j == cabecalho->largura -1){
				for(k=0;k<numPadding;k++){
					printf("$");
					if(k == numPadding -1)
						printf("\n");
					else
						printf(" ");
				}
			}
		}
	}
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

char *formarNomeNovoArquivo(int isNeg,char *nome){
	char *final;

	char negativo[] = "Negativo.bmp";
	char pretoBranco[] = "PretoBranco.bmp";

	if(isNeg){
		final = (char *) malloc(strlen(negativo)+(strlen(nome)-3));
		strncpy(final,nome,strlen(nome)-4);
		final = strcat(final,negativo);
		final[strlen(negativo)+(strlen(nome)-3)-1] = 0;
	}
	else{
		final = (char *) malloc(strlen(pretoBranco)+(strlen(nome)-3));
		strncpy(final,nome,strlen(nome)-4);
		final = strcat(final,pretoBranco);
		final[strlen(pretoBranco)+(strlen(nome)-3)-1] = 0;
	}

	return final;
}

int main(){
    FILE *origem;
    FILE *destino;
    Header *cabecalho;
    palheta cores[256];
    palheta novasCores[256];
    char *imagem;
	char *imagemFinal;
	long long int *soma;

    int **matriz;
    int **novaMatriz;
    int i,j,k,l;
	int op;
    
    scanf("%m[^\n\r]",&imagem);
    
	scanf("%d",&op);

    origem = fopen(imagem,"rb");

	if(origem == NULL){
		printf("Erro no arquivo\n");
		free(imagem);
		return 0;
	}
    
    initializeVector255bytes(cores);
    initializeVector255bytes(novasCores);

	cabecalho = readHeader(origem);

	if(cabecalho->inicial[0]!='B' || cabecalho->inicial[1] != 'M'){
		printf("Arquivo não é do formato BMP\n");
		fclose(origem);
		free(imagem);
		free(cabecalho);
		return 0;
	}

	if(cabecalho->reservado != 0 || cabecalho->numPlanos != 1){
		printf("Erro no arquivo\n");
		fclose(origem);
		free(imagem);
		free(cabecalho);
		return 0;
	}
	
	soma = (long long int *) calloc(cabecalho->altura,sizeof(long long int));

	matriz = allocateMatrix(cabecalho->altura,cabecalho->largura);
	novaMatriz = allocateMatrix(cabecalho->altura,cabecalho->largura);
	
	readPalheta(cores,origem);
	
	switch(op){
		//Foto Negativa
		case 1: 
			imagemFinal = formarNomeNovoArquivo(1,imagem);
			invertPalheta(cores,novasCores);
			break;
		//Preta e Branca
		case 2:
			imagemFinal = formarNomeNovoArquivo(0,imagem);
			cinzaPalheta(cores,novasCores);
			break;

		default:
			break;
	}
 	
 	showHeader(cabecalho);

	printf("PALETA ORIGINAL:\n");
	imprimePalheta(cores);
	printf("PALETA NOVA:\n");
	imprimePalheta(novasCores);

	int numPadding = calculaPadding(cabecalho);

	destino = fopen(imagemFinal,"wb");

	readData(origem,cabecalho,matriz,numPadding);
	
	//imprimeMatriz(matriz,cabecalho,numPadding);
	writeHeader(destino,cabecalho);
	
	writePalheta(novasCores,destino);
	
	//showHeader(cabecalho);

	//Escreve
	char lixo = '$';
	for(i=cabecalho->altura-1;i>=0;i--){
		for(j=0;j<cabecalho->largura;j++){
			soma[i] += matriz[i][j];
			fwrite(&matriz[i][j],1,1,destino);
		}
		if(numPadding != 0){
			for(k=0;k<numPadding;k++){
				fwrite(&lixo,1,1,destino);
				soma[i]--;
			}
		}
	}

	for(i=0;i<cabecalho->altura;i++){
		printf("Soma linha %d: %lld\n",i,soma[i]);
	}

	fclose(origem);
	fclose(destino);

	printf("%s\n",imagemFinal);

	return 0;
}
