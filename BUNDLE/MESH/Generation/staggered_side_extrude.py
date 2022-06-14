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
z_layer = 0.0005*4
n_layers = 8
####################################################
##        End of User variables section           ##
####################################################

###
### SMESH component
###

import  SMESH, SALOMEDS
from salome.smesh import smeshBuilder

# Staggered side base
#--------------------

smesh = smeshBuilder.New()


([Staggered_side_base], status) = smesh.CreateMeshesFromMED(r'./staggered_side_base.med')
[ smeshObj_1, smeshObj_2, smeshObj_3, smeshObj_4, smeshObj_5 ] = Staggered_side_base.GetGroups()
#base = Staggered_side_base.CreateEmptyGroup( SMESH.FACE, 'base' )
#nbAdd = base.AddFrom( Staggered_side_base.mesh )
[ smeshObj_6, smeshObj_7, smeshObj_8, smeshObj_9, smeshObj_10, smeshObj_11 ] = Staggered_side_base.ExtrusionSweepObject2D( Staggered_side_base, SMESH.DirStruct( SMESH.PointStruct ( 0, 0, z_layer)), n_layers ,True)
Staggered_side_base.RemoveGroup( smeshObj_1 )
Staggered_side_base.RemoveGroup( smeshObj_2 )
Staggered_side_base.RemoveGroup( smeshObj_3 )
Staggered_side_base.RemoveGroup( smeshObj_4 )
Staggered_side_base.RemoveGroup( smeshObj_5 )
smeshObj_6.SetName( 'wall' )
smeshObj_7.SetName( 'west' )
smeshObj_8.SetName( 'north' )
smeshObj_9.SetName( 'east' )
smeshObj_10.SetName( 'south' )
Staggered_side_base.RemoveGroup( smeshObj_11 )
for obj in [smeshObj_6, smeshObj_7, smeshObj_8, smeshObj_9, smeshObj_10]: 
  print dir(obj)
  print obj.GetName(), obj.Size()
basename = 'staggered_side_' + str(n_layers)
smesh.SetName(Staggered_side_base, 'Staggered_side')
Staggered_side_base.ExportMED( r'./' + basename + '.med', 0, SMESH.MED_V2_2, 1 )
#Staggered_side_base.ExportCGNS( r'./' + basename + '.cgns', 1, Staggered_side_base)
#Staggered_side_base.ExportUNV( r'./' + basename + '.unv' )

if salome.sg.hasDesktop():
  salome.sg.updateObjBrowser(1)
