#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define True 1

typedef struct Picture{ //ma complic prea mult
    char name[30];
    float size;
}Picture;

typedef struct Album{
    char name[30];
    Picture pic;
    struct Album *next,*prev;
    int index;
    float size;
}Album;

typedef struct Photos{
    Picture pic;
    struct Photos *next,*prev;
}Photos;

typedef struct Pic_list{
    Photos *first_pic,*last_pic;
}Pic_list;

int album = 1,option;
void meniu_principal();
void select_album();
void albumele_tale();
void edit_album();
Pic_list create_photo_list();
Photos* create_nod();
void add_pic(Pic_list *, char info[]);
void vizualizare_poze(Pic_list, Photos*);

int main()
{
    ///read_file
    char item[50];
    FILE *ptr;
    ptr = fopen("pictures6.txt","a+");
    if(ptr == NULL){
        printf("Eroare la deschiderea fisierului");
        return 0;
    }

    Pic_list av_pic;
    av_pic = create_photo_list();
    while(fgets(item,50,ptr) != NULL){
        add_pic(&av_pic,item);
    }
    //print(av_pic);
    fclose(ptr);
    while(True){
        meniu_principal(av_pic);
        break;
    }
    return 0;
}

void print(Pic_list l){
    Photos *item = l.first_pic;
    printf("%s %.2f\n",item->pic.name,item->pic.size);
    item = item->next;
    while(item != l.first_pic){
        printf("%s %.2f\n",item->pic.name,item->pic.size);
        item = item->next;
    }

}

Photos* create_nod(){
    Photos *item = malloc(sizeof(Photos));
    item->next = NULL;
    item->prev = NULL;
    return item;
}

void add_pic(Pic_list *av_pic, char info[]){
    char *aux;
    Photos *photo = create_nod();
    aux = strtok(info," ");
    strcpy(photo->pic.name,aux);
    aux = strtok(NULL," ");
    int length = strlen(aux)-1,comma=0;
    float nr=0,p=10;

    for(int i=0;i<length;i++){
        if(aux[i] == '.'){
            comma = 1;
            continue;
        }
        if(comma == 0)
            nr = nr*10 + (float)(aux[i] - 48);
        else{
            nr = nr + ((float)(aux[i] - 48))/p;
            p *= 10;
        }

        photo->pic.size = nr;
    }

    if(av_pic->first_pic == NULL){
        av_pic->first_pic = photo;
        av_pic->last_pic = av_pic->first_pic;
        photo->next = photo;
        photo->prev = photo;
    }
    else{
        photo->prev = av_pic->last_pic;
        av_pic->last_pic->next = photo;
        av_pic->last_pic = photo;
        photo->next = av_pic->first_pic;
        av_pic->first_pic->prev = photo;
    }
}

Pic_list create_photo_list(){
    Pic_list av_pic;
    av_pic.first_pic = NULL;
    av_pic.last_pic = NULL;
    return av_pic;
}

void vizualizare_poze(Pic_list list,Photos *item){
    //int option;
    printf("///// VIZUALIZARE POZE /////\n");
    printf("\n");
    printf("  %s  - %.2f MB\n",item->pic.name,item->pic.size);
    printf("\n");
    printf("1. Poza urmatoare\n");
    printf("2. Poza anterioara\n");//pentru prima afiseaza tot prima
    printf("3. Adauga in album nou\n");
    printf("4. Adauga in album existent\n");//printeaza nu exista d.e.c
    printf("5. Inapoi la meniul principal\n");
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    system("cls");
    switch(option){
        case 1:
            vizualizare_poze(list,item->next);
            break;
        case 2:
            vizualizare_poze(list,item->prev);
            break;
        case 3:
            //add_album_nou() / creaza_album + add poza
            break;
        case 4:
            //select_album();
            break;
        case 5:
            meniu_principal(list);
            break;
        default:
            system("cls");
            printf("%d nu e o optiune valabila\n",option);
            vizualizare_poze(list,item);
    }
}
void select_album(){
    //afisezi albume si il pui sa aleaga
}
void edit_album(){
    //int option;
    printf("///// EDITARE ALBUM /////");
    printf("\n\n");//vizualizare
    printf("1. Poza urmatoare\n");
    printf("2. Poza anterioara\n");//pentru prima afiseaza tot prima
    printf("3. Sterge\n");
    printf("4. Muta fotografia\n");//printeaza nu exista d.e.c
    printf("5. Redenumeste album\n");
    printf("6. Inapoi la meniul principal\n");
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    system("cls");
    switch(option){
        case 1:
            //poza_urmatoare()
            break;
        case 2:
            //poza anterioara()
            break;
        case 3:
            //serge_poza()
            break;
        case 4:
            //muta_fotografia()
            break;
        case 5:
            //redenumire()
            break;
        case 6:
            meniu_principal();break;
        default:
            system("cls");
            printf("%d nu e o optiune valabila\n",option);
            edit_album();
    }
}
void albumele_tale(){
    //int option;
    printf("///// ALBUMELE TALE /////");
    printf("\n\n");//vizualizare albume
    printf("1. Urmatorul album\n");
    printf("2. Albumul anterior\n");
    printf("3. Editeaza\n");
    printf("4. Sterge\n");
    printf("5. Dimensiune album\n");
    printf("6. Inapoi la meniul principal\n");
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    system("cls");
    switch(option){
        case 1:
            //urmatorul_album()
            break;
        case 2:
            //albumul_anterior()
            break;
        case 3:
            edit_album();
            break;
        case 4:
            //sterge album() +"are you sure?"
            break;
        case 5:
            //dim_album
        case 6:
            meniu_principal();
            break;
        default:
            system("cls");
            printf("%d nu e o optiune valabila\n",option);
            albumele_tale();
    }

}
void meniu_principal(Pic_list list){
    //int option;
    printf("///// MENIU PRINCIPAL /////\n");
    printf("1. Vezi lista de fotografii\n");
    //if(album == 1)
    printf("2. Albumele tale\n"); // afiseaza nu exista d.e.c +op creaza album
    printf("3. Opreste aplicatia\n");
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    system("cls");
    switch(option){
        case 1:
            vizualizare_poze(list,list.first_pic);break;
        case 2:
            albumele_tale();
            break;
        case 3:
            //return 0;
            break;
        default:
            system("cls");
            printf("%d nu e o optiune valabila\n",option);
            meniu_principal(list);
    }

}