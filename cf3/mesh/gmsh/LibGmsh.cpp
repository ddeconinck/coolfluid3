// Copyright (C) 2010-2013 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "common/RegistLibrary.hpp"

#include "mesh/gmsh/LibGmsh.hpp"

namespace cf3 {
namespace mesh {
namespace gmsh {

cf3::common::RegistLibrary<LibGmsh> libGmsh;

} // gmsh
} // mesh
} // cf3
