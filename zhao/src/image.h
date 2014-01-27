/*  Departamento de Informatica, PUC-Rio, INF1761 Computer Graphhics
*
*   @file cor.h TAD: digital image (interface).
*   @author Marcelo Gattass and others
*
*   @date
*         Last versio:     08/2011.
*
*   @version 3.1
*
*   @Copyright/License
*   DI PUC-Rio Educational Software
*   All the products under this license are free software: they can be used for both academic and commercial purposes at absolutely no cost.
*   There are no paperwork, no royalties, no GNU-like "copyleft" restrictions, either. Just download and use it.
*   They are licensed under the terms of the MIT license reproduced below, and so are compatible with GPL and also qualifies as Open Source software.
*   They are not in the public domain, PUC-Rio keeps their copyright. The legal details are below.
*
*   The spirit of this license is that you are free to use the libraries for any purpose at no cost without having to ask us.
*   The only requirement is that if you do use them, then you should give us credit by including the copyright notice below somewhere in your product or its documentation.
*
*   Copyright © 2010-2011 DI PUC-Rio Educational Software
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software
*   without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sub license, and/or sell copies of the Software, and to permit
*   persons to whom the Software is furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all copies or suavlantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/

#ifndef IMAGE_H
#define IMAGE_H

#include <complex>

/************************************************************************/
/* Tipos Exportados                                                     */
/************************************************************************/
/**
 *   Imagem com um buffer rgb.
 */

typedef struct Image_imp Image;


/************************************************************************/
/* Funcoes Exportadas                                                   */
/************************************************************************/

/**
 *	Cria uma nova imagem com as dimensoes especificadas.
 *
 *	@param w Largura da imagem.
 *	@param h Altura da imagem.
 *	@param dcs Dimensao do espaco de cor de cada pixel (1=luminancia ou 3=RGB).
 *
 *	@return Handle da imagem criada.
 */
Image  * imgCreate (int w, int h, int dcs);

/**
 *	Destroi a imagem.
 *
 *	@param image imagem a ser destruida.
 */
void    imgDestroy (Image*image);

/**
 *	Cria uma nova nova copia imagem dada.
 *
 *	@param image imagem a ser copiada.
 *
 *	@return Handle da imagem criada.
 */
Image* imgCopy(Image* image);

/**
 *	Cria uma nova nova copia imagem dada em tons de cinza.
 *
 *	@param image imagem a ser copiada em tons de cinza.
 *
 *	@return Handle da imagem criada.
 */
Image* imgGrey(Image* image);

/**
 *	Obtem a largura (width) de uma imagem.
 *
 *	@param image Handle para uma imagem.
 *	@return  a largura em pixels (width) da imagem.
 */
int imgGetWidth(Image* image);

/**
 *	Obtem a altura (heigth) de uma imagem.
 *
 *	@param image Handle para uma imagem.
 *	@return  a altura em pixels (height) da imagem.
 */
int imgGetHeight(Image* image);

/**
 *	Obtem a dimensao do espaco de cor de cada pixel (1=lminancia ou 3=RGB).
 *
 *	@param image Handle para uma imagem.
 *	@return  dimensao do espaco de cor de cada pixel (1=lminancia ou 3=RGB) da imagem.
 */

int imgGetDimColorSpace(Image* image);
/**
 *	Obtem as dimensoes de uma imagem.
 *
 *	@param image Handle para uma imagem.
 *	@param w [out]Retorna a largura da imagem.
 *	@param h [out]Retorna a altura da imagem.
 */
float*  imgGetData(Image* image);

/**
 *	Ajusta o pixel de uma imagem com a cor especificada.
 *
 *	@param image Handle para uma imagem.
 *	@param x Posicao x na imagem.
 *	@param y Posicao y na imagem.
 *	@param color Cor do pixel(valor em float [0,1]).
 */
void imgSetPixel3fv(Image* image, int x, int y, float*  color);
void imgSetPixel3f(Image* image, int x, int y, float R, float G, float B);
void imgSetPixelf( Image* image, int x, int y, float L );

