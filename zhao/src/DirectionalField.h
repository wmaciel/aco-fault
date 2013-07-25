/*
 * File:   DirectionalField.h
 * Author: keoma
 *
 * Created on July 19, 2013, 6:41 PM
 */

#ifndef DIRECTIONALFIELD_H
#define	DIRECTIONALFIELD_H

/**
 * Represents a 2D directional field computed trough the method presented by
 * Asker M. Bazen and Sabih H. Gerez in their article entitled "Directional
 * Field Computation for Fingerprints Based on The Principal Component Analysis
 * of Local Gradients" published in November of the year 2000.
 */
class DirectionalField
{
public:
    /**
     * Constructor
     * @param data Linearized matrix representing the pixel of the image. Each
     * value in the matrix should represent the luminance of the corresponding
     *  pixel. The matrix must be given by line.
     * @param width Number of pixels in each row of data
     * @param height Number of pixels in each column of data
     */
    DirectionalField( float* data, int width, int height, int window );

    /**
     * Copy Constructor
     */
    DirectionalField( const DirectionalField& orig );

    /**
     * Destructor
     */
    virtual ~DirectionalField();

    float getXDirection( int x, int y );

    float getYDirection( int x, int y );
    
    float getCoherence( int x, int y );

    void debugPrint();

private:
    /**
     * Lineariza as coordenadas do pixel
     */
    int pixel( int x, int y );

    /**
     * Derivada horizontal por diferenças centrais
     */
    float dx( int x, int y, float* data );

    /**
     * Derivada vertical por diferenças centrais
     */
    float dy( int x, int y, float* data );

    void direction( int pixel, float& dirX, float& dirY );

    float coherence( int pixel );

    void buildHorizontalDerivativeMatrix( float* data );

    void buildHorizontalWindowedDerivativeMatrix( int windowSize );

    void buildVerticalDerivativeMatrix( float* data );

    void buildVerticalWindowedDerivativeMatrix( int windowSize );

    void buildCrossedWindowedDerivativeMatrix( int windowSize );

    void buildCoherenceMatrix();

    void buildDirectionMatrices();

    void normalizeDirections();
    
    void normalizeCoherence();

    float getHorizontalWindowedDerivative( int px, int py, int windowSize );

    float getVerticalWindowedDerivative( int px, int py, int windowSize );

    float getCrossedWindowedDerivative( int px, int py, int windowsize );

    /** Coerência da direção de cada pixel */
    float* _coherence;

    /** Componete horizontal de cada pixel */
    float* _directionX;

    /** Componente vertical de cada pixel */
    float* _directionY;

    float* _gxMatrix;

    float* _gyMatrix;

    float* _gxxMatrix;

    float* _gyyMatrix;

    float* _gxyMatrix;

    int _width;

    int _height;


};

#endif	/* DIRECTIONALFIELD_H */

