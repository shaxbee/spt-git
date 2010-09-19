#include <iostream>
#include <fstream>

#include <osg/Geode>
#include <osg/PolygonMode>
#include <osg/LineWidth>

#include <osgUtil/SmoothingVisitor>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include <sptCore/Path.h>
#include <sptGFX/Extruder.h>

#include "SectorNode.h"
#include "SectorView.h"

using namespace sptCore;
using namespace sptGFX;

void print_vec(const osg::Vec3& vec)
{
    std::cout << vec.x() << " " << vec.y() << " " << vec.z() << std::endl;
}

osg::Geometry* createProfile()
{

    osg::Geometry* geometry = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-1.2f, 0.0f, 0.0f));
    vertices->push_back(osg::Vec3(-0.8f, 0.0f, 0.5f));
    vertices->push_back(osg::Vec3( 0.8f, 0.0f, 0.5f));
    vertices->push_back(osg::Vec3( 1.2f, 0.0f, 0.0f));
    vertices->push_back(osg::Vec3(-1.2f, 0.0f, 0.0f));
    geometry->setVertexArray(vertices);

    osg::Vec2Array* texCoords = new osg::Vec2Array;
    texCoords->push_back(osg::Vec2(0.0f, 0.0f));
    texCoords->push_back(osg::Vec2(0.7f / 3.0f, 0.0f));
    texCoords->push_back(osg::Vec2(2.3f / 3.0f, 0.0f));
    texCoords->push_back(osg::Vec2(1.0f, 0.0f));
    texCoords->push_back(osg::Vec2(0.0f, 0.0f));
    geometry->setTexCoordArray(0, texCoords);

    return geometry;

};

osg::Geode* createAxes(osg::Geode* geode)
{

    osg::BoundingBox box = geode->getBoundingBox();
    box.expandBy(osg::BoundingBox(box.corner(0), box.corner(0) + osg::Vec3(100, 100, 100)));

    osg::Vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
    osg::Vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
    osg::Vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
   
    osg::Geode* result = new osg::Geode; 
    osg::Geometry* geometry = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array;
    osg::Vec4Array* colors = new osg::Vec4Array;

    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

    // X axis
    vertices->push_back(box.corner(0));
    vertices->push_back(box.corner(1));
    colors->push_back(red);
    colors->push_back(red);

    // Y axis
    vertices->push_back(box.corner(0));
    vertices->push_back(box.corner(2));
    colors->push_back(green);
    colors->push_back(green);

    // Z axis
    vertices->push_back(box.corner(0));
    vertices->push_back(box.corner(4));
    colors->push_back(blue);
    colors->push_back(blue);

    // compose geometry
    geometry->setVertexArray(vertices);

    geometry->setColorArray(colors);
    geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    geometry->setNormalArray(normals);
    geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
        
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->getNumElements()));

    osg::StateSet* stateSet = new osg::StateSet;
    stateSet->setAttribute(new osg::LineWidth(3));

    result->setStateSet(stateSet);

    // add geometry to geode
    result->addDrawable(geometry);

    return result;
};

int main()
{

    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    osg::ref_ptr<SectorNode> sectorNode = dynamic_cast<SectorNode*>(osgDB::readNodeFile("../scenery/test123/0_0.sct"));

    osg::ref_ptr<osg::Geometry> profile(createProfile());
    createSectorGeometry(geode.get(), profile.get(), sectorNode->getSector());

    root->addChild(geode.get());
    root->addChild(createAxes(geode.get()));

    osgViewer::Viewer viewer;
    
    viewer.setSceneData(root.get());
    viewer.run();
    
    return 0;
    
}
