#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Ogrenci { char ogrID[9]; char ad[30]; char soyad[30]; int puan; struct Ogrenci *next; struct Ogrenci *prev;}OGRENCI;
typedef struct Yazar { int yazarID; char yazarAd[30]; char yazarSoyad[30]; struct Yazar *next;}YAZAR;
typedef struct KitapOrnek { char EtiketNo[20]; char Durum[8]; struct KitapOrnek *next; }KITAP_ORNEK;
typedef struct Kitap {char kitapAdi[30]; char ISBN[14]; int adet; struct Kitap *next; struct KitapOrnek *head; }KITAP;
typedef struct KitapYazar{ char ISBN[14]; int yazarID;}KITAP_YAZAR;
typedef struct Tarih{ int gun:5; int ay:4; int yil:12;}TARIH;
typedef struct KitapOdunc{ char EtiketNo[20]; char ogrID[9]; int islemTipi:1; struct Tarih islemTarihi;}KITAP_ODUNC;

typedef void (*YazarFunc)(YAZAR**, int, char*, char*);
typedef void (*OgrenciFunc)(OGRENCI**, char*, char*, char*,int);
typedef void (*KitapFunc)(KITAP**, char*, char*, int);

void addYazar(YAZAR **head, int yazarID, char *yazarAd, char *yazarSoyad);
void addKitap(KITAP **head, char *kitapAdi, char *ISBN, int adet);

int countLines(char *filename) {
  int count = 0;
  char c;

  
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Dosya acilirken hata olustu");
    return -1;
  }

  
  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      count++;
    }
  }
  
  count++;

  fclose(fp);

  return count;
}

void initKitapOrnek(KITAP_ORNEK **head,char *ISBN, int adet){
	int i = 1;
	for(i; i<=adet;i++){
		KITAP_ORNEK *newOrnek = (KITAP_ORNEK*)malloc(sizeof(KITAP_ORNEK));
		sprintf(newOrnek->EtiketNo,"%s_%d",ISBN,i);
		strcpy(newOrnek->Durum,"RAFTA");
		newOrnek->next = NULL;
		
		if (*head == NULL) {
   			*head = newOrnek;
   		 
  		}else{
  			KITAP_ORNEK *curr = *head;
  			while(curr->next != NULL){
  				curr = curr->next;
			 }
			 
			 curr->next = newOrnek;
		  }
		
	}
	
}

void updateKitap(KITAP **head, char *kitapAdi, char* ISBN, int adet){
	char yeniAd[30]; 
	char yeniISBN[14]; 
	int yeniAdet;
	if (*head == NULL) {
    printf("Kitap Listesi bos.");
  }
  else{
  	KITAP *curr = *head;
  	while(strcmp(curr->kitapAdi, kitapAdi) != 0 && curr != NULL){
  		curr = curr->next;
	}
	printf("Kitabin Yeni Adi:");
  	fgets(yeniAd, sizeof(yeniAd), stdin); fflush(stdin);
  	printf("Kitabin Yeni ISBN:");
  	scanf("%s",yeniISBN); fflush(stdin);
  	printf("Kitabin Yeni adedi:");
  	scanf("%d",&yeniAdet); fflush(stdin);
  	strcpy(curr->kitapAdi,yeniAd);
  	strcpy(curr->ISBN, yeniISBN);
  	curr->adet = yeniAdet;
  	free(curr->head);
  	initKitapOrnek(&(curr->head),curr->ISBN,curr->adet);
	  
  }
}

void deleteKitap1(KITAP **head, char *kitapAdi, char* ISBN, int adet){
	if (*head == NULL) {
    printf("Kitap Listesi bos.");
  }
  else{
  	KITAP *curr = *head;
	KITAP *prev = NULL;
  	while(strcmp(curr->kitapAdi, kitapAdi) != 0 && curr != NULL){
  		 prev = curr;
	   	 curr = curr->next;
	}
	if(curr != NULL){
		if(prev == NULL){
			*head = curr->next;
			free(curr);
		}else{
			prev->next =curr->next;
			free(curr);
		}				
	}else{
		printf("Bu kitap listede yok.");
	}

	
  }		
}

