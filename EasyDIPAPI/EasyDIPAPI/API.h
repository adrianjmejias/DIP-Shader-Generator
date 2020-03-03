#pragma once
#include "EDpch.h"
#include "API_CPU.h"
#include "API_GPU.h"

/**
 *  @file   API.h
 *  @brief  Graphics Context Interface
 *  @author Somebody else
 *  @date   2012-02-20
 ***********************************************/

 //-Cargar imagen.
		//- Salvar imagen.
 //	- Mostrar informaci�n sobre la imagen : dimensiones, bits por pixel, cantidad �nica de colores and dpi
 //	(Dots Per Inch).
 //	- Obtener el n�mero de colores �nicos que componen la imagen.
 //	- Obtener el histograma de la imagen
/*	-Aplicar un kernel arbitrario a la imagen.Para ello debe haber una forma simple y pr�ctica de asignar
		los valores a cada posici�n del kernel.
		- Se debe poder especificar el tama�o del kernel a utilizar para cada uno de los filtros.Cualquier
		combinaci�n de filas y / o columnas es v�lida partiendo desde un m�nimo de 2x1(vertical) � 1x2
		(horizontal) hasta un m�ximo de 7x7.
		- Debe agregar al menos un(1) efecto complejo opcional(e.g., Chromakey, Cuantizaci�n, Toon
			Shading, Embossing o Repujado, Estereograf�a, Gamma Correction, High Dynamic Range HDR, entre
			otros).
*/



namespace ED {

	RawData* HistogramToTexture(const std::vector<float> hist, int imgWidth, int imgHeight);

	std::vector<float> GetHistogram(RawData* data, unsigned int byteSize, int strideForNextColor, int offsetFrom0, bool normalize = true);

	int NumberOfUniqueColors(RawData* data, unsigned int byteSize, int strideForNextColor);

	bool Load(const std::string& path, RawData*& data, int& width, int& height, int& nChannels);

	bool Save(const std::string& fileName, RawData* data, int width, int height, int nChannels);
	
	bool EDInit();
}