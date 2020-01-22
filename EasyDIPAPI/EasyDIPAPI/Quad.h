#pragma once


#ifndef __ED_QUAD__
#define __ED_QUAD__


#include <glad/glad.h>

namespace ED
{
	class Quad {
		//static Quad* instance;
		unsigned int VBO, VAO;
	public:
		Quad();
		~Quad();
		void Draw();
		void Bind();
		//static Quad* Instance();
	};
}

#endif // !__ED_QUAD__

