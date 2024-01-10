# CVE-2023-36900
- About this vulnerability: https://msrc.microsoft.com/update-guide/vulnerability/CVE-2023-36900.
- [Writeup](https://github.com/RomanRybachek/CVE-2023-36900/blob/main/writeup/eng/writeup_eng.md)
- [Imhex pattern for .blf extension](#imhex-pattern-for-blf-extension)
- [Header with declaration of CLFS structures](https://github.com/RomanRybachek/CVE-2023-36900/blob/main/clfs_structures.h)
- [Denial-of-Service proof of concept](#denial-of-service-proof-of-concept)
## Imhex pattern for blf extension
### Before:
<img src="https://github.com/RomanRybachek/CVE-2023-36900/blob/main/git_resources/blf_before.png" alt="drawing" width="500"/></br>
### After:
<img src="https://github.com/RomanRybachek/CVE-2023-36900/blob/main/git_resources/blf_after.png" alt="drawing" width="500"/></br></br>
The pattern is far from ideal, but it can be useful. [Link](https://github.com/RomanRybachek/CVE-2023-36900/blob/main/imhex_pattern/blf_clfs.hexpat) </br>
### How to use:
```
File->Import->Pattern File
```
## Denial of Service proof of concept
The proof of concept requres [python](https://www.python.org/downloads/windows/) and the [pwntools](https://github.com/Gallopsled/pwntools) module installed. 
Because testing has to be done on a specific build of Windows, to prevent Microsoft from updating your OS, it must not be connected to the Internet. Therefore, all dependencies need to be installed offline. [How to install python modules without internet](http://srikanthtechnologies.com/blog/python/installing_libraries_offline.aspx).</br>
To run POC, copy [the folder with POC](https://github.com/RomanRybachek/CVE-2023-36900/tree/main/poc) to target machine and use following command:
```
python launch.py DoS
```
