# PeParser
Parser for PE files (windows executable format) which focus on correctness.

# Brief
Focus on correctness mean of course reduced performance sometimes. Also, for the sake of simpicity, when parsing headers, the code will usually not buffer
data as they are not meant to be accessed a lot in a short period of time. But unless you need to access the data in a near realtime fashion, this "slowness" should be absolutely no trouble.
This library is a collection of parser rather than a single parser, each with different goals. Right now, we have 4 differents parser available :
- The COFF header parser    : will parse only informations present in the COFF part of the PE header.
- The PE header parser      : an extension of the PE parser, it will parse information in both COFF and PE headers (along with data table informations).
- The section header parser : will parse informations about section headers.
- The section data parser   : a bit of a misnomer as it does not parser anything, but will extract data from sections.

# TODOs 
- Actually implement the section data parser.
- Add the possibility to buffer the calls.
- Complete the section header parser, and optimize it.
