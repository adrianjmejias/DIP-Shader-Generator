#pragma once
#include "EDpch.h"
#include "EDImage.h"
namespace ED {

	using GlobalConv = std::function<RawData*(RawData*, unsigned int, unsigned int, unsigned int)>;
	using LocalConv  = std::function<RawData*(RawData*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)>;
	

	std::vector<float> Normalize(std::vector<float> d);

	RawData* ApplyConvolutionHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, Shader& s, unsigned int *tex = nullptr);
	
	
	RawData* ApplyGreyHA(RawData*, unsigned int, unsigned int, unsigned int);
	RawData* ApplyNegativeHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);
	RawData* ApplyBWHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);

	RawData* ApplySobelHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);
	//RawData* ApplyRobertsHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyPrewittHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyBoxHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyMedianHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyLaplaceGaussHA(RawData*, unsigned int, unsigned int);



	//-Cargar imagen.
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
		- Salvar imagen.
		- Las funciones del API deben ser demostradas a trav�s de una aplicaci�n que incorpore escalamiento
		y rotaci�n libre de la imagen, as� como acercamiento y alejamiento(Zoom in / Zoom out).*/
	
	

	RawData* ApplyNegative(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);
	RawData* ApplyGrey(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);
	RawData* ApplyBW(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);


	RawData* ApplySobel(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

	RawData* ApplyRoberts(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

	RawData* ApplyPrewitt(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

	RawData* ApplyBox(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

	RawData* ApplyMedian(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

	RawData* ApplyLaplaceGauss(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);
	bool EDInit();
}