void deleteKitap(KITAP **head, char *kitapAdi, char* ISBN, int adet) {
  if (*head == NULL) {
    printf("Linklist bos.");
  }else{
	  KITAP *curr = *head;
	  KITAP *prev = NULL;
	  while (curr != NULL && (strcmp(curr->kitapAdi, kitapAdi) != 0 || strcmp(curr->ISBN, ISBN) != 0)) {
	    prev = curr;
	    curr = curr->next;
	  }
	
	  if (curr == NULL) {
	    printf("Linklist bos2.");
	  }else{
		  if (adet >= curr->adet) {
		    curr->adet += adet;
		    if (prev == NULL) {
		      *head = curr->next;
		    } else {
		      prev->next = curr->next;
		    }
		    free(curr);
		    printf("Linklist bos3.");
		  }	  	
	  }
	
  	
  }

}


void addKitap(KITAP **head, char *kitapAdi, char *ISBN, int adet) {
  KITAP *new_node = malloc(sizeof(KITAP));
  strcpy(new_node->kitapAdi, kitapAdi);
  strcpy(new_node->ISBN, ISBN);
  new_node->adet = adet;
  new_node->next = NULL;
  new_node->head = NULL;

  if (*head == NULL) {
    *head = new_node;
    initKitapOrnek(&(new_node->head),new_node->ISBN,new_node->adet);
  }else{
	  KITAP *curr = *head;
	  KITAP *prev = NULL;
	  while (curr != NULL && strcmp(curr->kitapAdi, kitapAdi) < 0) {
	    prev = curr;
	    curr = curr->next;
	  }
	
	  if (prev == NULL) {
	    *head = new_node;
	  } else {
	    prev->next = new_node;
	  }
	  new_node->next = curr;
	  initKitapOrnek(&(new_node->head),new_node->ISBN,new_node->adet);
  }

}

void runKitapFunction(KITAP **head, KitapFunc func, char* kitapAdi, char* ISBN, int adet) {
  func(head, kitapAdi, ISBN, adet);
}

void readKitapFromCSV(KITAP **head) {
  
  FILE *fp = fopen("Kitaplar.CSV", "r");
  if (fp == NULL) {
    printf("Error opening Kitaplar.CSV");
    return;
  }

  char line[1024];
  while (fgets(line, sizeof(line), fp)) {
    char kitapAdi[30];
    char ISBN[14];
    int adet;
    sscanf(line,"%29[^,], %13[^,], %d",kitapAdi, ISBN, &adet);
    addKitap(head, kitapAdi, ISBN, adet);
  }
   
  fclose(fp);
}

void readYazarlarFromCSV(YAZAR **head, int* IdCount) {
	
    FILE *fp = fopen("Yazarlar.csv", "r");
    if (fp == NULL) {
        printf("Error: unable to open Yazarlar.csv\n");
    }else{
	    char line[1024];
	    while (fgets(line, sizeof(line), fp)) {
	    	
	        int yazarID;
	        char yazarAd[30];
	        char yazarSoyad[30];
	        sscanf(line, "%d, %29[^,], %29[^\n]", &yazarID, yazarAd, yazarSoyad);
			addYazar(head,yazarID,yazarAd,yazarSoyad);
	
	        (*IdCount)++;
	    }
	
	    fclose(fp);	
	}

}

void printYazarLinkList(YAZAR *head) {
    YAZAR *current = head;
    while (current != NULL) {
        printf("ID: %d, Ad: %s, Soyad: %s\n", current->yazarID, current->yazarAd, current->yazarSoyad);
        current = current->next;
    }
}

void readOgrencilerFromCSV(OGRENCI **head) {

    FILE *fp = fopen("Ogrenciler.csv", "r");
    if (fp == NULL) {
        printf("Error: unable to open ogrenciler.csv\n");
    }
	else{
	    char line[1024];
	    while (fgets(line, sizeof(line), fp)) {
	        char ogrID[8];
	        char ad[30];
	        char soyad[30];
	        int puan;
	        sscanf(line, "%8[^,], %29[^,], %29[^,], %d", ogrID, ad, soyad, &puan);
	
	        OGRENCI *newOgrenci = malloc(sizeof(OGRENCI));
	        strcpy(newOgrenci->ogrID, ogrID);
	        strcpy(newOgrenci->ad, ad);
	        strcpy(newOgrenci->soyad, soyad);
	        newOgrenci->puan = puan;
	        newOgrenci->next = NULL;
	        newOgrenci->prev = NULL;
	
	        if (*head == NULL) {
	            *head = newOgrenci;
	        } else {
	            OGRENCI *current = *head;
	            while (current->next != NULL) {
	                current = current->next;
	            }
	            current->next = newOgrenci;
	            newOgrenci->prev = current;
	        }
	    }
	
	    fclose(fp);
	}

}

