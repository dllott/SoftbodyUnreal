// Ce fichier est un composant de la bibliothèque SofaFrontEnd développée 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette bibliothèque est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propriété 
// intellectuelle. Sa détention, sa reproduction ou sa diffusion sont donc 
// soumis à autorisation préalable de la société. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SFELITE_NODE_H
#define SFELITE_NODE_H

#include "Object.h"
#include "sofaFrontEnd.h"

namespace sfe
{

class NodeImpl;

/**
 * @brief Represnts a Sofa node.
 */
class SOFA_FRONTENDLITE_API Node
{
public:
	Node(); /// Creates a default, invalid Node (not pointing to a Sofa Node)

	bool isValid() const; /// Is it a valid sofa node ?
	explicit operator bool() const; /// Is it a valid sofa node ?

	bool operator==(const Node& obj) const; /// Return true if they are pointing to the same underlying Sofa object
	bool operator!=(const Node& obj) const;

	const std::string& name() const; /// Gets the name of the node.
	size_t uniqueId() const; /// Gives a number uniquely representing the underlying Sofa object. (Two proxy objects will have the same uniqueId if they are pointing to the same Sofa object.)

	typedef std::pair<std::string, std::string> Argument;
	typedef std::vector<Argument> ArgumentsList;
	typedef std::vector<Object> ObjectsList;
	typedef std::vector<Node> NodesList;

	NodesList parents() const; /// Returns the list of parents.
	Node root() const; /// Gets the root node of the graph.
	const std::string& path() const; /// Gets the path from the root to this Node (including "@/")

	void init(); /// Initializes the node.

	Node createChild(const std::string& name); /// Creates and returns a chid node having the given name.
	Object createObject(const std::string& type, const ArgumentsList& arguments = {}); /// Creates an object of the given type, using the given arguments. Returns an invalid object on failure.

	void addChild(const Node& child); /// Adds the given node as a child.
	void addObject(const Object& object); /// Adds the given object to the node.

	void removeChild(const Node& child); /// Removes the given node.
	void removeObject(const Object& object); /// Removes the given object.

	Node child(const std::string& name) const; /// Retrieves the first child node having the given name. Returns an invalid node on failure.
	NodesList children() const; /// gets all child nodes.
	Object object(const std::string& name) const; /// Retrieves the first object having the given name. Returns an invalid object on failure.
	Object objectOfType(const std::string& type) const; /// Retrieves the first object being of the given type. Returns an invalid object on failure.
	ObjectsList objects() const; /// Gets all objects in this node.

	enum class SearchDirection : int32_t
	{ Up = -1, Local = 0, Down = 1, Root = 2, Parents = 3 };

	Object findFirstObject(const std::string& type, const std::vector<std::string>& tags = {}, SearchDirection dir = SearchDirection::Up) const; /// Returns the first object in the hierarchy of this type and with these tags
	Object findObject(const std::string& path) const; /// Returns the object at the path relative to this node
	ObjectsList findObjects(const std::string& type, const std::vector<std::string>& tags = {}, SearchDirection dir = SearchDirection::Up) const; /// Returns all the objects in the hierarchy corresponding of this type and with these tags

	Data data(const std::string& name) const; /// Retrieves the first data having the given name. Returns an invalid data on failure.

	void exportXML(std::ostream& stream) const; /// Writes to the given stream the XML representation of the node, its objects and child nodes.

private:
	friend class NodeImpl;
	Node(std::shared_ptr<NodeImpl> impl);
	std::shared_ptr<NodeImpl> m_impl;
};

} // namespace sfe

#endif // SFELITE_NODE_H
