// Ce fichier est un composant de la bibliothèque SofaFrontEnd développée 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette bibliothèque est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propriété 
// intellectuelle. Sa détention, sa reproduction ou sa diffusion sont donc 
// soumis à autorisation préalable de la société. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SFELITE_SIMULATION_H
#define SFELITE_SIMULATION_H

#include "Node.h"
#include "sofaFrontEnd.h"

#include <ostream>

namespace sfe
{

class SimulationImpl;
class Helper;

/**
 * @brief Represents the Sofa simulation object.
 */
class SOFA_FRONTENDLITE_API Simulation
{
public:
	Simulation(); /// Creates a simulation (use the getSimulation function to get a valid one)

	bool isValid() const; /// Can we use this simulation ?
	explicit operator bool() const; /// Can we use this simulation ?

	void setDt(double dt); /// Assigns the duration of a time step.
	void setGravity(double x, double y, double z); /// Assigns the gravity
	void step(); /// Performs a simulation step.
	void reset(); /// Reset the current simulation graph to initial values.
	void clear(); /// Unloads and destroys the current simulation graph.
	void init(); /// Initializes the simulation graph.
	void draw(); /// Renders the simulation graph into the current openGL context.

	Node root(); /// Gets the root node.
	void setRoot(const Node& node); /// Assigns the root node.

	bool loadFile(const std::string& fileName); /// Loads a simulation from a scene file and replaces the current graph. The simulation is left cleared on failure.

	Node createNode(const std::string& name); /// Creates a node with the given name.
	Node loadNodeFromFile(const std::string& fileName); /// Creates a node from a scene file. Returns an invalid node on failure.
	Node loadNodeFromMemory(const std::string& xml); /// Creates a node from the given XML fragment. Returns an invalid node on failure.

	Helper* getHelper() const; /// Access to generic helper functions

private:
	friend class SimulationImpl;
	Simulation(std::shared_ptr<SimulationImpl> impl);
	std::shared_ptr<SimulationImpl> m_impl;
};


} // namespace sfe

#endif // SFELITE_SIMULATION_H
