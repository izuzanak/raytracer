
#include "raytracer_demo.h"

namespace rt_demo {

using dp_t = cclvar::dict_pair_t;

bool RaytracerDemo::render()
{/*{{{*/

  // - construct scene validator -
  cclvarval::validator_c scene_validator{cclvar::dict_t
  {/*{{{*/
    dp_t{"pos_integer",cclvar::dict_t{
      dp_t{"type","integer"},
      dp_t{">",0},
    }},
    dp_t{"pos_float",cclvar::dict_t{
      dp_t{"type","float"},
      dp_t{">",0.0},
    }},
    dp_t{"string_id",cclvar::dict_t{
      dp_t{"type","string"},
      dp_t{"size >",0},
    }},
    dp_t{"point",cclvar::dict_t{
      dp_t{"type","array"},
      dp_t{"size ==",3},
      dp_t{"all-items",cclvar::dict_t{
        dp_t{"type","float"},
      }},
    }},
    dp_t{"vector",cclvar::dict_t{
      dp_t{"type","array"},
      dp_t{"size ==",3},
      dp_t{"all-items",cclvar::dict_t{
        dp_t{"type","float"},
      }},
    }},
    dp_t{"color_comp",cclvar::dict_t{
      dp_t{"type","float"},
      dp_t{">=",0.0},
      dp_t{"<=",1.0},
    }},
    dp_t{"color",cclvar::dict_t{
      dp_t{"type","array"},
      dp_t{"size ==",3},
      dp_t{"all-items","color_comp"},
    }},
    dp_t{"camera",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"position","point"},
        dp_t{"look_at","vector"},
        dp_t{"up","vector"},
      }},
    }},
    dp_t{"image",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"width","pos_integer"},
        dp_t{"height","pos_integer"},
        dp_t{"distance","pos_float"},
        dp_t{"sample_side","pos_integer"},
      }},
    }},
    dp_t{"light",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"position","point"},
        dp_t{"color","color"},
      }},
    }},
    dp_t{"material",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"ambient","color"},
        dp_t{"diffuse","color_comp"},
        dp_t{"specular","color_comp"},
        dp_t{"specular_k","pos_integer"},
        dp_t{"reflection","color_comp"},
      }},
    }},
    dp_t{"texture",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"type",cclvar::dict_t{
          dp_t{"type","integer"},
          dp_t{">=",0},
          dp_t{"<=",5},
        }},
        dp_t{"target",cclvar::dict_t{
          dp_t{"regex",R"(^NORMAL\|AMBIENT$)"},
        }},
        dp_t{"frequency","pos_float"},
        dp_t{"amplitude","pos_float"},
      }},
    }},
    dp_t{"shape",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"material","string_id"},
        dp_t{"textures",cclvar::dict_t{
          dp_t{"type","array"},
          dp_t{"all-items","string_id"},
        }},
      }},
      dp_t{"opt-items",cclvar::dict_t{
        dp_t{"plane","sh_plane"},
        dp_t{"sphere","sh_sphere"},
        dp_t{"triangle","sh_triangle"},
      }},
    }},
    dp_t{"sh_plane",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"point","point"},
        dp_t{"normal","vector"},
      }},
    }},
    dp_t{"sh_sphere",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"center","point"},
        dp_t{"radius","pos_float"},
      }},
    }},
    dp_t{"sh_triangle",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"vertices",cclvar::dict_t{
          dp_t{"type","array"},
          dp_t{"size ==",3},
          dp_t{"all-items","point"},
        }},
      }},
    }},
    dp_t{"root",cclvar::dict_t{
      dp_t{"type","dict"},
      dp_t{"items",cclvar::dict_t{
        dp_t{"camera","camera"},
        dp_t{"image","image"},
        dp_t{"lights",cclvar::dict_t{
          dp_t{"type","array"},
          dp_t{"all-items","light"},
        }},
        dp_t{"materials",cclvar::dict_t{
          dp_t{"type","dict"},
          dp_t{"all-items","material"},
        }},
        dp_t{"textures",cclvar::dict_t{
          dp_t{"type","dict"},
          dp_t{"all-items","texture"},
        }},
        dp_t{"shapes",cclvar::dict_t{
          dp_t{"type","array"},
          dp_t{"all-items","shape"},
        }},
      }},
    }},
  }};/*}}}*/

  cclvar::var_c scene;

  try
  {
    // - read scene from json file -
    scene = ccljson::parse(cclsys::file_c(m_scene_file,"r").read_close());
  }

  // - ERROR -
  catch (std::exception &e)
  {
    std::cerr << m_prog_name
      << ": cannot read scene: " << e.what() << std::endl;

    return false;
  }

  try
  {
    // - validate scene -
    scene_validator.validate(scene);
  }

  // - ERROR -
  catch (std::exception &e)
  {
    std::cerr << m_prog_name
      << ": error " << e.what() << " at "
      << scene_validator.value_stack() << std::endl;

    return false;
  }

  // - material name to index map -
  cclvar::var_c material_map{cclvar::dict_t{}};

  // - texture name to index map -
  cclvar::var_c texture_map{cclvar::dict_t{}};

  // - texture target string to enum map -
  cclvar::var_c texture_target_map{cclvar::dict_t{
    dp_t{"AMBIENT",texture_t::Target::AMBIENT},
    dp_t{"NORMAL" ,texture_t::Target::NORMAL},
  }};

  // - create raytracer object -
  auto camera = scene["camera"];
  auto image = scene["image"];

  auto cam_position = camera["position"];
  auto cam_look_at = camera["look_at"];
  auto cam_up = camera["up"];

  Raytracer raytracer(
    point_t(cam_position[0].to_float(),
            cam_position[1].to_float(),
            cam_position[2].to_float()),
    point_t(cam_look_at[0].to_float(),
            cam_look_at[1].to_float(),
            cam_look_at[2].to_float()),
    vector_t(cam_up[0].to_float(),
             cam_up[1].to_float(),
             cam_up[2].to_float()),
    image["width"].to_int(),
    image["height"].to_int(),
    image["distance"].to_float(),
    image["sample_side"].to_int());

  // - process scene lights -
  for (auto &light : scene["lights"].to_array())
  {
    auto position = light["position"];
    auto color = light["color"];

    // - add light to raytracer scene -
    raytracer.add_light(light_t(
      point_t(position[0].to_float(),
              position[1].to_float(),
              position[2].to_float()),
      color_t(color[0].to_float(),
              color[1].to_float(),
              color[2].to_float())));
  }

  // - process scene materials -
  for (auto &material : scene["materials"].to_dict())
  {
    auto ambient = material.second["ambient"];

    // - add material to raytracer scene -
    material_map[material.first] = raytracer.add_material(material_t(
      color_t(ambient[0].to_float(),
              ambient[1].to_float(),
              ambient[2].to_float()),
      material.second["diffuse"].to_float(),
      material.second["specular"].to_float(),
      material.second["specular_k"].to_int(),
      material.second["reflection"].to_float(),
      0.0));
  }

  // - process scene textures -
  for (auto &texture : scene["textures"].to_dict())
  {
    // - add texture to raytracer scene -
    texture_map[texture.first] = raytracer.add_texture(texture_t(
      static_cast<texture_t::Type>(texture.second["type"].to_int()),
      static_cast<texture_t::Target>(texture_target_map[texture.second["target"]].to_int()),
      texture.second["frequency"].to_float(),
      texture.second["amplitude"].to_float()));
  }

  // - process scene shapes -
  tex_idxs_t texture_indexes;
  for (auto &shape : scene["shapes"].to_array())
  {
    // - retrieve material index -
    cclvar::var_c material_idx;
    if (!material_map.has_key(shape["material"],material_idx))
    {
      // - ERROR -
      throw std::runtime_error("invalid shape material name");
    }

    // - process shape textures -
    texture_indexes.clear();
    cclvar::var_c texture_idx;
    for (auto &tex_name : shape["textures"].to_array())
    {
      // - retrieve texture index -
      if (!texture_map.has_key(tex_name,texture_idx))
      {
        // - ERROR -
        throw std::runtime_error("invalid shape texture name");
      }

      texture_indexes.push_back(texture_idx.to_int());
    }

    cclvar::var_c geometry;

    // - process plane geometry -
    if (shape.has_key("plane",geometry))
    {
      auto point = geometry["point"];
      auto normal = geometry["normal"];

      // - add plane to raytracer scene -
      raytracer.add_shape(std::make_unique<plane_t>(
        point[0].to_float(),
        point[1].to_float(),
        point[2].to_float(),
        normal[0].to_float(),
        normal[1].to_float(),
        normal[2].to_float(),
        material_idx.to_int(),
        std::move(texture_indexes)));

      continue;
    }

    // - process sphere geometry -
    if (shape.has_key("sphere",geometry))
    {
      auto center = geometry["center"];

      // - add sphere to raytracer scene -
      raytracer.add_shape(std::make_unique<sphere_t>(
        center[0].to_float(),
        center[1].to_float(),
        center[2].to_float(),
        geometry["radius"].to_float(),
        material_idx.to_int(),
        std::move(texture_indexes)));

      continue;
    }

    // - process triangle geometry -
    if (shape.has_key("triangle",geometry))
    {
      auto vertices = geometry["vertices"];
      auto v0 = vertices[0];
      auto v1 = vertices[1];
      auto v2 = vertices[2];

      // - add triangle to raytracer scene -
      raytracer.add_shape(std::make_unique<triangle_t>(
        v0[0].to_float(),v0[1].to_float(),v0[2].to_float(),
        v1[0].to_float(),v1[1].to_float(),v1[2].to_float(),
        v2[0].to_float(),v2[1].to_float(),v2[2].to_float(),
        material_idx.to_int(),
        std::move(texture_indexes)));

      continue;
    }
  }

  // - render scene -
  unsigned thread_count = std::thread::hardware_concurrency();
  if (thread_count > 1)
  {
    // - parallel rendering -
    raytracer.render_parallel(thread_count);
  }
  else
  {
    // - single thread rendering -
    raytracer.render();
  }

  try
  {
    // - write image to file -
    raytracer.save_bmp_file(m_output_file);
  }

  // - ERROR -
  catch (std::exception &e)
  {
    std::cerr << m_prog_name << ": " << e.what() << std::endl;
    return false;
  }

  return true;
}/*}}}*/

} // namespace rt_demo

