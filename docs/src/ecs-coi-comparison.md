
# Comparison: ECS (Entity-Component-System) vs COI (Composition-Over-Inheritance)

This document compares two fundamental approaches used in game engines: **ECS** (Entity-Component-System) and **COI** (Composition-Over-Inheritance). These paradigms shape how entities, data, and logic are structured in a project.

---

## ECS (Entity-Component-System)

### Description:
- Entities are simple identifiers.
- Components store only data.
- Systems contain business logic and manipulate the associated components.

### Advantages:
- **Modularity**: Highly flexible, components can be dynamically added or removed.
- **Performance**: Optimized for multicore processors through parallelization.
- **Clear separation**: Data and logic are well isolated.

### Disadvantages:
- Higher initial complexity.
- Performance management requires special attention (e.g., frequent memory allocations).

### Use Cases:
- AAA engines like Unity and Unreal Engine.
- Games requiring the simultaneous management of thousands of entities.

### References:
- [Introduction to ECS](https://en.wikipedia.org/wiki/Entity_component_system)
- [Building an ECS engine](http://guillaume.belz.free.fr/doku.php?id=ecs)

---

## COI (Composition-Over-Inheritance)

### Description:
- Prefers object aggregation (objects containing other objects) over inheritance.
- Features are divided into reusable and composable components.

### Advantages:
- **Increased flexibility**: Components can be added or removed without modifying entities.
- **Independence**: Each behavior is autonomous and replaceable.
- **Avoids deep hierarchies**: Reduces rigid dependencies.

### Disadvantages:
- May require more code to explicitly manage components.
- Less performant and more scattered than ECS for projects with numerous entities.

### Use Cases:
- Frameworks like Godot (using combinable Nodes).
- Medium-sized projects or those needing flexible behavior.

---

## Comparison: ECS vs COI

| Aspect               | ECS                                   | COI                                       |
|----------------------|---------------------------------------|-------------------------------------------|
| **Description**      | Separate identifiers, data, and systems. | Independent components combined in entities. |
| **Entity structure** | Simple identifiers linked to components. | Entities directly containing their components. |
| **Data**             | Data containers without logic.       | Contain both data and behaviors.         |
| **Logic**            | Centralized in reusable systems.     | Encapsulated locally in each component.  |
| **Modularity**       | Dynamic: components easily added/removed. | Directly integrated into the entity.     |
| **Performance**      | Excellent (contiguous storage, cache-friendly). | Lower performance (frequent cache misses). |
| **Complexity**       | More complex to set up and understand. | Simpler for medium-sized projects.       |
| **Examples**         | Unity, Unreal Engine.                | Godot, flexible medium-sized projects.   |

---

## Recommendation

- **ECS**: Ideal for complex games with thousands of entities to manage simultaneously.
- **COI**: Perfect for medium-sized projects where readability and flexibility are priorities.