/**
 *	Ajusta o pixel de uma imagem com a cor especificada.
 *
 *	@param image Handle para uma imagem.
 *	@param x Posicao x na imagem.
 *	@param y Posicao y na imagem.
 *	@param color Cor do pixel (valor em unsigend char[0,255]).
 */
void imgSetPixel3ubv(Image* image, int x, int y, unsigned char * color);

/**
 *	Obtem o pixel de uma imagem na posicao especificada.
 *
 *	@param image Handle para uma imagem.
 *	@param x Posicao x na imagem.
 *	@param y Posicao y na imagem.
 *	@param color [out] Pixel da posicao especificada(valor em float [0,1]).
 */
void imgGetPixel3fv(Image* image, int x, int y, float* color);
void imgGetPixel3f(Image* image, int x, int y, float* R, float* G, float* B);
float imgGetPixelf( Image* image, int x, int y );

/**
 *	Obtem o pixel de uma imagem na posicao especificada.
 *
 *	@param image Handle para uma imagem.
 *	@param x Posicao x na imagem.
 *	@param y Posicao y na imagem.
 *	@param color [out] Pixel da posicao especificada (valor em unsigend char[0,255]).
 */
void imgGetPixel3ubv(Image* image, int x, int y, unsigned char *color);

/**
*	Calcula a diferenca entre duas imagens.
*
*	@param img0 img0 =  |img0-img1|^1/gamma
*	@param img1.
*
*	@return retorna o valor medio dos canais rgb de todos pixels (antes da correcao gamma).
*
*/
float imgDif(Image*img0, Image*img1, float gamma);

/**
*	Calcula o valor medio da diferenca entre as cores duas imagens.
*
*	@param img0.
*	@param img1.
*
*	@return retorna o valor medio dos canais rgb de todos pixels [0,1].
*
*/
float imgErr(Image*img0, Image*img1);

/**
 *	Le a imagem a partir do arquivo especificado.
 *
 *	@param filename Nome do arquivo de imagem.
 *
 *	@return imagem criada.
 */
Image* imgReadTGA(char *filename);

/**
 *	Salva a imagem no arquivo especificado em formato TGA.
 *
 *	@param filename Nome do arquivo de imagem.
 *	@param image Handle para uma imagem.
 *
 *	@return retorna 1 caso nao haja erros.
 */
int imgWriteTGA(char *filename, Image* image);

/**
 *	Salva a imagem no arquivo especificado em formato BMP.
 *
 *	@param filename Nome do arquivo de imagem.
 *	@param bmp Handle para uma imagem.
 *
 *	@return retorna 1 caso nao haja erros.
 */
int imgWriteBMP(char *filename, Image* bmp);

/**
 *	Le a imagem a partir do arquivo especificado.
 *
 *	@param filename Nome do arquivo de imagem.
 *
 *	@return imagem criada.
 */
Image* imgReadBMP (char *filename);


/**
 *	Le a imagem a partir do arquivo especificado.
 *  A imagem e' armazenada como um arquivo binario
 *  onde os tres campos da .
 *
 *	@param filename Nome do arquivo de imagem.
 *
 *	@return imagem criada.
 */
Image* imgReadPFM(char *filename);

/**
 *	Salva a imagem no arquivo especificado .
 *
 *	@param filename Nome do arquivo de imagem.
 *	@param image Handle para uma imagem.
 *
 *	@return retorna 1 caso nao haja erros.
 */
int imgWritePFM(char *filename, Image* image);


/**
 *	Conta o numero de cores diferentes na imagem
 *
 *	@param image Handle para uma imagem.
 *	@param w Nova largura da imagem.
 *	@param h Nova altura da imagem.
 */
int imgCountColor(Image* image, float);

/**
 *	 Aplica o filtro de Gauss para eliminar o ruido branco
 *  da imagem.
 *
 *	@param image Handle para uma imagem a ser filtrada.
 *
 */
