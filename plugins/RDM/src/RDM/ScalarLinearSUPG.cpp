// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/CBuilder.hpp"

#include "RDM/CSysSUPG.hpp"
#include "RDM/SchemeCSysSUPG.hpp"

#include "RDM/SupportedTypes.hpp"    // supported elements

#include "RDM/LinearAdv2D.hpp"       // supported physics
#include "RDM/LinearAdvSys2D.hpp"    // supported physics
#include "RDM/RotationAdv2D.hpp"     // supported physics

using namespace CF::Common;

namespace CF {
namespace RDM {

////////////////////////////////////////////////////////////////////////////////

Common::ComponentBuilder < ElementLoop<CSysSUPG,LinearAdv2D> , RDM::DomainLoop, LibRDM > CSysSUPG_LinearAdv2D_Builder;

Common::ComponentBuilder < ElementLoop<CSysSUPG,LinearAdvSys2D> , RDM::DomainLoop, LibRDM > CSysSUPG_LinearAdvSys2D_Builder;

Common::ComponentBuilder < ElementLoop<CSysSUPG,RotationAdv2D> , RDM::DomainLoop, LibRDM > CSysSUPG_RotationAdv2D_Builder;

////////////////////////////////////////////////////////////////////////////////

} // RDM
} // CF
