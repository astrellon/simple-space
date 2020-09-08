template <typename T>
inline Line<T>::Line()
{

}

template <typename T>
inline Line<T>::Line(const sf::Vector2<T> &point1, const sf::Vector2<T> &point2) :
    point1(point1), point2(point2)
 {

 }