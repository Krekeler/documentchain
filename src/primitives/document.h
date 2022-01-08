// Copyright (c) 2020-2021 The Documentchain developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_PRIMITIVES_DOCUMENT_H
#define BITCOIN_PRIMITIVES_DOCUMENT_H

#include <iostream>

class CDocumentHash
{
private:
public:
    int algo;
    std::string hash;

    CDocumentHash();
    CDocumentHash(int aalgo, std::string ahash);

    void SetNull();
    bool SameHash(CDocumentHash comparewith, bool allowempty = false);
    std::string AlgoName();
};

class CDocument
{
private:
    void SetNull();
    int HashHexLength(int algo);
public:
    int version;
    bool isvalid;
    std::string rawDocument;
    std::string guidcompr;
    std::string guid;
    CDocumentHash indexhash;
    CDocumentHash filehash;
    CDocumentHash attrhash;
    CDocumentHash ownerhash;

    CDocument();
    CDocument(std::string rawDoc);
};

#endif // BITCOIN_PRIMITIVES_DOCUMENT_H
