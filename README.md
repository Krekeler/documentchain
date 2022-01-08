DMS Core - Documentchain Project
=====================

https://documentchain.org/


testnet3 Archive
---------------------

Archive of Documenchain® testnet3, which ran from August 2019 to January 2022. Some incompatible tests were performed, 
which required special rules in the program code. A testnet should also be as close as possible to the mainnet. 
For the changeover to deterministic masternodes in 2022, a clean testnet is useful before the mainnet is switched.

The new testnet4 starts at the beginning of 2022.

## Tips

Default port of archived testnet3 is 41433.

There are no seed nodes defined, you have to use `addnode`.

### Wallet Download

Windows: https://documentchain.org/files/testnet3-archive/dms-core-testnet3-win64.zip

Linux: https://documentchain.org/files/testnet3-archive/dms-core-testnet3-linux.tar.gz


### dms.conf

```
testnet=1
rpcuser=myrpcuser
rpcpassword=mysecret
rpcallowip=127.0.0.1
rpcport=41432
server=1
listen=1
daemon=1
txindex=1
addressindex=1
documentindex=1
timestampindex=1
spentindex=1
addnode=85.214.65.60:41433
addnode=81.169.215.168:41433
```

### Command Line

`~/dms-testnet3-archive/dmsd -testnet -datadir=$HOME/dms-testnet3-archive/data --daemon`

`~/dms-testnet3-archive/dms-cli -testnet -datadir=$HOME/dms-testnet3-archive/data getinfo`

License
---------------------

DMS Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.
