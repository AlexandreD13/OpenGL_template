#ifndef _BaseMaterial_
#define _BaseMaterial_


#include "MaterialGL.h"

class BaseMaterial : public MaterialGL
{
	public :

		//Attributs
		
		//Constructeur-Destructeur

		/**
			Constructeur de la classe � partir du nom du mat�riau 
			@param name : nom du mat�riau
		*/
		BaseMaterial(string name="");

		/**
			Destructeur de la classe
		*/
		~BaseMaterial();

		//M�thodes

		/**
			M�thode virtuelle qui est appel�e pour faire le rendu d'un objet en utilisant ce mat�riau
			@param o : Node/Objet pour lequel on veut effectuer le rendu
		*/
		virtual void render(Node *o);

		/**
			M�thode virtuelle qui est appel�e pour modifier une valeur d'un param�tre n�cessaire pour le rendu
			@param o : Node/Objet concern� par le rendu
			@param elapsedTime : temps �coul� depuis la derni�re frame
		*/
		virtual void animate(Node* o, const float elapsedTime);

		void displayInterface(Node* o);

		
	protected :
		GLProgram* vp;
		GLProgram* fp;

		GLuint l_View,l_Proj, l_Model;
};

#endif