#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define True 1
#define GREEN 2
#define BLUE 9
#define LIGHT_BLUE 11
#define RED 12
#define YELLOW 14
#define WHITE 15

typedef struct Picture{
    char name[30];
    double size;
    int index;
    struct Picture *next,*prev;
}Picture;

typedef struct Album{
    char album_name[30];
    double size;
    int index;
    Picture *first_pic,*last_pic;
    struct Album *next,*prev;
}Album;

typedef struct Albums{
    Album *first,*last;

}Albums;

typedef struct Pic_list{
    Picture *first_pic,*last_pic;
}Pic_list;

int album_flag = 0,option;
void meniu_principal();
void select_album(Picture*);
void albumele_tale();
void edit_album(Album*);
Pic_list create_photo_list();
Picture* create_nod();
void add_pic(char *);
void vizualizare_poze(Picture*);
Album* creare_album();
Albums* create_albums();
void create_new_album(Picture*);
void menu_for_new_album(Picture*);
void print_album(Album*);
void add_photo(int, Picture*);
void add_photo_menu(Album*, Picture*);
void view_album(Album*);
Album* create_stored_album(char *);
double get_num(char *);
void vezi_albume();
void view_album_meniu(Album *);
void verify(Album *);
void dim_album(Album *);
void delete_photo(Album *);
void rename_album(Album *);
double get_size(char *);
void store_data();
void set_color(int);

Pic_list av_pic;
Albums *albums;

int main()
{
    ///read_file
    char item[50];
    FILE *ptr;
    ptr = fopen("pictures.txt","a+");
    if(ptr == NULL){
        printf("Eroare la deschiderea fisierului");
        return 0;
    }
    av_pic = create_photo_list();

    while(fgets(item,50,ptr) != NULL){
        add_pic(item);
    }

    fclose(ptr);
    FILE *ptr2;
    albums = create_albums();
    ///read created albums

    Album *album;
    ptr2 = fopen("albums.txt","a+");

    while(fgets(item,30,ptr2) != NULL && item[0] != '\n'){
        if(item[0] != '#'){
            album_flag = 1;
            album = create_stored_album(item);
            if(albums->first == NULL){
                albums->first = album;
                albums->last = album;
                album->index = 1;
                album->next = album->prev = album;
            }
            else{
                album->index = albums->last->index + 1;
                albums->last->next = album;
                album->next = albums->first;
                album->prev = albums->last;
                albums->last = album;
                albums->first->prev = album;
            }
        }
        if(item[0] == '#'){
            Picture *pic = create_nod();
            strncpy(pic->name,item,strlen(item)-1);
            pic->size = get_size(item);
            album->size += pic->size;
            if(album->first_pic == NULL){
                album->first_pic = pic;
                album->last_pic = pic;
                pic->index = 1;
                pic->next = pic;
                pic->prev = pic;
            }
            else{
                pic->index = album->last_pic->index + 1;
                album->last_pic->next = pic;
                album->first_pic->prev = pic;

                pic->prev = album->last_pic;
                pic->next = album->first_pic;


                album->last_pic = pic;
            }
        }
    }
    fclose(ptr2);

    while(True){
        meniu_principal();
        break;
    }
    return 0;
}

void set_color(int color_code) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color_code);
}

double get_size(char *info){
    Picture *pic;
    pic = av_pic.first_pic;
    while(strncmp(pic->name,info,strlen(info)-1) != 0)
        pic = pic->next;
    return pic->size;
}

Album* create_stored_album(char *info){
    Album *album;
    album = (Album*)malloc(sizeof(Album));
    strncpy(album->album_name,info,strlen(info)-1);
    album->size = 0;
    album->first_pic = NULL;
    album->last_pic = NULL;
    album->index = 0;
    return album;
}

