#include<iostream>
#include<string>
#include<vec3.h>
#include<fstream>
#include<vector>
#include<sstream>

struct vec2
{
	float x, y;
};

struct triangle
{
	vec3 a, b, c;
	vec3 n1, n2, n3;
	vec2 d1, d2, d3;
};

std::vector<triangle> maketrigon(std::string str)
{
	std::ifstream fin;
	fin.open(str);
	// Vertex position
	vec3 v;
	std::vector <vec3> vm;
	// Vertex normal
	vec3 vn;
	std::vector <vec3> vnm;
	// Vertex array
	vec2 vt;
	std::vector <triangle> vtm;
	//Triangles array
	std::vector <triangle> trm;
	//File open error check
	if (!fin.is_open())
	{
		throw "ERROR::OBJloader::Could not open file."
	}
	else
	{
		//Read one line at a time
		std::string line;
		std::stringstream ss;
		std::string prefix = "";
		while (std::getline(fin, line))
		{
			ss.clear();
			ss.str(line);
			ss >> prefix;

			if (prefix == "v") //Vertex position
			{
				float vx, vy, vz;
				v.x = vx;
				v.y = vy;
				v.z = vz;
				vm.push_back(v);
			}
			if (prefix == "vn") //Vertex normal
			{
				float vnx, vny, vnz;
				vn.x = vnx;
				vn.y = vny;
				vn.z = vnz;
				vnm.push_back(vn);
			}
			if (prefix == "vt") //Vertex normal indicies
			{
				float vtx, vty;
				vt.x = vtx;
				vt.y = vty;
				vtm.push_back(vt);
			}

		}
		while (getline(fin, line))
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3, v4, vt4, vn4;
				std::string line;
				const char* cline = line.c_str();
				sscanf(cline, "%s %d%c%d%c%d %d%c%d%c%d %d%c%d%c%d %d%c%d%c%d", &line, &v1, &line, &vt1, &line, &vn1, &v2, &line, &vt2, &line, &vn2, &v3, &line, &vt3, &line, &vn3, &v4, &line, &vt4, &line, &vn4);

				triangle trigon1, trigon2;
				//first triagle
				{
				trigon1.a = vm[(v1 - 1)];
				trigon1.b = vm[(v2 - 1)];
				trigon1.c = vm[(v4 - 1)];

				trigon1.n1 = vnm[(vn1 - 1)];
				trigon1.n2 = vnm[(vn2 - 1)];
				trigon1.n3 = vnm[(vn4 - 1)];

				trigon1.d1 = vtm[(vt1 - 1)];
				trigon1.d2 = vtm[(vt2 - 1)];
				trigon1.d3 = vtm[(vt4 - 1)]; }

				//second triagle
				{
				trigon2.a = vm[(v2 - 1)];
				trigon2.b = vm[(v3 - 1)];
				trigon2.c = vm[(v4 - 1)];

				trigon2.n1 = vnm[(vn2 - 1)];
				trigon2.n2 = vnm[(vn3 - 1)];
				trigon2.n3 = vnm[(vn4 - 1)];

				trigon2.d1 = vtm[(vt2 - 1)];
				trigon2.d2 = vtm[(vt3 - 1)];
				trigon2.d3 = vtm[(vt4 - 1)];
				}

				trm.push_back(trigon1);
				trm.push_back(trigon2);

			}
		}
}
	fin.close();
	return trm;
