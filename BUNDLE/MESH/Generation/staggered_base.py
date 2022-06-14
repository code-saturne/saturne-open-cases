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
##       Begin of NoteBook variables section      ##
####################################################
# Geometry
notebook.set("radius", "21.7 * 0.001 / 2")
notebook.set("half_step", "45 * 0.001 / 2")
notebook.set("bl_thickness", "radius * 0.1")
notebook.set("bl_radius", "radius + bl_thickness")
notebook.set("quarter_step", "half_step / 2")
notebook.set("x_half_corner", "half_step * 0.7")
notebook.set("y_half_corner", "half_step - x_half_corner")
notebook.set("x_base_corner", "x_half_corner * 1.04")
notebook.set("y_base_corner", "half_step - x_base_corner")
# Fine or coarse version
if True:
    notebook.set("n_wall_1", 41)
    notebook.set("n_wall_2", 41)
    notebook.set("n_diag", 38)
    notebook.set("n_boundary", 28)
    notebook.set("mult_boundary", 2)
    notebook.set("mult_diag", 2.5)
    notebook.set("n_fill_1_bottom", 38)
    notebook.set("n_half_corner", 12)
else:
    notebook.set("n_wall_1", 4)
    notebook.set("n_wall_2", 4)
    notebook.set("n_diag", 4)
    notebook.set("n_boundary", 3)
    notebook.set("mult_boundary", 2)
    notebook.set("mult_diag", 2.5)
    notebook.set("n_fill_1_bottom", 4)
    notebook.set("n_half_corner", 1)
####################################################
##        End of NoteBook variables section       ##
####################################################
import iparameters
ipar = iparameters.IParameters(salome.myStudy.GetCommonParameters("Interface Applicative", 1))

#Set up visual properties:
ipar.setProperty("AP_ACTIVE_VIEW", "VTKViewer_0_0")
ipar.setProperty("AP_WORKSTACK_INFO", "0000000100000000000000020100000001000003d4000000040000000200000001000000080000001a004f00430043005600690065007700650072005f0030005f00300000000002000000080000001a00560054004b005600690065007700650072005f0030005f00300000000102")
ipar.setProperty("AP_ACTIVE_MODULE", "Mesh")
ipar.setProperty("AP_SAVEPOINT_NAME", "GUI state: 2")
#Set up lists:
# fill list AP_VIEWERS_LIST
ipar.append("AP_VIEWERS_LIST", "OCCViewer_1")
ipar.append("AP_VIEWERS_LIST", "VTKViewer_2")
# fill list OCCViewer_1
ipar.append("OCCViewer_1", "OCC scene:1 - viewer:1")
ipar.append("OCCViewer_1", "scale=1.000000000000e+00*centerX=0.000000000000e+00*centerY=0.000000000000e+00*projX=5.773502588272e-01*projY=-5.773502588272e-01*projZ=5.773502588272e-01*twist=0.000000000000e+00*atX=0.000000000000e+00*atY=0.000000000000e+00*atZ=0.000000000000e+00*eyeX=2.886751294136e+02*eyeY=-2.886751294136e+02*eyeZ=2.886751294136e+02*scaleX=1.000000000000e+00*scaleY=1.000000000000e+00*scaleZ=1.000000000000e+00*isVisible=1*size=100.00*gtIsVisible=0*gtDrawNameX=1*gtDrawNameY=1*gtDrawNameZ=1*gtNameX=X*gtNameY=Z*gtNameZ=Z*gtNameColorRX=255*gtNameColorGX=0*gtNameColorBX=0*gtNameColorRY=0*gtNameColorGY=255*gtNameColorBY=0*gtNameColorRZ=0*gtNameColorGZ=0*gtNameColorBZ=255*gtDrawValuesX=1*gtDrawValuesY=1*gtDrawValuesZ=1*gtNbValuesX=3*gtNbValuesY=3*gtNbValuesZ=3*gtOffsetX=2*gtOffsetY=2*gtOffsetZ=2*gtColorRX=255*gtColorGX=0*gtColorBX=0*gtColorRY=0*gtColorGY=255*gtColorBY=0*gtColorRZ=0*gtColorGZ=0*gtColorBZ=255*gtDrawTickmarksX=1*gtDrawTickmarksY=1*gtDrawTickmarksZ=1*gtTickmarkLengthX=5*gtTickmarkLengthY=5*gtTickmarkLengthZ=5")
# fill list VTKViewer_2
ipar.append("VTKViewer_2", "VTK scene:1 - viewer:1")
ipar.append("VTKViewer_2", """<?xml version="1.0"?>
<ViewState>
    <Position X="0" Y="0" Z="0.475013"/>
    <FocalPoint X="0" Y="0" Z="0"/>
    <ViewUp X="0" Y="1" Z="0"/>
    <ViewScale Parallel="0.0264416" X="1" Y="1" Z="1"/>
    <DisplayCubeAxis Show="0"/>
    <GraduatedAxis Axis="X">
        <Title isVisible="1" Text="X" Font="0" Bold="0" Italic="0" Shadow="0">
            <Color R="1" G="0" B="0"/>
        </Title>
        <Labels isVisible="1" Number="3" Offset="2" Font="0" Bold="0" Italic="0" Shadow="0">
            <Color R="1" G="0" B="0"/>
        </Labels>
        <TickMarks isVisible="1" Length="5"/>
    </GraduatedAxis>
    <GraduatedAxis Axis="Y">
        <Title isVisible="1" Text="Y" Font="0" Bold="0" Italic="0" Shadow="0">
            <Color R="0" G="1" B="0"/>
        </Title>
        <Labels isVisible="1" Number="3" Offset="2" Font="0" Bold="0" Italic="0" Shadow="0">
            <Color R="0" G="1" B="0"/>
        </Labels>
        <TickMarks isVisible="1" Length="5"/>
    </GraduatedAxis>
    <GraduatedAxis Axis="Z">
        <Title isVisible="1" Text="Z" Font="0" Bold="0" Italic="0" Shadow="0">
            <Color R="0" G="0" B="1"/>
        </Title>
        <Labels isVisible="1" Number="3" Offset="2" Font="0" Bold="0" Italic="0" Shadow="0">
            <Color R="0" G="0" B="1"/>
        </Labels>
        <TickMarks isVisible="1" Length="5"/>
    </GraduatedAxis>
    <Trihedron isShown="0" Size="105"/>
</ViewState>
""")
# fill list AP_MODULES_LIST
ipar.append("AP_MODULES_LIST", "Geometry")
ipar.append("AP_MODULES_LIST", "Mesh")


