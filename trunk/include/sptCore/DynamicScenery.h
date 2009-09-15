#ifndef SPTCORE_DYNAMICSCENERY_H
#define SPTCORE_DYNAMICSCENERY_H 1

#include <sptCore/Scenery.h>

#include <map>

namespace sptCore
{

class Sector;

class Track;
class Switch;

class DynamicScenery: public Scenery
{
public:
	virtual ~DynamicScenery();

	virtual Sector& getSector(const osg::Vec3& position) const;

	virtual Track& getTrack(const std::string& name) const;
//	virtual EventedTrack& getEventedTrack(const std::string& name) const;	
	virtual Switch& getSwitch(const std::string& name) const;
	
	virtual const Statistics& getStatistics() const { return _statistics; };

    //! Add sector to scenery and manage its lifetime
	//! \throw SectorExistsException if Sector with same name exists	
	void addSector(Sector* sector);

    //! Add named Track    
	//! \throw RailTrackingExistsException if Track with same name exists
	void addTrack(const std::string& name, Track* track);

    //! Remove named Track
    //! \throw UnknownRailTrackingException when no Track with specified name is found
//    void removeTrack(const std::string& name);
	
//	//! \throw RailTrackingExistsException if EventedTrack with same name exists	
//	void addEventedTrack(const std::string& name, EventedTrack* track);

    //! Add name Switch    
	//! \throw RailTrackingExistsException if Switch with same name exists	
	void addSwitch(const std::string& name, Switch* track);

	class SectorExistsException: public boost::exception { };
	class RailTrackingExistsException: public boost::exception { };
	
protected:
	typedef std::map<osg::Vec3, Sector*> Sectors;	

	typedef std::map<std::string, Track*> Tracks;
//	typedef std::map<std::string, boost::shared_ptr<EventedTrack> > EventedTracks;
	typedef std::map<std::string, Switch*> Switches;

	Sectors _sectors;

	Tracks _tracks;
//	EventedTracks _eventedTracks;
	Switches _switches;

	Statistics _statistics;
		
}; // class sptCore::DynamicScenery

} // namespace sptCore

#endif // headerguard
