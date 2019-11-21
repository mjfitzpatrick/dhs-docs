load /usr/local/lib/libfitstcl.so
load $env(MONSOON_LIB)/libdhsTcl.so

proc dhsTest { } {

  # load the msdTcl.so libary
  global env
  load $env(MONSOON_LIB)/libmsdTcl.so

  # define some data
  set expID [msd::msd]
  set obsetID "Observation Set #1"
  set nelms 100
  set nlines 5
  set nitems 4
  set arry [list {0 1 2 3 4 5 6 7 8 9} {0 1 2 3 4 5 6 7 8 9} {0 1 2 3 4 5 6 7 8 9} {0 1 2 3 4 5 6 7 8 9} {0 1 2 3 4 5 6 7 8 9} {9 8 7 6 5 4 3 2 1 0} {9 8 7 6 5 4 3 2 1 0} {9 8 7 6 5 4 3 2 1 0} {9 8 7 6 5 4 3 2 1 0} {9 8 7 6 5 4 3 2 1 0} ]
  set fits [list TELESCOP "KPNO Mayall 4m" "Kitt Peak National Observatory" ASTRONOM "Joe Bloggs" "MONSOON Test System Username" INSTRUME "MONSOON+NEWFIRM" "MONSOON Image Acquisition" DETECTOR "aladdin_III" "aladdin_III 4096x4096" ]
  set avp [list  intTime 1.09365 "Integration time in milliseconds" biasVoltage -5.0982521 "Bias voltage for array" dataDir /MNSN/soft_dev/data/ "Current data directory" fileName 2453036.123645.fits "Current output filename" creg 0x02000000 "Control register in HEX representation" ]

  # SysOpen (0xFACE=OCS, 0xCAFE=MSL)
  set dhsID [dhs::SysOpen 0xFACE]
  puts "dhs::SysOpen ... (dhsID=$dhsID) [expr {$dhsID > 0 ? "OK" : "FAILED"}]"

  # SysClose
  set dhsStat [dhs::SysClose $dhsID]
  puts "dhs::SysClose ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # OpenConnect (0xABCD=PAN 0xFEED=WHO)
  set dhsID [dhs::OpenConnect 0xABCD {4096 4096 0 0 8}]
  puts "dhs::OpenConnect ... (dhsID=$dhsID) [expr {$dhsID > 0 ? "OK" : "FAILED"}]"

  # CloseConnect
  set dhsStat [dhs::CloseConnect $dhsID]
  puts "dhs::CloseConnect ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # OpenExp
  set dhsID [dhs::OpenExp $dhsID {4096 4096 0 0 8} $expID $obsetID]
  puts "dhs::OpenExp ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # CloseExp
  set dhsStat [dhs::CloseExp $dhsID $expID]
  puts "dhs::CloseExp ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # IOctl (0x1000=debug)
  set dhsStat [dhs::IOctl $dhsID 0x1000 $expID $obsetID]
  puts "dhs::IOctl ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # ReadImage
  set dhsStat [dhs::ReadImage $dhsID]
  puts "dhs::ReadImage ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # PixelData
  set dhsStat [dhs::PixelData $dhsID $arry $nelms {10 10 0 0 8} $expID $obsetID]
  puts "dhs::PixelData ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # MetaData (FITS)
  set dhsStat [dhs::MetaData $dhsID $fits $nitems {1 3 {8 20 46} {1 1 1}} $expID $obsetID]
  puts "dhs::MetaData (FITS) ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"

  # MetaData (AVP)
  set dhsStat [dhs::MetaData $dhsID $avp $nlines {2 3 {32 32 64} {1 1 1}} $expID $obsetID]
  puts "dhs::MetaData (AVP) ... (dhsID=$dhsID) [expr {$dhsStat == 0 ? "OK" : "FAILED"}]"
}