###
### GEOM component
###

import GEOM
import math
import SALOMEDS

from salome.geom import geomBuilder
geompy = geomBuilder.New()

O = geompy.MakeVertex(0, 0, 0)
c01 = geompy.MakeVertex("radius", 0, 0)
c02 = geompy.MakeVertex(0, "radius", 0)
Arc_1 = geompy.MakeArcCenter(O, c01, c02,False)
c11 = geompy.MakeVertex("bl_radius", 0, 0)
c12 = geompy.MakeVertex(0, "bl_radius", 0)
corner_half = geompy.MakeVertex("quarter_step", "quarter_step", 0)
c21 = geompy.MakeVertex("x_base_corner", 0, 0)
c22 = geompy.MakeVertex("x_half_corner", "y_half_corner", 0)
c23 = geompy.MakeVertex("half_step", "y_base_corner", 0)
c24 = geompy.MakeVertex("half_step", 0, 0)
diag_cut = geompy.MakeLineTwoPnt(O, corner_half)
Arc_2 = geompy.MakeArcCenter(O, c11, c12,False)
c31 = geompy.MakeVertexOnLinesIntersection(Arc_1, diag_cut)
c32 = geompy.MakeVertexOnLinesIntersection(Arc_2, diag_cut)
diag_cut_2 = geompy.MakeLineTwoPnt(O, c22)
c41 = geompy.MakeVertexOnLinesIntersection(Arc_1, diag_cut_2)
c42 = geompy.MakeVertexOnLinesIntersection(Arc_2, diag_cut_2)
Arc_3 = geompy.MakeArcCenter(O, c01, c41,False)
Arc_4 = geompy.MakeArcCenter(O, c41, c31,False)
Arc_5 = geompy.MakeArcCenter(O, c11, c42,False)
Arc_6 = geompy.MakeArcCenter(O, c42, c32,False)
Line_2 = geompy.MakeLineTwoPnt(c21, c22)
Line_1 = geompy.MakeLineTwoPnt(c22, corner_half)
Boundary_1 = geompy.MakeQuad2Edges(Arc_3, Arc_5)
[Edge_1,Edge_2,geomObj_1] = geompy.SubShapes(Boundary_1, [6, 10, 2])
[geomObj_2] = geompy.SubShapes(Boundary_1, [3])
[geomObj_3] = geompy.SubShapeAll(Boundary_1, geompy.ShapeType["FACE"])
listSubShapeIDs = geompy.SubShapeAllIDs(Boundary_1, geompy.ShapeType["FACE"])
listSubShapeIDs = geompy.SubShapeAllIDs(Boundary_1, geompy.ShapeType["EDGE"])
Fill_2 = geompy.MakeQuad2Edges(Arc_6, Line_1)
[Edge_9,Edge_10] = geompy.SubShapes(Fill_2, [10, 6])
Quadrangle_Face_1 = geompy.MakeQuad4Vertices(c21, c24, c23, c22)
[geomObj_4,geomObj_5] = geompy.SubShapes(Quadrangle_Face_1, [6, 3])
Boundary_2 = geompy.MakeQuad2Edges(Arc_4, Arc_6)
[geomObj_6] = geompy.SubShapes(Boundary_2, [0, 10])
[Edge_3,Edge_4] = geompy.SubShapes(Boundary_2, [6, 10])
[geomObj_7] = geompy.SubShapes(Boundary_2, [3])
[geomObj_8] = geompy.SubShapes(Boundary_2, [3])
Fill_1 = geompy.MakeQuad2Edges(Arc_5, Line_2)
[Edge_5,Edge_6,Edge_7,Edge_8] = geompy.ExtractShapes(Fill_1, geompy.ShapeType["EDGE"], True)
wall = geompy.CreateGroup(Boundary_1, geompy.ShapeType["EDGE"])
geompy.UnionIDs(wall, [3])
wall_1 = geompy.CreateGroup(Boundary_2, geompy.ShapeType["EDGE"])
geompy.UnionIDs(wall_1, [3])
bottom = geompy.CreateGroup(Boundary_1, geompy.ShapeType["EDGE"])
geompy.UnionIDs(bottom, [6])
bottom_1 = geompy.CreateGroup(Fill_1, geompy.ShapeType["EDGE"])
geompy.UnionIDs(bottom_1, [6])
bottom_2 = geompy.CreateGroup(Quadrangle_Face_1, geompy.ShapeType["EDGE"])
geompy.UnionIDs(bottom_2, [3])
right = geompy.CreateGroup(Quadrangle_Face_1, geompy.ShapeType["EDGE"])
geompy.UnionIDs(right, [6])
Auto_group_for_SubMesh_5 = geompy.CreateGroup(Fill_2, geompy.ShapeType["EDGE"])
geompy.UnionList(Auto_group_for_SubMesh_5, [Edge_9, Edge_10])
Auto_group_for_SubMesh_6 = geompy.CreateGroup(Fill_1, geompy.ShapeType["EDGE"])
geompy.UnionList(Auto_group_for_SubMesh_6, [Edge_6, Edge_7])
geompy.addToStudy( O, 'O' )
geompy.addToStudy( c01, 'c01' )
geompy.addToStudy( c02, 'c02' )
geompy.addToStudy( Arc_1, 'Arc_1' )
geompy.addToStudy( c11, 'c11' )
geompy.addToStudy( c12, 'c12' )
geompy.addToStudy( corner_half, 'corner_half' )
geompy.addToStudy( c21, 'c21' )
geompy.addToStudy( c22, 'c22' )
geompy.addToStudy( c23, 'c23' )
geompy.addToStudy( c24, 'c24' )
geompy.addToStudy( diag_cut, 'diag_cut' )
geompy.addToStudy( Arc_2, 'Arc_2' )
geompy.addToStudy( c31, 'c31' )
geompy.addToStudy( c32, 'c32' )
geompy.addToStudy( diag_cut_2, 'diag_cut_2' )
geompy.addToStudy( c41, 'c41' )
geompy.addToStudy( c42, 'c42' )
geompy.addToStudy( Arc_3, 'Arc_3' )
geompy.addToStudy( Arc_4, 'Arc_4' )
geompy.addToStudy( Arc_5, 'Arc_5' )
geompy.addToStudy( Arc_6, 'Arc_6' )
geompy.addToStudy( Line_2, 'Line_2' )
geompy.addToStudy( Line_1, 'Line_1' )
geompy.addToStudy( Boundary_1, 'Boundary_1' )
geompy.addToStudyInFather( Boundary_1, Edge_1, 'Edge_1' )
geompy.addToStudyInFather( Boundary_1, Edge_2, 'Edge_2' )
geompy.addToStudy( Fill_2, 'Fill_2' )
geompy.addToStudyInFather( Fill_2, Edge_9, 'Edge_9' )
geompy.addToStudyInFather( Fill_2, Edge_10, 'Edge_10' )
geompy.addToStudy( Quadrangle_Face_1, 'Quadrangle Face_1' )
geompy.addToStudy( Boundary_2, 'Boundary_2' )
geompy.addToStudyInFather( Boundary_2, Edge_3, 'Edge_3' )
geompy.addToStudyInFather( Boundary_2, Edge_4, 'Edge_4' )
geompy.addToStudy( Fill_1, 'Fill_1' )
geompy.addToStudyInFather( Fill_1, Edge_5, 'Edge_5' )
geompy.addToStudyInFather( Fill_1, Edge_6, 'Edge_6' )
geompy.addToStudyInFather( Fill_1, Edge_7, 'Edge_7' )
geompy.addToStudyInFather( Fill_1, Edge_8, 'Edge_8' )
geompy.addToStudyInFather( Boundary_1, wall, 'wall' )
geompy.addToStudyInFather( Boundary_2, wall_1, 'wall' )
geompy.addToStudyInFather( Boundary_1, bottom, 'bottom' )
geompy.addToStudyInFather( Fill_1, bottom_1, 'bottom' )
geompy.addToStudyInFather( Quadrangle_Face_1, bottom_2, 'bottom' )
geompy.addToStudyInFather( Quadrangle_Face_1, right, 'right' )
geompy.addToStudyInFather( Fill_2, Auto_group_for_SubMesh_5, 'Auto_group_for_SubMesh_5' )
geompy.addToStudyInFather( Fill_1, Auto_group_for_SubMesh_6, 'Auto_group_for_SubMesh_6' )

