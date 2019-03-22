// Ce fichier est un composant de la biblioth�que SofaFrontEnd d�velopp�e 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette biblioth�que est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propri�t� 
// intellectuelle. Sa d�tention, sa reproduction ou sa diffusion sont donc 
// soumis � autorisation pr�alable de la soci�t�. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SOFAFRONTENDLITE_H
#define SOFAFRONTENDLITE_H

#include <exception>

#ifndef WIN32
#	define SOFA_FRONTENDLITE_API
#else
#	ifdef SOFA_BUILD_SOFAFRONTENDLITE
#		define SOFA_FRONTENDLITE_API __declspec( dllexport )
#	else
#		pragma warning(disable: 4251)
#		define SOFA_FRONTENDLITE_API  __declspec( dllimport )
#	endif
#endif

namespace sfe
{
	class Simulation;

	SOFA_FRONTENDLITE_API void SetVerbose(bool verbose); /// Do we output warnings when some functions fail? (default: false)
	SOFA_FRONTENDLITE_API bool IsVerbose();
	SOFA_FRONTENDLITE_API Simulation getSimulation();
 
	class InvalidHandle : public std::exception { };

}

#endif // SOFAFRONTENDLITE_H
