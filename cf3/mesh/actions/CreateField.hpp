// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_mesh_actions_CreateField_hpp
#define cf3_mesh_actions_CreateField_hpp

////////////////////////////////////////////////////////////////////////////////

#include "mesh/MeshTransformer.hpp"
#include "mesh/actions/LibActions.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace mesh {

  class Dictionary;
  class LoadMesh;
  class AInterpolator;

namespace actions {

//////////////////////////////////////////////////////////////////////////////

/// @brief Import variables from another mesh
///
/// Optionally, a dictionary can be provided to load the variables into
/// Every imported variable will have its own field
/// @author Willem Deconinck
class mesh_actions_API CreateField : public MeshTransformer
{   
public: // functions
  
  /// constructor
  CreateField( const std::string& name );
  
  /// Gets the Class name
  static std::string type_name() { return "CreateField"; }

  virtual void execute();
  
private: // functions

  void replace_var_name (const std::string& var_from,
                         const std::string& var_to,
                         std::vector<std::string>& functions);

private: // data

  std::vector<std::string> m_import_var_names;
  std::vector<std::string> m_var_names;

  Handle<LoadMesh> m_mesh_loader;
  Handle<AInterpolator> m_interpolator;

}; // end CreateField

////////////////////////////////////////////////////////////////////////////////

} // actions
} // mesh
} // cf3

////////////////////////////////////////////////////////////////////////////////

#endif // cf3_mesh_actions_CreateField_hpp
