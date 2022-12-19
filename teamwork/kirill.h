#include "andrey.h"
#include <algorithm>
#include <cmath>
#include <variant>
#include <span>
#include <optional>

struct vec3
{
	float x, y, z;

	float &operator[](unsigned int const i)       {return &x + i;};
	float  operator[](unsigned int const i) const {return &x + i;};
};


inline vec3 operator+(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
    };
}
inline vec3 operator-(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
    };
}
inline vec3 operator-(vec3 const &v)
{
    return {-v.x, -v.y, -v.z};
}
inline vec3 operator*(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z,
    };
}
inline vec3 operator*(vec3 const &v, float const f)
{
    return
    {
        v.x * f,
        v.y * f,
        v.z * f,
    };
}
inline vec3 operator/(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.x / v2.x,
        v1.y / v2.y,
        v1.z / v2.z,
    };
}
inline vec3 operator/(vec3 const &v, float const f)
{
    return
    {
        v.x / f,
        v.y / f,
        v.z / f,
    };
}
inline float dot(vec3 const &v1, vec3 const &v2)
{
    return v1.x * v2.x
         + v1.y * v2.y
         + v1.z * v2.z;
}
inline vec3 cross(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}
inline float length(vec3 const &v)
{
    return std::sqrt(dot(v, v));
}
inline vec3 normalize(vec3 const &v)
{
    return v / length(v);
}

vec3 min_vec(vec3 a, vec3 b, vec3 c)
{
	return {std::min(a.x, b.x, c.x), std::min(a.y, b.y, c.y), std::min(a.z, b.z, c.z)};
}

vec3 max_vec(vec3 a, vec3 b, vec3 c)
{
	return {std::max(a.x, b.x, c.x), std::max(a.y, b.y, c.y), std::max(a.z, b.z, c.z)};
}

struct Triangle
{
	vec3 a, b, c;
};

struct bounding_box
{
	vec3 upper_bound;
	vec3 lower_bound;
};

bounding_box triangle_to_box(Triangle triangle)
{
	return { max_vec(triangle.a, triangle.b, triangle.c),
		       min_vec(triangle.a, triangle.b, triangle.c) };
}

bounding_box box_union(bounding_box left, bounding_box right)
{
	return {max_vec(left.upper_bound, right.upper_bound, right.upper_bound),
		      min_vec(left.lower_bound, right.lower_bound, right.lower_bound)};
}

vec3 box_center(bounding_box box)
{
	return {(box.upper_bound + box.lower_bound)/2};
}

//структурки нужные
struct Node
{
  struct leafs
  {
    Node * left;
    Node * right;
  };
  bounding_box box;
  std::variant<leafs, Triangle> content;
};
using Tree = Node *;


// каво что я тут делаю (сортировка)
	std::sort
	(
		triangles.begin(),
		triangles.end(),
		[](Triangle a, Triangle b)
		{
			return box_center(triangle_to_box(a)).x < box_center(triangle_to_box(b)).x;
		}
	);

// сборка дериваа
Tree cons(Tree left, bounding_box &box, Tree right)
{
  Node * const node = static_cast<Node *>(std::malloc(sizeof(Node)));
  node -> box = box;
  node -> content = Node::leafs{left, right};
  return node;
}

Tree cons(Triangle &triangle)
{
  Node * const node = static_cast<Node *>(std::malloc(sizeof(Node)));
  node -> box = triangle_to_box(triangle);
  node -> content = triangle;
  return node;
}

Tree createTree(std::span <Triangle> triangles)
{
  if (triangles.size() == 0)
    return nullptr;
  if (triangles.size() == 1)
    return cons(triangles[0]);

  auto mid = triangles.begin() + triangles.size()/2;

  bounding_box box = triangle_to_box(triangles[0]);
  for(Triangle * node = triangles.begin(); node != triangles.end(); node++)
    box = box_union(box, triangle_to_box(*node));

  return cons({triangles.begin(), mid}, box, {mid, triangles.end()});
}

// обход дериваа
struct RayTreeIntersection
{
  Triangle triangle;
  RayTriangleIntersection intersection;
};

std::optional<RayTreeIntersection> rayTreeIntersection(Ray &ray, Tree tree)
{
  if(std::holds_alternative<Triangle>(tree->content))
  {
    auto &triangle = std::get<Triangle>(tree->content);
    auto intersection = rayTreeIntersection(ray, triangle);
    return happened(intersection)
      ? std::optional<RayTreeIntersection>({.triangle = triangle, .intersection = intersection})
      : std::nullopt;
  }

  auto [left, right] = std::get<Node::leafs>(tree->content);
  auto left_intersection  = rayboxIntersection(ray,  left->box);
  auto right_intersection = rayboxIntersection(ray, right->box);

  auto tree_right_intersection = rayTreeIntersection(ray, right);
  auto  tree_left_intersection = rayTreeIntersection(ray,  left);
  if(!happened(left_intersection) && !happened(right_intersection))
    return std::nullopt;
  if(!happened(left_intersection))
    return tree_right_intersection;
  if(!happened(right_intersection))
    return tree_left_intersection;


  if (left_intersection.tMin < right_intersection.tMin)
  {
    auto t = right_intersection;
    auto furthest_intersection = tree_right_intersection;
    auto  closest_intersection =  tree_left_intersection;
  }
  else
  {
    auto t = left_intersection;
    auto  closest_intersection = tree_right_intersection;
    auto furthest_intersection =  tree_left_intersection;
  }

     if(!closest_intersection)
         return furthest_intersection;
     if(closest_intersection->intersection.t < t)
         return closest_intersection;
     if(!furthest_intersection)
         return closest_intersection;
     return closest_intersection->intersection.t < furthest_intersection->intersection.t
          ? closest_intersection
          : furthest_intersection;
}
