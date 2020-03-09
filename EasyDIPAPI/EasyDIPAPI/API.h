#pragma once
#include "EDpch.h"




/**
 *  @file   API.h
 *  @brief  Graphics Context Interface
 *  @author Somebody else
 *  @date   2012-02-20
 ***********************************************/
/* Funciones del API :

- Mostrar información sobre la imagen :  dpi
(Dots Per Inch).
- Filtro del  mediana 

- Debe agregar al menos un(1) efecto complejo opcional(e.g., Chromakey, Cuantización, Toon Shading, Estereografía, Gamma Correction, High Dynamic Range HDR, entre
	otros).
	- Las funciones del API deben ser demostradas a través de una aplicación que incorpore escalamiento
	y rotación libre de la imagen, así como acercamiento y alejamiento(Zoom in / Zoom out).
*/

#include "Filters/Sobel.h"
#include "Filters/BlackAndWhite.h"
#include "Filters/Embossing.h"
#include "Filters/ToonShading.h"
#include "Filters/Box.h"
#include "Filters/Greyscale.h"
#include "Filters/Median.h"
#include "Filters/Negative.h"
#include "Filters/Prewitt.h"
#include "Filters/Roberts.h"
#include "Filters/LaplacianOfGaussian.h"

namespace ED {

	RawData* HistogramToTexture(const std::vector<float> hist, int imgWidth, int imgHeight);

	std::vector<float> GetHistogram(RawData* data, unsigned int byteSize, int strideForNextColor, int offsetFrom0, bool normalize = true);

	int NumberOfUniqueColors(RawData* data, unsigned int byteSize, int strideForNextColor);

	bool Load(const std::string& path, RawData*& data, int& width, int& height, int& nChannels);

	bool Save(const std::string& fileName, RawData* data, int width, int height, int nChannels);

}