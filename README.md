# CommonUtilities
Assortment of useful utilites you can apply in your projects. It can be compiled as either a DLL or as a static library through Visual Studio.

### Allocators
- **Arena** - Simple arena allocator that works with stl containers.

### Event
- **Event** - Holds any number of callbacks that may all be executed manually. Expanded with thread-safety and an **EventID** that removes a callback when instance is destructed.
- **TimedEvent** - Same as **Event**, but calls events after a set duration.

### Input
Full **Windows** support for keyboard, mouse, cursor, and 4 gamepads. Expanded with being able to customize **bindings** for buttons to your liking.

### Math
- **Vector** - Support for 2D, 3D, 4D Vectors with many utility functions, e.g., Dot, Cross, AngleTo, Lerp, ProjectOnto.
- **Matrix** - Support for 3x3, 4x4 Matrices with many utility functions, e.g., GetTransposed, CreatePerspective, TransformPoint.
- **Quaternion** - Support for Quaternion with many utility functions, e.g., Slerp, ToEuler, and RotateTowards.
- **Shapes** - Support for several basic shapes, e.g., Ray, Sphere, AABB, Capsule, Triangle, Plane.
- **Intersection** - Basic intersection logic between some shapes, e.g., Segment vs Capsule, AABB vs Sphere.
- **Frustum** - Support for Frustum with many utility functions, e.g., Subdivide, IsInside, Contains.
- **Rect** - Support for Rect with many utility functions, e.g., Area, Union, Contains.

### Random
Comprehensive support for generating random things, e.g., number between a range, shuffled array, point in sphere, point in cone.

- **PerlinNoise** - Support for 1D, 2D, 3D Perlin Noise and Curl Noise.

### Serialization
Comprehensive support for serializing many structures to and from binary data. Can serialize, for example, all trivial types, std::string, std::vector, and std::tuple. You can provide serialization for your own types by specializing SerializeAsBinary.

### Structures
- **Blackboard** - Has similar interface with std::unordered_map where the difference being you can set and retrieve any kind of value.
- **EnumArray** - Use an enum to index an array instead of an integer.
- **FreeVector** - Elements always have the same position, where you have to save the returned identifier when inserted to remove later.
- **Octree** - Cache-friendly octree with very fast query, insertion, and removal. When removing from Octree, make sure to call Cleanup afterwards.
- **QuadTree** - Same as Octree, but in 2D.
- **PriorityQueue** - Min/Max Heap, useful for pathfinding.
- **SmallVector** - Uses stack when below a threshold, and switches to using heap when above it.
- **StaticVector** - Identical to std::vector, but uses the stack with a fixed capacity.

### System
- **Color** - Basic RGBA color structure with few utility functions.
- **IDGenerator** - Utility functions to generate unique IDs for types, or just an incrementing index.
- **StateMachine** - Easily extendable StateMachine with basic functions, e.g., Enter, Update, Exit.
- **StateStack** - Easily extendable StateStack with basic functions, e.g., Push, Pop, Clear, OnActive, OnDeactivate.

### Thread
- **Parallel** - Utility functions to execute a loop parallelized. Uses std::execution policies.
- **ThreadLoops** - Set a function to execute in a loop on another thread.
- **ThreadPool** - Enqueue a function to execute on another thread. You get a std::future when enqueued that you can use to query status of thread.

### Time
- **Timer** - Class used to calculate the delta time between each call to Update. Expanded with Fixed DT, Scaled Time, and Run Time.
- **StopWatch** - Uses **Timer** and functions similarly to C# StopWatch. You can Start, StartNew, Stop, and Reset.

### Utility
- **ArithmeticUtils** - Utilities relating to arithmetics, math constants, rad to deg, wrap an integer, map to range, clamp, etc.
- **Benchmark** - Utility you can use to benchmark code by checking its CPU and RAM usage.
- **BitUtils** - Few utilities to pack and extract values to and from a unsigned 64-bit integer.
- **Easings** - Easing functions from https://easings.net/
- **StringUtils** - Utilities for easy manipulation of std::string. 
