#ifndef BOOSTRELATIVEPATH_H
#define BOOSTRELATIVEPATH_H

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <stdexcept>
/**
 * \brief BoostRelativePath.h ...::: Boost - Relative path :::...
*
* Handles relative paths. It's useful when loading songs and porting the application to flash disk or similar. It's possible to use relative folders for music etc.
*
* author: tonis @ 2013
*
*/
class BoostRelativePath
{
public:
    BoostRelativePath();
    virtual ~BoostRelativePath();
protected:
private:
};

boost::filesystem::path naive_uncomplete(boost::filesystem::path const p, boost::filesystem::path const base);

#endif // BOOSTRELATIVEPATH_H
