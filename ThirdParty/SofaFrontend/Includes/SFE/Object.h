// Ce fichier est un composant de la bibliothèque SofaFrontEnd développée 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette bibliothèque est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propriété 
// intellectuelle. Sa détention, sa reproduction ou sa diffusion sont donc 
// soumis à autorisation préalable de la société. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SFELITE_OBJECT_H
#define SFELITE_OBJECT_H

#include "Data.h"
#include "sofaFrontEnd.h"

namespace sfe
{

class Data;
class Node;
class ObjectImpl;

/**
 * @brief	Represents a Sofa object.
 */
class SOFA_FRONTENDLITE_API Object
{
public:
	Object(); /// Creates a default, invalid Object (not pointing to a Sofa Object)

	bool isValid() const; /// Is it valid to operate ?
	explicit operator bool() const; /// Is it valid to operate ?

	bool operator==(const Object& obj) const; /// Return true if they are linking to the same underlying Sofa object
	bool operator!=(const Object& obj) const;

	const std::string& name() const; /// Gets the name.
	const std::string& className() const; /// Gets the class name.
	const std::string& templateName() const; /// Gets the template name.
	bool isInstanceOf(const std::string& type) const; /// Return true if this Object inherits from type
	size_t uniqueId() const; /// Gives a number uniquely representing the underlying Sofa object. (Two proxy objects will have the same uniqueId if they are pointing to the same Sofa object.)

	Data data(const std::string& name) const; /// Retrieves the first data having the given name. Returns an invalid data on failure.

	typedef std::vector<std::string> StringList;
	StringList listData() const; /// Gets all data names.

	Node parent() const; /// Gets the owning node. Returns an invalid node on failure.
	const std::string& path() const; /// Gets the path from the root to this object (including "@/")

private:
	friend class ObjectImpl;
	Object(std::shared_ptr<ObjectImpl> impl);
	std::shared_ptr<ObjectImpl> m_impl;
};

} // namespace sfe

#endif // SFELITE_OBJECT_H
