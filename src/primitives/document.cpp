// Copyright (c) 2020-2021 The Documentchain developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/document.h"
#include "tinyformat.h"

/** CDocumentHash
 */
CDocumentHash::CDocumentHash()
{
    SetNull();
}

CDocumentHash::CDocumentHash(int aalgo, std::string ahash)
{
    algo = aalgo;
    hash = ahash;
}

void CDocumentHash::SetNull()
{
    algo = -1;
    hash = "";
}

bool CDocumentHash::SameHash(CDocumentHash comparewith, bool allowempty/*=false*/)
{
    if (!allowempty && (hash.empty() || algo < 0))
        return false;

    return (hash == comparewith.hash && algo == comparewith.algo);
}

std::string CDocumentHash::AlgoName()
{
    switch (algo) {
        case  0: return "MD5"; // GUID/MD5
        case  1: return "SHA1";
        case 21: return "SHA2-224";
        case 22: return "SHA2-256";
        case 23: return "SHA2-384";
        case 25: return "SHA2-512";
        case 31: return "SHA3-224";
        case 32: return "SHA3-256";
        case 33: return "SHA3-384";
        case 35: return "SHA3-512";
        default: return "";
    }
}

/** CDocument
 */
CDocument::CDocument() {
    SetNull();
}

CDocument::CDocument(std::string rawDoc)
{
    SetNull();
    rawDocument = rawDoc; // "444d24..."
    std::transform(rawDocument.begin(), rawDocument.end(), rawDocument.begin(), ::toupper);
    if (rawDocument.length() < 42 || rawDocument.substr(0, 6) != "444D24")
        return;

    std::string sVersion = rawDocument.substr(6, 4);
    if (sVersion == "0001") {
        version = 1;
        guidcompr = rawDocument.substr(14, 32);
        filehash = CDocumentHash(0, rawDocument.substr(46, 32));
        indexhash = filehash;
        attrhash = CDocumentHash(0, rawDocument.substr(78, 32));
        if (rawDocument.length() == 142)
            ownerhash = CDocumentHash(0, rawDocument.substr(110, 32));
    }
    else if (sVersion == "0002") {
        version = 2;
        int idx = 14;
        int i = 0;
        while (idx < rawDocument.length()) {
            i++; // wrong data could cause an endless loop
            if (i > 19)
                throw("Invalid hash data");
            char htype = rawDocument[idx];
            int halgo = std::stoi(rawDocument.substr(idx+2, 2));
            std::string hash = rawDocument.substr(idx+4, HashHexLength(halgo));
            idx += 4 + HashHexLength(halgo);

            switch (htype) {
                case '0':
                    guidcompr = hash;
                    break;
                case 'F':
                    if (halgo == 0)
                        indexhash = CDocumentHash(halgo, hash);
                    else
                        filehash = CDocumentHash(halgo, hash);
                    break;
                case 'A':
                    attrhash = CDocumentHash(halgo, hash);
                    break;
                case 'B':
                    ownerhash = CDocumentHash(halgo, hash);
            }
        }
    }

    if (!guidcompr.empty())
        guid = strprintf("{%s-%s-%s-%s-%s}", guidcompr.substr(0, 8), guidcompr.substr(8, 4), 
                  guidcompr.substr(12, 4), guidcompr.substr(16, 4), guidcompr.substr(20, 12));
    isvalid = true;
}

void CDocument::SetNull()
{
    isvalid = false;
    version = 0;
    guidcompr = "";
    guid = "";
    indexhash.SetNull();
    filehash.SetNull();
    attrhash.SetNull();
    ownerhash.SetNull();
}

int CDocument::HashHexLength(int algo)
{
    switch (algo) {
        case  0: return 32;  // GUID/MD5
        case  1: return 40;  // SHA1
        case 21: return 56;  // SHA2-224
        case 22: return 64;  // SHA2-256
        case 23: return 96;  // SHA2-384
        case 25: return 128; // SHA2-512
        case 31: return 56;  // SHA3-224
        case 32: return 64;  // SHA3-256
        case 33: return 96;  // SHA3-384
        case 35: return 128; // SHA3-512
        default: return 0;
    }
}
