#!/usr/bin/python

import sys
import numpy as np
import Image
import vtk

print("yes")
rmat = [[1.,0,0],[0,0,-1.],[0,1.,0]]

class VtkPointCloud:
    def __init__(self, zMin=-10.0, zMax=10.0, maxNumPoints=1e6):
        self.maxNumPoints = maxNumPoints
        self.vtkPolyData = vtk.vtkPolyData()
        #setup colors
        self.colors = vtk.vtkUnsignedCharArray()
        self.colors.SetNumberOfComponents(3)
        self.colors.SetName("Colors")

        self.clearPoints()
        self.vtkPolyData.GetPointData().SetScalars(self.colors)
        
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInput(self.vtkPolyData)
        mapper.SetColorModeToDefault()
        mapper.SetScalarRange(zMin, zMax)
        mapper.SetScalarVisibility(1)
        self.vtkActor = vtk.vtkActor()
        self.vtkActor.SetMapper(mapper)

        self.vtkActor.GetProperty().SetPointSize(3)
        
    def addPoint(self, point, color):
        if self.vtkPoints.GetNumberOfPoints() < self.maxNumPoints:
            pointId = self.vtkPoints.InsertNextPoint(point[:])
            self.vtkDepth.InsertNextValue(point[2])
            self.vtkCells.InsertNextCell(1)
            self.vtkCells.InsertCellPoint(pointId)
            self.colors.InsertNextTuple3(color[0],color[1],color[2])
        else:
            r = random.randint(0, self.maxNumPoints)
            self.vtkPoints.SetPoint(r, point[:])
            self.vtkCells.Modified()
            self.vtkPoints.Modified()
            self.vtkDepth.Modified()

    def clearPoints(self):
        self.vtkPoints = vtk.vtkPoints()
        self.vtkCells = vtk.vtkCellArray()
        self.vtkDepth = vtk.vtkDoubleArray()
        self.vtkDepth.SetName('DepthArray')
        self.vtkPolyData.SetPoints(self.vtkPoints)
        self.vtkPolyData.SetVerts(self.vtkCells)
        self.vtkPolyData.GetPointData().SetScalars(self.vtkDepth)
        self.vtkPolyData.GetPointData().SetActiveScalars('DepthArray')

        
def gen_points(w,h,tex_map):
    points = VtkPointCloud()
    
    for x in range(w):
        percx = ((x+(1./w)) / w)
        lat = 2 * np.pi * percx - np.pi

        for y in range(h):
            percy = ((y+(1./h)) / h)
            lon = np.pi * percy 

            p = [np.cos(lat)*np.sin(lon), 
                 -1*np.cos(lon),
                 -1* np.sin(lat)*np.sin(lon)]

            new_p = []
            for row in rmat:
                new_p.append(np.dot(row, p))

            c = tex_map.getpixel((percx*tex_map.size[0],tex_map.size[1]-percy*tex_map.size[1]))

            points.addPoint(new_p, c)

    return points



if len(sys.argv) > 1:
    print "  using",sys.argv[1]," as tex map"
    
    tmap = Image.open(sys.argv[1])
    tmap.transpose(Image.FLIP_LEFT_RIGHT)
    tmap.transpose(Image.FLIP_TOP_BOTTOM)
    print "    width: ",tmap.size[0]
    print "    height:",tmap.size[1]
    
    pointCloud = gen_points(144,72,tmap)    

    # Renderer
    renderer = vtk.vtkRenderer()
    renderer.AddActor(pointCloud.vtkActor)
    #renderer.SetBackground(.2, .3, .4)
    renderer.SetBackground(0.0, 0.0, 0.0)
    renderer.ResetCamera()
    
    # Render Window
    renderWindow = vtk.vtkRenderWindow()
    renderWindow.AddRenderer(renderer)

    # Interactor
    renderWindowInteractor = vtk.vtkRenderWindowInteractor()
    renderWindowInteractor.SetRenderWindow(renderWindow)
    
    # Begin Interaction
    renderWindow.Render()
    renderWindow.SetWindowName("Globe Preview:"+sys.argv[1])
    renderWindowInteractor.Start()
