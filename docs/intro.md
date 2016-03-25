Structure
=========
cort uses a modified client-server model. The optional client-side allows for visualization of and interaction with the current simulation and runs in the browser. The server-side is a simulation written in C++ containing a brain model defined by a JSON file.
From highest to lowest level, the structure of the server-side looks like:
Simulation - A simulation holds zero or more entities. There is only one simulation per instance of Cort. The simulation proceeds in a series of steps. In each step, every entity is updated in an arbitrary order. Every step represents a predetermined amount of time. A simulation continues until the application exits.
Entity - An entity is anything that can be simulated indepedently of other entities. It receives its inputs from other entities and the environment (part of the Simulation) and then creates outputs which affect other entities or change the environment. Every entity is created based on a JSON file.
Brain(Entity) - A brain is a type of entity. A brain is composed of zero or more layers. 
Layer - A layer is a set of neurons which are created according to a certain pattern. We know that there is insufficient information in the human DNA to determine the type, position, behavior, etc. of every neuron so neurons must be created based on a generative algorithm. In general, a Layer is categorized by its dimension: 1D, 2D, or 3D.
Neuron - A neuron is the funademntal unit of compuation. Updating a neuron occurs in two steps. First, every neuron updates its membrane potential based on signals that were transmitted by presynaptic neurons and subsequently opened membrane receptors. 
Connection
Synapse
Neurotransmitter
