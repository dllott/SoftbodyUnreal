// Ce fichier est un composant de la biblioth�que SofaFrontEnd d�velopp�e 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette biblioth�que est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propri�t� 
// intellectuelle. Sa d�tention, sa reproduction ou sa diffusion sont donc 
// soumis � autorisation pr�alable de la soci�t�. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SFELITE_HELPERS_H
#define SFELITE_HELPERS_H

#include "Simulation.h"

namespace sfe
{

class SOFA_FRONTENDLITE_API Helper
{
public:
	const std::string& getFullPath(sfe::Data) const; /// Access to the class DataFileName
	bool findFile(std::string& filename, const std::string& basedir = "") const; /// Replace filename by the complete path to the file if it is known by Sofa
	const std::vector<char>& loadFile(const std::string& filename) const; /// Load a file and return its contents
};

}

#endif // SFELITE_HELPERS_H