void imgGauss(Image* img_dst, Image* img_src);

/**
 *	 Aplica o filtro de Mediana para eliminar o ruido sal e pimenta
 *  da imagem.
 *
 *	@param image Handle para uma imagem a ser filtrada.
 *
 */
void imgMedian(Image* image);

/**
 *	 Calcula uma imagem com pixels nas arestas
 *  da imagem dada.
 *
 *	@param image Handle para uma imagem.
 *
 * @return Handle para a image de luminosidade onde o branco destaca as arestas.
 */
Image* imgEdges(Image* image);

/**
*	Reduz o numero de cores distintas de uma imagem utilizando
*      um algoritimo de corte mediano.
*
*	@param image Handle para uma imagem.
*	@param image Handle para a imagem que vai ter as cores resuzidas.
*	@param maxCores numero de cores distintas que a nova imagem deve ter.
*
*/
void imgReduceColors(Image* image, Image* img_new, int ncolors);


/**
*	Reduz a imagem colorida para 2 tons (B&W ou Preto e Branco).
*
*   Baseado no artigo "A Threshold Selection Method from Gray-Level Histograms",
*   de N. Otsu, disponível em http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=04310076
*   Artigo citado em "Automatic Recognition Algorithm of Quick Response Code Based on Embedded System",
*   de Y. Liu e M. Liu, disponível em http://portal.acm.org/citation.cfm?id=1173502
*
*	@param img_cor    Handle para uma imagem em tons de cinza.
*
*   @return Handle para a image de luminosidade com dois tons 0 e 1 (B&W).
**/
Image* imgBinOtsu(Image* img_cgrey);

/**
*	Reduz a imagem colorida para 2 tons (B&W ou Preto e Branco).
*
*   Baseado no artigo "Barcode Readers using the Camera Device in Mobile Phones", de E. Ohbuchi,
*   disponível em http://portal.acm.org/citation.cfm?id=1033123
*   Artigo citado em "Automatic Recognition Algorithm of Quick Response Code Based on Embedded System",
*   de Y. Liu e M. Liu, disponível em http://portal.acm.org/citation.cfm?id=1173502
*
*	@param img_cor    Handle para uma imagem em tons de cinza.
*
*   @return Handle para a image de luminosidade com dois tons 0 e 1 (B&W).
**/
Image* imgBinOhbuchi(Image* img_cgrey);

/**
 * Generates a matrix of complex numbers representing the Fourier transform of
 * the given image
 *
 * @param img_in
 * @return
 */
std::complex<float>* imgFourier( Image* img_in );

/**
 * Shifts the image half way up and halfway left, wrapping the borders, so it's
 * corners will meet ateh the center of the image.
 *
 * @param img_in
 * @return
 */
Image* imgShift( Image* img_in );

/**
 * Applys a gamma factor to the image.
 *
 * @param img
 * @param factor
 */
void imgGamma( Image* img, float factor );

/**
 * Normalizes the image.
 * This functions makes sure that every value of each pixel is between 0 and 1
 * @param img
 */
void imgNormalize( Image* img );

/**
 * When given a matrix of complex numbers, it outputs the original image w x h.
 * @param buf_complex
 * @param w
 * @param h
 * @return
 */
Image* imgInverseFourier( std::complex<float>* buf_complex, int w, int h );

void imgDilate( Image* image_in, Image* kernel );

void imgErode( Image* image_in, Image* kernel );

void imgClipPositiveOutliers( Image* image, float clippingValue );

void imgClipOutliers( Image* image, float min, float max );

float imgComputeMean( Image* image );

float imgComputeVariance( Image* image );

float imgComputeVariance( Image* image, float mean );

void imgNormalize( Image* img, float numberOfStdDev );

void imgGauss( Image* img );

Image* imgCreateGaussianKernel( int halfWidth, int halfHeight, float stdDev );

bool imgAssert( Image* img );

#endif
