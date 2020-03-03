#pragma once
#include "EDpch.h"

#include "API_CPU.h"
#include "API_GPU.h"
#include "Filter.h"

namespace ED
{
	#define PARAMS_CONV RawData* imgData, int imgWidth, int imgHeight, int nChannels



	class Sobel : public Filter
	{
	public:
		RawData* ApplyCPU(PARAMS_CONV)
		{
			std::vector<float> sobelX = {
				1, 2, 1,
				0, 0, 0,
				-1, -2, -1,
			};
			std::vector<float> sobelY = {
				-1 , 0, 1,
				-2 , 0, 2,
				-1 , 0, 1,
			};
			float acumX[4];
			float acumY[4];
			return ForEachConvolution(imgData, imgWidth, imgHeight, nChannels, 3, 3, 1, 1,
				[&]() {
				Assign<float>(acumX, nChannels, 0.f);
				Assign<float>(acumY, nChannels, 0.f);
			},
				[&](RawData* src, int ix, int iy, int ic) {
				for (int ii = 0; ii < nChannels; ii++)
				{
					acumX[ii] += src[ii] * sobelX[ic];
					acumY[ii] += src[ii] * sobelY[ic];
				}
			},
				[&](RawData* dest) {
				for (int ii = 0; ii < nChannels; ii++)
				{
					float x = acumX[ii] * acumX[ii];
					float y = acumY[ii] * acumY[ii];
					dest[ii] = clamp(255, 0, sqrt(x + y));
				}
			});
		}
	
		RawData* ApplyGPU(PARAMS_CONV)
		{
			kernels[0].ApplyPadding();
			kernels[1].ApplyPadding();
			

			std::string sobel = BuildShaderConv(
				//uniforms
				UseGradient(),
					//before
				BuildConvolution(kernels[0].values, "convX") +
				BuildConvolution(kernels[0].values, "convY") +
				"vec3 avgX = vec3(0);\n",
				// op
				UseForConv(kernels[0].width, kernels[0].pivotY, kernels[0].pivotX, kernels[0].pivotY,
						"vec3 color = texture(tex, nUv).rgb;\n"
						"avgX +=  color * conv[convI];\n"			
					) +
				UseForConv(kernels[1].width, kernels[1].pivotY, kernels[1].pivotX, kernels[1].pivotY,
					"vec3 color = texture(tex, nUv).rgb;\n"
					"avgX +=  color * conv[convI];\n"
					),
				// after
				"fragColor = vec4(avgX, 1);\n"
			);


			auto sobelShader(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), sobel.c_str()));




			return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *sobelShader);
		}
	
	};


}

/**
 *  @file   API.h
 *  @brief  Graphics Context Interface
 *  @author Somebody else
 *  @date   2012-02-20
 ***********************************************/

 //-Cargar imagen.
		//- Salvar imagen.
 //	- Mostrar información sobre la imagen : dimensiones, bits por pixel, cantidad única de colores and dpi
 //	(Dots Per Inch).
 //	- Obtener el número de colores únicos que componen la imagen.
 //	- Obtener el histograma de la imagen
/*	-Aplicar un kernel arbitrario a la imagen.Para ello debe haber una forma simple y práctica de asignar
		los valores a cada posición del kernel.
		- Se debe poder especificar el tamaño del kernel a utilizar para cada uno de los filtros.Cualquier
		combinación de filas y / o columnas es válida partiendo desde un mínimo de 2x1(vertical) ó 1x2
		(horizontal) hasta un máximo de 7x7.
		- Debe agregar al menos un(1) efecto complejo opcional(e.g., Chromakey, Cuantización, Toon
			Shading, Embossing o Repujado, Estereografía, Gamma Correction, High Dynamic Range HDR, entre
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