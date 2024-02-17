#ifndef _GLPROGRAM_H
#define _GLPROGRAM_H

#include <glad/glad.h>
#include <string>
#include <stdexcept>

using namespace std;

class GLProgram
{
	public:

		//Attributs

		string info_text; //stocke les logs retourn�s par opengl

		//Constructeur-Destructeur

		/**
			Constructeur de la classe GLProgram prenant en param�tre son nom et son type
			Permet de r�cup�rer le GLProgramSource correspondant au nom gr�ce � GLProgramSourceManager, cr�e un nouveau GLUniformManager et d�termine le type du programme
			@param name : nom du programme
			@param type : type du programme
		*/
		GLProgram(std::string name,GLenum type);

		/**
			Destructeur de la classe qui appelle la fonction release du GLProgramSourceManager
		*/
		~GLProgram();

		//M�thodes


		/**
			M�thode retournant l'adresse m�moire allou� � l'instance du programme opengl
			@return : adresse m�moire du programme
		*/
		GLuint getId();

	
		/**
			M�thode affichant les erreurs du programme
			@return : compilation r�ussie du programme
		*/
		bool printInfoLog();

	private:

		//Attributs
		string m_filename;
		GLuint m_Id; // nom associ� � l'instance de GLProgram
		GLenum m_Type; //type du programme (vf, fs, gs, etc ...)
		
};





#endif
