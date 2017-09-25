# thinkflash
stupid little hack to blink the ThinkLight™ on FreeBSD

## Install
    
    make
    make install (as root)
  
  
## Usage
`acpi-ibm` must be loaded
  
    thinkflash [milliseconds]
  
Example: to have urxvt flash the light on bell, in your Xdefaults/Xresources
  
    URxvt*bell-command: thinkflash 200
  
