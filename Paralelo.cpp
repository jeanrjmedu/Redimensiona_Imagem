
#include "stdafx.h"
#define cimg_use_bmp 1
#include <omp.h>
#include <iostream>
#include "CImg.h"
#include <chrono>


using namespace std;
using namespace cimg_library;

int main()
{

	CImg<unsigned char> img("imagem8k.bmp");
	int largura = img.width();
	int altura = img.height();
	unsigned char * img_mem = img.data(0, 0);

	int qtd_pixels = largura * altura;

	
		_int64 inicio = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		#pragma omp parallel for

		for (int j = 0; j < altura*largura; j++)
		{
			int media = ((int)img_mem[j] + (int)img_mem[j + qtd_pixels] + (int)img_mem[j + 2 * qtd_pixels]) / 3;
			img_mem[j] = (unsigned char)media;
			img_mem[j + qtd_pixels] = (unsigned char)media;
			img_mem[j + 2 * qtd_pixels] = (unsigned char)media;

		}


		//TEMPO DE EXECUÇÃO
		__int64 fim = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::cout << "Tempo " << fim - inicio << std::endl;
	
	


		//DOWNSAMPLING
	CImg<unsigned char> Redim(largura / 2, altura / 2, 1, 3, 0);
	unsigned char * img_u = Redim.data(0, 0);

	int Redim_qtdpixels = (largura / 2) * (altura / 2);
	int pixel_atual = 0;
	int cols = 0;

	for (int a = 0; a < altura; a += 2)
	{
		for (int b = 0; b < largura; b += 2)
		{

			if (pixel_atual == 0)//para fins inicialização;
			{
				pixel_atual = b + (altura*cols);
			}

			//b+(largura*a) indice + linhas percorridas
			int R = ((int)img_mem[b + (largura*a)] + (int)img_mem[b + (largura*a) + 1] + (int)img_mem[b + (largura*a) + largura] + (int)img_mem[b + (largura*a) + largura + 1]) / 4;
			int G = ((int)img_mem[b + (largura*a) + qtd_pixels] + (int)img_mem[b + (largura*a) + 1 + qtd_pixels] + (int)img_mem[b + (largura*a) + largura + qtd_pixels] + (int)img_mem[b + (largura*a) + largura + 1 + qtd_pixels]) / 4;
			int B = ((int)img_mem[b + (largura*a) + qtd_pixels * 2] + (int)img_mem[b + (largura*a) + 1 + qtd_pixels * 2] + (int)img_mem[b + (largura*a) + largura + qtd_pixels * 2] + (int)img_mem[b + (largura*a) + largura + 1 + qtd_pixels * 2]) / 4;

			img_u[pixel_atual] = (unsigned char)R;

			img_u[pixel_atual + Redim_qtdpixels] = (unsigned char)G;

			img_u[pixel_atual + Redim_qtdpixels * 2] = (unsigned char)B;

			pixel_atual++;

		}
		cols++;
	}


}

