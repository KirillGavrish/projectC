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
//структурки нужные
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
 
 
// (сортировка)
void sort_triangles(std::span <Triangle> triangles)
{
	for(auto node = triangles.begin(); node != triangles.end(); node++)
    	box = box_union(box, triangle_to_box(*node));
  	int i, oss_number;
  	vec3 diag = {0, 0, 0};
  	for(i = 0; i < 3; i++)
  	{
  		if (length(box.upper_bound[i] - box.lower_bound[i]) > length(max_os))
  		{
  			max_os = box.upper_bound[i] - box.lower_bound[i];
  			oss_number = i;
  		}
  	}
	std::sort
    	(
        	triangles.begin(),
        	triangles.end(),
        	[](Triangle a, Triangle b)
        	{
            	return box_center(triangle_to_box(a))[oss_number] < box_center(triangle_to_box(b))[oss_number];
        	}
    	);
} 

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
  for(auto node = triangles.begin(); node != triangles.end(); node++)
    	box = box_union(box, triangle_to_box(*node));

  return cons({triangles.begin(), mid}, box, {mid, triangles.end()});
}
 
// обход дериваа

struct RayTriangleIntersection
{
    float t, p, q;
};

RayTriangleIntersection rayTriangleIntersection(Ray const &ray, Triangle const &triangle)
{
    vec3 const a = triangle.r1 - triangle.r0;
    vec3 const b = triangle.r2 - triangle.r0;
    vec3 const c = ray.origin - triangle.r0;
    vec3 const d = ray.direction;
    float const det0 = dot(-d, cross(a, b));
    float const det1 = dot( c, cross(a, b));
    float const det2 = dot(-d, cross(c, b));
    float const det3 = dot(-d, cross(a, c));
    return 
    {
        .t = det1 / det0,
        .p = det2 / det0,
        .q = det3 / det0,
    };
}

struct RayTreeIntersection
{
  Triangle triangle;
  RayTriangleIntersection intersection;
};

struct MaybeTwoIntersections
{
    float tMin, tMax;
};

MaybeTwoIntersections const None = {.tMin = 0.f, .tMax = -1.f};


MaybeTwoIntersections rayboxIntersection(Ray const &ray, bounding_box const &box)
{
    vec3 const a = (box.lower_bound - ray.origin) / ray.direction;
    vec3 const b = (box.upper_bound - ray.origin) / ray.direction;
    auto const min = [](float const x, float const y) {return x < y ? x : y;};
    auto const max = [](float const x, float const y) {return x < y ? y : x;};
    vec3 const tMin =
    {
        min(a.x, b.x),
        min(a.y, b.y),
        min(a.z, b.z),
    };
    vec3 const tMax =
    {
        max(a.x, b.x),
        max(a.y, b.y),
        max(a.z, b.z),
    };
    return
    {
        max(max(tMin.x, tMin.y), tMin.z),
        min(min(tMax.x, tMax.y), tMax.z),
    };
}

bool happened(RayTreeIntersection const i)
{
    return i.tMin < i.tMax && i.tMin > 1e-5f;
}

bool happened(RayTriangleIntersection const &i)
{
    return i.p >= 0.f
        && i.q >= 0.f
        && i.p + i.q <= 1.f;
}

bool happened(MaybeTwoIntersections const i)
{
    return i.tMin < i.tMax;
}
//treewalker
std::optional<RayTreeIntersection> rayTreeIntersection(Ray &ray, Tree tree)
{
  if(std::holds_alternative<Triangle>(tree->content))
  {
    auto &triangle = std::get<Triangle>(tree->content);
    auto intersection = rayTriangleIntersection(ray, triangle);
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
