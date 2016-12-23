/*
 * libretroshare/src/services: rsreputation.h
 *
 * Services for RetroShare.
 *
 * Copyright 2015 by Cyril Soler
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "csoler@users.sourceforge.net".
 *
 */

#pragma once

#include "retroshare/rsids.h"
#include "retroshare/rsgxsifacetypes.h"

class RsReputations
{
public:
        static const float REPUTATION_THRESHOLD_ANTI_SPAM;
        static const float REPUTATION_THRESHOLD_DEFAULT;
        
	// This is the interface file for the reputation system
	//
	enum Opinion         { OPINION_NEGATIVE = 0, OPINION_NEUTRAL = 1, OPINION_POSITIVE = 2 } ;

	enum ReputationLevel {  REPUTATION_LOCALLY_NEGATIVE  = 0x00, // local opinion is positive
		                    REPUTATION_REMOTELY_NEGATIVE = 0x01, // local opinion is neutral and friends are positive in average
		                    REPUTATION_NEUTRAL           = 0x02, // no reputation information ;
		                    REPUTATION_REMOTELY_POSITIVE = 0x03, // local opinion is neutral and friends are negative in average
		                    REPUTATION_LOCALLY_POSITIVE  = 0x04  // local opinion is negative
						 };

	struct ReputationInfo
	{
		ReputationInfo() : mOwnOpinion(OPINION_NEUTRAL), mFriendAverageScore(REPUTATION_THRESHOLD_DEFAULT),mOverallReputationLevel(REPUTATION_NEUTRAL){}
            
		RsReputations::Opinion mOwnOpinion ;

		uint32_t mFriendsPositiveVotes ;
		uint32_t mFriendsNegativeVotes ;

		float mFriendAverageScore ;

		RsReputations::ReputationLevel mOverallReputationLevel;	// this should help clients in taking decisions
	};

	virtual bool setOwnOpinion(const RsGxsId& key_id, const Opinion& op) =0;
    virtual bool getReputationInfo(const RsGxsId& id, const RsPgpId &ownerNode, ReputationInfo& info) =0;

    // parameters

    // virtual void setNodeAutoBanThreshold(uint32_t n) =0;
    // virtual uint32_t nodeAutoBanThreshold() =0;

    virtual void setNodeAutoPositiveOpinionForContacts(bool b) =0;
    virtual bool nodeAutoPositiveOpinionForContacts() =0;
    virtual float nodeAutoBanIdentitiesLimit() =0;
    virtual void setNodeAutoBanIdentitiesLimit(float f) =0;

        // This one is a proxy designed to allow fast checking of a GXS id.
        // it basically returns true if assessment is not ASSESSMENT_OK
        
    virtual bool isIdentityBanned(const RsGxsId& id) =0;

    virtual bool isNodeBanned(const RsPgpId& id) =0;
    virtual void banNode(const RsPgpId& id,bool b) =0;
};

// To access reputations from anywhere
//
extern RsReputations *rsReputations ;