### Store presentation parameters of displayed objects
import iparameters
ipar = iparameters.IParameters(theStudy.GetModuleParameters("Interface Applicative", "GEOM", 1))

#Set up entries:
# set up entry GEOM_29 (Fill_1) parameters
objId = geompy.getObjectID(Fill_1)
ipar.setParameter(objId, "VTKViewer_1_Color", "1:1:0")
# set up entry GEOM_3 (c02) parameters
objId = geompy.getObjectID(c02)
ipar.setParameter(objId, "VTKViewer_1_Color", "1:1:0")
# set up entry GEOM_4 (Arc_1) parameters
objId = geompy.getObjectID(Arc_1)
ipar.setParameter(objId, "VTKViewer_1_Color", "1:1:0")
# set up entry GEOM_5 (c11) parameters
objId = geompy.getObjectID(c11)
ipar.setParameter(objId, "VTKViewer_1_Color", "1:1:0")
# set up entry GEOM_6 (c12) parameters
objId = geompy.getObjectID(c12)
ipar.setParameter(objId, "VTKViewer_1_Color", "1:1:0")
# set up entry GEOM_7 (corner_half) parameters
objId = geompy.getObjectID(corner_half)
ipar.setParameter(objId, "VTKViewer_1_Color", "1:1:0")

