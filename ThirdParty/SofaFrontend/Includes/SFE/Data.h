// Ce fichier est un composant de la bibliothèque SofaFrontEnd développée 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette bibliothèque est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propriété 
// intellectuelle. Sa détention, sa reproduction ou sa diffusion sont donc 
// soumis à autorisation préalable de la société. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SFELITE_DATA_H
#define SFELITE_DATA_H

#include <memory>
#include <string>
#include <vector>

#include "Object.h"
#include "DataTypeTrait.h"
#include "sofaFrontEnd.h"

namespace sfe
{

class DataImpl;
class Object;

/**
 * @brief Represents data fields of Sofa nodes and objects
 */
class SOFA_FRONTENDLITE_API Data
{
public:
	Data(); /// Creates a default, invalid Data (not pointing to a Sofa Data)

	bool isValid() const; /// Is it valid for value access ?
	explicit operator bool() const; /// Is it valid for value access ?

	bool operator==(const Data& data) const; /// Return true if they are linking to the same underlying Sofa object
	bool operator!=(const Data& data) const;

	const std::string& name() const; /// Gets the name.
	size_t uniqueId() const; /// Gives a number uniquely representing the underlying Sofa object. (Two proxy objects will have the same uniqueId if they are pointing to the same Sofa object.)
	int counter() const; /// Returns the number of changes since the Data creation (can be used to detect changes and updates)

	bool setParent(const Data& data); /// Link to a parent data. The value of this data will automatically duplicate the value of the parent data.

	Object owner() const; /// Gets the data's owner (returns an invalid handle if the owner is actually not an object)
	const std::string& path() const; /// Gets the path from the root to this Data (including "@/")

	template <class T> bool get(T& value) const /// Gets the value.
	{ return getValue(&value, VirtualTypeTrait<T>::get()); }

	template <class T> bool set(const T& value) /// Assigns the value of the data.
	{ return setValue(&value, VirtualTypeTrait<T>::get()); }

private:
	bool getValue(void* ptr, const AbstractTypeTrait* typeTrait) const;
	bool setValue(const void* ptr, const AbstractTypeTrait* typeTrait);

	friend DataImpl;
	Data(std::shared_ptr<DataImpl> impl);
	std::shared_ptr<DataImpl> m_impl;
};

} // namespace sfe

#endif // SFELITE_DATA_H
