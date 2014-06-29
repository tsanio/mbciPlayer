#ifndef BOOSTCOPYDIR_H
#define BOOSTCOPYDIR_H

#include "playerCommon.h"
/**
 * \brief BoostCopyDir.h ...::: Boost's directory copying :::...
*
* Copies boost directory.
*
* author: tonis @ 2013
*
*/
class BoostCopyDir
{
public:
    BoostCopyDir();
    virtual ~BoostCopyDir();
protected:
private:
};


namespace plrCommon
{

bool copyDir(boost::filesystem::path const & source,boost::filesystem::path const & destination);

};


#endif // BOOSTCOPYDIR_H
