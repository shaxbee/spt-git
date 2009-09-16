#ifndef SPTCORE_DYNAMICSECTOR_H
#define SPTCORE_DYNAMICSECTOR_H

#include <sptCore/Sector.h>

#include <map>
#include <set>

namespace sptCore
{

//! \brief Modifiable Sector
//! \author Zbyszek "ShaXbee" Mandziejewicz
class DynamicSector: public Sector
{

public:
    DynamicSector(Scenery& scenery, osg::Vec3 position): Sector(scenery, position) { };
    virtual ~DynamicSector();

    virtual RailTracking& getNextTrack(const osg::Vec3& position, RailTracking* from) const;
    virtual const Connection& getConnection(const osg::Vec3& position) const;
    virtual size_t getTotalTracks() const { return _tracks.size(); };

    //! \brief Register track at sector
    //! Track instance will be managed by Sector
    void addTrack(RailTracking* track) { _tracks.insert(track); };

    //! \brief Unregister track from sector
    void removeTrack(RailTracking* track) { _tracks.erase(track); };

    //! \brief Add track to connection
    //! \throw InvalidConnectionException if complementary connection exists at given position 
    void addConnection(const osg::Vec3& position, RailTracking* track);

    //! \brief Add connection of tracks pair
    //! \throw InvalidConnectionException if complementary connection exists at given position 
    void addConnection(const osg::Vec3& position, RailTracking* left, RailTracking* right);

    //! \brief Remove connection
    void removeConnection(const osg::Vec3& position) { _connections.erase(position); };

	//! \brief Removed orphaned connections
	//! Search for connections with one or both NULL trackings
	void cleanup();

    typedef std::map<osg::Vec3, Connection> Connections;
    typedef std::set<RailTracking*> Tracks;

    const Connections& getConnections() { return _connections; };
    const Tracks& getTracks() { return _tracks; };

    class InvalidConnectionException: public boost::exception { };

private:
    struct IsOrphaned
    {

        bool operator()(Connections::value_type entry) { return !entry.second.first || !entry.second.second; }

    }; // struct IsOrphaned

    Connections _connections;
    Tracks _tracks;

}; // class sptCore::DynamicSector

} // namespace sptCore

#endif // headerguard
