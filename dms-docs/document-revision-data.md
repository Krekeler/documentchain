Document Data Documentation
===========================

This document describes the structure of the document revision data. Applies as of DMS Core version 0.12.16

## Revision Data

Documentchain stores serveral hash values in transactions. Below the data is described as hex string.

### Prefix

`444D24000104D2`

| Data | Size | Description |
| ---------- | ---------- | ----------- |
| 444D24 | 6 | Constant prefix, magic chars "DM$" |
| 0001 | 4 | Blockchain-defined data type/version |
| 04D2 | 4 | App-defined data type/version |

#### Blockchain-defined Data Type 0001

Fixed order of 128 bit hash values as hex string with 32 characters each.

| Start | End | Size | Description |
| ---------- | ---------- | ----------- | ----------- |
| 0 | 5 | 6 | 444D24 |
| 6 | 9 | 4 | 0001 |
| 10 | 13 | 4 | App-defined type |
| 14 | 45 | 32 | Document GUID without brackets and hyphens |
| 46 | 77 | 32 | File hash |
| 78 | 109 | 32 | Attribute hash |
| 110 | 141  | 32 | (optional) any hash, such as owner hash |

##### Example

A text file with content "Documentchain" and file name "Testfile.txt" has the following hex data:

444d2400010003a819374e9e5e4c98aeed1adc6feccc0de5ce47fb53fba975afa0060d490f4f309e0d97e0b0ef38bb22d44819dc9b6d3db048bc1a059d83d52de7dac1d0cf01c4

#### Blockchain-defined Data Type 0002

Any combination of individual hash values. The whole data must not exceed 600 bytes.

The first value should be an MD5<sup>1)</sup> file hash. This allows the user to perform a [document revision](https://documentchain.org/news/document-index/) 
in his web browser. The MD5 hash is used to search for the corresponding blockchain transaction. In addition, another more secure file hash should be stored for the revision.

| Start | End | Size | Description |
| ---------- | ---------- | ---------- | ---------- |
| 0 | 5 | 6 | 444D24 |
| 6 | 9 | 4 | 0002 |
| 10 | 13 | 4 | App-defined type |

Followed by one or more hash values with four-digit prefix:

| Start | End | Size | Description |
| ---------- | ---------- | ----------- | ----------- |
| 0 | 0 | 1 | Type: 0=compressed GUID, F=file hash, A=attribute hash , B=owner hash |
| 1 | 1 | 1 | App-defined char 0..F |
| 2 | 3 | 2 | Algorithm (see below), i.e. 22 |
| 4 | .. | 32-128 | Hex encoding hash |

##### Algorithm
* 00=MD5/GUID <sup>1)</sup>
* 01=SHA1<sup>1)</sup>
* 21=SHA2-224 <sup>1)</sup>
* 22=SHA2-256
* 23=SHA2-384
* 25=SHA2-512
* 31=SHA3-224 <sup>1)</sup>
* 32=SHA3-256
* 33=SHA3-384
* 35=SHA3-512

<sup>1)</sup> These hashes are not considered collision proof and should only be used together with longer hashes. See [BSI](https://www.bsi.bund.de/EN/Publications/TechnicalGuidelines/tr02102/tr02102_node.html).

#### App Defined Data Type

* [Office Manager DMS](https://www.officemanager.de/) uses "0001" and "0003"
* DMS Core uses "0002"
* WebAPI on [documentchain.org](https://api.documentchain.org/) uses "0004"

Use another ID for your application. You can reserve your app ID with us.

### Example Code

* [Qt/C++ API Example in DMS Core](../src/qt/documentlist.cpp)
* [JavaScript API Example](https://github.com/Krekeler/api-example-js)
* [Delphi API Example](https://github.com/Krekeler/api-example-pas)