void store_data(){
    if(albums->first != NULL && album_flag){
    FILE *ptr;
    ptr = fopen("albums.txt","w+");
    Album *alb;
    alb = albums->first;
    while(alb->next != albums->first){
        fprintf(ptr,"%s\n",alb->album_name);
        Picture *pic;
        pic = alb->first_pic;
        while(pic->next != alb->first_pic){
            fprintf(ptr,"%s\n",pic->name);
            pic = pic->next;
        }
        fprintf(ptr,"%s\n",pic->name);

        alb = alb->next;
    }
    fprintf(ptr,"%s\n",alb->album_name);
    Picture *pic;
    pic = alb->first_pic;
    while(pic->next != alb->first_pic){
        fprintf(ptr,"%s\n",pic->name);
        pic = pic->next;
    }
    fprintf(ptr,"%s\n",pic->name);

    fclose(ptr);
    }
    else
        fclose(fopen("albums.txt", "w"));
}
double get_num(char *info){
    double nr=0,p=10;
    int comma = 0;
    for(int i=0;i<strlen(info)-1;i++){
        if(info[i] == '.'){
            comma = 1;
            continue;
        }
        if(comma == 0)
            nr = nr*10 + (double)(info[i] - '0');
        else{
            nr = nr + ((double)(info[i] - '0'))/p;
            p *= 10;
        }
    }
    return nr;
}
void menu_for_new_album(Picture *item){

    printf("1. Inapoi la fotografii\n");
    printf("2. Meniu principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea:");
    scanf("%d",&option);
    set_color(WHITE);
    switch(option){
        case 1:
            system("cls");
            vizualizare_poze(item);
            break;
        case 2:
            system("cls");
            meniu_principal();
            break;
        default:
            system("cls");
            set_color(RED);
            printf("%d nu e o optiune valabila\n\n",option);
            set_color(WHITE);
            menu_for_new_album(item);

    }
}

void create_new_album(Picture *item){
    album_flag = 1;
    Album *new_album;
    new_album = creare_album();
    Picture *pic;
    pic = (Picture*)malloc(sizeof(Picture));
    strcpy(pic->name,item->name);
    pic->size = item->size;
    pic->next = pic;
    pic->prev = pic;
    new_album->first_pic = pic;
    new_album->first_pic->index = 1;
    new_album->last_pic = pic;
    new_album->size = item->size;
    if(albums->first == NULL){

        albums->first = new_album;
        albums->last = new_album;
        new_album->next = new_album;
        new_album->prev = new_album;
        new_album->index = 1;

    }
    else{
        new_album->index = albums->last->index + 1;
        albums->last->next = new_album;
        new_album->prev = albums->last;
        albums->last = new_album;
        new_album->next = albums->first;
        albums->first->prev = new_album;
    }
    store_data();
    system("cls");
    set_color(YELLOW);
    printf("Albumul a fost creat\n\n");
    set_color(WHITE);
    menu_for_new_album(item);
}
Albums* create_albums(){
    Albums *albums;
    albums = (Albums*)malloc(sizeof(Albums));
    albums->first = NULL;
    albums->last = NULL;
    return albums;
}

Album* creare_album(){
    char name[30];
    Album *album;
    album = (Album*)malloc(sizeof(Album));
    set_color(LIGHT_BLUE);
    printf("Introdu un nume pentru album\n");
    set_color(WHITE);
    printf("Nume album: ");
    fflush(stdin);
    gets(name);
    strcpy(album->album_name,name);
    album->size = 0;
    album->first_pic = NULL;
    album->last_pic = NULL;
    album->index = 0;
    return album;
}

void print(){
    Picture *item = av_pic.first_pic;
    printf("%s %.2f\n",item->name,item->size);
    item = item->next;
    while(item != av_pic.first_pic){
        printf("%s %.2f\n",item->name,item->size);
        item = item->next;
    }

}

Picture* create_nod(){
    Picture *item = malloc(sizeof(Picture));
    item->index = 0;
    item->next = NULL;
    item->prev = NULL;
    return item;
}

void add_pic(char info[]){
    char *aux;
    Picture *photo = create_nod();
    aux = strtok(info," ");
    strcpy(photo->name,aux);
    aux = strtok(NULL," ");
    int length = strlen(aux)-1,comma=0;

    photo->size = get_num(aux);


    if(av_pic.first_pic == NULL){
        av_pic.first_pic = photo;
        av_pic.last_pic = av_pic.first_pic;
        photo->next = photo;
        photo->prev = photo;
        photo->index = 1;
    }
    else{
        photo->index = av_pic.last_pic->index +1;
        photo->prev = av_pic.last_pic;
        av_pic.last_pic->next = photo;
        av_pic.last_pic = photo;
        photo->next = av_pic.first_pic;
        av_pic.first_pic->prev = photo;
    }
}

