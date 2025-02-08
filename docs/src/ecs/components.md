# Components

A component is a data container that holds specific attributes of an entity.

| **Component**    | **Description**                               | **Values**                                                                                 |
|------------------|-----------------------------------------------|--------------------------------------------------------------------------------------------|
| **Button**       | Defines entity as a button                    | • `std::string text`<br/> • `std::string fontFile`<br/> • `unsigned int characterSize`     |
| **OptionButton** | Defines entity as a checkbox button           | • `std::pair<double, double> size`                                                         |
| **Slider**       | Defines entity as a slider button             | • `std::pair<double, double> length`<br/> • `std::pair<double, double> size`               |
| **ButtonRect**   | Defines entity as an input text               | • `const std::pair<int, int> sizeRect`<br/> • `sf::Color color`<br/> • `bool showOutline`  |
| **Sprite**       | Defines entity as a Sprite                    | • `std::pair<float, float> size`                                                           |
| **Texture**      | Defines the texture of an entity              | • `std::string texturePath`<br/> • `std::vector<int> textureRect`                          |
| **Text**         | Defines entity as a text                      | • `std::string text`<br/> • `std::string fontFile`<br/> • `unsigned int characterSize`     |
| **Position**     | Stores positions of an entity                 | • `std::vector<std::pair<float, float>> positions`                                         |
| **Color**        | Defines the color of an entity.               | • `std::vector<double> color`                                                              |
| **Shape**        | Defines entity as shape (rectangle or circle) | • `ShapeType type`<br/> • `std::pair<double, double> size`<br/> • `float radius`           |
| **Sound**        | Defines a sound at an entity                  | • `std::string soundFile`                                                                  |
| **Link**         | Link an entity to another entity              | • `int id`                                                                                 |
| **View**         | Set the window view to follow the entity.              | • `sf::View view` <br/> • `std::pair<float, float> size`                                                                                |