void printOgrenciLinkList(OGRENCI *head) {
    OGRENCI *current = head;
    while (current != NULL) {
        printf("ogrID: %s, Ad: %s, Soyad: %s, Puan: %d\n", current->ogrID, current->ad, current->soyad, current->puan);
        current = current->next;
    }
}

void deleteOgrenciById(OGRENCI **head) {
    char id[9];

    printf("Enter the ID of the ogrenci to delete: ");
    scanf("%8s", id);

    OGRENCI *current = *head;
    OGRENCI *prev = NULL;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->ogrID, id) == 0) {
            found = 1;
        } else {
            prev = current;
            current = current->next;
        }
    }

    if (!found) {
        printf("Error: Ogrenci with ID %s not found\n", id);
    } else {
        if (prev == NULL) {
            *head = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }
}

void deleteOgrenciByName(OGRENCI **head) {
    char name[30];

    printf("Enter the name of the ogrenci to delete: ");
    scanf("%29s", name);

    OGRENCI *current = *head;
    OGRENCI *prev = NULL;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->ad, name) == 0) {
            found = 1;
        } else {
            prev = current;
            current = current->next;
        }
    }

    if (!found) {
        printf("Error: Ogrenci with name %s not found\n", name);
    } else {
        if (prev == NULL) {
            *head = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }
}

void addYazar(YAZAR **head, int yazarID, char *yazarAd, char *yazarSoyad) {
    YAZAR *newYazar = malloc(sizeof(YAZAR));
    newYazar->yazarID = yazarID;
    strcpy(newYazar->yazarAd, yazarAd);
    strcpy(newYazar->yazarSoyad, yazarSoyad);
    newYazar->next = NULL;

    if (*head == NULL) {
        *head = newYazar;
    } else {
        YAZAR *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newYazar;
    }
}

void deleteYazar(YAZAR **head, int yazarID, char *yazarAd, char *yazarSoyad) {
    int found = 0;
    if (*head == NULL) {
        printf("Error: cannot delete from an empty link list\n");
    } else if ((*head)->yazarID == yazarID && (*head)->next != NULL) {
        YAZAR *temp = *head;
        *head = (*head)->next;
        free(temp);
    } else if ((*head)->yazarID == yazarID && (*head)->next == NULL) {
        free(*head);
        *head = NULL;
    } else {
        YAZAR *current = *head;
        while (current != NULL && !found) {
            if (current->next != NULL && current->next->yazarID == yazarID) {
                YAZAR *temp = current->next;
                current->next = current->next->next;
                free(temp);
                found = 1;
            }
            current = current->next;
        }
    }

    if (!found) {
        printf("Error: yazar not found in the link list\n");
    }
}

void updateYazar(YAZAR **head, int yazarID, char *yazarAd, char *yazarSoyad) {
    if (head == NULL) {
        printf("Error: cannot update an empty link list\n");
    }else{
	    YAZAR *current = *head;
	    int found = 0;
	    while (current != NULL && !found) {
	        if (current->yazarID == yazarID) {
	            strcpy(current->yazarAd, yazarAd);
	            strcpy(current->yazarSoyad, yazarSoyad);
	            found = 1;
	        }
	        current = current->next;
	    }
	    
	    if(found == 0){
	    	
	    	printf("Error: yazar not found in the link list\n");
		}    	
	}


}

void runYazarFunction(YAZAR **head, YazarFunc func, int yazarID, char *yazarAd, char *yazarSoyad) {
    func(head, yazarID, yazarAd, yazarSoyad);
}

void addOgrenci(OGRENCI **head, char* ogrID, char* ad, char* soyad, int puan) {
  OGRENCI *newNode = (OGRENCI*) malloc(sizeof(OGRENCI));
  if (newNode == NULL) {
    printf("Failed to allocate memory for the new node\n");
    return;
  }else{
	  strcpy(newNode->ogrID, ogrID);
	  strcpy(newNode->ad, ad);
	  strcpy(newNode->soyad, soyad);
	  newNode->puan = puan;
	
	  newNode->next = NULL;
	  newNode->prev = NULL;
	
	
	  if (*head == NULL) {
	    *head = newNode; 
	  } else {
	    OGRENCI *curr = *head;
	    while (curr->next != NULL) {
	      curr = curr->next;
	    }
	
	    curr->next = newNode;
	    newNode->prev = curr;
	  }  	
  }


}