Pic_list create_photo_list(){
    Pic_list av_pic;
    av_pic.first_pic = NULL;
    av_pic.last_pic = NULL;
    return av_pic;
}

void vizualizare_poze(Picture *item){

    set_color(GREEN);
    printf("///// VIZUALIZARE POZE /////\n");
    printf("\n");
    set_color(WHITE);
    printf("  %s  \n",item->name);
    printf("\n");

    printf("1. Poza urmatoare\n");
    printf("2. Poza anterioara\n");
    printf("3. Adauga in album nou\n");
    printf("4. Adauga in album existent\n");
    printf("5. Inapoi la meniul principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
    case 1:
        vizualizare_poze(item->next);
        break;
    case 2:
        vizualizare_poze(item->prev);
        break;
    case 3:
        create_new_album(item);
        break;
    case 4:
        if(album_flag)
            select_album(item);
        else{
            system("cls");
            set_color(RED);
            printf("Nu exista albume create\n\n");
            set_color(WHITE);
            printf("1. Inapoi\n\n");
            set_color(LIGHT_BLUE);
            printf("Introdu optiunea: ");
            scanf("%d",&option);
            set_color(WHITE);
            while(option != 1){
                system("cls");
                set_color(RED);
                printf("%d nu e o optiune valabila\n",option);
                printf("Nu exista albume create\n\n");
                set_color(WHITE);
                printf("1. Meniu principal\n\n");
                set_color(LIGHT_BLUE);
                printf("Introdu optiunea: ");
                scanf("%d",&option);
                set_color(WHITE);
            }
            system("cls");
            vizualizare_poze(item);
        }
        break;
    case 5:
        meniu_principal();
        break;
    default:
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
        set_color(WHITE);
        vizualizare_poze(item);
    }
}

void view_album(Album *album){
    printf("Nume album: %s\n",album->album_name);
    Picture *current_pic;
    current_pic = album->first_pic;
    while(current_pic->next != album->first_pic){
        printf("%d. %s\n",current_pic->index,current_pic->name);
        current_pic = current_pic->next;
    }
        printf("%d. %s\n",current_pic->index,current_pic->name);
}

void add_photo_menu(Album *album,Picture *pic){
    set_color(YELLOW);
    printf("Fotografia a fost adaugata\n\n");
    set_color(WHITE);
    printf("1. Vizualizare album\n");
    printf("2. Inapoi la poze\n");
    printf("3. Meniu principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea:"); scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
        case 1:
            view_album_meniu(album);
            break;
        case 2:
            vizualizare_poze(pic);
            break;
        case 3:
            meniu_principal();
            break;
        default:
            system("cls");
            set_color(RED);
            printf("%d nu e o optiune valabila\n\n");
            set_color(WHITE);
            add_photo_menu(album,pic);
    }
}

void add_photo(int key,Picture *pic){

    Album *album;
    album = albums->first;
    while(album->index != key)
        album = album->next;
    ///o pun la final
    Picture *last_pic,*new_pic;
    album->size += pic->size;
    last_pic = album->first_pic->prev;
    new_pic = (Picture*)malloc(sizeof(Picture));
    strcpy(new_pic->name,pic->name);
    new_pic->index = last_pic->index+1;
    new_pic->size = pic->size;
    new_pic->prev = last_pic;
    new_pic->next = album->first_pic;
    last_pic->next = new_pic;
    album->last_pic = new_pic;
    album->first_pic->prev = new_pic;
    store_data();
    add_photo_menu(album,pic);
}

void vezi_albume(){
    if(album_flag){
    Album *item;
    item = albums->first;
    while(item != albums->last){
        printf("%d. %s\n",item->index,item->album_name);
        item = item->next;
    }
    printf("%d. %s\n",item->index,item->album_name);
    }
    else{
        set_color(RED);
        printf("Nu exista albume create\n\n");
    set_color(WHITE);
    }
}

void select_album(Picture *pic){
    set_color(YELLOW);
    printf("Selecteaza un album:\n\n");
    set_color(WHITE);

    vezi_albume();
    printf("0. Meniu principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea:");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    if(option > 0 && option <= albums->last->index)
        add_photo(option,pic);
    else if(option == 0)
        meniu_principal();
        else{
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
        set_color(WHITE);
        select_album(pic);
    }
}

