
#ifndef _GLPROGRAMPIPELINE_H
#define _GLPROGRAMPIPELINE_H
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>

#include "GLProgram.h"

using namespace std;

class  GLProgramPipeline
{
	public:

		GLProgramPipeline(string name = "");

		~GLProgramPipeline();

		void useProgramStage(GLProgram* p,GLenum programType) const;
		bool link();

		void bind() const;
		static void release();
			
		void printInfoLog();
		GLuint getId() const;

	private:
		GLuint m_Pipeline{};
		std::string m_Name;
		string info_text;

};





#endif
