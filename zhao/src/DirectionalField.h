/*
 * File:   DirectionalField.h
 * Author: keoma
 *
 * Created on July 19, 2013, 6:41 PM
 */

#ifndef DIRECTIONALFIELD_H
#define	DIRECTIONALFIELD_H

#include "image.h"

/**
 * Represents a 2D directional field computed trough the method presented by
 * Asker M. Bazen and Sabih H. Gerez in their article entitled "Directional
 * Field Computation for Fingerprints Based on The Principal Component Analysis
 * of Local Gradients" published in November of the year 2000.
 */
class DirectionalField
{
public:
    DirectionalField( Image* img, int openKernelRadius, int closeKernelRadius );

    /**
     * Destructor
     */
    virtual ~DirectionalField();
    
    void getDirection( int x, int y, float& dirX, float& dirY );

    float getCoherence( int x, int y );

    bool getCoherenceMask( int x, int y );

    void debugPrint();

    void debugImages();
    
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

    void computeDirection( int pixel, float& dirX, float& dirY );

    float computeCoherence( int pixel );

    void buildHorizontalDerivativeMatrix( float* data );

    void buildHorizontalWindowedDerivativeMatrix();

    void buildVerticalDerivativeMatrix( float* data );

    void buildVerticalWindowedDerivativeMatrix();

    void buildCrossedWindowedDerivativeMatrix();

    void buildCoherenceImage();

    void buildCoherenceMask();

    void buildDirectionImage();

    float getHorizontalWindowedDerivative( int px, int py );

    float getVerticalWindowedDerivative( int px, int py );

    float getCrossedWindowedDerivative( int px, int py );

    /** Coerência da direção de cada pixel */
    Image* _coherence;

    /** Imagem binária de coerência */
    Image* _coherenceMask;

    float _coherenceThreshold;
    
    int _openKernelRadius;
    
    int _closeKernelRadius;

    /** Componete horizontal de cada pixel */
    Image* _direction;

    float* _gxMatrix;

    float* _gyMatrix;

    float* _gxxMatrix;

    float* _gyyMatrix;

    float* _gxyMatrix;

    int _width;

    int _height;

    Image* _gaussianKernel;
};

#endif	/* DIRECTIONALFIELD_H */

