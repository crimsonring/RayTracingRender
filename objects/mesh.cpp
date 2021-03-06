#include "mesh.h"
#include "../accel/aabbox.h"
namespace Orchid
{
	Mesh::Mesh(Vector3d p_, const char * file_path, Material m_)
	{
        _p = p_;
        m_m = m_;
        //mychange:
        
        box.Init();
        
        //-------------
		std::string mtlbasepath;
		std::string inputfile = file_path;
		unsigned long pos = inputfile.find_last_of("/");
		mtlbasepath = inputfile.substr(0, pos + 1);

		printf("Loading %s...\n", file_path);
		// Attempt to load mesh
		std::string err = tinyobj::LoadObj(m_shapes, m_materials, inputfile.c_str(), mtlbasepath.c_str());

		if (!err.empty()) {
			std::cerr << err << std::endl;
			exit(1);
		}
		printf(" - Generating k-d tree...\n\n");
        
        
		long shapes_size, indices_size, materials_size;
		shapes_size = m_shapes.size();
		materials_size = m_materials.size();

		// Load materials/textures from obj
		// TODO: Only texture is loaded at the moment, need to implement material types and colours
		for (int i = 0; i < materials_size; i++) {
			std::string texture_path = "";

			if (!m_materials[i].diffuse_texname.empty()) {
				if (m_materials[i].diffuse_texname[0] == '/') texture_path = m_materials[i].diffuse_texname;
				texture_path = mtlbasepath + m_materials[i].diffuse_texname;
				materials.push_back(Material(DIFF, Vector3d(1, 1, 1), Vector3d(), texture_path.c_str()));
			}
			else {
				materials.push_back(Material(DIFF, Vector3d(1, 1, 1), Vector3d()));
			}

		}

		// Load triangles from obj
		for (int i = 0; i < shapes_size; i++) {
			indices_size = m_shapes[i].mesh.indices.size() / 3;
			for (size_t f = 0; f < indices_size; f++) {

				// Triangle vertex coordinates
				Vector3d v0_ = Vector3d(
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 1],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 2]
				) *5+ _p;

				Vector3d v1_ = Vector3d(
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 1],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 2]
				) *5+ _p;

				Vector3d v2_ = Vector3d(
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 1],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 2]
				) *5+ _p;

				Vector3d t0_, t1_, t2_;

				//Attempt to load triangle texture coordinates
				if (m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1 < m_shapes[i].mesh.texcoords.size()) {
					t0_ = Vector3d(
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2],
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2 + 1],
						0
					);

					t1_ = Vector3d(
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2],
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2 + 1],
						0
					);

					t2_ = Vector3d(
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2],
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1],
						0
					);
				}
				else {
					t0_ = Vector3d();
					t1_ = Vector3d();
					t2_ = Vector3d();
				}
                //mychange:
                
                box.Total.expand(v0_);
                box.Total.expand(v1_);
                box.Total.expand(v2_);
                //-----------------------------
				if (m_shapes[i].mesh.material_ids[f] < materials.size())
					tris.push_back(new Triangle(v0_, v1_, v2_, materials[m_shapes[i].mesh.material_ids[f]]));
				else
					tris.push_back(new Triangle(v0_, v1_, v2_, m_m));
                
			}
		}
        //mychange:
        box.Dividebox();
        Vector3d p0,p1,p2;
        for(int i=0;i<tris.size();i++){
            p0=tris[i]->get(0);
            p1=tris[i]->get(1);
            p2=tris[i]->get(2);
            box.Grid(i,p0,p1,p2);
        }
        //----------------
		// Clean up
		m_shapes.clear();
		m_materials.clear();
		printf("\n");
	}
    
	ObjectIntersection Mesh::getIntersection(const Ray & r)
	{
		double t = 0, tmin = INFINITY;
		Vector3d normal = Vector3d();
		Vector3d colour = Vector3d();
        int tri_id=0,box_id=0;
        ObjectIntersection isct = ObjectIntersection();
        ObjectIntersection temp;
        if(!box.Total.intersection(r, t)){
            return isct;
        }
        t=0;
        for (int i = 0; i < 8; i++)
        {
            if(box.getbox(i).intersection(r, t))
            {
                if(t<tmin)
                {
                    tmin=t;
                    box_id=i;
                }
            }
        }
        long size=box.array[box_id].size();
        for(int j=0;j<size;j++){
            tri_id=box.array[box_id][j];
            temp=tris.at((unsigned)tri_id)->getIntersection(r);
            if (temp._hit) {
                //                        std::cout<<"hit"<<std::endl;
                if (isct._u == 0 || temp._u < isct._u) isct = temp;
            }
        }
        return isct;
//        return ObjectIntersection(false, tmin, normal, Material(DIFF, colour, Vector3d()));

	}
}
