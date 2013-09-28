#ifndef PFM_IMAGE_H
#define	PFM_IMAGE_H

#include <string>

class PFMImage 
{
public:

    /**
     * Construtor
     */
    PFMImage();
    
    /**
     * Construtor
     * @param width Largura da imagem
     * @param height Altura da imagem
     */
    PFMImage( int width, int height );
    
    /**
     * Copy Constructor
     * @param orginal
     */
    PFMImage( PFMImage& original );

    /**
     * Destrutor
     */
    virtual ~PFMImage();
    
    /**
     * Recupera o valor de um pixel da imagem por parâmetro
     * @param x Coordenada horizontal do pixel
     * @param y Coordenada vertical do pixel
     * @param luminanceValue Valor de lumin�ncia
     */
    void getPixel( int x, int y, float& luminanceValue );
    
    /**
     * Recupera o valor de um pixel da imagem
     * @param x Coordenada horizontal do pixel
     * @param y Coordenada vertical do pixel
     * @return luminanceValue Valor de lumin�ncia
     */
    float getPixel( int x, int y );
    
    /**
     * Define o valor de um pixel da imagem
     * @param x Coordenada horizontal do pixel
     * @param y Coordenada vertical do pixel
     * @param luminance Valor de lumin�ncia
     */
    void setPixel( int x, int y, float luminance );

    /**
     * Recupera dimens�es da imagem
     * @param[out] width  Largura da imagem
     * @param[out] height Altura da imagem
     */
    void getDimension( int& width, int& height );
    
    /**
     * Salva a imagem em formato PFM
     * @param filePath Caminho do arquivo a ser salvo
     * @return True, se a imagem foi salva com sucesso
     */
    bool exportPFMFile( std::string filePath );
    
    /**
     * Carrega a imagem em formato PFM
     * @param filePath Caminho do arquivo a ser carregado
     * @return True, se a imagem foi carregado com sucesso
     */
    bool loadPFMFile( std::string filePath );
    
    /**
     * Fills the whole image with the given luminance value
     * @param luminance Value that will fill the whole image
     */
    void fill( float luminance );
    
    
    /**
     * Copies the contents of another PFM Image
     * @param orig
     */
    void copy( PFMImage& original );
    
    /**
     * 
     * @return 
     */
    void generateGauss( int scale );
    
private:
    
    /**
     * Retorna o �ndice 1D um pixel
     * @param x Coordenada horizontal do pixel
     * @param y Coordenada vertical do pixel
     */
    int getIndex( int x, int y );
    
    /** Largura e altura da imagem */
   int _width;
   int _height;
   
   /** Buffer da imagem */
   float* _buffer;   
};

#endif	/* PFMIMAGE_H */