void updateOgrenci(OGRENCI **head, char* ogrID, char* ad, char* soyad, int puan) {
  OGRENCI *curr = *head;  
  int flag = 0; 

  if (curr == NULL) {
    printf("The list is empty\n");
  } else {
    while (curr != NULL && flag == 0) {
      if (strcmp(curr->ogrID, ogrID) == 0) {
        flag = 1;  
        strcpy(curr->ad, ad);
        strcpy(curr->soyad, soyad);
        curr->puan = puan;
      }
      curr = curr->next;
    }
  	if (flag == 0) {
    	printf("The node was not found\n");
	}
  }
}

void deleteOgrenci(OGRENCI **head, char* ogrID, char* ad, char* soyad, int puan) {
  OGRENCI *curr = *head;  
  int flag = 0;  

  if (curr == NULL) {
    printf("The list is empty\n");
  } else {
    while (curr != NULL && flag == 0) {
      if (strcmp(curr->ogrID, ogrID) == 0 && strcmp(curr->ad, ad) == 0 && strcmp(curr->soyad, soyad) == 0 && curr->puan == puan) {
        flag = 1;  
        if (curr == *head) {
          *head = curr->next;
          (*head)->prev = NULL;
        }
        else if (curr->prev != NULL && curr->next != NULL) {
          curr->prev->next = curr->next;
          curr->next->prev = curr->prev;
        }
        else if (curr->prev != NULL && curr->next == NULL) {
          curr->prev->next = NULL;
        }

        free(curr);
      }
      curr = curr->next;
    }
  }

  if (flag == 0) {
    printf("The node was not found\n");
  }
}

void runOgrenciFunction(OGRENCI **head, OgrenciFunc func, char* ogrID, char* ad, char* soyad, int puan) {
  func(head, ogrID, ad, soyad, puan);
}

int isOgrenciIdAvailable(OGRENCI **head, char* ogrID) {
  OGRENCI *curr = *head;
  
  while (curr != NULL) {
    if (strcmp(curr->ogrID, ogrID) == 0) {
      return 0;  // found
    }
    curr = curr->next;
  }
  
  if(head==NULL){
	printf("List is empty");
  }
  
  return 1;  
}

void printKitaplarWithCopies(KITAP *kitapLinklist) {
  KITAP *curr = kitapLinklist;
 
  while (curr != NULL) {
    printf("Kitap Adi: %s\n", curr->kitapAdi);
    printf("ISBN: %s\n", curr->ISBN);
    printf("Adet: %d\n", curr->adet);
    
    KITAP_ORNEK *subCurr = curr->head;
	printKitapOrnek(subCurr);
    curr = curr->next;
  }
}

void printKitaplarWithoutCopies(KITAP *kitapLinklist) {
  KITAP *curr = kitapLinklist;
 
  while (curr != NULL) {
    printf("Kitap Adi: %s\n", curr->kitapAdi);
    printf("ISBN: %s\n", curr->ISBN);
    printf("Adet: %d\n", curr->adet);
    printf("\n");
    curr = curr->next;
  }
}

void printKitapOrnek(KITAP_ORNEK *head){
	KITAP_ORNEK *curr = head;
	printf("--KITAP ORNEKLERI--\n");
    while (curr != NULL){
    	
    	printf("%s %s\n",curr->EtiketNo, curr->Durum);
    
    	curr = curr->next;
    	
	}
	printf("----------------------\n");
}

KITAP_YAZAR* initKitapYazarArray(int arrLength){
	
	KITAP_YAZAR* arr =(KITAP_YAZAR*)malloc(arrLength * sizeof(KITAP_YAZAR));
	int i;
	
	FILE *fp = fopen("KitapYazar.csv", "r");
    if (fp == NULL) {
        printf("Error: unable to open KitapYazar.csv\n");
        fclose(fp);
        return ;
    }
    
	rewind(fp);
    
	for (i = 0; i < arrLength; i++) {
        fscanf(fp, "%13[^,], %d\n", arr[i].ISBN, &arr[i].yazarID);
    }
    
    fclose(fp);
    
    return arr;
}

void printKitapYazarArray(KITAP_YAZAR* arr, int length){
	int i;
	for(i=0; i<length; i++){
		printf("%s, %d\n",arr[i].ISBN, arr[i].yazarID);
	}
}

void updateKitapYazarArray(KITAP_YAZAR* arr, char* ISBN, int yazarID, int length, int flag){
	int i;
	if(flag==1){
		for(i=0; i<length; i++){
			if(arr[i].yazarID == yazarID){
				arr[i].yazarID = -1;
			}
		}
	}
}

