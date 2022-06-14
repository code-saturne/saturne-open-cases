# -*- coding: iso-8859-1 -*-

#-------------------------------------------------------------------------------

# This file is part of the code_saturne test suite.
#
# Copyright (C) 1998-2022 EDF S.A.
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
# Street, Fifth Floor, Boston, MA 02110-1301, USA.

#-------------------------------------------------------------------------------

### This file was generated automatically by SALOME v7.2.0 with
### dump python functionality

import sys
import salome

salome.salome_init()
theStudy = salome.myStudy

import salome_notebook
notebook = salome_notebook.NoteBook(theStudy)

####################################################
##       Start of User variables section          ##
####################################################
z_layer = 0.0005         # 0.0015 for coarse version
n_layers = 1
####################################################
##        End of User variables section           ##
####################################################

###
### SMESH component
###

import  SMESH, SALOMEDS
from salome.smesh import smeshBuilder

# Staggered tube base
#--------------------

smesh = smeshBuilder.New()

([Staggered_tube_base], status) = smesh.CreateMeshesFromMED(r'./staggered_tube_base.med')
[ left, bottom, top, wall, right ] = Staggered_tube_base.GetGroups()
#base = Staggered_tube_base.CreateEmptyGroup( SMESH.FACE, 'base' )
#nbAdd = base.AddFrom( Staggered_tube_base.mesh )
[ left_extruded, bottom_extruded, top_extruded, wall_extruded, right_extruded, left_top, bottom_top, top_top, wall_top, right_top ] = Staggered_tube_base.ExtrusionSweepObject2D( Staggered_tube_base, SMESH.DirStruct( SMESH.PointStruct ( 0, 0, z_layer )), n_layers ,True)
Staggered_tube_base.RemoveGroup( left   )
Staggered_tube_base.RemoveGroup( bottom )
Staggered_tube_base.RemoveGroup( top    )
Staggered_tube_base.RemoveGroup( wall   )
Staggered_tube_base.RemoveGroup( right  )
left_extruded.SetName( 'west' )
bottom_extruded.SetName( 'south' )
top_extruded.SetName( 'north' )
wall_extruded.SetName( 'wall' )
right_extruded.SetName( 'east' )
for obj in [left_extruded, bottom_extruded, top_extruded, right_extruded, wall_extruded]: 
  print(obj.GetName(), obj.Size())
basename = 'staggered_tube_' + str(n_layers)
smesh.SetName(Staggered_tube_base, 'Staggered_tube')
Staggered_tube_base.ExportMED( r'./' + basename + '.med', 0, SMESH.MED_V2_2, 1 )
#Staggered_tube_base.ExportCGNS( r'./' + basename + '.cgns', 1, Staggered_tube_base)
#Staggered_tube_base.ExportUNV( r'./' + basename + '.unv' )


if salome.sg.hasDesktop():
  salome.sg.updateObjBrowser(1)
