#ifndef _MATERIALGL_
#define _MATERIALGL_


#include "GLProgramPipeline.h"
#include "Scene.h"

class Node;

class MaterialGL
{
	public :

		//Attributs

		GLProgramPipeline* m_ProgramPipeline;
		
		//Constructeur-Destructeur

		/**
			Constructeur de la classe � partir du nom du mat�riau 
			@param name : nom du mat�riau
		*/
		MaterialGL(string name="");

		/**
			Destructeur de la classe
		*/
		~MaterialGL();

		//M�thodes

		/**
			M�thode virtuelle qui est appel�e pour faire le rendu d'un objet en utilisant ce mat�riau
			@param o : Node/Objet pour lequel on veut effectuer le rendu
		*/
		virtual void render(Node *o);

		/**
			M�thode virtuelle qui est appel�e pour modifier une valeur d'un param�tre n�cessaire pour le rendu
			@param o : Node/Objet concern� par le rendu
			@param elapsedTime : temps
		*/
		virtual void animate(Node* o, const float elapsedTime);


		string getName() { return m_Name; };

		virtual void displayInterface(Node* o) {};

		
	protected :
		std::string m_Name;

		Scene* scene;
};

#endif