//flag dondurecek fonksiyon
int isInYazarLinklist(YAZAR *head, int yazarID){

	while(head != NULL){
		if(head->yazarID == yazarID){
			return 1;
		}
	}
	
	return 0;
}

int matchKitapYazar(KITAP_YAZAR** array, int* array_size, char* ISBN, int yazarID) {
    KITAP_YAZAR element;
    strcpy(element.ISBN, ISBN);
    element.yazarID = yazarID;

    if (*array_size == 0) {

        *array = malloc(sizeof(KITAP_YAZAR));
        if (*array == NULL) {
            perror("Error allocating memory");
            return -1;
        }
        *array_size = 1;
    } else {

        KITAP_YAZAR* temp = realloc(*array, (*array_size + 1) * sizeof(KITAP_YAZAR));
        if (temp == NULL) {
            perror("Error allocating memory");
            return -1;
        }
        *array = temp;
        (*array_size)++;
    }

    (*array)[*array_size - 1] = element;

    return 0;
}

void printOgrenci(OGRENCI **head, char* ogrID, char* ad, char* soyad, int puan) {
  OGRENCI *curr = *head;  
  int flag = 0;  

  if (curr == NULL) {
    printf("The list is empty\n");
  } else {

    while (curr != NULL && flag == 0) {
      if (strcmp(curr->ogrID, ogrID) == 0) {
        flag = 1;  
        printf("%s %s %s %d",curr->ogrID, curr->ad, curr->soyad, curr->puan);
      }
      curr = curr->next;
    }
    
  	if (flag == 0) {
    	printf("The node was not found\n");
	}
  }
}

