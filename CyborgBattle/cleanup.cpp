#include "cleanup.h"


void cleanup(SDL_Window *win){
	if (!win) { return; }
	SDL_DestroyWindow(win);
	win = nullptr;
}

void cleanup(SDL_Renderer *ren){
	if (!ren) { return; }
	SDL_DestroyRenderer(ren);
	ren = nullptr;
}

void cleanup(SDL_Texture *tex){
	if (!tex) { return; }
	SDL_DestroyTexture(tex);
	tex = nullptr;
}

void cleanup(SDL_Surface *surf){
	if (!surf) { return; }
	SDL_FreeSurface(surf);
	surf = nullptr;
}
