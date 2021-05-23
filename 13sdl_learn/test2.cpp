
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "Window.h"

//Buffer:
//|-----------|-------------|
//chunk-------pos---len-----|
static  Uint8* audio_chunk;
static  Uint32  audio_len;
static  Uint8* audio_pos;

void  fill_audio(void* udata, Uint8* stream, int len) {
	//SDL 2.0
	SDL_memset(stream, 0, len);
	if (audio_len == 0)
		return;
	len = (len > audio_len ? audio_len : len);

	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

int main() {

	Window::Init("Zoom and Rotate Pic");


	//SDL_AudioSpec
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = 44100;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = 2;
	wanted_spec.silence = 0;
	wanted_spec.samples = 1024;
	wanted_spec.callback = fill_audio;


	if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
		printf("can't open audio.\n");
		return -1;
	}

	FILE* fp = fopen("..\\..\\res\\13sdl_learn\\NocturneNo2inEflat_44.1k_s16le.pcm", "rb+");
	if (fp == NULL) {
		printf("cannot open this file\n");
		return -1;
	}
	int pcm_buffer_size = 4096;
	char* pcm_buffer = (char*)malloc(pcm_buffer_size);
	int data_count = 0;

	//Play
	SDL_PauseAudio(0);

	SDL_Texture* pic = Window::LoadImage("..\\..\\res\\13sdl_learn\\prompt.png");

	Window::HandleEvents([](SDL_Event e) {
		//User requests quit
		if (e.key.keysym.sym == SDLK_q)
		{
			Window::m_eventQuit = true;
		}
	}, [&pic,&pcm_buffer,&pcm_buffer_size,&fp,&data_count]() {

		if (fread(pcm_buffer, 1, pcm_buffer_size, fp) != pcm_buffer_size) {
			// Loop
			fseek(fp, 0, SEEK_SET);
			fread(pcm_buffer, 1, pcm_buffer_size, fp);
			data_count = 0;
		}
		printf("Now Playing %10d Bytes data.\n", data_count);
		data_count += pcm_buffer_size;
		//Set audio buffer (PCM data)
		audio_chunk = (Uint8*)pcm_buffer;
		//Audio buffer length
		audio_len = pcm_buffer_size;
		audio_pos = audio_chunk;

		while (audio_len > 0) {
			//Wait until finish
			SDL_Delay(1);
		}
		//Window::Clear();
		//Window::Draw(pic, Window::Box());
		//Window::Present();
	});

	//Free loaded images
	SDL_DestroyTexture(pic);

	//Quit SDL subsystems
	Mix_Quit();

	Window::Quit();
	return 0;
}
