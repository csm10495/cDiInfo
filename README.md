# cDiInfo
An application to get information via the Windows SetupDi... APIs

Usage options:

cDiInfo.exe -a 
    Prints all found device information
    
cDiInfo.exe -disable (or -enable) key value
    Looks for a device with an attribute that has the given key and value
      then disables/enables it. This option usually requires that the application
         runs as administrator

cDiInfo.exe -get key value
    Looks for a device with an attribute that has the given key and value
      then returns information about it  
    
