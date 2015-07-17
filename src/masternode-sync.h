// Copyright (c) 2014-2015 The Dash developers

// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef MASTERNODE_SYNC_H
#define MASTERNODE_SYNC_H

#define MASTERNODE_INITIAL                0
#define MASTERNODE_SPORK_SETTINGS         1
#define MASTERNODE_SYNC_LIST              2
#define MASTERNODE_SYNC_MNW               3
#define MASTERNODE_SYNC_BUDGET            4
#define MASTERNODE_LIST_SYNCED            999

#define MASTERNODE_SYNC_TIMEOUT           5

class CMasternodeSync;
extern CMasternodeSync masternodeSync;

//
// CMasternodeSync : Sync masternode assets in stages
//

class CMasternodeSync
{
public:
    int64_t lastMasternodeList;
    int64_t lastMasternodeWinner;
    int64_t lastBudgetItem;

    // Count peers we've requested the list from
    int RequestedMasternodeAssets;
    int RequestedMasternodeAttempt;


    CMasternodeSync();

    void AddedMasternodeList();
    void AddedMasternodeWinner();
    void AddedBudgetItem();
    void GetNextAsset();

    void Process();
    bool IsSynced();
};

#endif