int main(){
	char empty;
	int option=2;
	YAZAR *yazarLinklist = NULL;
	OGRENCI *ogrenciLinklist = NULL;
	KITAP *kitapLinklist = NULL;
	KITAP_ODUNC* kitapOduncLinklist = NULL;
	
	int nextYazarIdCount = 1; // bir sonraki eklenecek yazarin id si tutuluyor.
	int lineCount;//kitap yazar eslestirmesinde dinamik array icin size counter
	
	//gecici degiskenler
	char ogrID[9];
  	char ogrAd[30];
  	char ogrSoyad[30];
  	int ogrPuan = 100;
	int islem;
	
	char kitapAdi[30]; 
	char kitapISBN[14]; 
	int kitapAdet;
	//func ptr arrayleri
	YazarFunc yazarFuncs[3] = {&addYazar, &deleteYazar, &updateYazar};
	OgrenciFunc ogrenciFuncs[4]={&addOgrenci, &deleteOgrenci, &updateOgrenci, &printOgrenci};
	KitapFunc kitapFuncs[3]={&addKitap, &deleteKitap, &updateKitap};
	
	readKitapFromCSV(&kitapLinklist);
	readYazarlarFromCSV(&yazarLinklist, &nextYazarIdCount);
	readOgrencilerFromCSV(&ogrenciLinklist);
	printKitaplarWithoutCopies(kitapLinklist);
	char *fname = "KitapYazar.csv";
	lineCount = countLines(fname);
	KITAP_YAZAR* kitapYazarArray = initKitapYazarArray(lineCount);
	
	//printKitapYazarArray(kitapYazarArray, lineCount);
	//printYazarLinkList(yazarLinklist);
	//printOgrenciLinkList(ogrenciLinklist);
	//deleteOgrenciById(&ogrenciLinklist);
		
	//OGRENCI ekleme-silme-guncelleme
/*	
	char eogrID[9]="21011616";
  	char ad[30] = "ATA CAN";
  	char soyad[30] = "TURK";
  	int puan =100;
	
	runOgrenciFunction(&ogrenciLinklist, ogrenciFuncs[0],eogrID,ad,soyad,puan);
	printf("\n");
	printOgrenciLinkList(ogrenciLinklist);
	
*/

	//YAZAR ekleme-silme-guncelleme
	/*
	int yazarID =9;
    char yazarAd[30]="atacan";
    char yazarSoyad[30]="turk";	
	
	
	runYazarFunction(&yazarLinklist,yazarFuncs[0],yazarID,yazarAd,yazarSoyad);
	printYazarLinkList(yazarLinklist);
	
	char yazarAd2[30]="atacaaaaaan";
	runYazarFunction(&yazarLinklist,yazarFuncs[2],yazarID,yazarAd2,yazarSoyad);
	printYazarLinkList(yazarLinklist);

	runYazarFunction(&yazarLinklist,yazarFuncs[1],yazarID,yazarAd2,yazarSoyad);
	printYazarLinkList(yazarLinklist);
	*/
	
	while(option!=15){
		printf("\n\nmenuyu gormek icin bir tusa basip enter'a basiniz.\n");
		fflush(stdin);
		scanf("%c",&empty);
		fflush(stdin);
		printf("ODEV TESLIM TARIHINI, 4 OCAKTAKI ING SPEAKING SINAVIMIN TARIHI ILE KARISTIRDIGIMDAN DOLAYI ODEV TAMAMLANAMADI");
		printf("\n1- Ogrenci Ekle, Sil, Guncelle:\n2- Ogrenci Bilgisi Goruntuleme:\n3- Kitap Teslim Etmemis Ogrencileri Listele:\n4- Cezali Ogrencileri Listele:");
		printf("\n5- Tum Ogrencileri Listele:\n6- Kitap Odunc Al/Teslim Et\n7- Kitap Ekle, Sil, Guncelle: \n8- Kitap Bilgisi Goruntuleme:");
		printf("\n9- Raftaki Kitaplari Listele:\n10- Zamaninda Teslim Edilmeyen Kitaplari Listele:\n11- Kitap-Yazar Eslestir : \n12- Kitabin Yazarini Guncelle:\n");
		printf("\n13- Yazar Ekle, Sil, Guncelle:\n14- Yazar Bilgisi Goruntuleme:\nSECIMINIZ:");
		scanf("%d",&option);
		printf("\n");
		switch (option) {
		  case 1:
		  	printf("1-Ogrenci Ekle\n");
			printf("2-Ogrenci Sil\n");
			printf("3-Ogrenci Guncelle\n");
			printf("Seciminiz : ");
			scanf("%d",&islem);
			fflush(stdin);
		    printf("ogrID ad soyad puan  (birer bosluklu giriniz. Guncelleme yapiyorsaniz ogrID'den sonra ad soyad ve puanin yeni halini giriniz):");
	  		fflush(stdin);
			scanf("%s %s %s %d", ogrID, ogrAd, ogrSoyad, &ogrPuan);
	  		runOgrenciFunction(&ogrenciLinklist, ogrenciFuncs[islem-1],ogrID,ogrAd,ogrSoyad,ogrPuan);
	  		printf("\n%s numarali ogrenci eklendi.\n",ogrID);
		    break;
		  case 2:
		  	
		    
	  		printf("ogrID ad soyad  (birer bosluklu giriniz):");
	  		scanf("%s %s %s", ogrID, ogrAd, ogrSoyad);
	  		runOgrenciFunction(&ogrenciLinklist, ogrenciFuncs[3],ogrID,ogrAd,ogrSoyad,ogrPuan);
	  		printf("\n");
	  		
		    break;
		  case 3:
		    // code block
		    break;
		  case 4:
		    // code block
		    break;
		  case 5:
		    printOgrenciLinkList(ogrenciLinklist);
		    
		    break;
		  case 6:
		    // code block
		    break;
		  case 7:
		  	fflush(stdin);
		    printf("1-Kitap Ekle\n");
			printf("2-Kitap Sil\n");
			printf("3-Kitap Guncelle\n");
			printf("Seciminiz : ");
			fflush(stdin);
			scanf("%d",&islem);
			
		    fflush(stdin);
		    printf("Kitabin Adi:");
  			fgets(kitapAdi, sizeof(kitapAdi), stdin); fflush(stdin);
  			printf("Kitabin ISBN:");
  			scanf("%s",kitapISBN); fflush(stdin);
  			printf("Kitabin adedi:");
  			scanf("%d",&kitapAdet); fflush(stdin);
  			
			runKitapFunction(&kitapLinklist,kitapFuncs[islem-1],kitapAdi,kitapISBN,kitapAdet);
  			printKitaplarWithoutCopies(kitapLinklist);
		    break;
		  case 8:
		    // code block
		    break;
		  case 9:
		    // code block
		    break;
		  case 10:
		    // code block
		    break;
		  case 11:
		    // code block
		    break;
		  case 12:
		    // code block
		    break;
		  case 13:
		    // code block
		    break;
		  case 14:
		    // code block
		    break;
		  //default:
		    // code block
		}
}
	return 0;
}
