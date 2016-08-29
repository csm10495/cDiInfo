# cDiInfo - Charles Machalow - MIT License - (C) 2016
An application to get information via the Windows SetupDi... APIs... and some other calls. 

Usage options:

Use -xml to get xml output where allowed. Otherwise, human-readable output is given.

cDiInfo.exe <-xml> -a 

    Prints all found device information
    
cDiInfo.exe -disable (or -enable, or -restart) key value

    Looks for a device with an attribute that has the given key and value
      then disables/enables/restart it. This option usually requires that the application
         runs as administrator

cDiInfo.exe <-xml> -get key value

    Looks for a device with an attribute that has the given key and value
      then returns information about it  
    
cDiInfo.exe <-xml> -getAll key value

    Looks for all devices with an attribute that has the given key and value
      then returns all information about it  

cDiInfo.exe <-xml> -getAllWithout key value

    Looks for all devices with an attribute that doesn't has the given key and value
      then returns all information about it  

cDiInfo.exe <-xml> -getAllJust key value (other,keys)

    Looks for all devices with an attribute that has the given key and value
      then returns that key/value and all optional keys and their values as well. 
      
cDiInfo.exe -keys

    Returns possible keys list.

cDiInfo.exe -enumerators

    Returns list of enumerators
    
cDiInfo.exe -classes

    Returns list of classes
