#include <sptCore/SceneryBuilder.h>

#include <cmath>
#include <boost/cast.hpp>

#include <sptCore/Track.h>
#include <sptCore/Switch.h>

#include <sptCore/DynamicScenery.h>
#include <sptCore/DynamicSector.h>

using namespace sptCore;

SceneryBuilder::SceneryBuilder():
    _scenery(new DynamicScenery())
{

    _sector = createSector(osg::Vec3());

};

SceneryBuilder::SceneryBuilder(DynamicScenery* scenery):
    _scenery(scenery)
{

    _sector = createSector(osg::Vec3());

};

DynamicSector* SceneryBuilder::createSector(const osg::Vec3& position)
{

    DynamicSector* result = new DynamicSector(*_scenery, position);
    _scenery->addSector(result);
    return result;

};

DynamicSector& SceneryBuilder::getOrCreateSector(const osg::Vec3& position)
{

    if(_scenery->hasSector(position))
        return *(boost::polymorphic_cast<DynamicSector*>(&_scenery->getSector(position)));

    return *createSector(position);

}; // SceneryBuilder::getOrCreateSector

DynamicSector& SceneryBuilder::setCurrentSector(const osg::Vec3& position)
{

    _sector = &(getOrCreateSector(position));
    return *_sector;

}; // SceneryBuilder::setCurrentSector

void SceneryBuilder::addConnection(const osg::Vec3& position, RailTracking* track)
{

    osg::Vec3 offset(floor(position.x() / Sector::SIZE), 0, floor(position.z() / Sector::SIZE));

    if(offset != osg::Vec3())
    {
        offset *= Sector::SIZE;
        getOrCreateSector(_sector->getPosition() + offset).addConnection(position - offset, track);
    }
    else
    {
        _sector->addConnection(position, track);
    };

}; // SceneryBuilder::addConnection

Track& SceneryBuilder::createTrack(const osg::Vec3& p1, const osg::Vec3& p2)
{

    Track* track = new Track(getCurrentSector(), p1, p2);

    _sector->addTrack(track);
    addConnection(p1, track);
    addConnection(p2, track);

    return *track;

};

Track& SceneryBuilder::createTrack(const osg::Vec3& p1, const osg::Vec3& cp1, const osg::Vec3& p2, const osg::Vec3& cp2)
{

    Track* track = new Track(getCurrentSector(), p1, cp1, p2, cp2);

    _sector->addTrack(track);
    addConnection(p1, track);
    addConnection(p2, track);

    return *track;

};

Track& SceneryBuilder::createTrack(const std::string& name, const osg::Vec3& p1, const osg::Vec3& p2)
{

    Track* track = &(createTrack(p1, p2));
    _scenery->addTrack(name, track);
    return *track;

};

Track& SceneryBuilder::createTrack(const std::string& name, const osg::Vec3& p1, const osg::Vec3& cp1, const osg::Vec3& p2, const osg::Vec3& cp2)
{

    Track* track = &(createTrack(p1, cp1, p2, cp2));
    _scenery->addTrack(name, track);
    return *track;

};

void SceneryBuilder::removeTrack(Track& track)
{

    DynamicSector& sector = dynamic_cast<DynamicSector&>(track.getSector());
    const Path& path = track.getDefaultPath();
    
    sector.removeConnection(path.front());
    sector.removeConnection(path.back());

    sector.removeTrack(&track);

}; // SceneryBuilder::removeTrack(track)

void SceneryBuilder::removeTrack(const std::string& name)
{

    removeTrack(_scenery->getTrack(name));

}; // SceneryBuilder::removeTrack(name)

Switch& SceneryBuilder::createSwitch(const osg::Vec3& p1, const osg::Vec3& cp1, const osg::Vec3& p2, const osg::Vec3& cp2, const osg::Vec3& p3, const osg::Vec3& cp3)
{

    Switch* result = new Switch(getCurrentSector(), p1, cp1, p2, cp2, p3, cp3);

    _sector->addTrack(result);
    addConnection(p1, result);
    addConnection(p2, result);
    addConnection(p3, result);

    return *result;

};

Switch& SceneryBuilder::createSwitch(const std::string& name, const osg::Vec3& p1, const osg::Vec3& cp1, const osg::Vec3& p2, const osg::Vec3& cp2, const osg::Vec3& p3, const osg::Vec3& cp3)
{

    Switch* result = &(createSwitch(p1, cp1, p2, cp2, p3, cp3));
    _scenery->addSwitch(name, result);
    return *result;

};

void SceneryBuilder::removeSwitch(Switch& tracking)
{

    DynamicSector& sector = dynamic_cast<DynamicSector&>(tracking.getSector());
    const Path& straight = tracking.getStraightPath();
    const Path& diverted = tracking.getDivertedPath();

    sector.removeConnection(straight.front());
    sector.removeConnection(straight.back());
    sector.removeConnection(diverted.back());

    sector.removeTrack(&tracking);

};

void SceneryBuilder::removeSwitch(const std::string& name)
{

    removeSwitch(dynamic_cast<Switch&>(_scenery->getSwitch(name)));

}; // SceneryBuilder::removeSwitch(name)