void delete_photo(Album *item){
    set_color(YELLOW);
    printf("Alege o fotografie\n\n");
    set_color(WHITE);
    view_album(item);
    printf("0. Inapoi\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    if(!option)
        edit_album(item);
    else if(option > 0 && option <= item->last_pic->index){
            if(item->first_pic == item->last_pic)
                delete_album(item);
            else{
            Picture *pic;
            pic = item->first_pic;
            while(pic->index != option)
                pic = pic->next;
            Picture *aux = pic;
            item->size -= pic->size;
            ///index
            while(pic != item->last_pic){
                pic->index--;
                pic = pic->next;
            }
            pic->index--;
            pic = aux;
            ///stergere
            if(pic == item->first_pic)
                item->first_pic = pic->next;
            if(pic == item->last_pic)
                item->last_pic = pic->prev;
            pic->next->prev = pic->prev;
            pic->prev->next = pic->next;

            free(pic);
            store_data();
            set_color(YELLOW);
            printf("Fotografia a fost stearsa\n\n");
            set_color(WHITE);
            edit_album(item);
            }
    }
    else{
            set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
    set_color(WHITE);
        delete_photo(item);
    }
}

void rename_album(Album *item){
    set_color(GREEN);
    printf("///// REDENUMIRE ALBUM /////\n\n");
    set_color(WHITE);
    printf("Nume album: %s\n\n",item->album_name);
    set_color(LIGHT_BLUE);
    printf("Introdu noul nume: ");
    char *name;
    name = (char*)malloc(30*sizeof(char));
    fflush(stdin);
    gets(name);
    set_color(WHITE);
    strcpy(item->album_name,name);
    free(name);
    store_data();
    system("cls");
    edit_album(item);
}

Picture* get_pic(Album *item,int key){
    Picture *pic;
    pic = item->first_pic;
    while(pic->index != key)
        pic = pic->next;
    return pic;
}

void interschimbare(Album *item){
    set_color(GREEN);
    printf("///// INTERSCHIMBA 2 POZE /////\n\n");
    set_color(WHITE);
    view_album(item);
    printf("\n");
    set_color(LIGHT_BLUE);
    printf("Introdu prima poza: \n");
    int pic1,pic2;
    scanf("%d",&pic1);
    if(pic1 > 0 && pic1 <= item->last_pic->index){
    printf("Introdu a doua poza: \n");
    scanf("%d",&pic2);
    set_color(WHITE);
    if(pic2 > 0 && pic2 <= item->last_pic->index){

        Picture *fpic,*lpic;
        fpic = get_pic(item,pic1);
        lpic = get_pic(item,pic2);


        ///nume
        char naux[30];
        strcpy(naux,fpic->name);
        strcpy(fpic->name,lpic->name);
        strcpy(lpic->name,naux);
        system("cls");
        ///size
        double saux;
        saux = fpic->size;
        fpic->size = lpic->size;
        lpic->size = saux;


        store_data();
        edit_album(item);

    }
    else{
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",pic2);
        set_color(WHITE);
        interschimbare(item);
    }
    }
    else{
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",pic1);
        set_color(WHITE);
        interschimbare(item);
    }
}

void edit_album(Album *item){
    //system("cls");
    set_color(GREEN);
    printf("///// EDITARE ALBUM /////\n\n");
    set_color(WHITE);
    view_album(item);
    printf("\n");
    printf("1. Interschimba 2 fotografii\n");
    printf("2. Sterge o fotografie\n");
    printf("3. Redenumeste album\n");
    printf("4. Meniu principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
    case 1:
        interschimbare(item);
        break;
    case 2:
        delete_photo(item);
        break;
    case 3:
        rename_album(item);
        break;
    case 4:
        meniu_principal();
        break;
    default:
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
        set_color(WHITE);
        edit_album(item);
    }
}

void delete_album_menu(){
    printf("\n1. Vezi albume\n");
    printf("2. Meniu principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
    case 1:
        albumele_tale();
        break;
    case 2:
        meniu_principal();
    default:
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
        set_color(WHITE);
        delete_album_menu();
    }
}

void delete_album(Album *item){
    item->next->prev = item->prev;
    item->prev->next = item->next;

    if(albums->first == albums->last)
        album_flag = 0;

    if(item != albums->last){
    Album *alb = item->next;
    while(alb != albums->last){
        alb->index--;
        alb = alb->next;
    }
    alb->index--;
    }
    if(item == albums->first)
        albums->first = item->next;
    else if(item == albums->last)
        albums->last = item->prev;
    free(item);
    store_data();
    printf("Albumul a fost sters.\n\n");
    delete_album_menu();
}

void verify(Album *item){
    set_color(RED);
    printf("Esti sigur?\n\n");
    set_color(WHITE);
    printf("1. Da\n");
    printf("2. Nu\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
    case 1:
        delete_album(item);
        break;
    case 2:
        view_album_meniu(item);
        break;
    default:
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
        set_color(WHITE);
        verify(item);
    }
}

void dim_album(Album *item){
    printf("Album: %s\n",item->album_name);
    printf("Dimensiune: %.2f\n",item->size);
    printf("\n1. Inapoi\n");
    printf("2. Meniu principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
    case 1:
        view_album_meniu(item);
        break;
    case 2:
        meniu_principal();
        break;
    default:
        printf("%d nu e o optiune valabila\n\n",option);
        dim_album(item);
        break;
    }
}

void view_album_meniu(Album *item){
    set_color(GREEN);
    printf("///// VIZUALIZARE ALBUM /////\n\n");
    set_color(WHITE);
    view_album(item);
    printf("\n");
    printf("1. Editeaza\n");
    printf("2. Sterge\n");
    printf("3. Dimensiune album\n");
    printf("4. Inapoi la meniul principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
    case 1:
        edit_album(item);
        break;
    case 2:
        system("cls");
        verify(item);
        break;
    case 3:
        system("cls");
        dim_album(item);
        break;
    case 4:
        meniu_principal();
        break;
    default:
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
        set_color(WHITE);
        view_album_meniu(item);
    }
}

void albumele_tale(){

    set_color(GREEN);
    printf("///// ALBUMELE TALE ///// \n\n");
    set_color(YELLOW);
    printf("Selecteaza un album:\n");
    set_color(WHITE);
    vezi_albume();
    printf("0. Meniu principal\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    if(option > 0 && option <= albums->last->index){
        Album *item;
        item = albums->first;

        while(item->index != option)
            item = item->next;


        view_album_meniu(item);
    }
    else if(option == 0)
            meniu_principal();
        else{
        system("cls");
        set_color(RED);
        printf("%d nu e o optiune valabila\n\n",option);
        set_color(WHITE);
        albumele_tale();
    }

}
void meniu_principal(){

    set_color(GREEN);
    printf("///// MENIU PRINCIPAL /////\n\n");
    set_color(WHITE);
    printf("1. Vezi lista de fotografii\n");

    printf("2. Albumele tale\n");
    printf("3. Opreste aplicatia\n\n");
    set_color(LIGHT_BLUE);
    printf("Introdu optiunea: ");
    scanf("%d",&option);
    set_color(WHITE);
    system("cls");
    switch(option){
        case 1:
            vizualizare_poze(av_pic.first_pic);
            break;
        case 2:
            if(album_flag)
                albumele_tale();
            else{
            system("cls");
            set_color(RED);
            printf("Nu exista albume create\n\n");
            set_color(WHITE);
            printf("1. Meniu principal\n\n");
            set_color(LIGHT_BLUE);
            printf("Introdu optiunea: ");
            scanf("%d",&option);
            set_color(WHITE);
            while(option != 1){
                system("cls");
                set_color(RED);
                printf("%d nu e o optiune valabila\n",option);
                printf("Nu exista albume create\n\n");
                set_color(LIGHT_BLUE);
                printf("1. Meniu principal\n\n");
                printf("Introdu optiunea: ");
                scanf("%d",&option);
                set_color(WHITE);
            }
            system("cls");
            meniu_principal();
            }
            break;
        case 3:
            store_data();
            return 0;
            break;
        default:
            system("cls");
            set_color(RED);
            printf("%d nu e o optiune valabila\n\n",option);
            set_color(WHITE);
            meniu_principal();
    }

}
