#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define MAXLINELEN 4096


//read the next line
char* read_next_line(FILE* p_f, long* p_size){
    char* s = malloc(MAXLINELEN*sizeof(char));
    if(!s){
        fprintf(stderr,"Not enaugh memory");
        exit(EXIT_FAILURE);
    }
    long old_pos = ftell(p_f);
    long len = 0;
    if(fgets(s,MAXLINELEN,p_f)!=NULL){
        len = ftell(p_f)-old_pos;
        if(s[len-1]=='\n'){
            s[len-1]='\0';
            len--;
        }else{
            if(!feof(p_f)){
                fprintf(stderr,"Line too long...");
                exit(EXIT_FAILURE);
            }
        }
        (*p_size)=len;
        return s;
    }
    free(s);
    return NULL;
}

//convert the line 
long* convert_line(char* line, long* p_size){
    long* arr = malloc((*p_size)*sizeof(long));
    if(!arr){
        fprintf(stderr,"Not enaugh memory");
        exit(EXIT_FAILURE);
    }
    long arr_s=0;
    char* token = strtok(line," ");
    while(token != NULL){
        char* endval = token;
        long val = strtol(token, &endval, 10);
        if((*endval)=='\0'){
            arr[arr_s]=val;
            arr_s++;
        }else{
            free(arr);
            return NULL;
        }
        token = strtok(NULL," ");
    }
    (*p_size)=arr_s;
    return arr;
}



/*
*@brief: load the game 
*@param: the file name of the game
*@return: the game loaded
*/
game load_game(char* filename){
    FILE* f=fopen(filename,"r");
    //test param
    if(!f){
        fprintf(stderr,"Problem opening file");
        exit(EXIT_FAILURE);
    }


    //get the size of board
    long size=0;
    char* line=read_next_line(f,&size);
    long* arr= convert_line(line,&size);
	game game1 = new_game_ext(arr[0],arr[1]);
    free(arr);
    free(line);

	//get nm_monsters value of the file
    line=read_next_line(f,&size);
	arr= convert_line(line,&size);
	set_required_nb_monsters(game1, VAMPIRE, arr[0]);
	set_required_nb_monsters(game1, GHOST, arr[1]);
	set_required_nb_monsters(game1, ZOMBIE, arr[2]);
	set_required_nb_monsters(game1, SPIRIT, arr[3]);
	free(arr);
    free(line);

	//get labels values of the file
	line=read_next_line(f,&size);
	arr= convert_line(line,&size);
    for(int i=0;i!=game_width(game1);i++){
        set_required_nb_seen(game1, N, i, arr[i]);
    }
    free(arr);
    free(line);
    
	line=read_next_line(f,&size);
	arr= convert_line(line,&size);
    for(int i=0;i!=game_width(game1);i++){
        set_required_nb_seen(game1, S, i, arr[i]);
    }
    free(arr);
    free(line);

	line=read_next_line(f,&size);
	arr= convert_line(line,&size);
    for(int i=0;i!=game_height(game1);i++){
        set_required_nb_seen(game1, E, i, arr[i]);
    }
    free(arr);
    free(line);

	line=read_next_line(f,&size);
	arr= convert_line(line,&size);
    for(int i=0;i!=game_height(game1);i++){
        set_required_nb_seen(game1, W, i, arr[i]);
    }
    free(arr);
    free(line);

	//get monsters/mirror 
	line=read_next_line(f,&size);
	int j = game_height(game1)-1;
	while(line!=NULL){
		for(int i=0;i<game_width(game1);i++){
			switch(line[i*2]){
				case '/':
					add_mirror_ext(game1,MIRROR,i,j);
					break;
				case '\\':
					add_mirror_ext(game1,ANTIMIRROR,i,j);
					break;
				case '-':
					add_mirror_ext(game1,HMIRROR,i,j);
					break;
				case '|':
					add_mirror_ext(game1,VMIRROR,i,j);
					break;
				case 'Z':
					add_monster(game1,ZOMBIE,i,j);
					break;
				case 'G':
					add_monster(game1,GHOST,i,j);
					break;
				case 'V':
					add_monster(game1,VAMPIRE,i,j);
					break;
				case 'S':
					add_monster(game1,SPIRIT,i,j);
					break;
				default:
					break;
			}
		}
		free(line);
		line=read_next_line(f,&size);
		j--;
	}
	free(line);
	fclose(f);
	return game1;
}


/*
*@brief: save the current in a file
*@param: the game and the file name 
*return: void
*/
void save_game(cgame g, char* filename){
    if(!filename){
        fprintf(stderr,"Unvalid pointer\n");
        exit(EXIT_FAILURE);
    }
    if(!g){
        fprintf(stderr,"Unvalid pointer\n");
        exit(EXIT_FAILURE);
    }
    FILE* f=fopen(filename,"w");
    int w = game_width(g);
    int h = game_height(g); 
    //<width> <height>
    fprintf(f,"%d %d\n",w,h);
    //<required_nb_monsters>
    fprintf(f,"%d %d %d %d\n", required_nb_monsters(g, VAMPIRE), required_nb_monsters(g, GHOST), required_nb_monsters(g, ZOMBIE), required_nb_monsters(g, SPIRIT));
    //<labels[side][pos]>
    for(int i=0; i<w;i++){
        fprintf(f,"%d ",required_nb_seen(g, N, i));
    }
    fprintf(f,"\n");
    for(int i=0; i<w;i++){
        fprintf(f,"%d ",required_nb_seen(g, S, i));
    }
    fprintf(f,"\n");
    for(int i=0; i<h;i++){
        fprintf(f,"%d ",required_nb_seen(g, E, i));
    }
    fprintf(f,"\n");
    for(int i=0; i<h;i++){
        fprintf(f,"%d ",required_nb_seen(g, W, i));
    }
    //<board>
    for (int line = h-1; line >= 0; line--){							// lines
		fprintf(f,"\n");
		for (int col = 0; col < w; col++) {								// cols
			int val = get_content(g, col, line); 									// content for each position
			switch(val){
				case EMPTY:
				    fprintf(f,". ");
				    break;
				case MIRROR:
				    fprintf(f,"/ ");
				    break;
				case ANTIMIRROR:
				    fprintf(f,"\\ ");
				    break;
				case HMIRROR:
				    fprintf(f,"- ");
				    break;
				case VMIRROR:
				    fprintf(f,"| ");
				    break;
				case GHOST:
				    fprintf(f,"G ");
				    break;
				case VAMPIRE:
				    fprintf(f,"V ");
				    break;
				case ZOMBIE:
				    fprintf(f,"Z ");
				    break;
				case SPIRIT:
				    fprintf(f,"S ");
				    break;
                default:
                    break;
			}
		}
	}
    fclose(f);
}