###
### SMESH component
###

import SMESH, SALOMEDS

from salome.smesh import smeshBuilder
smesh = smeshBuilder.New()

import StdMeshers
Corner = smesh.Mesh(Quadrangle_Face_1)
Regular_1D = Corner.Segment()
Nb_Segments_half_corner = Regular_1D.NumberOfSegments("n_wall_1")
Nb_Segments_half_corner.SetDistrType( 0 )
Quadrangle_2D = Corner.Quadrangle()
isDone = Corner.Compute()
Boundary_1_1 = smesh.Mesh(Boundary_1)
Regular_1D_1 = Boundary_1_1.Segment()
Nb_Segments_wall_1 = Regular_1D_1.NumberOfSegments("n_wall_1")
Nb_Segments_wall_1.SetDistrType( 0 )
Quadrangle_2D_1 = Boundary_1_1.Quadrangle()
isDone = Boundary_1_1.Compute()
Regular_1D_2 = Boundary_1_1.Segment(geom=Edge_1)
Nb_Segments_boundary = Regular_1D_2.NumberOfSegments("n_boundary","mult_boundary",[  ])
isDone = Boundary_1_1.Compute()
[ SubMesh_1 ] = Boundary_1_1.GetMesh().GetSubMeshes()
Regular_1D_3 = Boundary_1_1.Segment(geom=Edge_2)
status = Boundary_1_1.AddHypothesis(Nb_Segments_boundary,Edge_2)
isDone = Boundary_1_1.Compute()
Boundary_2_1 = smesh.Mesh(Boundary_2)
Regular_1D_4 = Boundary_2_1.Segment()
Nb_Segments_wall_2 = Regular_1D_4.NumberOfSegments("n_wall_2")
Nb_Segments_wall_2.SetDistrType( 0 )
Quadrangle_2D_2 = Boundary_2_1.Quadrangle()
isDone = Boundary_2_1.Compute()
Regular_1D_5 = Boundary_2_1.Segment(geom=Edge_3)
status = Boundary_2_1.AddHypothesis(Nb_Segments_boundary,Edge_3)
isDone = Boundary_2_1.Compute()
Regular_1D_6 = Boundary_2_1.Segment(geom=Edge_4)
status = Boundary_2_1.AddHypothesis(Nb_Segments_boundary,Edge_4)
isDone = Boundary_2_1.Compute()
Fill_2_1 = smesh.Mesh(Fill_2)
Regular_1D_7 = Fill_2_1.Segment()
Nb_Segments_n_diag = Regular_1D_7.NumberOfSegments("n_diag","mult_diag",[  ])
isDone = Fill_2_1.Compute()
Quadrangle_2D_3 = Fill_2_1.Quadrangle()
status = Fill_2_1.RemoveHypothesis(Nb_Segments_n_diag)
status = Fill_2_1.AddHypothesis(Nb_Segments_wall_2)
Regular_1D_8 = Fill_2_1.Segment(geom=Auto_group_for_SubMesh_5)
status = Fill_2_1.AddHypothesis(Nb_Segments_n_diag,Auto_group_for_SubMesh_5)
isDone = Fill_2_1.Compute()
[ SubMesh_5 ] = Fill_2_1.GetMesh().GetSubMeshes()
isDone = Fill_2_1.Compute()
[ SubMesh_3, SubMesh_4 ] = Boundary_2_1.GetMesh().GetSubMeshes()
[ SubMesh_5 ] = Fill_2_1.GetMesh().GetSubMeshes()
[  ] = Fill_2_1.GetMeshOrder()
Fill_1_a = smesh.Mesh(Fill_1)
status = Fill_1_a.AddHypothesis(Nb_Segments_wall_1)
Regular_1D_9 = Fill_1_a.Segment()
Quadrangle_2D_4 = Fill_1_a.Quadrangle()
Regular_1D_10 = Fill_1_a.Segment(geom=Auto_group_for_SubMesh_6)
status = Fill_1_a.AddHypothesis(Nb_Segments_n_diag,Auto_group_for_SubMesh_6)
isDone = Fill_1_a.Compute()
bottom_3 = Corner.GroupOnGeom(bottom_2,'bottom',SMESH.EDGE)
right_1 = Corner.GroupOnGeom(right,'right',SMESH.EDGE)
wall_2 = Boundary_1_1.GroupOnGeom(wall,'wall',SMESH.EDGE)
bottom_4 = Boundary_1_1.GroupOnGeom(bottom,'bottom',SMESH.EDGE)
wall_3 = Boundary_2_1.GroupOnGeom(wall_1,'wall',SMESH.EDGE)
bottom_5 = Fill_1_a.GroupOnGeom(bottom_1,'bottom',SMESH.EDGE)
Quadrangle_Parameters_1 = smesh.CreateHypothesis('QuadrangleParams')
Quadrangle_Parameters_1.SetQuadType( StdMeshers.QUAD_REDUCED )
Fill_1_b = smesh.Mesh(Fill_1)
status = Fill_1_b.AddHypothesis(Nb_Segments_wall_1)
Regular_1D_11 = Fill_1_b.Segment()
status = Fill_1_b.AddHypothesis(Quadrangle_Parameters_1)
Quadrangle_2D_5 = Fill_1_b.Quadrangle()
Regular_1D_12 = Fill_1_b.Segment(geom=Edge_5)
status = Fill_1_b.AddHypothesis(Nb_Segments_wall_1,Edge_5)
Nb_Segments_n_diag.SetNumberOfSegments( "n_diag" )
Nb_Segments_n_diag.SetDistrType( 1 )
Nb_Segments_n_diag.SetScaleFactor( "mult_diag" )
Nb_Segments_n_diag.SetReversedEdges( [  ] )
Regular_1D_13 = Fill_1_b.Segment(geom=Edge_7)
status = Fill_1_b.AddHypothesis(Nb_Segments_n_diag,Edge_6)
Regular_1D_14 = Fill_1_b.GetSubMesh( Edge_6, 'Regular_1D' )
Regular_1D_14_1 = Fill_1_b.Segment(geom=Edge_6)
Nb_Segments_n_fill_1_bottom = Regular_1D_13.NumberOfSegments("n_fill_1_bottom","mult_diag",[  ])
Regular_1D_15 = Fill_1_b.Segment(geom=Edge_8)
status = Fill_1_b.AddHypothesis(Nb_Segments_half_corner,Edge_8)
isDone = Fill_1_b.Compute()
isDone = Fill_1_b.SmoothObject( Fill_1_b, [  ], 2, 1.1, SMESH.SMESH_MeshEditor.LAPLACIAN_SMOOTH )
isDone = Fill_1_b.SmoothObject( Fill_1_b, [  ], 2, 1.1, SMESH.SMESH_MeshEditor.LAPLACIAN_SMOOTH )
Quadrangle_Parameters_1.SetQuadType( StdMeshers.QUAD_STANDARD )
Fill_1_b.Clear()
isDone = Fill_1_b.Compute()
bottom_6 = Fill_1_b.GroupOnGeom(bottom_1,'bottom',SMESH.EDGE)
isDone = Fill_2_1.Compute()
Compound_Mesh_1 = smesh.Concatenate([Boundary_1_1.GetMesh(), Boundary_2_1.GetMesh(), Fill_2_1.GetMesh(), Fill_1_b.GetMesh()], 1, 1, 1e-05)
[ wall_4, bottom_7 ] = Compound_Mesh_1.GetGroups()
simPt = geompy.PointCoordinates(c22)
[ wall_5, right_2 ] = Compound_Mesh_1.MirrorObject( Compound_Mesh_1, SMESH.AxisStruct( simPt[0], simPt[1], 0, -1, 1, 0 ), SMESH.SMESH_MeshEditor.AXIS ,True,True)
wall_5.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
wall_5.SetName( 'wall' )
right_2.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
right_2.SetName( 'right' )
Compound_Mesh_2 = smesh.Concatenate([Corner.GetMesh(), Compound_Mesh_1.GetMesh()], 1, 1, 1e-05)
[ bottom_8, right_3, wall_6 ] = Compound_Mesh_2.GetGroups()
bottom_8.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
right_3.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
wall_6.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
Compound_Mesh_2_mirrored = Compound_Mesh_2.MirrorObjectMakeMesh( Compound_Mesh_2, SMESH.AxisStruct( 0, 0, 0, 1, 1, 0 ), SMESH.SMESH_MeshEditor.AXIS, 1, 'Compound_Mesh_2_mirrored' )
[ Oy, top, wall_7 ] = Compound_Mesh_2_mirrored.GetGroups()
Oy.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
Oy.SetName( 'Oy' )
top.SetName( 'top' )
isDone = Compound_Mesh_2_mirrored.Compute()
Compound_Mesh_3 = smesh.Concatenate([Compound_Mesh_2.GetMesh(), Compound_Mesh_2_mirrored.GetMesh()], 1, 1, 1e-05)
[ smeshObj_1, right_4, wall_8, smeshObj_2, top_1 ] = Compound_Mesh_3.GetGroups()
smesh.SetName(Compound_Mesh_3, 'Compound_Mesh_3')
Compound_Mesh_3.ExportMED( r'./staggered_base.med', 0, SMESH.MED_V2_2, 1 )
smeshObj_1.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
Compound_Mesh_3.RemoveGroup( smeshObj_1 )
smeshObj_2.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
Compound_Mesh_3.RemoveGroup( smeshObj_2 )
Compound_Mesh_3_mirrored = Compound_Mesh_3.MirrorObjectMakeMesh( Compound_Mesh_3, SMESH.AxisStruct( 0, 0, 0, 1, 0, 0 ), SMESH.SMESH_MeshEditor.AXIS, 1, 'Compound_Mesh_3_mirrored' )
[ right_5, wall_9, bottom_9 ] = Compound_Mesh_3_mirrored.GetGroups()
bottom_9.SetName( 'bottom' )
Compound_Mesh_4 = smesh.Concatenate([Compound_Mesh_3.GetMesh(), Compound_Mesh_3_mirrored.GetMesh()], 1, 1, 1e-05)
[ right_6, wall_10, top_2, bottom_10 ] = Compound_Mesh_4.GetGroups()
Compound_Mesh_4_mirrored = Compound_Mesh_4.MirrorObjectMakeMesh( Compound_Mesh_4, SMESH.AxisStruct( 0, 0, 0, 0, 1, 0 ), SMESH.SMESH_MeshEditor.AXIS, 1, 'Compound_Mesh_4_mirrored' )
[ left, wall_11, top_3, bottom_11 ] = Compound_Mesh_4_mirrored.GetGroups()
smeshObj_3 = smesh.Concatenate([Compound_Mesh_4_mirrored.GetMesh()], 1, 1, 1e-05)
[ smeshObj_4, smeshObj_5, smeshObj_6, smeshObj_7 ] = smeshObj_3.GetGroups()
smeshObj_3.RemoveGroup( smeshObj_7 )
smeshObj_3.RemoveGroup( smeshObj_6 )
smeshObj_3.RemoveGroup( smeshObj_5 )
smeshObj_3.RemoveGroup( smeshObj_4 )
smeshObj_8 = smesh.Concatenate([Compound_Mesh_4.GetMesh(), Compound_Mesh_4_mirrored.GetMesh()], 1, 1, 1e-05)
[ smeshObj_9, smeshObj_10, smeshObj_11, smeshObj_12 ] = smeshObj_8.GetGroups()
[ smeshObj_9, smeshObj_10, smeshObj_11, smeshObj_12 ] = smeshObj_8.GetGroups()
[ smeshObj_9, smeshObj_10, smeshObj_11, smeshObj_12 ] = smeshObj_8.GetGroups()
smeshObj_8.RemoveGroup( smeshObj_12 )
smeshObj_8.RemoveGroup( smeshObj_11 )
smeshObj_8.RemoveGroup( smeshObj_10 )
smeshObj_8.RemoveGroup( smeshObj_9 )
left.SetColor( SALOMEDS.Color( 0, 0.666667, 1 ))
left.SetName( 'left' )
Staggered_tube_base = smesh.Concatenate([Compound_Mesh_4.GetMesh(), Compound_Mesh_4_mirrored.GetMesh()], 1, 1, 1e-05)
[ right_7, wall_12, top_4, bottom_12, left_1 ] = Staggered_tube_base.GetGroups()
smesh.SetName(Staggered_tube_base, 'Staggered_tube_base')
Staggered_tube_base.ExportMED( r'./staggered_tube_base.med', 0, SMESH.MED_V2_2, 1 )
SubMesh_1 = Regular_1D_2.GetSubMesh()
Regular_1D_16 = Regular_1D_3.GetSubMesh()
SubMesh_3 = Regular_1D_5.GetSubMesh()
SubMesh_4 = Regular_1D_6.GetSubMesh()
SubMesh_5 = Regular_1D_8.GetSubMesh()
Regular_1D_17 = Regular_1D_10.GetSubMesh()
Regular_1D_18 = Regular_1D_12.GetSubMesh()
Regular_1D_14 = Regular_1D_13.GetSubMesh()
Regular_1D_19 = Regular_1D_15.GetSubMesh()

