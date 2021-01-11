# FIEA Game Engine
### Cross platform Data-Driven game engine developed using modern C++ standard ( C++17 )

This engine is component-based & JSON is used as a configuration language. 

#### Features custom implementation of various STL like containers :-
* Singly Linked List
* Vector
* Hashmap

#### Features Various design patterns :-
* Singleton
* Composite
* Factory
* Chain of Responsibility
* Observer

#### Various components of this engine are :-
* Datum : A run-time polymorphic homogeneous container which  stores values of type Integer, Float, Vector, Matrix, Table, String, Pointer.
* Scope : A Scope is a dynamic hierarchical database which stores Datums by name.
* Factory : A class that allows instantiation of other objects at runtime by their Typename. Suitable for use with any interface class which has a default constructor.
* World contains everything that exists in the game. Its divided into Sectors. Sectors can be considered as a part of the world ( Scene ). Sectors are composed of Entities which are basic building block of world.
* Action : It is a specific action that can be performed by any entity
* Reaction : Reaction is a abstract base class which ties Event System to Rest of the Engine which acts as a response to an action
* Event System : Entities can subscribe to events. Events are processed and delivered through an event queue which resides in the World.

Me and my collegues ported a 2D rapid prototype game made in Unity engine into this custom engine. Gameplay video for the same can be found here.
https://www.youtube.com/watch?v=AJFU5yuVLiY