## some objects were removed
aStudyBuilder = theStudy.NewBuilder()
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_1))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_2))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_3.GetMesh()))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_4))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_5))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_6))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_7))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_8.GetMesh()))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_9))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_10))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_11))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
SO = theStudy.FindObjectIOR(theStudy.ConvertObjectToIOR(smeshObj_12))
if SO is not None: aStudyBuilder.RemoveObjectWithChildren(SO)
## set object names
smesh.SetName(Corner.GetMesh(), 'Corner')
smesh.SetName(Regular_1D.GetAlgorithm(), 'Regular_1D')
smesh.SetName(Nb_Segments_half_corner, 'Nb. Segments half_corner')
smesh.SetName(Quadrangle_2D.GetAlgorithm(), 'Quadrangle_2D')
smesh.SetName(Boundary_1_1.GetMesh(), 'Boundary_1')
smesh.SetName(Nb_Segments_wall_1, 'Nb. Segments wall_1')
smesh.SetName(Nb_Segments_boundary, 'Nb. Segments boundary')
smesh.SetName(SubMesh_1, 'SubMesh_1')
smesh.SetName(Boundary_2_1.GetMesh(), 'Boundary_2')
smesh.SetName(Nb_Segments_wall_2, 'Nb. Segments wall_2')
smesh.SetName(Fill_2_1.GetMesh(), 'Fill_2')
smesh.SetName(Nb_Segments_n_diag, 'Nb. Segments n_diag')
smesh.SetName(SubMesh_5, 'SubMesh_5')
smesh.SetName(SubMesh_3, 'SubMesh_3')
smesh.SetName(SubMesh_4, 'SubMesh_4')
smesh.SetName(Fill_1_a.GetMesh(), 'Fill_1_a')
smesh.SetName(bottom_3, 'bottom')
smesh.SetName(right_1, 'right')
smesh.SetName(wall_2, 'wall')
smesh.SetName(bottom_4, 'bottom')
smesh.SetName(wall_3, 'wall')
smesh.SetName(bottom_5, 'bottom')
smesh.SetName(Quadrangle_Parameters_1, 'Quadrangle Parameters_1')
smesh.SetName(Fill_1_b.GetMesh(), 'Fill_1_b')
smesh.SetName(Regular_1D_14, 'Regular_1D')
smesh.SetName(Nb_Segments_n_fill_1_bottom, 'Nb. Segments n_fill_1_bottom')
smesh.SetName(bottom_6, 'bottom')
smesh.SetName(Compound_Mesh_1.GetMesh(), 'Compound_Mesh_1')
smesh.SetName(wall_4, 'wall')
smesh.SetName(bottom_7, 'bottom')
smesh.SetName(wall_5, 'wall')
smesh.SetName(right_2, 'right')
smesh.SetName(Compound_Mesh_2.GetMesh(), 'Compound_Mesh_2')
smesh.SetName(bottom_8, 'bottom')
smesh.SetName(right_3, 'right')
smesh.SetName(wall_6, 'wall')
smesh.SetName(Compound_Mesh_2_mirrored.GetMesh(), 'Compound_Mesh_2_mirrored')
smesh.SetName(Oy, 'Oy')
smesh.SetName(top, 'top')
smesh.SetName(wall_7, 'wall')
smesh.SetName(Compound_Mesh_3.GetMesh(), 'Compound_Mesh_3')
smesh.SetName(right_4, 'right')
smesh.SetName(wall_8, 'wall')
smesh.SetName(top_1, 'top')
smesh.SetName(Compound_Mesh_3_mirrored.GetMesh(), 'Compound_Mesh_3_mirrored')
smesh.SetName(right_5, 'right')
smesh.SetName(wall_9, 'wall')
smesh.SetName(bottom_9, 'bottom')
smesh.SetName(Compound_Mesh_4.GetMesh(), 'Compound_Mesh_4')
smesh.SetName(right_6, 'right')
smesh.SetName(wall_10, 'wall')
smesh.SetName(top_2, 'top')
smesh.SetName(bottom_10, 'bottom')
smesh.SetName(Compound_Mesh_4_mirrored.GetMesh(), 'Compound_Mesh_4_mirrored')
smesh.SetName(left, 'left')
smesh.SetName(wall_11, 'wall')
smesh.SetName(top_3, 'top')
smesh.SetName(bottom_11, 'bottom')
smesh.SetName(Staggered_tube_base.GetMesh(), 'Staggered_tube_base')
smesh.SetName(right_7, 'right')
smesh.SetName(wall_12, 'wall')
smesh.SetName(top_4, 'top')
smesh.SetName(bottom_12, 'bottom')
smesh.SetName(left_1, 'left')
smesh.SetName(Regular_1D_16, 'Regular_1D')
smesh.SetName(Regular_1D_17, 'Regular_1D')
smesh.SetName(Regular_1D_18, 'Regular_1D')
smesh.SetName(Regular_1D_19, 'Regular_1D')

